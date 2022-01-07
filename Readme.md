# TypeManagement_NuCpp
A first idea of how i will manage type creation and type usage in my Nu language (that will be translated in cpp)

## How to use it ?
Every type we want to record must respect the concept `NuType_t` (define in _NuType.h_).
The best way to do it is to specify the template structure: `NuType<size_t>`. Then we shall specify the template structure: `cpp_type_info<type T>` to make a link between RValue of C++ and the type for Nu.

<u>Let's see an example:</u>

```c++
#include "NuType.h"

template <>
struct NuType<42> : nutype_address {
    struct Complex {
        float re, im;
    };
    using t = Complex;
    inline static constexpr auto Name = "Complex";

    inline static constexpr size_t ID = 42;
    inline static constexpr auto IsBuiltin = false;

    constexpr Complex operator() (float&& re = 0, float&& im = 0) const {
        return Complex{re, im};
    }
    
    constexpr bool operator == (const NuType<42>& t) const {
        return &t == this;
    }
    constexpr bool operator == (const nutype_address* t) const {
        return t == this;
    }

    static const NuType<42>& Self;

    inline static constexpr const typeinfo_getter_of<NuType<42>> TypeVal = {};
};
constexpr const NuType<42> Complex = NuType<42>();
const NuType<42>& NuType<42>::Self = Complex;
template <>
struct cpp_type_info<NuType<42>::t> {
    using NuTypeRef = NuType<42>;
    inline static constexpr auto& NuTypeVal = Complex;
};


```