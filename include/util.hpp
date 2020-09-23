#pragma once

#include <type_traits>

namespace bnfpg {

template <typename...>
struct list;

template <typename container>
struct size;

template <template <typename...> typename container, typename e, typename... es>
struct size<container<e, es...>> {
  constexpr static const size_t value = 1 + size<container<es...>>::value;
};

template <template <typename...> typename container>
struct size<container<>> {
  constexpr static const size_t value = 0;
};

template <typename element, typename container>
struct prepend;

template <typename element, template <typename...> typename container, typename... es>
struct prepend<element, container<es...>> {
  using type = container<element, es...>;
};

template <typename element, typename container>
struct remove;

template <typename element, template <typename...> typename container, typename... es>
struct remove<element, container<element, es...>> {
  using type = typename remove<element, container<es...>>::type;
};

template <typename element, template <typename...> typename container, typename e, typename... es>
struct remove<element, container<e, es...>> {
  using type = typename prepend<e, typename remove<element, container<es...>>::type>::type;
};

template <typename element, template <typename...> typename container>
struct remove<element, container<>> {
  using type = container<>;
};

template <typename element, typename container>
struct append;

template <typename element, template <typename...> typename container, typename... es>
struct append<element, container<es...>> {
  using type = container<es..., element>;
};

template <typename element, typename container>
struct contains;

template <typename element, template <typename...> typename container, typename... es>
struct contains<element, container<element, es...>> {
  constexpr static const bool value = true;
};

template <typename element, template <typename...> typename container, typename e, typename... es>
struct contains<element, container<e, es...>> {
  constexpr static const bool value = contains<element, container<es...>>::value;
};

template <typename element, template <typename...> typename container>
struct contains<element, container<>> {
  constexpr static const bool value = false;
};

template <typename container>
struct unique;

template <template <typename...> typename container, typename e, typename... es>
struct unique<container<e, es...>> {
  using on_contains = typename unique<container<es...>>::type;
  using on_not_contains = typename prepend<e, on_contains>::type;
  using type = typename std::conditional<contains<e, container<es...>>::value, on_contains, on_not_contains>::type;
};

template <template <typename...> typename container>
struct unique<container<>> {
  using type = container<>;
};


template <typename... containers>
struct concat;

template <template <typename...> typename container, typename... ls, typename... rs, typename... rest>
struct concat<container<ls...>, container<rs...>, rest...> {
  using type = typename concat<container<ls..., rs...>, rest...>::type;
};

template <typename container>
struct concat<container> {
  using type = container;
};

template <>
struct concat<> {
  using type = list<>;
};

template <typename left, typename right>
struct difference;

template <typename... ls, typename r, typename... rs>
struct difference<list<ls...>, list<r, rs...>> {
  using left = typename remove<r, list<ls...>>::type;
  using type = typename difference<left, list<rs...>>::type;
};

template <typename... ls>
struct difference<list<ls...>, list<>> {
  using type = list<ls...>;
};

template <typename from, template <typename...> typename to>
struct rewrap;

template <template <typename...> typename from, typename... es, template <typename...> typename to>
struct rewrap<from<es...>, to> {
  using type = to<es...>;
};

template <template <typename> typename wrapper, typename container>
struct map;

template <template <typename> typename wrapper, template <typename...> typename container, typename... es>
struct map<wrapper, container<es...>> {
  using type = container<typename wrapper<es>::type...>;
};

template <template <typename> typename pred, typename container>
struct filter;

template <template <typename> typename pred, template <typename...> typename container, typename e, typename... es>
struct filter<pred, container<e, es...>> {
  using pred_false = typename filter<pred, container<es...>>::type;
  using pred_true = typename prepend<e, pred_false>::type;
  using type = typename std::conditional<pred<e>::value, pred_true, pred_false>::type;
};

template <template <typename> typename pred, template <typename...> typename container>
struct filter<pred, container<>> {
  using type = container<>;
};

template <template <typename, typename> typename func, typename init, typename container>
struct foldl;

template <template <typename, typename> typename func, typename init, template <typename...> typename container, typename e, typename... es>
struct foldl<func, init, container<e, es...>> {
  using type = typename foldl<func, typename func<init, e>::type, container<es...>>::type;
};

template <template <typename, typename> typename func, typename init, template <typename...> typename container>
struct foldl<func, init, container<>> {
  using type = init;
};

} // namespace bnfpg
