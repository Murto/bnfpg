#pragma once

namespace bnfpg {

template <typename...>
struct list;

template <typename from, template <typename...> typename to>
struct rewrap;

template <template <typename...> typename from, typename... es, template <typename...> typename to>
struct rewrap<from<es...>, to> {
  using type = to<es...>;
};

} // namespace bnfpg
