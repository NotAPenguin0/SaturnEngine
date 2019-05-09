#ifndef MVG_TYPE_ERASED_HPP_
#define MVG_TYPE_ERASED_HPP_

#include <memory>

namespace mvg {

template<typename Interface, template<typename> typename Impl>
class type_erased {
public:
    type_erased() = default;

    template<typename ImplT>
    type_erased(ImplT&& t) : val(std::make_unique<ImplT>(std::forward<ImplT>(t))) {}

    template<typename ImplT, typename... Args>
    type_erased(Args&&... args) :
        val(std::make_unique<ImplT>(std::forward<Args>(args)...)) {}

    template<typename ImplT>
    type_erased& operator=(ImplT&& t) {
        val = std::make_unique<ImplT>(std::forward<T>(t));
        return *this;
    }

    template<typename ImplT, typename... Args>
    void emplace(Args&&... args) {
        val = std::make_unique<ImplT>(std::forward<Args>(args)...);
    }

    auto operator-> () { return val; }
    auto operator-> () const { return val; }

    auto get() { return val; }
    auto get() const { return val; }

    template<typename ImplT>
    auto get_as() {
        return static_cast<ImplT*>(val.get());
    }

    template<typename ImplT>
    auto get_as() const {
        return static_cast<ImplT const*>(val.get());
    }

private:
    std::unique_ptr<Interface> val;
};

} // namespace mvg

#endif
