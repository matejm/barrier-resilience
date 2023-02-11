#ifndef UTILS_UTILS_HPP
#define UTILS_UTILS_HPP

#include <variant>

// Some functional helpers found here:
// https://github.com/AVasK/vx/blob/main/vx.hpp

template<typename... Fs>
struct match : Fs ... {
    using Fs::operator()...;

    // constexpr match(Fs &&... fs) : Fs{fs}... {}
};
template<class... Ts> match(Ts...) -> match<Ts...>;

template<typename... Ts, typename... Fs>
constexpr decltype(auto) operator|(std::variant<Ts...> const &v, match<Fs...> const &match) {
    return std::visit(match, v);
}

#endif //UTILS_UTILS_HPP