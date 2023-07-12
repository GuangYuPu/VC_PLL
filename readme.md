# Template of STM32 CubeMX project for EIDE

## 模板做了些什么

- 在 .eide/debug.files.options.yml 中给 `./UserCode/**` 添加 -Wextra 编译参数
- c++ 标准选为 c++17
- "CXX_FLAGS": "-fno-rtti"
- "LIB_FLAGS": "-lm -lstdc++"
- 添加了一些源文件夹

## 获取模板

克隆本仓库，用 EIDE 打开，在项目上右键-导出 EIDE 模板

## 模板使用方法

1. EIDE 新建项目-本地项目模板，选择 .ept 文件
2. 使用 CubeMX 创建 STM32 工程，并注意以下设置：
    - Project Manager 标签下，`Toolchain/IDE` 选择 STM32CubeIDE 并勾选 `Generate Under Root`
    - Project Location 先随便选择一个路径（后续步骤会移动它）
    - 在 Code Generator 页面下，选上 `Copy only the necessary library files` 和 `Generate peripheral initialization as a pair of '.c/.h' files per pperipheral`
3. 在 CubeMX 中生成代码，生成完后打开生成目录，然后关闭 CubeMX
4. 将刚才生成的所有文件移动到 EIDE 工程的 `CubeMX` 文件夹下
    > 注意不要将 .ioc 文件所在的父文件夹也移动过来，  
    > 移动完后 CubeMX 文件夹下应该有 `Core`、`Driver`、`.cproject` 等文件或文件夹

5. 添加包含目录
    1. 在 `CubeMX/.cproject` 中搜索 `Include`，找到如下这段:

        ```xml
        <listOptionValue builtIn="false" value="../Core/Inc"/>
        <listOptionValue builtIn="false" value="../Drivers/STM32F4xx_HAL_Driver/Inc"/>
        <listOptionValue builtIn="false" value="../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy"/>
        <listOptionValue builtIn="false" value="../Drivers/CMSIS/Device/ST/STM32F4xx/Include"/>
        <listOptionValue builtIn="false" value="../Drivers/CMSIS/Include"/>
        ```

    2. 将其填入 `.eide/eide.json` 的 `incList` 中，并修改为对应格式
        > 修改说明：  
        > 删去 `<listOptionValue builtIn="false" value=`  
        > 将 `/>` 改为 `,`，并使最后一项末尾没有逗号  
        > 将 `"../` 改为 `"CubeMX/`  
        > 举例如下：

        ```json
        ...
        "incList": [
        ".eide/deps",
        "CubeMX/Core/Inc",
        "CubeMX/Drivers/STM32F4xx_HAL_Driver/Inc",
        "CubeMX/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy",
        "CubeMX/Drivers/CMSIS/Device/ST/STM32F4xx/Include",
        "CubeMX/Drivers/CMSIS/Include"
        ],
        ...
        ```

6. 添加预处理宏定义
    1. 在刚才的 `CubeMX/.cproject` 中搜索 `USE_HAL_DRIVER`，找到如下这段:

        ```xml
        <listOptionValue builtIn="false" value="DEBUG"/>
        <listOptionValue builtIn="false" value="USE_HAL_DRIVER"/>
        <listOptionValue builtIn="false" value="STM32F407xx"/>
        ```

    2. 在 EIDE 项目的项目属性-预处理宏定义中添加这几个宏，其中 `DEBUG` 可以不添加
        > 添加宏的输入框可以用分号分隔，如输入`USE_HAL_DRIVER;STM32F407xx`

7. 在构建配置中：  
    - 选择合适的 CPU 类型、链接脚本、硬件浮点选项（如果有）  
    - 如果有硬件浮点，则把构建器选项里的硬件浮点ABI选为 hard

8. 编译，应该能编译通过了
9. 配置烧录器

## 工程说明

### 目录结构

- CubeMX: 用于存放 CubeMX 的 ioc 文件和 CubeMX 生成的所有文件
- Docs: 存放一些文档
- System: 如果你修改了 startup 文件、ld 文件，并且有自己写的系统初始化代码，请把这些文件放在这个文件夹
- UserCode: 用于存放用户代码文件
- .eide: EIDE 相关的配置文件
  - debug.files.options.yml: 可以给不同的文件指定不同的编译选项

## 一些可能用到的构建器选项

### 编译器选项

#### -Wextra

开启额外的 warning，这样会打印更多的 warning，更容易找到 bug

#### 选项：One ELF Section per Function 和 One ELF Section per Data

勾上这两个选项分别等价于添加 `-ffunction-sections` 和 `-fdata-sections` 编译选项。

`-ffunction-sections` 和 `-fdata-sections` 用于告诉编译器将每个函数和数据放在单独的段中，以便在链接时可以更好地优化代码大小。

#### -fno-exceptions

-fno-exceptions 是 GCC 编译器的一个选项，它表示禁用 C++ 异常机制。这个选项会让编译器生成更小、更快的代码，但是会使得程序无法使用 C++ 异常机制。如果你的程序中没有使用 C++ 异常机制，那么可以使用这个选项来优化你的程序。

#### -pipe

-pipe 选项告诉 GCC 使用管道而不是临时文件来在编译的各个阶段之间进行通信。这可以加快编译速度，特别是在 I/O 较慢的系统上。

#### -fno-rtti

-fno-rtti 选项告诉 GCC 不要生成与运行时类型信息有关的代码。这可以减小程序的大小并提高编译速度，但是会使得程序无法使用 RTTI（Run-Time Type Identification）功能。

#### --specs=nosys.specs

用于告诉链接器不使用系统调用（system calls）

标准 C 库中的一些功能（比如控制台输入输出、文件读写、获取时间）需要调用操作系统（如 Windows, Linux）提供的函数，这些函数称为 system calls。

但是 STM32 没有操作系统，所以也就没有这些函数（下注），于是链接时就会产生找不到这些函数的错误。加上 `--specs=nosys.specs` 之后，链接器就会自己弱定义这些函数，于是就不会产生上述错误了。

> （注）CubeMX 生成的代码里可能包括 syscalls.c 文件（取决于Toolchain/IDE配置），这里面就定义了所需 system calls. 所以如果有这个文件，也可以不添加这个编译选项

链接器自己定义的这些函数不实现任何功能，只是用于去除链接错误的。（所以如果你此时调用`printf`，并不会有任何东西打印出来）

当然，你也可以自己实现一些系统调用。例如，实现 `_write()` 和 `_read()` 将 `printf` 重定向到串口。链接器会优先链接你定义的这些函数。如果你将所有的系统调用都定义了，就可以不加 `--specs=nosys.specs`

#### --specs=nano.specs

用于告诉编译器和链接器使用 Nano libc. Nano libc 是一个轻量级的 C 库，它的目标是减小可执行文件的大小。使用 Nano libc 可以减小可执行文件的大小，但是会牺牲一些功能。

注意：如果要使用 Nano libc, 请在编译器选项中添加这个选项，不要在链接器选项中添加。在编译器选项中加会同时作用于编译器和链接器，而在链接器选项中加只会作用于链接器，从而导致一些问题，详情见：[issue](https://github.com/github0null/eide/issues/259)。

默认 Nano libc 关闭了 printf 和 scanf 浮点数的功能。如果要使用该功能，需要在链接器选项中添加 `-u _printf_float` 和 `-u _scanf_float`

不添加这一条则会使用标准 C 库。标准 C 库默认支持 printf 和 scanf 浮点数，因此使用标准 C 库时不用添加 `-u _printf_float` 和 `-u _scanf_float`

### 链接器选项

#### 选项：Remove Unused Input Sections

勾上这个选项等价于添加 `-Wl,--gc-sections` 编译选项。

其中，`-Wl` 是 GCC 的编译选项，用于将后面的参数传递给链接器。`--gc-sections`用于告诉链接器在链接时删除未使用的段。

因此，勾上这个选项以及编译器选项里的 `One ELF Section per Function` 和 `One ELF Section per Data`，可以实现删除未使用的函数和数据。

### 链接库

使用 `-lLibraryName` 链接名称为 `LibraryName` 的库。

以下为常用链接库选项：

- `-lm`：数学库，提供了 math.h 中的各种数学函数。（有时候编译器会默认帮你链接这个库）
- `-lstdc++`: 标准 C++ 库，提供了许多 C++ 中的函数。

> 编译时使用 `gcc` 会默认链接 C 库，不会链接 C++ 库；  
> 编译时使用 `g++` 会默认链接 C++ 库；  
> 所以通常情况下使用 `gcc` 编译 .c 文件，使用 `g++` 编译 .cpp 文件  
> 但 EIDE 编译 .cpp 文件时也使用 `gcc` 命令，所以必须指定要链接 C++ 库
