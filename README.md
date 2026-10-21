# 学生管理系统（Student Management System）

一个用 C++20 写的控制台学生信息管理程序，用来练习**指针、动态内存、函数指针和二进制文件读写**。

## 功能

| 序号 | 功能 | 说明 |
| --- | --- | --- |
| 1 | 添加学生 | 学号唯一性校验、年龄范围校验（1–100）、班级选择 |
| 2 | 打印所有学生 | 表格形式列出学号 / 姓名 / 年龄 / 成绩 / 班级 |
| 3 | 删除学生 | 按学号定位，后面的元素整体前移 |
| 4 | 按成绩降序 | 冒泡排序 + 函数指针 `compareByScoreDesc` |
| 5 | 按年龄升序 | 冒泡排序 + 函数指针 `compareByAgeAsc` |
| 6 | 修改信息 | 用 lambda 做输入校验（年龄、班级） |
| 7 | 数据统计 | 平均分 / 最高分 / 最低分 |
| 0 | 退出 | 自动保存到 `students.dat` |

程序启动时从 `students.dat` 读取已有数据，退出时写回；每次操作都会往 `system.log` 追加一条带时间戳的日志。

## 目录结构

```
学生管理系统/
├── CMakeLists.txt          构建脚本：项目名、源文件、编译参数
├── CMakePresets.json       预设：MinGW g++ + Ninja (Debug)
├── main.cpp                主程序：菜单循环、调用各功能
├── student.h               数据结构与函数声明（Student、ClassType）
├── student.cpp             增删改查：addStudent / deleteStudent / modifyStudent / printAllStudents
├── sort.cpp                排序策略：函数指针比较器 + 冒泡排序
├── file_io.cpp             二进制文件读写：saveToFile / loadFromFile
├── stat_and_log_file.cpp   统计与日志：showStatistics / writeLog
├── .clang-format           代码格式化规则（clang-format）
└── .clang-tidy             命名与静态检查规则（clang-tidy）
```

## 编译与运行

需要 **CMake ≥ 3.20**、**支持 C++20 的 GCC**（本项目用 MinGW-w64 g++ 16.2.0）和 **Ninja**。

```bash
# 1. 配置（在 build/ 里生成构建文件）
cmake --preset mingw-ninja

# 2. 构建
cmake --build --preset mingw-debug

# 3. 运行
./build/student_manager.exe
```

如果你不用 MinGW，把 `CMakePresets.json` 里的 `CMAKE_CXX_COMPILER` 换成自己的编译器即可，`CMakeLists.txt` 本身不用改。

## 代码里值得看的几个点

- **动态数组**：`Student* students = new Student[capacity]{}`，装满后 `addStudent` 会按 2 倍扩容（申请新内存 → 拷贝 → 释放旧内存）。
- **函数指针**：`using CompareFunc = int (*)(const Student*, const Student*)`，排序时把比较器当参数传进去，策略可替换。
- **二叉写文件的坑**：`out.write(reinterpret_cast<const char*>(students), sizeof(Student) * count)` 直接把结构体内存块写盘，所以 `Student` 里不能有 `std::string` 这类含指针的成员——只能存 POD 的 `char[]`。
- **防脏数据**：`loadFromFile` 里检查 `count < 0 || count > 10000`，避免损坏的文件把内存撑爆。
- **输入校验**：`modifyStudent` 用两个 lambda（`inputValidAge`、`inputClass`）把校验逻辑就地封装。

## 编译参数说明

- `-fexec-charset=GBK`：让运行时输出的中文在 Windows 控制台（默认 GBK 代码页）里不乱码。
- `-Wall -Wextra`：开常见警告。

## 已知限制

- 数据存的是结构体内存快照，换平台 / 换编译器 / 改结构体定义后，旧的 `students.dat` 可能读不出来，删掉重新生成即可。
- 姓名用 `char name[20]` 固定长度，只能存 ASCII 或 GBK 的短名，不支持超长中文名。
- 冒泡排序在数据量大时偏慢（教学用，没换快排）。

## 许可

个人学习项目，随便参考。
