#pragma once
#include "NuType.h"

#define BuiltinNuTypeInner(name, id) \
using t = name; \
inline static constexpr auto Name = #name; \
inline static constexpr size_t ID = id; \
inline static constexpr auto IsBuiltin = true;\
using cpp_type = name; \
 \
constexpr name operator() (name&& val = {}) const { \
    return val; \
} \
constexpr bool operator == (const NuType<id>& t) const { \
    return &t == this; \
}                                    \
constexpr bool operator == (const nutype_address* t) const { \
    return t == this; \
}\
static const NuType<id>& Self;       \
inline static constexpr const typeinfo_getter_of<NuType<id>> TypeVal = {};

#define BuiltinNuTypeOuter(NuName, id) \
static constexpr const NuType<id> NuName = NuType<id>(); \
const NuType<id>& NuType<id>::Self = ::NuName; \
template <> \
struct cpp_type_info<NuType<id>::t> { \
    using NuTypeRef = NuType<id>; \
    inline static constexpr auto& NuTypeVal = NuName; \
};                                     \

template <>
struct NuType<1>: nutype_address {
    using t = int;
    inline static constexpr auto Name = "int";

    inline static constexpr size_t ID = 1;
    inline static constexpr auto IsBuiltin = true;
    using cpp_type = int;

    constexpr int operator() (int&& val = 0) const {
        return val;
    }
    constexpr bool operator == (const NuType<1>& t) const {
        return &t == this;
    }
    constexpr bool operator == (const nutype_address* t) const {
        return t == this;
    }

    static const NuType<1>& Self;

    inline static constexpr const typeinfo_getter_of<NuType<1>> TypeVal = {};
};
constexpr const NuType<1> Int = NuType<1>();
const NuType<1>& NuType<1>::Self = Int;
template <>
struct cpp_type_info<int> {
    using NuTypeRef = NuType<1>;
    inline static constexpr auto& NuTypeVal = Int;
};

template<>
struct NuType<0>: nutype_address {
    struct Type {
        const typeinfo_getter* type;
        constexpr Type();
        template <NuType_t T>
        constexpr Type(const T&): type(&T::TypeVal) {}
        //Type(const typeinfo_getter* type): type(type) {}

        constexpr const typeinfo_getter* operator -> () const {
            return type;
        }
        constexpr operator const nutype_address* () const {
            return *type;
        }
    };

    BuiltinNuTypeInner(Type, 0)
};
BuiltinNuTypeOuter(Type, 0)
constexpr NuType<0>::Type::Type(): type(&NuType<0>::TypeVal){}
