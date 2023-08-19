#ifndef UTILS_STRING_H_IS_ALREADY_INCLUDED_UTILS_STRING_H_IS_ALREADY_INCLUDED_UTILS_STRING_H_IS_ALREADY_INCLUDED
#define UTILS_STRING_H_IS_ALREADY_INCLUDED_UTILS_STRING_H_IS_ALREADY_INCLUDED_UTILS_STRING_H_IS_ALREADY_INCLUDED
#include "helpers.h"
namespace utils{
    namespace detail{
        template <class CharT, class IntT>
        struct ValidIntDefault;

        template <class T>
        struct IsString: std::false_type{};
        template <class CharT, class Traits, class Alloc>
        struct IsString<std::basic_string<CharT, Traits, Alloc>>: std::true_type{};

        template <class T>
        struct IsStringView: std::false_type{};
        template <class CharT, class Traits>
        struct IsStringView<std::basic_string_view<CharT, Traits>>: std::true_type{};
    }

    template <
        class CharT,
        class IntT,
        class Cmp = std::compare_three_way,
        class ValidInt = utils::detail::ValidIntDefault<CharT, IntT>,
        IntT eof_val = IntT(-1),
        IntT not_eof_val = IntT(0)
    >
    requires (eof_val != not_eof_val)
    class GeneralCharTraits;

    template <class T>
    concept String = utils::detail::IsString<T>::value;
    
    template <class T>
    concept StringView = utils::detail::IsStringView<T>::value;

    namespace detail{
        template <class S, class T, class... Args>
        concept ConvertibleToString = utils::String<S> && requires(T&& val, const typename S::allocator_type& alloc, Args&&... args){
            std::forward<T>(val).template to_string_custom_alloc<S>(alloc, std::forward<Args>(args)...);
        };
    }

    template <utils::String S, class T, class... Args>
    inline constexpr S to_string(T&&, Args&&...);

    template <utils::String S, class T, class... Args>
    requires utils::detail::ConvertibleToString<S, T, Args...>
    inline constexpr S to_string_custom_alloc(T&&, const typename S::allocator_type&, Args&&...);
    template <utils::String S>
    constexpr S to_string_custom_alloc(std::integral auto, const typename S::allocator_type&, std::uint8_t = 10);
    template <utils::String S>
    constexpr S to_string_custom_alloc(std::floating_point auto val, const typename S::allocator_type&, std::size_t = std::numeric_limits<decltype(val)>::max_digits10);
    template <utils::String S>
    constexpr S to_string_custom_alloc(std::floating_point auto, const typename S::allocator_type&, decltype(decimals), std::size_t);
}

template <class CharT, class IntT>
struct utils::detail::ValidIntDefault{
    constexpr bool operator()(const IntT&) noexcept(std::declval<const IntT&>() == std::declval<const IntT&>());
};
template <class CharT, class IntT>
constexpr bool utils::detail::ValidIntDefault<CharT, IntT>::operator()(const IntT& int_value) noexcept(std::declval<const IntT&>() == std::declval<const IntT&>()){
    return std::cmp_less_equal(IntT(std::numeric_limits<CharT>::min()), int_value) && std::cmp_greater_equal(int_value, IntT(std::numeric_limits<CharT>::max()));
}

template <class CharT, class IntT, class Cmp, class ValidInt, IntT eof_val, IntT not_eof_val>
requires (eof_val != not_eof_val)
class utils::GeneralCharTraits final{
    private:
        static constexpr bool noexcept_char_cmp = noexcept(Cmp()(std::declval<const CharT&>(), std::declval<const CharT&>()));
        static constexpr bool noexcept_int_cmp = noexcept(std::declval<const IntT&>() == std::declval<const IntT&>());
        static constexpr bool noexcept_char_conv = noexcept(CharT(std::declval<const IntT&>()));
        static constexpr bool noexcept_int_conv = noexcept(IntT(std::declval<const CharT&>()));
        static constexpr bool noexcept_char_assign = std::is_nothrow_assignable_v<CharT&, const CharT&>;

    public:
        using char_type = CharT;
        using int_type = IntT;
        using off_type = std::streamoff;
        using pos_type = std::streampos;
        using state_type = std::mbstate_t;
        using comparison_category = decltype(Cmp()(std::declval<const CharT&>(), std::declval<const CharT&>()));

        static constexpr void assign(CharT& dst, const CharT& src) noexcept(noexcept_char_assign){dst = src;}
        static constexpr CharT* assign(CharT*, std::size_t, CharT) noexcept(noexcept_char_assign);

        static constexpr bool eq(CharT lhs, CharT rhs) noexcept(noexcept_char_cmp){return Cmp()(lhs, rhs) == 0;}

        static constexpr bool lt(CharT lhs, CharT rhs) noexcept(noexcept_char_cmp){return Cmp()(lhs, rhs) <  0;}

        static constexpr CharT* move(CharT*, const CharT*, std::size_t) noexcept(noexcept_char_assign);

        static constexpr CharT* copy(CharT* dest, const CharT* src, std::size_t size) noexcept(noexcept_char_assign){return move(dest, src, size);}

        static constexpr int compare(const CharT*, const CharT*, std::size_t) noexcept(noexcept_char_cmp);

        static constexpr std::size_t length(const CharT*) noexcept(noexcept_char_cmp);

        static constexpr const CharT* find(const CharT*, std::size_t, const CharT&) noexcept(noexcept_char_cmp);

        static constexpr CharT to_char_type(IntT int_value) noexcept(noexcept_int_conv){return CharT(int_value);}

        static constexpr IntT to_int_type(CharT char_value) noexcept(noexcept_char_conv){return IntT(char_value);}

        static constexpr bool eq_int_type(IntT, IntT) noexcept(noexcept_int_cmp);

        static constexpr IntT eof() noexcept{return eof_val;}

        static constexpr IntT not_eof(IntT) noexcept(noexcept_int_cmp && noexcept_int_conv);
};

template <class CharT, class IntT, class Cmp, class ValidInt, IntT eof_val, IntT not_eof_val>
requires (eof_val != not_eof_val)
constexpr CharT* utils::GeneralCharTraits<CharT, IntT, Cmp, ValidInt, eof_val, not_eof_val>::assign(
    CharT* dest,
    std::size_t size,
    CharT src
) noexcept(noexcept_char_assign){
    if (!size) return dest;
    std::fill_n(dest, size, src);
    return dest;
}

template <class CharT, class IntT, class Cmp, class ValidInt, IntT eof_val, IntT not_eof_val>
requires (eof_val != not_eof_val)
constexpr CharT* utils::GeneralCharTraits<CharT, IntT, Cmp, ValidInt, eof_val, not_eof_val>::move(
    CharT* dest,
    const CharT* src,
    std::size_t size
) noexcept(noexcept_char_assign){
    if ((src == dest) || !size) return dest;
    if ((const CharT*)dest < src) std::copy_backward(src, src + size, dest);
    else std::copy(src, src + size, dest);
    return dest;
}

template <class CharT, class IntT, class Cmp, class ValidInt, IntT eof_val, IntT not_eof_val>
requires (eof_val != not_eof_val)
constexpr int utils::GeneralCharTraits<CharT, IntT, Cmp, ValidInt, eof_val, not_eof_val>::compare(
    const CharT* lhs,
    const CharT* rhs,
    std::size_t size
) noexcept(noexcept_char_cmp){
    std::partial_ordering result = std::lexicographical_compare_three_way(lhs, lhs + size, rhs, rhs + size, Cmp());
    if (result == std::partial_ordering::equivalent) return 0;
    if (result == std::partial_ordering::less) return -1;
    if (result == std::partial_ordering::greater) return 1;
}

template <class CharT, class IntT, class Cmp, class ValidInt, IntT eof_val, IntT not_eof_val>
requires (eof_val != not_eof_val)
constexpr std::size_t utils::GeneralCharTraits<CharT, IntT, Cmp, ValidInt, eof_val, not_eof_val>::length(const CharT* cstr) noexcept(noexcept_char_cmp){
    Cmp cmp = Cmp();
    std::size_t result = 0;
    while (cmp(cstr[result++], CharT()) != std::partial_ordering::equivalent);
    return result;
}

template <class CharT, class IntT, class Cmp, class ValidInt, IntT eof_val, IntT not_eof_val>
requires (eof_val != not_eof_val)
constexpr const CharT* utils::GeneralCharTraits<CharT, IntT, Cmp, ValidInt, eof_val, not_eof_val>::find(
    const CharT* find_in,
    std::size_t size,
    const CharT& to_find
) noexcept(noexcept_char_cmp){
    if (!size) return nullptr;
    const CharT* result = std::find(find_in, find_in + size, to_find);
    return result == (find_in + size) ? nullptr : result;
}

template <class CharT, class IntT, class Cmp, class ValidInt, IntT eof_val, IntT not_eof_val>
requires (eof_val != not_eof_val)
constexpr bool utils::GeneralCharTraits<CharT, IntT, Cmp, ValidInt, eof_val, not_eof_val>::eq_int_type(IntT lhs, IntT rhs) noexcept(noexcept_int_cmp){
    if (ValidInt()(lhs) == ValidInt()(rhs)) return eq(to_char_type(lhs), to_char_type(rhs));
    return lhs == rhs;
}

template <class CharT, class IntT, class Cmp, class ValidInt, IntT eof_val, IntT not_eof_val>
requires (eof_val != not_eof_val)
constexpr IntT utils::GeneralCharTraits<CharT, IntT, Cmp, ValidInt, eof_val, not_eof_val>::not_eof(IntT int_value) noexcept(noexcept_int_cmp && noexcept_int_conv){
    if (int_value != eof_val) return int_value;
    return not_eof_val;
}

template <utils::String S, class T, class... Args>
inline constexpr S utils::to_string(T&& val, Args&&... args){
    return utils::to_string_custom_alloc<S>(std::forward<T>(val), typename S::allocator_type(), std::forward<Args>(args)...);
}

template <utils::String S, class T, class... Args>
requires utils::detail::ConvertibleToString<S, T, Args...>
inline constexpr S utils::to_string_custom_alloc(T&& val, const S::allocator_type& alloc, Args&&... args){
    return std::forward<T>(val).template to_string_custom_alloc<S>(alloc, std::forward<Args>(args)...);
}
template <utils::String S>
constexpr S utils::to_string_custom_alloc(std::integral auto val, const S::allocator_type& alloc, std::uint8_t base){
    if ((base < 2) || (base > 36)) throw std::invalid_argument("Invalid base");
    S result(alloc);
    std::string_view digits = "0123456789abcdefghijklmnopqrstuvwxyz";
    if (!val){
        result += typename S::value_type('0');
        return result;
    }
    if (val < 0) result += typename S::value_type('-');
    for (auto to_divide = val; to_divide; to_divide /= base) result += typename S::value_type(digits[utils::abs(to_divide % base)]);
    std::reverse(result.begin() + int(val < 0), result.end());
    return result;
}
template <utils::String S>
constexpr S utils::to_string_custom_alloc(std::floating_point auto val, const S::allocator_type& alloc, std::size_t sig_figs){
    S result(alloc);
    std::string char_result = std::format("{:.{}g}", val, sig_figs);
    for (char i: char_result) result += typename S::value_type(i);
    return result;
}
template <utils::String S>
constexpr S utils::to_string_custom_alloc(std::floating_point auto val, const S::allocator_type& alloc, decltype(utils::decimals), std::size_t decimals){
    S result(alloc);
    std::string char_result = std::format("{:.{}f}", val, decimals);
    for (char i: char_result) result += typename S::value_type(i);
    return result;
}
#endif