#pragma once
#include "NuType.h"
#include "Var.h"
#include "Typeof.h"
#include <functional>

using NuTypeAny = NuType<size_t(-1)>;

struct Any;

template <>
struct typeinfo_getter_of<NuTypeAny>: public typeinfo_getter {
    const NuTypeAny* typeAny;
    typeinfo_getter_of(const NuTypeAny* t): typeAny(t) {}
    const char *Name() const override;

    size_t ID() const override;

    operator const nutype_address *() const override;
};

template <>
struct NuType<size_t(-1)>: nutype_address {
    const typeinfo_getter* type = &Type.TypeVal;

    using t = Any;

    const char* Name = type->Name();
    size_t ID = type->ID();
    inline static constexpr bool IsBuiltin = false;

    const NuType<size_t(-1)>& Self = *this;
    const typeinfo_getter_of<NuType<size_t(-1)>> TypeVal = {this};

    NuType() = default;
    NuType(const typeinfo_getter* type): type(type) {}
    void operator = (const typeinfo_getter* t) {
        type = t;
        Name = type->Name();
        ID = type->ID();
    }
};

const char* typeinfo_getter_of<NuTypeAny>::Name() const {
    return typeAny->Name;
}
size_t typeinfo_getter_of<NuTypeAny>::ID() const {
    return typeAny->ID;
}
typeinfo_getter_of<NuTypeAny>::operator const nutype_address *() const {
    return typeAny;
}

struct Any {
    NuTypeAny type;
    void* value = nullptr;
    std::function<void(void*&)> _delete;

    Any() = default;

    template <valid_cpp_type_info T>
    explicit Any(T&& elem):
        value(new T(elem)),
        type(&cpp_type_info<std::just_t<T>>::NuTypeVal.TypeVal),
        _delete([](void*& value) { delete (T*)value; value = nullptr; })
    {}
    Any(Any&& mv): value(mv.value), type(mv.type), _delete(move(mv._delete)) {
        mv.value = nullptr;
        mv.type = &Type.TypeVal;
    }
//    Any(const Any& cpy): value(cpy.type->_cpy(cpy.value)), type(cpy.type) {}
    ~Any() {
        //type->_delete(value);
    }
    template <NuType_t Type> // requires requires { !Type::IsDynamicType; }
    void PointTo(const Type&) {
        _delete(value);
        value = new typename Type::t;
        _delete = [](void*& value) {
            delete (typename Type::t*)value;
            value = nullptr;
        };
        type = &Type::TypeVal;
    }
    template <valid_cpp_type_info Type>
    void PointTo(Type&& val) {
        value = new Type(val);
        _delete = [](void*& value) {
            delete (Type*)value;
            value = nullptr;
        };
        type = &cpp_type_info<Type>::TypeValue.TypeVal;
    }
};

template<>
struct var<Any> {
    Any val;
    var() = default;
    var(Any&& elem): val(std::move(elem)) {}

    Any* operator -> () {
        return &val;
    }
};

const NuTypeAny& Typeof(var<Any>& v) {
    return v->type;
}

//TODO: var<NuTypeAny> --> auto / typeof(any) / typeof(auto) --> fixed dynamic type