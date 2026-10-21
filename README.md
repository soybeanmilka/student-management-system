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
| 6 | 修改信息 | 用 lambda 封装输入逻辑（年龄做校验、班级做映射） |
| 7 | 数据统计 | 平均分 / 最高分 / 最低分 |
| 0 | 退出 | 自动保存到 `students.dat` |

程序启动时从 `students.dat` 读取已有数据，退出时写回。

日志方面：**添加、删除、两种排序、修改**这 5 个操作会往 `system.log` 追加一条带时间戳的记录
（见 `main.cpp` 里对应的 `writeLog(...)` 调用）；**打印（2）和统计（7）不写日志**。

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
├── README.md               本文件
├── .gitignore              忽略构建产物与运行时数据
├── .gitattributes          统一换行符（仓库内 LF）
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

### 换编译器 / 换平台

项目默认按 **GCC (MinGW)** 配置：`CMakePresets.json` 里写死了 `CMAKE_CXX_COMPILER`，
`CMakeLists.txt` 里也用了两个 GCC 专属选项（`-fexec-charset=GBK`、`-Wall -Wextra`）。

- **换成别的 GCC / Clang 路径**：只改 `CMakePresets.json` 里的 `CMAKE_CXX_COMPILER` 就够了，`CMakeLists.txt` 不用动。
- **换成 MSVC (cl.exe)**：`CMakeLists.txt` **也必须改** —— 把 `-Wall -Wextra` 换成 `/W4`，并删掉 `-fexec-charset=GBK`
  （MSVC 没有这个选项，会直接构建失败）。删掉之后 Windows 控制台里的中文可能乱码，需要另外用
  `SetConsoleOutputCP(CP_UTF8)` 或 `system("chcp 65001")` 处理。

## 代码里值得看的几个点

- **动态数组**：`Student* students = new Student[capacity]{}`，装满后 `addStudent` 会按 2 倍扩容（申请新内存 → 拷贝 → 释放旧内存）。
- **函数指针**：`using CompareFunc = int (*)(const Student*, const Student*)`，排序时把比较器当参数传进去，策略可替换。
- **二进制写文件的坑**：`out.write(reinterpret_cast<const char*>(students), sizeof(Student) * count)` 直接把结构体内存块写盘，所以 `Student` 里不能有 `std::string` 这类含指针的成员——只能存 POD 的 `char[]`。
- **防脏数据**：`loadFromFile` 里检查 `count < 0 || count > 10000`，避免损坏的文件把内存撑爆。
- **lambda 就地封装**：`modifyStudent` 里用两个 lambda —— `inputValidAge` 会循环校验年龄必须落在 1–100，
  `inputClass` 只把输入映射成枚举值（选 1/2/3 之外的任何东西都会变成 `kClassNone`），不做重问。

## 编译参数说明

- `-fexec-charset=GBK`：让运行时输出的中文在 Windows 控制台（默认 GBK 代码页）里不乱码。
- `-Wall -Wextra`：开常见警告。

## 已知限制

- 数据存的是结构体内存快照，换平台 / 换编译器 / 改结构体定义后，旧的 `students.dat` 可能读不出来，删掉重新生成即可。
- 姓名用 `char name[20]` 固定长度，只放得下 19 字节以内（约 6 个中文），不支持超长名字。
- 冒泡排序在数据量大时偏慢（教学用，没换快排）。
- `loadFromFile` 扩容后直接把 `capacity` 设成 `count`，不留余量，所以下次添加必然再触发一次扩容。
- `sortStudents` 在 `student.h` 里声明了两次（第 24 行和第 33 行），不影响编译，但属于冗余。

## 已知问题（尚未修复）

这些都是"输入不按套路来"时会暴露的问题，正常按提示输入不会碰到：

1. **班级显示差 1**：`printAllStudents` 打印的是 `static_cast<int>(class_type)`，而枚举 `kClassA/B/C` 是 `0/1/2`，
   录入时菜单却是 `1/2/3`。所以录入"1. Class A"会打印成"班级: 0"，录入"2"打印成"班级: 1"，非法值显示 `-999`。
   修法：改成 `static_cast<int>(...) + 1`，或者用 `switch` 打印 A / B / C。
2. **定长数组 + 不带宽度限制的 `cin >>` 会越界**：`char id[10]`、`char name[20]` 直接用
   `std::cin >> s->id` / `>> s->name` 接收，输入的字符串比数组长就会写穿缓冲区。
   修法：`std::cin >> std::setw(10) >> s->id;`（注意 `setw` 只作用于紧邻的一次读取，要每次都写）。
3. **数字字段输错会死循环**：在"请输入年龄"处输入 `abc`，`cin` 置上 failbit 且不再恢复，
   后面的循环会立刻失败 → 无限刷"年龄不合法"。修法：读取失败时 `std::cin.clear()` 并
   `std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n')` 把坏输入丢掉。
4. **菜单输错会静默退出**：`std::cin >> choice` 失败时 C++11 会把 `choice` 置 0，程序就把这当成"选了 0 退出"
   并保存退出，既不报错也不重问。
5. **`loadFromFile` 不校验实际读到的字节数**：`file_io.cpp` 里那次 `in.read(...)` 的返回值没检查；
   且扩容分支用的是 `new Student[capacity]`（没加 `{}`），截断的 `students.dat` 会把未初始化的内存读出来。

## 许可

个人学习项目，随便参考。
