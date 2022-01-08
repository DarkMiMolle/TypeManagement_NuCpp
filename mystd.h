#pragma once
#include <type_traits>
#include <concepts>
#include <vector>
#include <array>
#include <tuple>
#include <variant>
#include <optional>
#include <algorithm>

namespace std {
    template <typename T>
    using just_t = remove_cvref_t<T>;
    template<typename T1, typename T2>
    concept are_similar = is_same_v<remove_cvref_t<T1>, remove_cvref_t<T2>>;

    template<typename T1, typename T2>
    concept is_similar_to = are_similar<T1, T2>;

    template <typename T1, typename T2>
    concept has_implicit_cast_to = requires (T1& from, T2& to){
        to = from;
    };

    template<size_t N>
    struct StringLiteral {
        constexpr StringLiteral(const char (&str)[N]) {
            copy_n(str, N, value);
        }
    
        char value[N];
    };

}
