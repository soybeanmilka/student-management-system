# ============================================================
# 学生管理系统 —— CMake 构建脚本
#
# 这个文件回答三个问题：
#   1. 这个项目叫什么、用什么语言、要什么标准；
#   2. 由哪些源文件编成哪个可执行文件；
#   3. 编译时加什么参数。
#
# 它不回答"用 Ninja 还是 MinGW Makefiles""编到哪个目录"——那些由
# 生成器和 CMakePresets.json 决定。所以换构建工具、换 IDE 时，
# 这个文件不用改。
#
# 常用命令（在本目录执行）：
#   cmake --preset mingw-ninja          # 配置：在 build/ 里生成构建文件
#   cmake --build --preset mingw-debug  # 构建
#   build/student_manager.exe           # 运行
# ============================================================

cmake_minimum_required(VERSION 3.20)

# 项目名用英文标识符：它会进 Makefile / 解决方案文件名，
# 中文项目名在部分生成器上会出编码问题。显示用的名字放注释里就够了。
project(StudentManager VERSION 0.1.0 LANGUAGES CXX)

# 告诉 GCC 编译器，把执行字符集设为 GBK

# C++20：和 .clang-format 里的 Standard: c++20、
# 以及 g++ 16.2.0 的默认标准保持一致。
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # 关掉 gnu++20 扩展，用标准 C++20

# 生成 build/compile_commands.json：clangd / clang-tidy 可以据此拿到
# 真实的编译参数，而不是自己猜。
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 源文件逐个列出来，不用 file(GLOB *.cpp)：
#   - 新增文件时显式加一行，构建系统立刻知道；
#   - GLOB 的代价是"加了文件但没重新 configure 就编不到"，新手极容易中招。
add_executable(student_manager
    main.cpp
    student.cpp
        file_io.cpp
        sort.cpp
        stat_and_log_file.cpp
)
target_compile_options(student_manager PRIVATE -fexec-charset=GBK)
# 告警级别与 acm.ps1、clangd 保持一致。MinGW/GCC 用 -Wall -Wextra；
# 如果哪天换 MSVC，这里要改成 /W4。
target_compile_options(student_manager PRIVATE -Wall -Wextra)

# 统一可执行文件的位置：单配置生成器（Ninja、MinGW Makefiles）默认放
# build/ 根目录，多配置生成器（Visual Studio）会多一层 Debug/ Release/。
# 显式统一成 build/ 根目录，避免"我明明编了，怎么找不到 exe"。
set_target_properties(student_manager PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY         "${CMAKE_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}"
)
