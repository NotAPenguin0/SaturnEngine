#ifndef MVG_BIND_GUARD_HPP_
#define MVG_BIND_GUARD_HPP_

#include "Utility\bind_tracker.hpp"

namespace Saturn {

template<typename T, auto BindFun = T::bind, auto UnbindFun = T::unbind>
class [[nodiscard]] bind_guard {
public:
    bind_guard(T & obj) noexcept(noexcept(BindFun(obj))) {
        if (bind_tracker<T>::current() == &obj) {
            bound_flag = false;
            to_rebind = nullptr;
        } else {
            to_rebind = bind_tracker<T>::current();
            BindFun(obj);
            bind_tracker<T>::set(obj);
            bound_flag = true;
        }
    }

    bind_guard(bind_guard const&) = delete;
    bind_guard(bind_guard && rhs) noexcept {
        bound_flag = rhs.bound_flag;
        to_rebind = rhs.to_rebind;
        rhs.bound_flag = false;
        rhs.to_rebind = nullptr;
    }

    bind_guard& operator=(bind_guard const&) = delete;
    bind_guard& operator=(bind_guard&& rhs) noexcept {
        bound_flag = rhs.bound_flag;
        to_rebind = rhs.to_rebind;
        rhs.bound_flag = false;
        rhs.to_rebind = nullptr;
        return *this;
    }

    ~bind_guard() noexcept(noexcept(UnbindFun())) {
        if (bound_flag) {
            UnbindFun();
            if (to_rebind != nullptr) {
                BindFun(*to_rebind);
                bind_tracker<T>::set(*to_rebind);
            } else {
                bind_tracker<T>::reset();
            }
        }
    }

private:
    bool bound_flag;
    T* to_rebind = nullptr;
};

} // namespace Saturn

#endif
