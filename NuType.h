#pragma once
#include "mystd.h"

struct nutype_address {
};
template <size_t id>
struct NuType;

struct typeinfo_getter {
    virtual constexpr const char* Name() const = 0;
    virtual constexpr size_t ID() const = 0;
    virtual constexpr operator const nutype_address* () const = 0;

    /*
    virtual constexpr void Init(void* into) const = 0;
    virtual constexpr void _delete(void *data) const = 0;
    virtual constexpr void* _new() const = 0;
    virtual constexpr void* _cpy(void* data) const = 0;
    */
};
template <typename T>
struct typeinfo_getter_of: public typeinfo_getter {
    constexpr const char *Name() const override {
        return T::Name;
    }

    constexpr size_t ID() const override {
        return T::ID;
    }

    constexpr operator const nutype_address *() const override {
        return &T::Self;
    }
/*
    constexpr void Init(void *into) const override {
        //*(typename T::t*)into = new typename T::t;
    }
    constexpr void* _new() const override {
        return new typename T::t;
    }
    constexpr void* _cpy(void* data) const override {
        if (!data) return nullptr;
        using Type = typename T::t;
        Type* elem = (Type*)_new();
        *elem = *(Type*)data;
        return elem;
    }
    constexpr void _delete(void *data) const override {
        delete (typename T::t*)data;
    }
    */
};
/*
template <>
struct typeinfo_getter_of<typeinfo_getter>: nutype_address, public typeinfo_getter {
    constexpr const char *Name() const override {
        return "nil";
    }

    constexpr size_t ID() const override {
        return size_t(-1);
    }

    constexpr operator const nutype_address *() const override {
        return this;
    }

    constexpr void Init(void *into) const override {
        //*(typename T::t*)into = new typename T::t;
    }
    constexpr void* _new() const override {
        return nullptr;
    }
    constexpr void* _cpy(void* data) const override {
        return nullptr;
    }
    constexpr void _delete(void *data) const override {
        void;
    }
};

constexpr const auto DynamicNuType = typeinfo_getter_of<typeinfo_getter>();
constinit const typeinfo_getter& typeinfo_getter::Self = DynamicNuType;
*/

template <typename T>
concept NuType_t = requires (const std::just_t<T>& Elem){
    typename std::just_t<T>::t;
    { Elem.Name } -> std::has_implicit_cast_to<const char*>;
    { Elem.ID } -> std::is_similar_to<std::size_t>;
    { Elem.IsBuiltin } -> std::is_similar_to<bool>;
    { Elem.Self } -> std::same_as<const std::just_t<T>&>;
    { Elem.TypeVal } -> std::same_as<const typeinfo_getter_of<std::just_t<T>> &>;
    //...
};

template <typename T>
concept NuBuiltinType = NuType_t<T> && requires {
    T::IsBuiltin;
    typename T::cpp_type;
};
template <typename T>
concept NuSimpleType = NuType_t<T> && requires {
    !T::IsBuiltin;
    typename T::BuiltinType;
    typename T::OriginType;
};

template <typename T>
struct cpp_type_info;
template <typename T>
concept valid_cpp_type_info = requires {
    typename cpp_type_info<std::just_t<T>>;
    { cpp_type_info<std::just_t<T>>::NuTypeVal } -> NuType_t;
    typename cpp_type_info<std::just_t<T>>::NuTypeRef;
    NuType_t<typename cpp_type_info<std::just_t<T>>::NuTypeRef>;
};