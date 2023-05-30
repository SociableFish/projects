#ifndef UTILS_HELPERS_H_IS_ALREADY_INCLUDED_UTILS_HELPERS_H_IS_ALREADY_INCLUDED_UTILS_HELPERS_H_IS_ALREADY_INCLUDED
#define UTILS_HELPERS_H_IS_ALREADY_INCLUDED_UTILS_HELPERS_H_IS_ALREADY_INCLUDED_UTILS_HELPERS_H_IS_ALREADY_INCLUDED
#include <algorithm>
#include <array>
#include <charconv>
#include <cmath>
#include <compare>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <format>
#include <forward_list>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <stack>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
namespace utils{
    namespace detail{
        template <class T>
        struct Wrapper{using type = T;};
        template <class T>
        constexpr T pow(const T&, std::ptrdiff_t);
        template <class T>
        constexpr T pow(const T&, std::size_t);
    }
    template <class T>
    inline constexpr T abs(const T& arg){return std::max(arg, -arg);}
    template <std::unsigned_integral T>
    inline constexpr T abs(const T& arg){return arg;}
}
template <class T>
constexpr T utils::detail::pow(const T& lhs, std::ptrdiff_t rhs){
    if (rhs >= 0) return utils::detail::pow<T>(lhs, std::size_t(rhs));
    return 1 / utils::detail::pow<T>(lhs, std::size_t(-rhs));
}
template <class T>
constexpr T utils::detail::pow(const T& lhs, std::size_t rhs){
    if (!rhs) return T(1);
    if (lhs == T()) return lhs;
    if (rhs == 1) return lhs;
    T result = T(), to_multiply = lhs;
    while (true){
        if (rhs % 2) result *= to_multiply;
        rhs /= 2;
        if (!rhs) break; // to prevent to_multiply from getting too large
        to_multiply *= to_multiply;
    }
    return result;
}
#endif