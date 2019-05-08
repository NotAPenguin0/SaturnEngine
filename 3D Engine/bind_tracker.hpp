#ifndef MVG_BIND_TRACKER_HPP_
#define MVG_BIND_TRACKER_HPP_

namespace Saturn {

template<typename T>
class bind_tracker {
public:
    bind_tracker() { (void)&cur; }

    static void set(T& obj) noexcept { cur = &obj; }

    static void reset() noexcept { cur = nullptr; }

    static T* current() noexcept { return cur; }

private:
    static T* cur;
};

template<typename T>
T* bind_tracker<T>::cur = nullptr;

} // namespace Saturn

#endif
