#ifndef UTILS_HASH_H_IS_ALREADY_INCLUDED_UTILS_HASH_H_IS_ALREADY_INCLUDED_UTILS_HASH_H_IS_ALREADY_INCLUDED
#define UTILS_HASH_H_IS_ALREADY_INCLUDED_UTILS_HASH_H_IS_ALREADY_INCLUDED_UTILS_HASH_H_IS_ALREADY_INCLUDED
#include "helpers.h"
namespace utils{
    template <class T, class Hash = std::hash<T>>
    concept hashable = requires(T value, const Hash hash){{hash(value)} -> std::convertible_to<std::size_t>;};
    template <class Hash, class T>
    concept hash_of = utils::hashable<T, Hash>;

    struct BetterNumHash;
    class HashCombiner;

    inline constexpr bool operator==(const utils::BetterNumHash&, const utils::BetterNumHash&) noexcept{return true;}
    inline constexpr bool operator==(const utils::HashCombiner&, const utils::HashCombiner&) noexcept;
}
struct utils::BetterNumHash final{
    public:
        constexpr BetterNumHash() noexcept = default;
        constexpr BetterNumHash(const BetterNumHash&) noexcept = default;
        constexpr BetterNumHash(BetterNumHash&&) noexcept = default;
        constexpr BetterNumHash& operator=(const BetterNumHash&) noexcept = default;
        constexpr BetterNumHash& operator=(BetterNumHash&&) noexcept = default;
        template <class T>
        requires std::integral<T> || std::floating_point<T>
        constexpr std::size_t operator()(T) const noexcept;
};
template <class T>
requires std::integral<T> || std::floating_point<T>
constexpr std::size_t utils::BetterNumHash::operator()(T number) const noexcept{
    std::size_t value = std::hash<T>()(number);
    value += 0x2b880ed60fd5f452;
    value ^= value >> 30;
    value *= 0xbf58476d1ce4e5b9;
    value ^= value >> 27;
    value *= 0x94d049bb133111eb;
    value ^= value >> 31;
    return value;
}
class utils::HashCombiner final{
    public:
        constexpr HashCombiner() noexcept: state(0){}
        constexpr explicit HashCombiner(std::size_t initial_state) noexcept: state(initial_state){};
        constexpr HashCombiner(const HashCombiner& other) noexcept = default;
        constexpr HashCombiner(HashCombiner&& other) noexcept = default;
        constexpr HashCombiner& operator=(const HashCombiner&) noexcept = default;
        constexpr HashCombiner& operator=(HashCombiner&&) noexcept = default;
        constexpr HashCombiner& write(std::size_t) noexcept;
        constexpr HashCombiner& write(std::initializer_list<std::size_t>);
        constexpr std::size_t finalize() const noexcept{return utils::BetterNumHash()(state);}
        friend inline constexpr bool operator==(const HashCombiner&, const HashCombiner&) noexcept = default;
    private:
        std::size_t state = 0;
};
constexpr utils::HashCombiner& utils::HashCombiner::write(std::size_t hash) noexcept{
    hash += 0x2b880ed60fd5f452;
    hash *= 0xc6a4a7935bd1e995;
    hash ^= hash >> 47;
    hash *= 0xc6a4a7935bd1e995;
    state += 0xd477f129f02a0bad;
    state ^= hash;
    state *= 0xc6a4a7935bd1e995;
    state ^= state >> 47;
    state *= 0xc6a4a7935bd1e995;
    state ^= state >> 47;
    state = utils::BetterNumHash()(state);
    return *this;
}
constexpr utils::HashCombiner& utils::HashCombiner::write(std::initializer_list<std::size_t> hashes){
    for (std::size_t hash: hashes) write(hash);
    return *this;
}
#endif