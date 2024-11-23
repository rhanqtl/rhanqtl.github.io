---
title: "Visual Studio Code C/C++ 开发环境搭建"
date: 2023-02-13T10:50:20+08:00
tags: ["c/c++", "vscode", "toolchain"]
draft: false
---

# 安装 VS Code

[官网](https://code.visualstudio.com)下载

> 小技巧：如果官网下载比较慢，可以把下载地址中的 `az764295.vo.msecnd.net` 更换为 `vscode.cdn.azure.cn`

# 安装 clangd[^clangd-homepage]

[^clangd-homepage]: https://clangd.llvm.org/

在 Arch Linux 上，`clangd` 在 `clang` 这个包中，用如下命令安装：

```bash
$ pacman -S clang
```

> 其他平台可能会使用 `clang-extra-tools` 这个名称

安装完成之后，还需要安装 `llvm-vs-code-extensions.vscode-clangd` 这个扩展。


# 生成 compile\_commands.json

## CMake

> 当然你先需要安装 CMake

项目目录结构如下：

```plaintext
.
|- CMakeLists.txt
|- include
|  `- demo
|     `- Support
|        `- Strings.h
`- src
   `- main.cpp
   `- Support
      `- Strings.cpp
```

使用

```bash
cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=1
```

> CMake 目前（2023 年）只能支持 GNU Make 和 Ninja 生成 `compile_commands.json`

配置项目，然后你会看到 `build` 目录下有一个 `compile_commands.json` 文件。为了兼容其他工具（例如 GNU Make），我们能够在项目根目录下访问到 `compile_commands.json`（否则的话你可能要在 `.vscode/settings.json` 中加入一大坨 clangd 的参数来覆盖默认值），在使用 CMake 的情况下，我们可以使用符号链接：

```plaintext
$ ln -s $PWD/build/compile_commands.json $PWD/compile_commands.json
```

## GNU Make

很不幸，GNU Make 没有内置的生成 `compile_commands.json` 的功能，但是可以通过 `bear` 工具来实现：

```plaintext
$ make clean
$ bear -- make
```

> 在 Arch Linux 中，使用 `pacman -S bear` 就可以安装该工具

会在 `$PWD` 生成 `compile_commands.json`。

> 网上有一些不使用外部工具，只用 `make` 生成的办法（例如通过 `make` 的 `--dry-run` 选项），但是这些办法生成的文件用起来有点问题，还是推荐使用 `bear`，这也是 clangd 官方推荐的工具

# 配置 clangd

如果你安装了 M$ 默认的 cpp-tools，首先需要在 VS Code 中将一些功能关掉（设为 `disabled`），包括：

- `C_Cpp.autocomplete`
- `C_Cpp.errorSquiggles`
- `C_Cpp.intelliSenseEngine`
- `C_Cpp.intelliSenseEngineFallback`

> 在配置之前，你应该会看到 `*.cpp` 文件中的 `#include "stringutil.hpp"` 报错，因为这个头文件在当前目录下找不到。

然后，打开 clangd 插件的配置，在“Clangd: Arguments”加入如下的内容：

```text
--background-index
--completion-style=detailed
--all-scopes-completion
--clang-tidy
--enable-config
--log=verbose
--pretty
--pch-storage=memory
--header-insertion=iwyu
--ranking-model=heuristics
--completion-parse=auto
--fallback-style=Google
--compile-commands-dir=${workspaceFolder}
-j=2
```

当然，你也可以选择在项目根目录下创建 `.vscode/settings.json` 文件然后将如下内容加入：

```json
{
    "clangd.arguments": [
        "--all-scopes-completion",
        "--background-index",
        "--clang-tidy",
        "--compile-commands-dir=build",
        "--completion-parse=auto",
        "--completion-style=detailed",
        "--enable-config",
        "--fallback-style=Google",
        "--header-insertion=iwyu",
        "--log=verbose",
        "--pch-storage=memory",
        "--pretty",
        "--ranking-model=heuristics"
    ]
}
```

然后使用 `Ctrl-Shift-P` 开启命令面板，选择 `clangd: Restart language server` 运行，应该会有如下现象：

* 项目根目录下出现了一个 `.cache` 目录，这是 clangd 存储编译数据库的地方
* 之前的报错消失了（除了因为没有 `#include` 导致找不到符号的问题）
* 底部的状态栏出现了“clangd:idle”字样（下图左半部分从右向左第三列）

    ![](./images/status-bar.png)

# 调试

安装插件 CodeLLDB（ID 为 `vadimcn.vscode-lldb`）。在 `launch.json` 加入如下内容：

```json
{
    // Use IntelliSense to learn about possible attributes.
    // Hover to view descriptions of existing attributes.
    // For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug",
            "type": "lldb",
            "request": "launch",
            "program": "${workspaceFolder}/build/a.out",
            "args": [ "-arg1", "-arg2" ],
            // "preLaunchTask": ""
        }
    ]
}
```

# 参考

1. [My C/C++ Dev Setup with VSCode - ANT-HEM'S TECH BLOG](https://ahemery.dev/2020/08/24/c-cpp-vscode/)
2. [为vscode配置clangd](https://juejin.cn/post/7126880493668139021)
3. [CodeLLDB Manual](https://github.com/vadimcn/codelldb/blob/v1.9.0/MANUAL.md)
4. https://clangd.llvm.org/installation
