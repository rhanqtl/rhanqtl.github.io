#ifndef COROUTINE__H
#define COROUTINE__H

namespace co {
class Coroutine {};

template <typename F>
Coroutine make(F&& f);

template <typename T>
void yield(T&& value);

template <typename Awaitable>
void await(Awaitable&&);
}  // namespace co

#endif  // COROUTINE__H
