#ifndef MVG_TYPE_ERASED_HPP_
#define MVG_TYPE_ERASED_HPP_

#include <memory>

namespace mvg {

template<typename Interface, template<typename> typename Impl>
class type_erased {
public:
    type_erased() = default;

    template<typename T>
    type_erased(T&& t) : val(std::make_unique<Impl<T>>(std::forward<T>(t))) {}

    template<typename T, typename... Args>
    type_erased(Args&&... args) :
        val(std::make_unique<Impl<T>>(std::forward<Args>(args)...)) {}

    template<typename T>
    type_erased& operator=(T&& t) {
        val = std::make_unique<Impl<T>>(std::forward<T>(t));
        return *this;
    }

    template<typename T, typename... Args>
    void emplace(Args&&... args) {
        val = std::make_unique<Impl<T>>(std::forward<Args>(args)...);
    }

    auto operator-> () { return val; }
    auto operator-> () const { return val; }

    auto get() { return val; }
    auto get() const { return val; }

    template<typename T>
    auto get_as() {
        return static_cast<T*>(val.get());
    }

    template<typename T>
    auto get_as() const {
        return static_cast<T const*>(val.get());
    }

private:
    std::unique_ptr<Interface> val;
};

} // namespace mvg

#endif
