#include "student.h"

#include <fstream>
#include <iostream>
//
// Created by 27811 on 2026/10/21.
//

void saveToFile(const char* filename, const Student* students, int count) {
    std::ofstream out;
    out.open(filename, std::ios::binary);
    if (!out) {
        std::cout << "文件打开失败，无法保存！\n";
        return;
    }
    out.write(reinterpret_cast<char*>(&count), sizeof(int));
    out.write(reinterpret_cast<const char*>(students), sizeof(Student) * count);

    out.close();
    std::cout << "数据已保存到 " << filename << "\n";
}
// 从文件加载
void loadFromFile(const char* filename, Student** students, int& count, int& capacity) {
    std::ifstream in(filename, std::ios::binary);

    if (!in) {
        std::cout << "未找到 " << filename << "，可能是首次运行。\n";
        return;
    }

    // 1. 先读出有多少个学生
    in.read(reinterpret_cast<char*>(&count), sizeof(int));

    if (count < 0 || count > 10000) { // 防脏数据
        count = 0; return;
    }

    // 2. 如果文件里有数据，检查内存够不够，不够就扩容
    if (count > capacity) {
        delete[] *students; // 释放旧的
        capacity = count; // 新容量
        *students = new Student[capacity]; // 申请新内存
    }

    // 3. 把数据读到内存里
    in.read(reinterpret_cast<char*>(*students), sizeof(Student) * count);

    in.close();
    std::cout << "成功加载 " << count << " 个学生数据。\n";
}