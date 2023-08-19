#ifndef UTILS_FRACTION_H_IS_ALREADY_INCLUDED_UTILS_FRACTION_H_IS_ALREADY_INCLUDED_UTILS_FRACTION_H_IS_ALREADY_INCLUDED
#define UTILS_FRACTION_H_IS_ALREADY_INCLUDED_UTILS_FRACTION_H_IS_ALREADY_INCLUDED_UTILS_FRACTION_H_IS_ALREADY_INCLUDED
#include "integer.h"
namespace utils{
    class Fraction;

    namespace detail{
        const std::string fraction_chrs = "0123456789+-.'/eE";
        const utils::Integer one = 1; // exists so that utils::Fraction::denominator can always return const utils::Integer&
        
        template <class T>
        concept FloatFractionComparable = std::floating_point<T> && std::convertible_to<T, utils::Fraction>;
    }

    inline constexpr utils::Fraction operator+(const utils::Fraction&, const utils::Fraction&);
    inline constexpr utils::Fraction operator+(const utils::Fraction&, const std::convertible_to<utils::Fraction> auto&);
    inline constexpr utils::Fraction operator+(const std::convertible_to<utils::Fraction> auto&, const utils::Fraction&);
    inline constexpr utils::Fraction operator+(const utils::Fraction&);

    inline constexpr utils::Fraction operator-(const utils::Fraction&, const utils::Fraction&);
    inline constexpr utils::Fraction operator-(const utils::Fraction&, const std::convertible_to<utils::Fraction> auto&);
    inline constexpr utils::Fraction operator-(const std::convertible_to<utils::Fraction> auto&, const utils::Fraction&);
    inline constexpr utils::Fraction operator-(const utils::Fraction&);

    inline constexpr utils::Fraction operator*(const utils::Fraction&, const utils::Fraction&);
    inline constexpr utils::Fraction operator*(const utils::Fraction&, const std::convertible_to<utils::Fraction> auto&);
    inline constexpr utils::Fraction operator*(const std::convertible_to<utils::Fraction> auto&, const utils::Fraction&);

    inline constexpr utils::Fraction operator/(const utils::Fraction&, const utils::Fraction&);
    inline constexpr utils::Fraction operator/(const utils::Fraction&, const std::convertible_to<utils::Fraction> auto&);
    inline constexpr utils::Fraction operator/(const std::convertible_to<utils::Fraction> auto&, const utils::Fraction&);

    inline constexpr bool operator==(const utils::Fraction&, const utils::Fraction&) noexcept;
    inline constexpr bool operator==(const utils::Fraction&, const std::convertible_to<utils::Fraction> auto&);
    inline constexpr bool operator==(const std::convertible_to<utils::Fraction> auto&, const utils::Fraction&);
    constexpr bool        operator==(const utils::Fraction&, utils::detail::FloatFractionComparable auto);
    constexpr bool        operator==(utils::detail::FloatFractionComparable auto, const utils::Fraction&);

    inline constexpr std::strong_ordering  operator<=>(const utils::Fraction&, const utils::Fraction&);
    inline constexpr std::strong_ordering  operator<=>(const utils::Fraction&, const std::convertible_to<utils::Fraction> auto&);
    constexpr std::partial_ordering        operator<=>(const utils::Fraction&, utils::detail::FloatFractionComparable auto);

    inline constexpr utils::Integer trunc(const utils::Fraction&);

    constexpr utils::Integer floor(const utils::Fraction&);

    constexpr utils::Integer ceil(const utils::Fraction&);

    constexpr utils::Integer round(const utils::Fraction&);

    inline constexpr utils::Fraction pow(const utils::Fraction&, std::integral auto);
}

template <std::convertible_to<utils::Fraction> T>
struct std::common_type<utils::Fraction, T>: std::type_identity<utils::Fraction>{};
template <std::convertible_to<utils::Fraction> T>
struct std::common_type<T, utils::Fraction>: std::type_identity<utils::Fraction>{};

template <>
struct std::hash<utils::Fraction>{constexpr std::size_t operator()(const utils::Fraction&) const noexcept;};

/**
 * @brief A type that represents a fraction of 2 @c utils::Integer values
 */
class utils::Fraction{
    public:
        constexpr Fraction() = default;
        constexpr Fraction(const Fraction&) = default;
        constexpr Fraction(Fraction&&) noexcept = default;

        template <std::integral T>
        constexpr Fraction(T number): numerator_var(number), denominator_var_0_eq_1(){}
        template <std::floating_point T>
        constexpr Fraction(T number){from_num(number);}
        constexpr Fraction(const utils::Integer& number): numerator_var(number), denominator_var_0_eq_1(){}
        constexpr Fraction(utils::Integer&& number) noexcept: numerator_var(number), denominator_var_0_eq_1(){}

        constexpr Fraction(const utils::Integer&, const utils::Integer&);

        template <class CharT, class Traits = std::char_traits<CharT>>
        constexpr explicit Fraction(const CharT* cstr): Fraction(std::basic_string_view<CharT, Traits>(cstr)){}
        constexpr explicit Fraction(std::nullptr_t) = delete;
        template <utils::String S>
        constexpr explicit Fraction(const S& str): Fraction(std::basic_string_view<typename S::value_type, typename S::traits_type>(str)){}
        template <utils::StringView SV>
        constexpr explicit Fraction(SV);

        constexpr Fraction& operator=(const Fraction&) = default;
        constexpr Fraction& operator=(Fraction&&) noexcept = default;

        constexpr const utils::Integer& numerator() const noexcept{return numerator_var;}
        constexpr const utils::Integer& denominator() const noexcept{return bool(denominator_var_0_eq_1) ? denominator_var_0_eq_1 : utils::detail::one;}

        constexpr Fraction& operator+=(const Fraction& rhs){return *this = Fraction(numerator() * rhs.denominator() + denominator() * rhs.numerator(), denominator() * rhs.denominator());}
        constexpr Fraction& operator+=(const std::convertible_to<Fraction> auto& rhs){return *this += Fraction(rhs);}

        constexpr Fraction& operator-=(const Fraction& rhs){return *this = Fraction(numerator() * rhs.denominator() - denominator() * rhs.numerator(), denominator() * rhs.denominator());}
        constexpr Fraction& operator-=(const std::convertible_to<Fraction> auto& rhs){return *this -= Fraction(rhs);}

        constexpr Fraction& operator*=(const Fraction& rhs){return *this = Fraction(numerator() * rhs.numerator(), denominator() * rhs.denominator());}
        constexpr Fraction& operator*=(const std::convertible_to<Fraction> auto& rhs){return *this *= Fraction(rhs);}

        constexpr Fraction& operator/=(const Fraction& rhs){return *this = Fraction(numerator() * rhs.denominator(), denominator() * rhs.numerator());}
        constexpr Fraction& operator/=(const std::convertible_to<Fraction> auto& rhs){return *this /= Fraction(rhs);}

        constexpr Fraction& operator++(){return *this += 1;}
        constexpr Fraction operator++(int);

        constexpr Fraction& operator--(){return *this -= 1;}
        constexpr Fraction operator--(int);

        friend inline constexpr Fraction operator+(const Fraction& lhs, const std::convertible_to<Fraction> auto& rhs){return lhs + Fraction(rhs);}
        friend inline constexpr Fraction operator+(const std::convertible_to<Fraction> auto& lhs, const Fraction& rhs){return Fraction(lhs) + rhs;}
        friend inline constexpr Fraction operator+(const Fraction& val){return val;}

        friend inline constexpr Fraction operator-(const Fraction& lhs, const std::convertible_to<Fraction> auto& rhs){return lhs + (-Fraction(rhs));}
        friend inline constexpr Fraction operator-(const std::convertible_to<Fraction> auto& lhs, const Fraction& rhs){return Fraction(lhs) + (-rhs);}
        friend inline constexpr Fraction operator-(const Fraction& val){return Fraction(-val.numerator(), val.denominator());}

        friend inline constexpr Fraction operator*(const Fraction& lhs, const std::convertible_to<Fraction> auto& rhs){return lhs * Fraction(rhs);}
        friend inline constexpr Fraction operator*(const std::convertible_to<Fraction> auto& lhs, const Fraction& rhs){return Fraction(lhs) * rhs;}

        friend inline constexpr Fraction operator/(const Fraction& lhs, const std::convertible_to<Fraction> auto& rhs){return lhs / Fraction(rhs);}
        friend inline constexpr Fraction operator/(const std::convertible_to<Fraction> auto& lhs, const Fraction& rhs){return Fraction(lhs) / rhs;}

        friend inline constexpr bool operator==(const Fraction&, const Fraction&) noexcept = default;
        friend inline constexpr bool operator==(const Fraction& lhs, const std::convertible_to<Fraction> auto& rhs){return lhs == Fraction(rhs);}
        friend inline constexpr bool operator==(const std::convertible_to<Fraction> auto& lhs, const Fraction& rhs){return Fraction(lhs) == rhs;}

        friend inline constexpr std::strong_ordering operator<=>(const Fraction&, const Fraction&);
        friend inline constexpr std::strong_ordering operator<=>(const Fraction& lhs, const std::convertible_to<Fraction> auto& rhs){return lhs <=> Fraction(rhs);}

        explicit constexpr operator bool() const noexcept{return bool(numerator());}
        template <std::integral T>
        explicit constexpr operator T() const{return numerator() / denominator();}
        explicit constexpr operator utils::Integer() const{return numerator() / denominator();}
        template <std::floating_point T>
        explicit constexpr operator T() const;

        enum{fraction_form};
        
        template <utils::String S>
        constexpr S to_string(std::size_t sig_figs = 50) const{return this->to_string_custom_alloc<S>(typename S::allocator_type(), sig_figs);}
        template <utils::String S>
        constexpr S to_string(decltype(utils::decimals), std::size_t precision = 50) const;
        template <utils::String S>
        constexpr S to_string(decltype(fraction_form)) const{return this->to_string_custom_alloc<S>(typename S::allocator_type(), fraction_form);}

        template <utils::String S>
        constexpr S to_string_custom_alloc(const typename S::allocator_type&, std::size_t = 50) const;
        template <utils::String S>
        constexpr S to_string_custom_alloc(const typename S::allocator_type&, decltype(utils::decimals), std::size_t = 50) const;
        template <utils::String S>
        constexpr S to_string_custom_alloc(const typename S::allocator_type&, decltype(fraction_form)) const;

        friend inline constexpr utils::Integer trunc(const Fraction& val){return val.numerator() / val.denominator();}
        
        friend inline constexpr Fraction pow(const Fraction& lhs, std::integral auto rhs){return utils::detail::pow<Fraction>(lhs, rhs);}
    private:
        utils::Integer numerator_var = 0;
        utils::Integer denominator_var_0_eq_1 = 0; // 0 "==" 1 because no one likes throwing move constructors or throwing move assignment operators

        constexpr void from_num(long double);
        
        constexpr void from_str(std::string_view);
};
constexpr utils::Fraction::Fraction(const utils::Integer& numerator, const utils::Integer& denominator){
    if (!denominator) throw utils::detail::div_by_zero_error;
    if (!numerator) return;
    if (denominator == utils::detail::one){
        *this = utils::Fraction(numerator);
        return;
    }
    if (denominator == -1){
        *this = utils::Fraction(-numerator);
        return;
    }
    numerator_var = numerator;
    denominator_var_0_eq_1 = denominator;
    if (denominator_var_0_eq_1 < 0){
        numerator_var *= -1;
        denominator_var_0_eq_1 *= -1;
    }
    utils::Integer gcd = utils::gcd(numerator_var, denominator_var_0_eq_1);
    numerator_var /= gcd;
    denominator_var_0_eq_1 /= gcd;
    if (denominator_var_0_eq_1 == utils::detail::one) denominator_var_0_eq_1 = 0;
}
template <utils::StringView SV>
constexpr utils::Fraction::Fraction(SV sv){
    std::string char_string;
    for (const typename SV::value_type& i: sv){
        char to_add = char(i);
        if (!SV::traits_type::eq(i, typename SV::value_type(to_add))) throw utils::detail::invalid_char_error;
        if (!utils::detail::fraction_chrs.contains(to_add)) throw utils::detail::invalid_char_error;
        if (to_add == '\'') continue;
        char_string += to_add;
    }
    from_str(std::string_view(char_string));
}

constexpr void utils::Fraction::from_num(long double num){
    if (num == std::truncl(num)){
        numerator_var = utils::Integer(num);
        return;
    }
    std::string to_convert;
    if (num < 0){
        to_convert += '-';
        num *= -1;
    }
    (to_convert += utils::Integer(num).to_string<std::string>()) += '.'; // constexpr
    num -= std::truncl(num);
    while (num){
        int digit = num * 10;
        to_convert += ('0' + digit);
        (num *= 10) -= digit;
    }
    from_str(std::string_view(to_convert.data(), to_convert.size()));
}

constexpr void utils::Fraction::from_str(std::string_view sv){
    using namespace utils::literals;
    if (sv.starts_with("++") || sv.starts_with("+-") || sv.starts_with("-+") || sv.starts_with("--")) throw utils::detail::invalid_char_error;
    if (sv.empty()) return;
    if (sv == "0") return;
    if (sv.contains('/')){
        *this = utils::Fraction(utils::Integer(sv.substr(0, sv.find('/')), 10), utils::Integer(sv.substr(sv.find('/') + 1), 10));
        return;
    }
    bool is_negative = false;
    if (sv.starts_with('-')){
        is_negative = true;
        sv.remove_prefix(1);
    }
    if (sv.starts_with('+')) sv.remove_prefix(1);
    std::size_t dp_location = sv.find('.'), e_location = sv.find_first_of("eE");
    if ((e_location != std::string_view::npos) && sv.substr(e_location + 1).contains('.')) throw utils::detail::invalid_char_error;
    if ((e_location == std::string_view::npos) && ((dp_location == std::string_view::npos) || (dp_location == (sv.size() - 1)))){
        numerator_var = utils::Integer(sv.substr(0, dp_location), 10);
        if (is_negative) numerator_var *= -1;
        return;
    }
    std::string_view integer_part = sv.substr(0, dp_location), decimal_part = sv.substr(dp_location + 1), exponent_part;
    std::ptrdiff_t exponent = 0;
    auto set_check_exponent_part = [sv, e_location, &exponent_part](){
        exponent_part = sv.substr(e_location + 1);
        if (
            exponent_part.starts_with("++") ||
            exponent_part.starts_with("+-") ||
            exponent_part.starts_with("-+") ||
            exponent_part.starts_with("--")
        ) throw utils::detail::invalid_char_error;
        if (exponent_part[0] == '+') exponent_part.remove_prefix(1);
        if (exponent_part == "-") exponent_part = "0";
        if (exponent_part == "") exponent_part = "0";
    };
    if (dp_location == std::string_view::npos){
        integer_part = sv.substr(0, e_location);
        decimal_part = "";
        set_check_exponent_part();
    }
    else if (e_location == std::string_view::npos) exponent_part = "0";
    else{
        decimal_part = decimal_part.substr(0, decimal_part.find_first_of("eE"));
        set_check_exponent_part();
    }
    utils::Integer numerator = utils::Integer(integer_part, 10) * utils::pow(10_i, decimal_part.size()) + utils::Integer(decimal_part, 10);
    utils::Integer denominator = utils::pow(10_i, decimal_part.size());
    std::from_chars_result res = std::from_chars(exponent_part.begin(), exponent_part.end(), exponent);
    if (res.ptr != exponent_part.end()) throw utils::detail::invalid_char_error;
    if (res.ec != std::errc()) throw utils::detail::invalid_char_error;
    if (exponent > 0) numerator *= utils::pow(10_i, std::size_t(exponent));
    if (exponent < 0) denominator *= utils::pow(10_i, std::size_t(-exponent));
    if (is_negative) numerator *= -1;
    *this = utils::Fraction(numerator, denominator);
}

template <std::floating_point T>
constexpr utils::Fraction::operator T() const{
    std::string string_version = to_string(std::numeric_limits<T>::digits10() + 1);
    T result = 0;
    std::from_chars_result res = std::from_chars(string_version.data(), string_version.data() + string_version.size(), result);
    if (res.ptr != (string_version.data() + string_version.size())) throw std::invalid_argument("Invalid argument");
    if (res.ec == std::errc::result_out_of_range){
        if (numerator() > 0) return std::numeric_limits<T>::infinity();
        return -std::numeric_limits<T>::infinity();
    }
    if (res.ec != std::errc()) throw std::invalid_argument("Invalid argument");
    return result;
}

template <utils::String S>
inline constexpr S utils::Fraction::to_string(decltype(utils::decimals), std::size_t precision) const{
    return this->to_string_custom_alloc<S>(typename S::allocator_type(), utils::decimals, precision);
}

template <utils::String S>
inline constexpr S utils::Fraction::to_string_custom_alloc(const S::allocator_type& alloc, std::size_t sig_figs) const{
    using namespace utils::literals;
    std::ptrdiff_t precision = 0;
    if (utils::abs(*this) >= 1) precision -= utils::floor_log(utils::Integer(*this), 10);
    else if ((utils::abs(numerator()) == 1) && (utils::pow(10_i, utils::floor_log(denominator(), 10)) == denominator())) precision += utils::floor_log(denominator(), 10);
    else precision += (utils::floor_log(utils::Integer(1 / utils::abs(*this)), 10) + 1);
    if ((precision <= std::ptrdiff_t(sig_figs)) && (precision >= -std::ptrdiff_t(sig_figs)))
        return this->to_string_custom_alloc<S>(alloc, utils::decimals, std::size_t(precision + sig_figs));
    S result = (*this * utils::pow(10, precision)).to_string_custom_alloc<S>(alloc, utils::decimals, std::size_t(sig_figs));
    std::string char_exp_str = std::format("{}", -precision);
    result += typename S::value_type('e');
    for (char i: char_exp_str) result += typename S::value_type(i);
    return result;
}
template <utils::String S>
inline constexpr S utils::Fraction::to_string_custom_alloc(const S::allocator_type& alloc, decltype(utils::decimals), std::size_t precision) const{
    using namespace utils::literals;
    if (!precision || (!(numerator() % denominator()))) return utils::round(*this).to_string_custom_alloc<S>(alloc);
    S result = utils::round(utils::abs(*this) * utils::pow(10_i, precision)).to_string_custom_alloc<S>(alloc);
    if (result.size() < (precision + 1)) result.insert(result.cbegin(), precision + 1 - result.size(), typename S::value_type('0'));
    result.insert(result.cend() - precision, typename S::value_type('.'));
    while (result.size() && result.back() == typename S::value_type('0')) result.pop_back();
    if (numerator() < 0) result.insert(result.cbegin(), typename S::value_type('-'));
    return result;
}
template <utils::String S>
inline constexpr S utils::Fraction::to_string_custom_alloc(const S::allocator_type& alloc, decltype(utils::Fraction::fraction_form)) const{
    S result(alloc);
    result += numerator().to_string_custom_alloc<S>(alloc);
    result += typename S::value_type('/');
    result += denominator().to_string_custom_alloc<S>(alloc);
    return result;
}

inline constexpr utils::Fraction utils::operator+(const utils::Fraction& lhs, const utils::Fraction& rhs){
    return utils::Fraction(lhs.numerator() * rhs.denominator() + lhs.denominator() * rhs.numerator(), lhs.denominator() * rhs.denominator());
}

inline constexpr utils::Fraction utils::operator-(const utils::Fraction& lhs, const utils::Fraction& rhs){
    return utils::Fraction(lhs.numerator() * rhs.denominator() - lhs.denominator() * rhs.numerator(), lhs.denominator() * rhs.denominator());
}

inline constexpr utils::Fraction utils::operator*(const utils::Fraction& lhs, const utils::Fraction& rhs){
    return utils::Fraction(lhs.numerator() * rhs.numerator(), lhs.denominator() * rhs.denominator());
}

inline constexpr utils::Fraction utils::operator/(const utils::Fraction& lhs, const utils::Fraction& rhs){
    return utils::Fraction(lhs.numerator() * rhs.denominator(), lhs.denominator() * rhs.numerator());
}

template <std::floating_point T>
requires std::convertible_to<T, utils::Fraction>
constexpr bool utils::operator==(const utils::Fraction& lhs, const T& rhs){
    if (std::isinf((long double)rhs) || std::isnan((long double)rhs)) return false;
    return lhs == utils::Fraction(rhs);
}
template <std::floating_point T>
requires std::convertible_to<T, utils::Fraction>
constexpr bool utils::operator==(const T& lhs, const utils::Fraction& rhs){
    if (std::isinf((long double)rhs) || std::isnan((long double)rhs)) return false;
    return utils::Fraction(lhs) == rhs;
}

inline constexpr std::strong_ordering utils::operator<=>(const Fraction& lhs, const Fraction& rhs){
    return lhs.numerator() * rhs.denominator() <=> lhs.denominator() * rhs.numerator();
}
template <std::floating_point T>
requires std::convertible_to<T, utils::Fraction>
constexpr std::partial_ordering utils::operator<=>(const Fraction& lhs, const T& rhs){
    if (std::isinf((long double)rhs)) return 0 <=> rhs;
    if (std::isnan((long double)rhs)) return std::partial_ordering::unordered;
    return std::partial_ordering(lhs <=> utils::Fraction(rhs));
}

namespace utils::literals{
    inline constexpr utils::Fraction operator""_f(const char* cstr){return utils::Fraction(cstr);}
    inline constexpr utils::Fraction operator""_f(const char* cstr,     std::size_t n){return utils::Fraction(std::   string_view(cstr, n));}
    inline constexpr utils::Fraction operator""_f(const wchar_t* cstr,  std::size_t n){return utils::Fraction(std::  wstring_view(cstr, n));}
    inline constexpr utils::Fraction operator""_f(const char8_t* cstr,  std::size_t n){return utils::Fraction(std:: u8string_view(cstr, n));}
    inline constexpr utils::Fraction operator""_f(const char16_t* cstr, std::size_t n){return utils::Fraction(std::u16string_view(cstr, n));}
    inline constexpr utils::Fraction operator""_f(const char32_t* cstr, std::size_t n){return utils::Fraction(std::u32string_view(cstr, n));}
}

constexpr utils::Integer utils::floor(const utils::Fraction& val){
    if (!(val.numerator() % val.denominator())) return utils::Integer(val);
    else if (val >= 0) return utils::trunc(val);
    else return utils::trunc(val) - 1;
}

constexpr utils::Integer utils::ceil(const utils::Fraction& val){
    if (!(val.numerator() % val.denominator())) return utils::Integer(val);
    else if (val >= 0) return utils::trunc(val) + 1;
    else return utils::trunc(val);
}

constexpr utils::Integer utils::round(const utils::Fraction& val){
    utils::Fraction frac = val - utils::floor(val);
    std::strong_ordering which_case = (frac * 2) <=> 1;
    if (which_case == std::strong_ordering::less) return utils::floor(val);
    else if (which_case == std::strong_ordering::equal) return utils::trunc(val); // To match the behavior of round for floating-point types in c++
    else return utils::ceil(val);
}

constexpr std::size_t std::hash<utils::Fraction>::operator()(const utils::Fraction& val) const noexcept{
    utils::HashCombiner combiner;
    return combiner.write(std::hash<utils::Integer>()(val.numerator())).write(std::hash<utils::Integer>()(val.denominator())).finalize();
}
#endif