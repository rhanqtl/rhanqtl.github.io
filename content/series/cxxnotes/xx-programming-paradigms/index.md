---
title: "第 x 讲：编程范式"
summary: "本文介绍各种常见的编程范式，并展示它们在 C++ 中的实现"
draft: true
---

== 命令式编程

命令式编程一个重要的特点是会对数据对象的状态进行修改，例如：

[source,c++]
----
```c++
template <typename R>
int sum(R &&rng) {
    // 中文
    int ans = 0;
    for (auto el : rng) {
        ans += el;
    }
    return ans;
}
```
----


== 函数式编程

函数式编程中 immutability 是很重要的部分，还是上面的 `sum`，函数式编程是这样实现的：

[source,c++]
----
template <typename Iter>
int sum_internal(Iter first, Iter last, int acc) {
    if (first == last)
        return acc;
    /* const */ int new_acc = acc + *first;
    // Tail-recursion.
    // Only bidirectional iterators provide '+n', so it's a compromise.
    // Luckily, the modification to first is not visible to caller and
    // the function call is the last statement.
    return sum_internal(++first, last, new_acc);
}

template <typename R>
int sum(R &&rng) {
    return sum_internal(rng.cbegin(), rng.cend(), 0);
}
----

== 声明式编程

命令式编程和函数式编程都能够体现计算过程，而声明式编程的一个典型特点是不体现计算过程。例如 SQL 就是典型的声明式编程：

[source,sql]
----
SELECT * FROM t_users WHERE age >= 18;
----

我们只知道有 t_users 这样一张表（被操作的数据）、指定一些条件，但是并不关心（1）数据是如何存储的（底层可能是文本、二进制、CSV 等等）；（2）如何根据条件找到数据。

`sum` 的例子这时候就不是特别有区分度，我们来看看 SystemC 的约束子语言：

[source,c++]
----
BitVec</* width= */ 32, /* is_signed= */ true> x, y, z;
Generator g;
g.add(x < 1024);
g.add(y < 4096);
g.add(x + y < z);
----

`g.add` 的参数实际上称为“约束”，这三条约束是没有顺序关系的，仅仅对变量的取值关系进行了约束，也就是说，我们可以先给 `x` 赋值，再计算 `z` 和 `y`，也可以先给 `z` 赋值，再计算 `x` 和 `y`。
