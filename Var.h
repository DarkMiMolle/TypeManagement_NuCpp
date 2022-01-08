#pragma once
#include "BuiltinTypes.h"


template <typename T>
struct var;

template <valid_cpp_type_info T>
struct var<T>: public var<typename cpp_type_info<T>::NuTypeRef> {};

template <NuBuiltinType Type>
struct var<Type> {
    using T = typename Type::cpp_type;
    T val = T();

    var() = default;
    var(T val): val(val) {}

    template <valid_cpp_type_info With> requires requires { T() + With(); }
    constexpr auto operator + (With&& t) const {
        using Ret = decltype(val + t);
        static_assert(valid_cpp_type_info<Ret>, "every type used must be valid cpp type info");
        return cpp_type_info<Ret>::NuTypeVal(val + t);
    }
    template <valid_cpp_type_info With> requires requires { T() + With(); }
    constexpr auto operator + (var<With>& t) const {
        using Ret = decltype(val + t.val);
        static_assert(valid_cpp_type_info<Ret>, "every type used must be valid cpp type info");
        return cpp_type_info<Ret>::NuTypeVal(val + t.val);
    }

};
template <typename T>
var(T&&) -> var<typename cpp_type_info<T>::NuTypeRef>;

template <>
struct var<NuType<"Type">> {
    using T = NuType<"Type">::cpp_type;
    T val = T();

    var() = default;
    var(T val): val(val) {}
    template <NuType_t Type>
    var(const Type& t): val(t) {}

    operator const nutype_address* () const {
        return val;
    }
    const typeinfo_getter* operator -> () const {
        return val.type;
    }

};

template <NuType_t T>
var(const T&) -> var<NuType<"Type">>;
