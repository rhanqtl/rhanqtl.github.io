#ifndef CPPNOTES_DBG_TYPETRAITS_H
#define CPPNOTES_DBG_TYPETRAITS_H

#include <type_traits>

#include <map>
#include <unordered_map>
#include <vector>

template <typename T> struct is_sequential_container : std::false_type {};

template <typename T>
struct is_sequential_container<std::vector<T>> : std::true_type {};

template <typename T>
inline constexpr bool is_sequential_container_v =
    is_sequential_container<T>::value;

template <typename T> struct is_associative_container : std::false_type {};

template <typename K, typename V>
struct is_associative_container<std::map<K, V>> : std::true_type {};

template <typename K, typename V>
struct is_associative_container<std::unordered_map<K, V>> : std::true_type {};

template <typename T>
inline constexpr bool is_associative_container_v =
    is_associative_container<T>::value;

template <typename T> struct is_container : std::false_type {};

template <typename T> struct is_container<std::vector<T>> : std::true_type {};

template <typename K, typename V>
struct is_container<std::map<K, V>> : std::true_type {};

template <typename K, typename V>
struct is_container<std::unordered_map<K, V>> : std::true_type {};

template <typename T>
inline constexpr bool is_container_v = is_container<T>::value;

#endif // CPPNOTES_DBG_TYPETRAITS_H
