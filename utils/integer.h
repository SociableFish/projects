#ifndef UTILS_INTEGER_H_IS_ALREADY_INCLUDED_UTILS_INTEGER_H_IS_ALREADY_INCLUDED_UTILS_INTEGER_H_IS_ALREADY_INCLUDED
#define UTILS_INTEGER_H_IS_ALREADY_INCLUDED_UTILS_INTEGER_H_IS_ALREADY_INCLUDED_UTILS_INTEGER_H_IS_ALREADY_INCLUDED
#include "helpers.h"
#include "hash.h"
namespace utils{
    class Integer;
    namespace detail{
        const auto invalid_char_error = std::invalid_argument("The (C-)string( view) to be converted contains invalid characters");
        const auto div_by_zero_error = std::invalid_argument("Division by 0 is illegal");
        const auto invalid_shift_error = std::invalid_argument("Cannot shift by a negative number");
        const std::string chrs = "0123456789abcdefghijklmnopqrstuvwxyz+-.\'";
    }

    constexpr utils::Integer operator+       (const utils::Integer&, const utils::Integer&);
    template <std::integral T>
    inline constexpr utils::Integer operator+(const utils::Integer&, const T&);
    template <std::integral T>
    inline constexpr utils::Integer operator+(const T&,              const utils::Integer&);
    template <std::floating_point T>
    inline constexpr T operator+             (const utils::Integer&, const T&)              noexcept;
    template <std::floating_point T>
    inline constexpr T operator+             (const T&,              const utils::Integer&) noexcept;
    inline constexpr utils::Integer operator+(const utils::Integer&);

    constexpr utils::Integer operator-       (const utils::Integer&, const utils::Integer&);
    template <std::integral T>
    inline constexpr utils::Integer operator-(const utils::Integer&, const T&);
    template <std::integral T>
    inline constexpr utils::Integer operator-(const T&,              const utils::Integer&);
    template <std::floating_point T>
    inline constexpr T operator-             (const utils::Integer&, const T&)              noexcept;
    template <std::floating_point T>
    inline constexpr T operator-             (const T&,              const utils::Integer&) noexcept;
    inline constexpr utils::Integer operator-(const utils::Integer&);

    constexpr utils::Integer operator*       (const utils::Integer&, const utils::Integer&);
    template <std::integral T>
    inline constexpr utils::Integer operator*(const utils::Integer&, const T&);
    template <std::integral T>
    inline constexpr utils::Integer operator*(const T&,              const utils::Integer&);
    template <std::floating_point T>
    inline constexpr T operator*             (const utils::Integer&, const T&)              noexcept;
    template <std::floating_point T>
    inline constexpr T operator*             (const T&,              const utils::Integer&) noexcept;

    constexpr utils::Integer operator/       (const utils::Integer&, const utils::Integer&);
    template <std::integral T>
    inline constexpr utils::Integer operator/(const utils::Integer&, const T&);
    template <std::integral T>
    inline constexpr utils::Integer operator/(const T&,              const utils::Integer&);
    template <std::floating_point T>
    inline constexpr T operator/             (const utils::Integer&, const T&)              noexcept;
    template <std::floating_point T>
    inline constexpr T operator/             (const T&,              const utils::Integer&) noexcept;

    constexpr utils::Integer operator%       (const utils::Integer&, const utils::Integer&);
    template <std::integral T>
    inline constexpr utils::Integer operator%(const utils::Integer&, const T&);
    template <std::integral T>
    inline constexpr utils::Integer operator%(const T&,              const utils::Integer&);

    constexpr utils::Integer operator&       (const utils::Integer&, const utils::Integer&);
    template <std::integral T>
    inline constexpr utils::Integer operator&(const utils::Integer&, const T&);
    template <std::integral T>
    inline constexpr utils::Integer operator&(const T&,              const utils::Integer&);

    constexpr utils::Integer operator|       (const utils::Integer&, const utils::Integer&);
    template <std::integral T>
    inline constexpr utils::Integer operator|(const utils::Integer&, const T&);
    template <std::integral T>
    inline constexpr utils::Integer operator|(const T&,              const utils::Integer&);

    constexpr utils::Integer operator^       (const utils::Integer&, const utils::Integer&);
    template <std::integral T>
    inline constexpr utils::Integer operator^(const utils::Integer&, const T&);
    template <std::integral T>
    inline constexpr utils::Integer operator^(const T&,              const utils::Integer&);

    template <std::integral T>
    constexpr utils::Integer operator<<      (const utils::Integer&, const T&);

    template <std::integral T>
    constexpr utils::Integer operator>>      (const utils::Integer&, const T&);

    inline constexpr utils::Integer operator~(const utils::Integer&);

    inline constexpr bool operator==(const utils::Integer&, const utils::Integer&) noexcept;
    template <std::integral T>
    inline constexpr bool operator==(const utils::Integer&, const T&);
    template <std::floating_point T>
    inline constexpr bool operator==(const utils::Integer&, const T&)              noexcept;

    constexpr std::strong_ordering operator<=>        (const utils::Integer&, const utils::Integer&) noexcept;
    template <std::integral T>
    inline constexpr std::strong_ordering operator<=> (const utils::Integer&, const T&);
    template <std::floating_point T>
    inline constexpr std::partial_ordering operator<=>(const utils::Integer&, const T&)              noexcept;

    constexpr utils::Integer gcd(const utils::Integer&, const utils::Integer&);

    inline constexpr utils::Integer lcm(const utils::Integer&, const utils::Integer&);
    
    inline constexpr utils::Integer pow(const utils::Integer&, std::size_t);
}

template <std::integral T>
struct std::common_type<utils::Integer, T>: utils::detail::Wrapper<utils::Integer>{};
template <std::integral T>
struct std::common_type<T, utils::Integer>: utils::detail::Wrapper<utils::Integer>{};
template <std::floating_point T>
struct std::common_type<utils::Integer, T>: utils::detail::Wrapper<T>{};
template <std::floating_point T>
struct std::common_type<T, utils::Integer>: utils::detail::Wrapper<T>{};

template <>
struct std::hash<utils::Integer>{constexpr std::size_t operator()(const utils::Integer& val) const noexcept;};

/**
 * @brief An integer type with values within the range @c [1-1e20'752'587'082'923'245'559, @c 1e20'752'587'082'923'245'559-1]
 */
class utils::Integer final{
    public:
        friend inline constexpr Integer utils::operator-(const Integer&);
        friend inline constexpr bool operator==(const Integer&, const Integer&) noexcept = default;
        friend constexpr std::strong_ordering utils::operator<=>(const Integer&, const Integer&) noexcept;
        friend struct std::hash<utils::Integer>;

        constexpr Integer() noexcept = default;
        constexpr Integer(const Integer&) = default;
        constexpr Integer(Integer&&) noexcept = default;

        template <std::integral T>
        constexpr Integer(T number){from_num(std::intmax_t(number));}
        template <std::unsigned_integral T>
        constexpr Integer(T number){from_num(std::uintmax_t(number));}
        template <std::floating_point T>
        constexpr Integer(T number){from_num((long double)(number));}
        
        template <class CharT, class Traits = std::char_traits<CharT>>
        constexpr explicit Integer(const CharT* cstr, std::uint8_t base = 0): Integer(std::basic_string_view<CharT, Traits>(cstr), base){}
        constexpr explicit Integer(std::nullptr_t, std::uint8_t base = 0) = delete;
        template <class CharT, class Traits, class Allocator>
        constexpr explicit Integer(const std::basic_string<CharT, Traits, Allocator>& str, std::uint8_t base = 0): Integer(
            std::basic_string_view<CharT, Traits>(str.data(), str.size()),
            base
        ){}
        template <class CharT, class Traits>
        constexpr explicit Integer(std::basic_string_view<CharT, Traits>, std::uint8_t =0);

        constexpr Integer& operator=(const Integer&) = default;
        constexpr Integer& operator=(Integer&&) noexcept = default;

        constexpr Integer& operator+=(const Integer&);
        template <std::convertible_to<Integer> T>
        constexpr Integer& operator+=(const T& rhs){return *this = Integer(T(*this) + rhs);}
        template <std::integral T>
        requires std::convertible_to<T, Integer>
        constexpr Integer& operator+=(const T& rhs){return *this += Integer(rhs);}

        constexpr Integer& operator-=(const Integer& rhs){return *this += (-rhs);}
        template <std::convertible_to<Integer> T>
        constexpr Integer& operator-=(const T& rhs){return *this = Integer(T(*this) - rhs);}
        template <std::integral T>
        requires std::convertible_to<T, Integer>
        constexpr Integer& operator-=(const T& rhs){return *this += (-Integer(rhs));}

        constexpr Integer& operator*=(const Integer&);
        template <std::convertible_to<Integer> T>
        constexpr Integer& operator*=(const T& rhs){return *this = Integer(T(*this) * rhs);}
        template <std::integral T>
        requires std::convertible_to<T, Integer>
        constexpr Integer& operator*=(const T& rhs){return *this *= Integer(rhs);}

        constexpr Integer& operator/=(const Integer&);
        template <std::convertible_to<Integer> T>
        constexpr Integer& operator/=(const T& rhs){return *this = Integer(T(*this) / rhs);}
        template <std::integral T>
        requires std::convertible_to<T, Integer>
        constexpr Integer& operator/=(const T& rhs){return *this /= Integer(rhs);}

        constexpr Integer& operator%=(const Integer& rhs){return *this -= *this / rhs * rhs;}
        template <std::integral T>
        constexpr Integer& operator%=(const T& rhs){return *this %= Integer(rhs);}

        constexpr Integer& operator&=(const Integer& rhs){return *this = bitwise<std::logical_and<>>(*this, rhs);}
        template <std::integral T>
        constexpr Integer& operator&=(const T& rhs){return *this &= Integer(rhs);}

        constexpr Integer& operator|=(const Integer& rhs){return *this = bitwise<std::logical_or<>>(*this, rhs);}
        template <std::integral T>
        constexpr Integer& operator|=(const T& rhs){return *this |= Integer(rhs);}

        constexpr Integer& operator^=(const Integer& rhs){return *this = bitwise<std::not_equal_to<>>(*this, rhs);}
        template <std::integral T>
        constexpr Integer& operator^=(const T& rhs){return *this ^= Integer(rhs);}

        template <std::integral T>
        constexpr Integer& operator<<=(const T&);
        template <std::unsigned_integral T>
        constexpr Integer& operator<<=(const T& rhs){return *this *= utils::pow(utils::Integer(2), rhs);}

        template <std::integral T>
        constexpr Integer& operator>>=(const T&);
        template <std::unsigned_integral T>
        constexpr Integer& operator>>=(const T&);

        constexpr Integer& operator++(){return *this += 1;}
        constexpr Integer operator++(int);

        constexpr Integer& operator--(){return *this -= 1;}
        constexpr Integer operator--(int);

        constexpr operator bool() const noexcept{return digits.size();}
        template <std::integral T>
        constexpr operator T() const noexcept{return T(to_intmax());}
        template <std::unsigned_integral T>
        constexpr operator T() const noexcept{return T(to_uintmax());}
        template <std::floating_point T>
        constexpr operator T() const noexcept{return T(to_long_double());}
        
        template <class CharT, class Traits = std::char_traits<CharT>, class Allocator = std::allocator<CharT>>
        constexpr std::basic_string<CharT, Traits, Allocator> to_string(std::uint8_t base = 10) const{return to_string_custom_alloc<CharT, Traits, Allocator>(Allocator(), base);}
        template <class CharT, class Traits = std::char_traits<CharT>, class Allocator = std::allocator<CharT>>
        constexpr std::basic_string<CharT, Traits, Allocator> to_string_custom_alloc(const Allocator&, std::uint8_t = 10) const;
    private:
        std::vector<std::uint32_t> digits = {};
        bool is_negative = false;

        static constexpr std::uint32_t base = 1'000'000'000;

        constexpr void correct() noexcept;

        constexpr Integer(const std::vector<std::uint32_t>&, bool = false);

        constexpr void from_num(std::intmax_t);
        constexpr void from_num(std::uintmax_t);
        constexpr void from_num(long double);

        constexpr void from_str(std::string_view, std::uint8_t);

        constexpr std::intmax_t to_intmax() const noexcept;
        constexpr std::uintmax_t to_uintmax() const noexcept;
        constexpr long double to_long_double() const noexcept;

        static constexpr std::vector<std::uint32_t> add(const std::vector<std::uint32_t>&, const std::vector<std::uint32_t>&);
        static constexpr std::vector<std::uint32_t> subtract(const std::vector<std::uint32_t>&, const std::vector<std::uint32_t>&);
        static constexpr std::vector<std::uint32_t> multiply(const std::vector<std::uint32_t>&, std::uint32_t);
        static constexpr std::uint32_t divide(const std::vector<std::uint32_t>&, const std::vector<std::uint32_t>&);
        template <class T>
        static constexpr Integer bitwise(const Integer&, const Integer&);
};

template <class CharT, class Traits>
constexpr utils::Integer::Integer(std::basic_string_view<CharT, Traits> sv, std::uint8_t base){
    if ((base == 1) || (base > 36)) throw std::invalid_argument("Invalid base");
    std::string char_string;
    for (const CharT& i: sv){
        char to_add = char(i);
        if (!Traits::eq(i, CharT(to_add))) throw utils::detail::invalid_char_error;
        if (!utils::detail::chrs.contains(to_add)) throw utils::detail::invalid_char_error;
        if (to_add == '\'') continue;
        if (to_add == '.') break;
        char_string += to_add;
    }
    from_str(std::string_view(char_string.data(), char_string.size()), base);
}
constexpr utils::Integer::Integer(const std::vector<std::uint32_t>& digits_arg, bool is_negative_arg){
    digits = digits_arg;
    is_negative = is_negative_arg;
    correct();
}

constexpr void utils::Integer::correct() noexcept{
    while (digits.size() && !digits.back()) digits.pop_back();
    if (digits.empty()) is_negative = false;
}

constexpr void utils::Integer::from_num(std::intmax_t num){
    if (num < 0) is_negative = true;
    while (num){
        digits.push_back(utils::abs(num % base));
        num /= base;
    }
}
constexpr void utils::Integer::from_num(std::uintmax_t num){
    while (num){
        digits.push_back(num % base);
        num /= base;
    }
}
constexpr void utils::Integer::from_num(long double num){
    if (num < 0){
        is_negative = true;
        num = -num;
    }
    while (num){
        digits.push_back(num - std::trunc(num / base) * base);
        num = std::trunc(num / base);
    }
}

constexpr void utils::Integer::from_str(std::string_view sv, std::uint8_t base){
    if (sv.starts_with("++") || sv.starts_with("+-") || sv.starts_with("-+") || sv.starts_with("--")) throw utils::detail::invalid_char_error;
    if (sv.starts_with('-')){
        is_negative = true;
        sv.remove_prefix(1);
    }
    if (sv.starts_with('+')) sv.remove_prefix(1);
    if (sv.starts_with("0x") && !base){
        base = 16;
        sv.remove_prefix(2);
    }
    if (sv.starts_with("0X") && !base){
        base = 16;
        sv.remove_prefix(2);
    }
    if (sv.starts_with("0b") && !base){
        base = 2;
        sv.remove_prefix(2);
    }
    if (sv.starts_with("0B") && !base){
        base = 2;
        sv.remove_prefix(2);
    }
    if (sv.starts_with("0") && !base){
        base = 8;
        sv.remove_prefix(1);
    }
    if (!base) base = 10;
    if (sv.empty()) return;
    if (sv == "0") return;
    if (base != 10){
        for (char i: sv){
            std::size_t digit = utils::detail::chrs.substr(0, base).find(i);
            if (digit == std::string_view::npos) throw utils::detail::invalid_char_error;
            *this *= base;
            if (is_negative) *this -= digit;
            else *this += digit;
        }
        return;
    }
    for (std::size_t i = 0; i < (sv.size() / 9); i++){
        digits.push_back(0);
        std::from_chars_result res = std::from_chars(sv.end() - 9 * (i + 1), sv.end() - 9 * i, digits.back());
        if (res.ptr != (sv.end() - 9 * i)) throw utils::detail::invalid_char_error;
        if (res.ec != std::errc()) throw utils::detail::invalid_char_error;
    }
    if (sv.size() % 9){
        digits.push_back(0);
        std::from_chars_result res = std::from_chars(sv.begin(), sv.begin() + sv.size() % 9, digits.back());
        if (res.ptr != (sv.begin() + sv.size() % 9)) throw utils::detail::invalid_char_error;
        if (res.ec != std::errc()) throw utils::detail::invalid_char_error;
    }
    correct();
}

constexpr std::intmax_t utils::Integer::to_intmax() const noexcept{
    std::intmax_t result = 0;
    for (auto it = digits.crbegin(), end = digits.crend(); it != end; it++) (result *= base) += (is_negative ? -*it : *it);
    return result;
}

constexpr std::uintmax_t utils::Integer::to_uintmax() const noexcept{
    std::uintmax_t result = 0;
    for (auto it = digits.crbegin(), end = digits.crend(); it != end; it++) (result *= base) += (is_negative ? -*it : *it);
    return result;
}

constexpr long double utils::Integer::to_long_double() const noexcept{
    long double result = 0;
    for (auto it = digits.crbegin(), end = digits.crend(); it != end; it++) (result *= base) += (is_negative ? -*it : *it);
    return result;
}

constexpr std::vector<std::uint32_t> utils::Integer::add(const std::vector<std::uint32_t>& lhs, const std::vector<std::uint32_t>& rhs){
    if (lhs.empty()) return rhs;
    if (rhs.empty()) return lhs;
    std::vector<std::uint32_t> result(std::max(lhs.size(), rhs.size()) + 1);
    for (std::size_t i = 0; i < lhs.size(); i++) result[i] = lhs[i];
    for (std::size_t i = 0; i < rhs.size(); i++) result[i] += rhs[i];
    for (std::size_t i = 1; i < result.size(); i++) if (result[i - 1] >= base){
        result[i - 1] -= base;
        result[i]++;
    }
    if (!result.back()) result.pop_back();
    return result;
}

constexpr std::vector<std::uint32_t> utils::Integer::subtract(const std::vector<std::uint32_t>& lhs, const std::vector<std::uint32_t>& rhs){
    // Assuming lhs >= rhs
    if (lhs == rhs) return {};
    if (rhs.empty()) return lhs;
    std::vector<std::int32_t> intermediate(lhs.cbegin(), lhs.cend());
    for (std::size_t i = 0; i < rhs.size(); i++) intermediate[i] -= rhs[i];
    for (std::size_t i = 1; i < intermediate.size(); i++) if (intermediate[i - 1] < 0){
        intermediate[i - 1] += base;
        intermediate[i]--;
    }
    while (intermediate.size() && !intermediate.back()) intermediate.pop_back();
    std::vector<std::uint32_t> result(intermediate.size());
    for (std::size_t i = 0; i < intermediate.size(); i++) result[i] = std::uint32_t(intermediate[i]);
    return result;
}

constexpr std::vector<std::uint32_t> utils::Integer::multiply(const std::vector<std::uint32_t>& lhs, std::uint32_t rhs){
    if (lhs.empty()) return {};
    if (!rhs) return {};
    if (lhs == std::vector<std::uint32_t>({1})) return {rhs};
    if (rhs == 1) return lhs;
    std::vector<std::uint32_t> result(lhs.size() + 1);
    for (std::size_t i = 1; i < result.size(); i++){
        std::uint64_t intermediate = std::uint64_t(lhs[i - 1]) * rhs;
        result[i - 1] += intermediate % base;
        result[i] = intermediate / base;
    }
    for (std::size_t i = 1; i < result.size(); i++) if (result[i - 1] >= base){
        result[i - 1] -= base;
        result[i]++;
    }
    if (!result.back()) result.pop_back();
    return result;
}

constexpr std::uint32_t utils::Integer::divide(const std::vector<std::uint32_t>& lhs, const std::vector<std::uint32_t>& rhs){
    // Assuming rhs != 0
    if (lhs.empty()) return 0;
    if (rhs == std::vector<std::uint32_t>({1})) return lhs[0];
    std::uint32_t min = 0, max = base - 1;
    utils::Integer ilhs = utils::Integer(lhs);
    while (max > min){
        std::uint32_t mid = (min + max) / 2 + (min + max) % 2;
        utils::Integer irhs_mid = utils::Integer(rhs) * mid;
        std::strong_ordering cmp = ilhs <=> irhs_mid;
        if (cmp == 0) return mid;
        else if (cmp > 0) min = mid;
        else max = mid - 1;
    }
    return min;
}

constexpr utils::Integer& utils::Integer::operator+=(const utils::Integer& rhs){
    if (!*this) return *this = rhs;
    if (!rhs) return *this;
    if (*this == -rhs) return *this = 0;
    if (is_negative == rhs.is_negative) return *this = utils::Integer(add(digits, rhs.digits), is_negative);
    const utils::Integer* pos_ptr = (const utils::Integer*)this;
    const utils::Integer* neg_ptr = std::addressof(rhs);
    if (is_negative) std::swap(pos_ptr, neg_ptr);
    if (*pos_ptr > -*neg_ptr) return *this = utils::Integer(subtract(pos_ptr->digits, neg_ptr->digits));
    return *this = utils::Integer(subtract(neg_ptr->digits, pos_ptr->digits), true);
}

constexpr utils::Integer& utils::Integer::operator*=(const utils::Integer& rhs){
    if (*this == 1) return *this = rhs;
    if (!*this) return *this = 0;
    if (*this == -1) return *this = -rhs;
    if (rhs == 1) return *this;
    if (!rhs) return *this = 0;
    if (rhs == -1) return *this = -*this;
    utils::Integer result;
    for (std::size_t i = 0; i < rhs.digits.size(); i++){
        std::vector<std::uint32_t> to_add = multiply(digits, rhs.digits[i]);
        to_add.insert(to_add.cbegin(), i, 0);
        result += utils::Integer(to_add, is_negative != rhs.is_negative);
    }
    return *this = result;
}

constexpr utils::Integer& utils::Integer::operator/=(const utils::Integer& rhs){
    if (!rhs) throw utils::detail::div_by_zero_error;
    if (rhs == 1) return *this;
    if (rhs == -1) return *this = -*this;
    if (!*this) return *this;
    if (utils::abs(*this) < utils::abs(rhs)) return *this = 0;
    is_negative = is_negative != rhs.is_negative;
    utils::Integer remainder, abs_this = utils::abs(*this), abs_rhs = utils::abs(rhs);
    std::fill(digits.begin(), digits.end(), 0);
    for (std::size_t i = abs_this.digits.size(); i--;){
        remainder.digits.insert(remainder.digits.cbegin(), abs_this.digits[i]);
        remainder.correct();
        std::uint32_t cnt = divide(remainder.digits, rhs.digits);
        remainder -= abs_rhs * cnt;
        digits[i] = cnt;
    }
    correct();
    return *this;
}

template <std::integral T>
constexpr utils::Integer& utils::Integer::operator<<=(const T& rhs){
    if (rhs < 0) throw utils::detail::invalid_shift_error;
    return *this <<= std::uintmax_t(rhs);
}

template <std::integral T>
constexpr utils::Integer& utils::Integer::operator>>=(const T& rhs){
    if (rhs < 0) throw utils::detail::invalid_shift_error;
    return *this >>= std::uintmax_t(rhs);
}
template <std::unsigned_integral T>
constexpr utils::Integer& utils::Integer::operator>>=(const T& rhs){
    if (!is_negative) return *this /= utils::pow(utils::Integer(2), rhs);
    if (*this % utils::pow(utils::Integer(2), rhs)) return (*this /= utils::pow(utils::Integer(2), rhs)) -= 1;
    return *this /= utils::pow(utils::Integer(2), rhs);
}

constexpr utils::Integer utils::Integer::operator++(int){
    utils::Integer prev = *this;
    ++*this;
    return prev;
}

constexpr utils::Integer utils::Integer::operator--(int){
    utils::Integer prev = *this;
    --*this;
    return prev;
}

template <class T>
constexpr utils::Integer utils::Integer::bitwise(const utils::Integer& lhs, const utils::Integer& rhs){
    std::string lhs_bits = "0";
    std::string rhs_bits = "0";
    constexpr auto invert_char = [](char x){return char((1 - (x - '0')) + '0');};
    constexpr auto t_char = [](char x, char y){return char(int(T()(bool(x - '0'), bool(y - '0'))) + '0');};
    if (lhs.is_negative){
        lhs_bits += (~lhs).to_string<char>(2);
        std::transform(lhs_bits.cbegin(), lhs_bits.cend(), lhs_bits.begin(), invert_char);
    }
    else lhs_bits += lhs.to_string<char>(2);
    if (rhs.is_negative){
        rhs_bits += (~rhs).to_string<char>(2);
        std::transform(rhs_bits.cbegin(), rhs_bits.cend(), rhs_bits.begin(), invert_char);
    }
    else rhs_bits += rhs.to_string<char>(2);
    if (lhs_bits.size() > rhs_bits.size()) rhs_bits.insert(rhs_bits.cbegin(), lhs_bits.size() - rhs_bits.size(), rhs_bits.front());
    if (lhs_bits.size() < rhs_bits.size()) lhs_bits.insert(lhs_bits.cbegin(), rhs_bits.size() - lhs_bits.size(), lhs_bits.front());
    std::string result_str_2c;
    std::transform(lhs_bits.cbegin(), lhs_bits.cend(), rhs_bits.cbegin(), std::back_inserter(result_str_2c), t_char);
    utils::Integer result = utils::Integer(result_str_2c, 2);
    if (result_str_2c.front() == '1') return ~result;
    return result;
}

template <class CharT, class Traits, class Allocator>
constexpr std::basic_string<CharT, Traits, Allocator> utils::Integer::to_string_custom_alloc(const Allocator& alloc, std::uint8_t base) const{
    std::basic_string<CharT, Traits, Allocator> result(alloc);
    if (!*this){
        result += CharT('0');
        return result;
    }
    if (is_negative) result += CharT('-');
    if (base != 10){
        for (utils::Integer to_divide = utils::abs(*this); to_divide; to_divide /= base) result += CharT(utils::detail::chrs[std::uint8_t(to_divide % base)]);
        std::reverse(result.begin() + int(is_negative), result.end());
        return result;
    }
    for (auto it = digits.crbegin(), end = digits.crend(); it != end; it++)
        for (char i: std::vformat("{:0{}}", std::make_format_args(*it, int(it != digits.crbegin()) * 9))) result += CharT(i);
    return result;
}

constexpr utils::Integer        utils::operator+ (const utils::Integer& lhs, const utils::Integer& rhs)         {
    utils::Integer result = lhs;
    return result += rhs;
}
template <std::integral T>
inline constexpr utils::Integer utils::operator+ (const utils::Integer& lhs, const T& rhs)                      {return lhs + utils::Integer(rhs);}
template <std::integral T>
inline constexpr utils::Integer utils::operator+ (const T& lhs,              const utils::Integer& rhs)         {return utils::Integer(lhs) + rhs;}
template <std::floating_point T>
inline constexpr T utils::operator+              (const utils::Integer& lhs, const T& rhs)              noexcept{return T(lhs) + rhs;}
template <std::floating_point T>
inline constexpr T utils::operator+              (const T& lhs,              const utils::Integer& rhs) noexcept{return lhs + T(rhs);}
inline constexpr utils::Integer utils::operator+ (const utils::Integer& val){return val;}

constexpr utils::Integer utils::operator-        (const utils::Integer& lhs, const utils::Integer& rhs)         {return lhs + (-rhs);}
template <std::integral T>
inline constexpr utils::Integer utils::operator- (const utils::Integer& lhs, const T& rhs)                      {return lhs + (-utils::Integer(rhs));}
template <std::integral T>
inline constexpr utils::Integer utils::operator- (const T& lhs,              const utils::Integer& rhs)         {return utils::Integer(lhs) + (-rhs);}
template <std::floating_point T>
inline constexpr T utils::operator-              (const utils::Integer& lhs, const T& rhs)              noexcept{return T(lhs) + (-rhs);}
template <std::floating_point T>
inline constexpr T utils::operator-              (const T& lhs,              const utils::Integer& rhs) noexcept{return lhs + (-T(rhs));}
inline constexpr utils::Integer utils::operator- (const utils::Integer& val){return utils::Integer(val.digits, !val.is_negative);}

constexpr utils::Integer        utils::operator* (const utils::Integer& lhs, const utils::Integer& rhs)         {
    utils::Integer result = lhs;
    return result *= rhs;
}
template <std::integral T>
inline constexpr utils::Integer utils::operator* (const utils::Integer& lhs, const T& rhs)                      {return lhs * utils::Integer(rhs);}
template <std::integral T>
inline constexpr utils::Integer utils::operator* (const T& lhs,              const utils::Integer& rhs)         {return utils::Integer(lhs) * rhs;}
template <std::floating_point T>
inline constexpr T utils::operator*              (const utils::Integer& lhs, const T& rhs)              noexcept{return T(lhs) * rhs;}
template <std::floating_point T>
inline constexpr T utils::operator*              (const T& lhs,              const utils::Integer& rhs) noexcept{return lhs * T(rhs);}

constexpr utils::Integer        utils::operator/ (const utils::Integer& lhs, const utils::Integer& rhs)         {
    utils::Integer result = lhs;
    return result /= rhs;
}
template <std::integral T>
inline constexpr utils::Integer utils::operator/ (const utils::Integer& lhs, const T& rhs)                      {return lhs / utils::Integer(rhs);}
template <std::integral T>
inline constexpr utils::Integer utils::operator/ (const T& lhs,              const utils::Integer& rhs)         {return utils::Integer(lhs) / rhs;}
template <std::floating_point T>
inline constexpr T utils::operator/              (const utils::Integer& lhs, const T& rhs)              noexcept{return T(lhs) / rhs;}
template <std::floating_point T>
inline constexpr T utils::operator/              (const T& lhs,              const utils::Integer& rhs) noexcept{return lhs / T(rhs);}

constexpr utils::Integer        utils::operator% (const utils::Integer& lhs, const utils::Integer& rhs)         {
    utils::Integer result = lhs;
    return result %= rhs;
}
template <std::integral T>
inline constexpr utils::Integer utils::operator% (const utils::Integer& lhs, const T& rhs)             {return lhs % utils::Integer(rhs);}
template <std::integral T>
inline constexpr utils::Integer utils::operator% (const T& lhs,              const utils::Integer& rhs){return utils::Integer(lhs) % rhs;}

constexpr utils::Integer        utils::operator& (const utils::Integer& lhs, const utils::Integer& rhs)         {
    utils::Integer result = lhs;
    return result &= rhs;
}
template <std::integral T>
inline constexpr utils::Integer utils::operator& (const utils::Integer& lhs, const T& rhs)             {return lhs & utils::Integer(rhs);}
template <std::integral T>
inline constexpr utils::Integer utils::operator& (const T& lhs,              const utils::Integer& rhs){return utils::Integer(lhs) & rhs;}

constexpr utils::Integer        utils::operator| (const utils::Integer& lhs, const utils::Integer& rhs)         {
    utils::Integer result = lhs;
    return result |= rhs;
}
template <std::integral T>
inline constexpr utils::Integer utils::operator| (const utils::Integer& lhs, const T& rhs)             {return lhs | utils::Integer(rhs);}
template <std::integral T>
inline constexpr utils::Integer utils::operator| (const T& lhs,              const utils::Integer& rhs){return utils::Integer(lhs) | rhs;}

constexpr utils::Integer        utils::operator^ (const utils::Integer& lhs, const utils::Integer& rhs)         {
    utils::Integer result = lhs;
    return result ^= rhs;
}
template <std::integral T>
inline constexpr utils::Integer utils::operator^ (const utils::Integer& lhs, const T& rhs)             {return lhs ^ utils::Integer(rhs);}
template <std::integral T>
inline constexpr utils::Integer utils::operator^ (const T& lhs,              const utils::Integer& rhs){return utils::Integer(lhs) ^ rhs;}

inline constexpr utils::Integer utils::operator~ (const utils::Integer& val){return -val - 1;}

template <std::integral T>
constexpr utils::Integer        utils::operator<<(const utils::Integer& lhs, const T& rhs)         {
    utils::Integer result = lhs;
    return result <<= rhs;
}

template <std::integral T>
constexpr utils::Integer        utils::operator>>(const utils::Integer& lhs, const T& rhs)         {
    utils::Integer result = lhs;
    return result >>= rhs;
}

template <std::integral T>
inline constexpr bool utils::operator==(const utils::Integer& lhs, const T& rhs)                      {return lhs == utils::Integer(rhs);}
template <std::floating_point T>
inline constexpr bool utils::operator==(const utils::Integer& lhs, const T& rhs)              noexcept{return T(lhs) == rhs;}

constexpr std::strong_ordering utils::operator<=>        (const utils::Integer& lhs, const utils::Integer& rhs) noexcept{
    if (lhs == rhs) return std::strong_ordering::equal;
    std::int8_t lsign = (lhs.is_negative ? -1 : (lhs.digits.size() ? 1 : 0)), rsign = (rhs.is_negative ? -1 : (rhs.digits.size() ? 1 : 0));
    if (lsign != rsign) return lsign <=> rsign;
    if (lhs.digits.size() != rhs.digits.size()){
        if (lhs.is_negative) return rhs.digits.size() <=> lhs.digits.size();
        return lhs.digits.size() <=> rhs.digits.size();
    }
    if (lhs.is_negative) return std::lexicographical_compare_three_way(rhs.digits.crbegin(), rhs.digits.crend(), lhs.digits.crbegin(), lhs.digits.crend());
    return std::lexicographical_compare_three_way(lhs.digits.crbegin(), lhs.digits.crend(), rhs.digits.crbegin(), rhs.digits.crend());
}
template <std::integral T>
inline constexpr std::strong_ordering utils::operator<=> (const utils::Integer& lhs, const T& rhs)                      {return lhs <=> utils::Integer(rhs);}
template <std::floating_point T>
inline constexpr std::partial_ordering utils::operator<=>(const utils::Integer& lhs, const T& rhs)              noexcept{return T(lhs) <=> rhs;}

constexpr std::size_t std::hash<utils::Integer>::operator()(const utils::Integer& val) const noexcept{
    utils::HashCombiner combiner(std::invoke_r<std::size_t>(std::hash<bool>(), val.is_negative));
    combiner.write(std::invoke_r<std::size_t>(std::hash<std::size_t>(), val.digits.size()));
    for (std::uint32_t i: val.digits) combiner.write(std::invoke_r<std::size_t>(std::hash<std::uint32_t>(), i));
    return combiner.finalize();
};

namespace utils::literals{
    inline constexpr utils::Integer operator""_i(const char* cstr){return utils::Integer(cstr);}
    inline constexpr utils::Integer operator""_i(const char* cstr,     std::size_t n){return utils::Integer(std::   string_view(cstr, n));}
    inline constexpr utils::Integer operator""_i(const wchar_t* cstr,  std::size_t n){return utils::Integer(std::  wstring_view(cstr, n));}
    inline constexpr utils::Integer operator""_i(const char8_t* cstr,  std::size_t n){return utils::Integer(std:: u8string_view(cstr, n));}
    inline constexpr utils::Integer operator""_i(const char16_t* cstr, std::size_t n){return utils::Integer(std::u16string_view(cstr, n));}
    inline constexpr utils::Integer operator""_i(const char32_t* cstr, std::size_t n){return utils::Integer(std::u32string_view(cstr, n));}
}

constexpr utils::Integer utils::gcd(const utils::Integer& lhs, const utils::Integer& rhs){
    utils::Integer big = utils::abs(lhs), small = utils::abs(rhs);
    if (small == big) return small;
    if (small > big) std::swap(small, big);
    if (!small) return big;
    if (small == 1) return 1;
    while (small) big = std::exchange(small, big % small);
    return big;
}

inline constexpr utils::Integer utils::lcm(const utils::Integer& lhs, const utils::Integer& rhs){return utils::abs(lhs) * utils::abs(rhs) / utils::gcd(lhs, rhs);}

inline constexpr utils::Integer utils::pow(const utils::Integer& lhs, std::size_t rhs){return utils::detail::pow<utils::Integer>(lhs, rhs);}
#endif