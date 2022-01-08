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
constexpr bool operator == (const NuType<#name>& t) const { \
    return &t == this; \
}                                    \
constexpr bool operator == (const nutype_address* t) const { \
    return t == this; \
}\
static const NuType<#name>& Self;       \
inline static constexpr const typeinfo_getter_of<NuType<#name>> TypeVal = {};

#define BuiltinNuTypeOuter(NuName, name) \
static constexpr const NuType<#name> NuName = NuType<#name>(); \
const NuType<#name>& NuType<#name>::Self = ::NuName; \
template <> \
struct cpp_type_info<NuType<#name>::t> { \
    using NuTypeRef = NuType<#name>; \
    inline static constexpr auto& NuTypeVal = NuName; \
};                                     \

template <>
struct NuType<"int">: nutype_address {
    using t = int;
    inline static constexpr auto Name = "int";

    inline static constexpr size_t ID = 1;
    inline static constexpr auto IsBuiltin = true;
    using cpp_type = int;

    constexpr int operator() (int&& val = 0) const {
        return val;
    }
    constexpr bool operator == (const NuType<"int">& t) const {
        return &t == this;
    }
    constexpr bool operator == (const nutype_address* t) const {
        return t == this;
    }

    static const NuType<"int">& Self;

    inline static constexpr const typeinfo_getter_of<NuType<"int">> TypeVal = {};
};
constexpr const NuType<"int"> Int = NuType<"int">();
const NuType<"int">& NuType<"int">::Self = Int;
template <>
struct cpp_type_info<int> {
    using NuTypeRef = NuType<"int">;
    inline static constexpr auto& NuTypeVal = Int;
};

template<>
struct NuType<"Type">: nutype_address {
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
BuiltinNuTypeOuter(Type, Type)
constexpr NuType<"Type">::Type::Type(): type(&NuType<"Type">::TypeVal){}
