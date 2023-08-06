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
#include <optional>
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
#include <variant>
#include <vector>
namespace utils{
    namespace detail{
        template <class T>
        constexpr T pow(const T&, std::ptrdiff_t);
        template <class T>
        constexpr T pow(const T&, std::size_t);
        template <class CharT, class Traits, class Alloc, class T, class... Args>
        concept ConvertibleToString = requires(const T& val, Args&&... args){
            val.template to_string<CharT, Traits, Alloc>(std::forward<Args>(args)...);
        };
    }

    namespace literals{
        using namespace std::literals;
    }

    template <class T>
    constexpr T abs(const T& arg);

    enum{decimals};
    
    template <class CharT, class Traits = std::char_traits<CharT>, class Alloc = std::allocator<CharT>, class T, class... Args>
    inline constexpr std::basic_string<CharT, Traits, Alloc> to_string(const T&, Args&&...);

    template <class CharT, class Traits = std::char_traits<CharT>, class Alloc = std::allocator<CharT>, class T, class... Args>
    requires utils::detail::ConvertibleToString<T, CharT, Traits, Alloc, Args...>
    inline constexpr std::basic_string<CharT, Traits, Alloc> to_string_custom_alloc(const T&, const Alloc&, Args&&...);
    template <class CharT, class Traits = std::char_traits<CharT>, class Alloc = std::allocator<CharT>, std::integral T>
    inline constexpr std::basic_string<CharT, Traits, Alloc> to_string_custom_alloc(const T&, const Alloc&);
    template <class CharT, class Traits = std::char_traits<CharT>, class Alloc = std::allocator<CharT>, std::integral T>
    constexpr std::basic_string<CharT, Traits, Alloc> to_string_custom_alloc(const T&, const Alloc&, std::uint8_t);
    template <class CharT, class Traits = std::char_traits<CharT>, class Alloc = std::allocator<CharT>, std::floating_point T>
    inline constexpr std::basic_string<CharT, Traits, Alloc> to_string_custom_alloc(const T&, const Alloc&);
    template <class CharT, class Traits = std::char_traits<CharT>, class Alloc = std::allocator<CharT>, std::floating_point T>
    constexpr std::basic_string<CharT, Traits, Alloc> to_string_custom_alloc(const T&, const Alloc&, std::size_t);
    template <class CharT, class Traits = std::char_traits<CharT>, class Alloc = std::allocator<CharT>, std::floating_point T>
    constexpr std::basic_string<CharT, Traits, Alloc> to_string_custom_alloc(const T&, const Alloc&, decltype(decimals), std::size_t);
}

template <class T>
constexpr T utils::detail::pow(const T& lhs, std::ptrdiff_t rhs){
    if (rhs >= 0) return utils::detail::pow<T>(lhs, std::size_t(rhs));
    return 1 / utils::detail::pow<T>(lhs, std::size_t(-rhs));
}
template <class T>
constexpr T utils::detail::pow(const T& lhs, std::size_t rhs){
    // T::operator*= must be an associative operator
    if (!rhs) return T(1);
    if (lhs == T()) return lhs;
    if (rhs == 1) return lhs;
    // https://en.wikipedia.org/wiki/Exponentiation_by_squaring
    T result = T(1), to_multiply = lhs;
    while (true){
        if (rhs % 2) result *= to_multiply;
        rhs /= 2;
        if (!rhs) break; // to prevent to_multiply from getting too large
        to_multiply *= to_multiply;
    }
    return result;
}

template <class T>
constexpr T utils::abs(const T& arg){
    if (arg < 0) return -arg;
    return arg;
}

template <class CharT, class Traits, class Alloc, class T, class... Args>
inline constexpr std::basic_string<CharT, Traits, Alloc> utils::to_string(const T& val, Args&&... args){
    return to_string_custom_alloc<CharT, Traits, Alloc, T>(val, Alloc(), std::forward<Args>(args)...);
}

template <class CharT, class Traits, class Alloc, class T, class... Args>
requires utils::detail::ConvertibleToString<T, CharT, Traits, Alloc, Args...>
inline constexpr std::basic_string<CharT, Traits, Alloc> utils::to_string_custom_alloc(const T& val, const Alloc& alloc, Args&&... args){
    return val.template to_string_custom_alloc<CharT, Traits, Alloc>(alloc, std::forward<Args>(args)...);
}
template <class CharT, class Traits, class Alloc, std::integral T>
inline constexpr std::basic_string<CharT, Traits, Alloc> utils::to_string_custom_alloc(const T& val, const Alloc& alloc){
    return utils::to_string_custom_alloc<CharT, Traits, Alloc, T>(val, alloc, 10);
}
template <class CharT, class Traits, class Alloc, std::integral T>
constexpr std::basic_string<CharT, Traits, Alloc> utils::to_string_custom_alloc(const T& val, const Alloc& alloc, std::uint8_t base){
    if ((base < 2) || (base > 36)) throw std::invalid_argument("Invalid base");
    std::basic_string<CharT, Traits, Alloc> result(alloc);
    std::string_view digits = "0123456789abcdefghijklmnopqrstuvwxyz";
    if (!val){
        result += CharT('0');
        return result;
    }
    if (val < 0) result += CharT('-');
    for (T to_divide = val; to_divide; to_divide /= base) result += CharT(digits[utils::abs(to_divide % base)]);
    std::reverse(result.begin() + int(val < 0), result.end());
    return result;
}
template <class CharT, class Traits, class Alloc, std::floating_point T>
inline constexpr std::basic_string<CharT, Traits, Alloc> utils::to_string_custom_alloc(const T& val, const Alloc& alloc){
    return utils::to_string_custom_alloc<CharT, Traits, Alloc, T>(val, alloc, std::numeric_limits<T>::digits10());
}
template <class CharT, class Traits, class Alloc, std::floating_point T>
constexpr std::basic_string<CharT, Traits, Alloc> utils::to_string_custom_alloc(const T& val, const Alloc& alloc, std::size_t sig_figs){
    std::basic_string<CharT, Traits, Alloc> result(alloc);
    std::string char_result = std::format("{:.{}g}", val, sig_figs);
    for (char i: char_result) result += CharT(i);
    return result;
}
template <class CharT, class Traits, class Alloc, std::floating_point T>
constexpr std::basic_string<CharT, Traits, Alloc> utils::to_string_custom_alloc(const T& val, const Alloc& alloc, decltype(utils::decimals), std::size_t decimals){
    std::basic_string<CharT, Traits, Alloc> result(alloc);
    std::string char_result = std::format("{:.{}f}", val, decimals);
    for (char i: char_result) result += CharT(i);
    return result;
}

template <class InputIt, class Compare>
constexpr void utils::sort(InputIt first, InputIt last, Compare cmp){
    std::vector<typename std::iterator_traits<InputIt>::value_type> to_sort;
    std::move(first, last, std::back_inserter(to_sort));
    std::stable_sort(to_sort.begin(), to_sort.end(), cmp);
    std::move(to_sort.begin(), to_sort.end(), first);
}
#endif
