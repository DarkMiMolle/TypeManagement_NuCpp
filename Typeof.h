#pragma once
#include "NuType.h"
#include "Var.h"

template <valid_cpp_type_info T>
constexpr const NuType_t auto& Typeof(T&&) {
    return cpp_type_info<T>::NuTypeVal;
}

template <typename T>
constexpr const NuType_t auto& Typeof(var<T>&) {
    if constexpr (NuType_t<T>) { return T::Self; }
    else if constexpr (requires { typename cpp_type_info<T>; }) {
        return cpp_type_info<T>::NuTypeVal;
    }
}

template<NuType_t T>
constexpr const auto& Typeof(const T& val) {
    if constexpr(std::are_similar<T, NuType<0>>) { return val; }
    if constexpr(!std::are_similar<T, NuType<0>>) { return T::Self; }
}

#define TypeofExpr(expr) cpp_type_info<std::just_t<decltype(expr)>>::NuTypeVal