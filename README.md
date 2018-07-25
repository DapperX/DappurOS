#Dappur OS

一个轻量级的小项目，主要用于练手，目前的重点是实现内核的基础功能。

A small project for practice, focusing on implementing basic functions of the kernel now.

开源许可： GPL v2

License: GPL v2



### 描述 (Description)

DappurOS是一个小巧高效、扩展性强的操作系统。其目前只能运行在32位x86平台上，但计划支持其他的位宽和指令集。内核部分主要由汇编和C语言编写；这是一个高度模块化的宏内核，整个内核共享同一地址空间，但按照功能划分为各个模块，于装载时进行链接；各个模块根据约定设置各自的内核调用表(KCT)来实现跨模块的函数调用。

DappurOS is a small but efficient operating system with high augmentability. For now it can only run on the 32-bit x86 platform but it plans to support other bit width and instruction sets. The kernel is written in assembly and C language. It is a macro kernel with high modularity, which means the whole kernel shares the same address space but are divided into modules by their functions. These modules will be linked while loading. Each module will set its kernel call table (KCT) accroding to the convention to achieve cross-module process call.



### 注意事项 (Attention)

1. **注意：本项目的开发环境在32位的Linux上，目前对在其他平台上的编译运行结果不做任何保证。**

   **Notice: The development environment of this project is on 32-bit Linux. The result of compiling or running on other platforms is uncertain.**

2. 本项目的代码注释、文档和另外的说明性文字，应由中文或/和英文书写。在中英双语同时出现的情况下，如果两种语言表意存在冲突，以中文表意为准。

   The code comment, documentation and other illustrative text should written in Chinese or/and English language. In the case that Chinese and English both appear and their meanings are conflicting, it is subject to the meaning in Chinese.



### 编译与运行 (Compile and Run)

本项目的编译运行依赖于如下软件：

To compile and run this project, the following softwares are need:

1. nasm
2. gcc
3. ld
4. ar
5. bochs

请确保你的系统上装有如上软件。之后只需键入

Please your system has installed these softwares. Then just type

```bash
cd kernel
make
```

即可编译并在bochs上运行本项目。

to compile this project and run it on bochs.

如果仅仅需要编译，可以键入

If you just need to compile it, type

```shell
make build
```

你将在`bin`目录下找到编译得到的文件。

and you will find the generated files in the directory `bin`

要获取更多操作说明，请键入

To get more information, please type

```shell
make help
```

及阅读`doc`目录下的说明文档。

and read the documentation in the directory `doc`



### 常见问题 (Q&A)

> 我的终端卡在了提示符界面，同时bochs暂停了执行
>
> The terminal is stuck in the prompt as bochs is suspended

这是由于bochs带有调试功能（类比于gdb），会在**虚拟机启动**及遇到断点时触发调试并暂停执行。要继续执行，请在提示符界面输入`c`并回车。更多bochs命令请参阅[官网](http://bochs.sourceforge.net/)。

Since bochs has debugging function (like gdb), it will be trapped when **starting emulating** or meeting a breakpoint. To continue emulating, just type `c` in the terminal and press `enter`. For more command, please refer to the [bochs website](http://bochs.sourceforge.net/).



> 我的bochs似乎没有遇到上述的情况，不仅如此，它甚至都没有理会代码中的魔术断点`DEBUG_BREAKPOINT`
>
> My bochs seems not to encounter the above case. Moreover it even doesn't respond to the magic breakpoint `DEBUG_BREAKPOINT` in the code.

二进制发布的bochs版本似乎没有调试功能，如果你是通过包管理器（apt等）安装的bochs，就有可能遇到这个问题。如果你确实有调试的需求，请以编译源码的方式重新安装bochs。

The binary releases of bochs may not contain the debug function. If you installed bochs by package managers, such as apt, you can probably meet this problem. If you do need to debug the program, please reinstall the bochs by compling its source code.



> 我能在我的64位Linux上编译并运行此项目吗？
>
> Can I compile and run this project on my 64-bit Linux ?

正如前文所述，我尚未在除开发环境以外的任何平台上进行过测试，所以我无法做出确切的回答。不过理论上来说，通过修改`kernel/makefile`文件，为开头处的`LDFLAG`增加`-m elf_i386`项，以及为`CFLAG`增加`-m32`项后应该就能成功。

As I mentioned, yet there is no testing done on any platform except the development environemnt, so I cannot give a certain answer. Though by modifying `kernel/makefile`, adding `-m elf_i386` to `LDFLAG` and `-m32` to `CFLAG`, it should work fine, theoretically.



> 出现了编译或者链接错误？
>
> A compiling/linking error appears ?

可能是由于编译器/链接器版本不同造成的问题，烦请提交issue，我对此深表感谢。

Possibly caused by different compiler/linker version. Please submit a issue about this and I'll appreciate it.