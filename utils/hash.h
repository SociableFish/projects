#ifndef UTILS_HASH_H_IS_ALREADY_INCLUDED_UTILS_HASH_H_IS_ALREADY_INCLUDED_UTILS_HASH_H_IS_ALREADY_INCLUDED
#define UTILS_HASH_H_IS_ALREADY_INCLUDED_UTILS_HASH_H_IS_ALREADY_INCLUDED_UTILS_HASH_H_IS_ALREADY_INCLUDED
#include "helpers.h"
namespace utils{
    namespace detail{
        template <class T, template <class> class Map, class U>
        struct MapTupleTypesHelper{};
        template <class T, template <class> class Map, std::size_t... Ints>
        struct MapTupleTypesHelper<T, Map, std::integer_sequence<std::size_t, Ints...>>: std::type_identity<std::tuple<Map<std::tuple_element_t<Ints, T>>...>>{};
        template <class T, template <class> class Map>
        using MapTupleTypes = utils::detail::MapTupleTypesHelper<T, Map, std::make_index_sequence<std::tuple_size_v<T>>>::type;
    }
    template <class T, class Hash = std::hash<T>>
    concept hashable = requires(T value, const Hash hash){{hash(value)} -> std::convertible_to<std::size_t>;} && std::semiregular<Hash>;
    template <class Hash, class T>
    concept hash_of = utils::hashable<T, Hash>;

    struct BetterNumHash;
    class HashCombiner;
    template <class T, class Hashes = utils::detail::MapTupleTypes<T, std::hash>>
    requires (std::tuple_size_v<T> == std::tuple_size_v<Hashes>)
    class TupleLikeHash;
    template <class T, utils::hash_of<typename T::value_type> Hash = std::hash<typename T::value_type>>
    class LinearContainerHash;

    inline constexpr bool operator==(const utils::BetterNumHash&, const utils::BetterNumHash&) noexcept{return true;}
    inline constexpr bool operator==(const utils::HashCombiner&, const utils::HashCombiner&) noexcept;
    template <class T, class Hashes>
    inline constexpr bool operator==(const utils::TupleLikeHash<T, Hashes>& lhs, const utils::TupleLikeHash<T, Hashes>& rhs){return lhs.hashes() == rhs.hashes();}
    template <class T, class Hash>
    inline constexpr bool operator==(const utils::LinearContainerHash<T, Hash>& lhs, const utils::LinearContainerHash<T, Hash>& rhs){return lhs.hash() == rhs.hash();}
}
struct utils::BetterNumHash final{
    public:
        constexpr BetterNumHash() noexcept{}
        constexpr BetterNumHash(const BetterNumHash&) noexcept{}
        constexpr BetterNumHash(const BetterNumHash&&) noexcept{}
        constexpr BetterNumHash& operator=(const BetterNumHash&) noexcept{return *this;}
        constexpr BetterNumHash& operator=(const BetterNumHash&&) noexcept{return *this;}
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
        constexpr explicit HashCombiner(std::size_t initial_state) noexcept: state(initial_state){}
        constexpr HashCombiner(const HashCombiner& other) noexcept{state = other.state;}
        constexpr HashCombiner(const HashCombiner&& other) noexcept{state = std::move(other.state);}
        constexpr HashCombiner& operator=(const HashCombiner&) noexcept = default;
        constexpr HashCombiner& operator=(const HashCombiner&&) noexcept;
        constexpr HashCombiner& write(std::size_t) noexcept;
        constexpr HashCombiner& write(std::initializer_list<std::size_t>);
        constexpr std::size_t finalize() const noexcept{return utils::BetterNumHash()(state);}
        friend inline constexpr bool operator==(const HashCombiner&, const HashCombiner&) noexcept = default;
    private:
        std::size_t state = 0;
};
constexpr utils::HashCombiner& utils::HashCombiner::operator=(const utils::HashCombiner&& rhs) noexcept{
    if (this != std::addressof(rhs)) state = std::move(rhs.state);
    return *this;
}
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
template <class T, class Hashes>
requires (std::tuple_size_v<T> == std::tuple_size_v<Hashes>)
class utils::TupleLikeHash final{
    public:
        constexpr TupleLikeHash(): internal_hashes(){}
        constexpr TupleLikeHash(const Hashes& hashes): internal_hashes(hashes){}
        constexpr TupleLikeHash(Hashes&& hashes): internal_hashes(std::move(hashes)){}
        constexpr TupleLikeHash(const TupleLikeHash&) = default;
        constexpr TupleLikeHash(TupleLikeHash&&) = default;
        constexpr TupleLikeHash& operator=(const TupleLikeHash&) = default;
        constexpr TupleLikeHash& operator=(TupleLikeHash&&) = default;
        constexpr std::size_t operator()(const T& value) const{return hash(value, std::make_index_sequence<std::tuple_size_v<T>>());}
        constexpr const Hashes& hashes() const{return internal_hashes;}
    private:
        template <std::size_t... Ints>
        constexpr std::size_t hash(const T&, std::integer_sequence<std::size_t, Ints...>) const;
        Hashes internal_hashes;
};
template <class T, class Hashes>
requires (std::tuple_size_v<T> == std::tuple_size_v<Hashes>)
template <std::size_t... Ints>
constexpr std::size_t utils::TupleLikeHash<T, Hashes>::hash(const T& value, std::integer_sequence<std::size_t, Ints...>) const{
    std::vector<std::size_t> hashes = {std::invoke_r<std::size_t>(std::get<Ints>(internal_hashes), std::get<Ints>(value))...};
    utils::HashCombiner combiner;
    for (std::size_t i: hashes) combiner.write(i);
    return combiner.finalize();
    return 0;
}
template <class T, utils::hash_of<typename T::value_type> Hash>
class utils::LinearContainerHash final{
    public:
        constexpr LinearContainerHash(): internal_hash(){}
        constexpr LinearContainerHash(const Hash& hash): internal_hash(hash){}
        constexpr LinearContainerHash(Hash&& hash): internal_hash(std::move(hash)){}
        constexpr LinearContainerHash(const LinearContainerHash&) = default;
        constexpr LinearContainerHash(LinearContainerHash&&) = default;
        constexpr LinearContainerHash& operator=(const LinearContainerHash&) = default;
        constexpr LinearContainerHash& operator=(LinearContainerHash&&) = default;
        constexpr std::size_t operator()(const T& value) const;
        constexpr const Hash& hash() const{return internal_hash;}
    private:
        Hash internal_hash;
};
template <class T, utils::hash_of<typename T::value_type> Hash>
constexpr std::size_t utils::LinearContainerHash<T, Hash>::operator()(const T& value) const{
    utils::HashCombiner combiner = utils::HashCombiner(value.size());
    for (const auto& i: value) combiner.write(std::invoke_r<std::size_t>(internal_hash, i));
    return combiner.finalize();
}
#endif