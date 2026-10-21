#include "student.h"
#include <cstring>
#include <iostream>

void addStudent(Student** students, int& count, int& capacity) {
    if (count >= capacity) {
        std::cout << "满了，正在扩容...\n";
        const int new_capacity = capacity * 2;
        Student* new_students = new Student[new_capacity];
        for (int i = 0; i < count; i++) {
            new_students[i] = (*students)[i];
        }
        delete[] *students;
        *students = new_students;
        capacity = new_capacity;
    }

    Student* s = &(*students)[count];

    // 1. 校验学号唯一 (修复了 students[i]->id 的错)
    while (true) {
        std::cout << "请输入学号: ";
        std::cin >> s->id;
        bool is_duplicate = false;
        for (int i = 0; i < count; i++) {
            if (strcmp(s->id, (*students)[i].id) == 0) {
                is_duplicate = true;
                break;
            }
        }
        if (!is_duplicate) break;
        std::cout << "学号已存在，请重新输入！\n";
    }

    std::cout << "请输入姓名: ";
    std::cin >> s->name;

    // 2. 校验年龄范围 (修复了提示语)
    while (true) {
        std::cout << "请输入年龄(1-100): ";
        std::cin >> s->age;
        if (s->age >= 1 && s->age <= 100) break;
        std::cout << "年龄不合法，请重新输入！\n";
    }

    std::cout << "请输入成绩: ";
    std::cin >> s->score;

    int class_choice;
    std::cout << "请选择班级 (1-3): 1. Class A 2. Class B 3. Class C\n";
    std::cin >> class_choice;
    switch (class_choice) {
        case 1:
            s->class_type = ClassType::kClassA;
            break;
        case 2:
            s->class_type = ClassType::kClassB;
            break;
        case 3:
            s->class_type = ClassType::kClassC;
            break;
        default:
            s->class_type = ClassType::kClassNone;
            break;
    }

    count++;
    std::cout << "添加成功！当前共 " << count << " 人。\n";
}

void deleteStudent(Student** students, int& count){
    if (count == 0) { std::cout << "没有学生可删除\n"; return; }
    char target_id[10];
    std::cout << "请输入要删除的学号: ";
    std::cin >> target_id;
    int target_index = -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(target_id, (*students)[i].id) == 0) {
            target_index = i; break;
        }
    }
    if (target_index == -1) { std::cout << "没找到该名学生\n"; return; }
    for (int i = target_index; i < count - 1; i++) {
        (*students)[i] = (*students)[i + 1];
    }
    count--;
    std::cout << "删除成功,剩余" << count << "人。\n";
}

void printAllStudents(const Student* students, int count){
    if (count == 0) { std::cout << "没有学生数据\n"; return; }
    std::cout <<"---------------------------------\n";
    for (int i = 0; i < count; i++) {
        std::cout << "学号: " << students[i].id
                  << " | 姓名: " << students[i].name
                  << " | 年龄: " << students[i].age
                  << " | 成绩: " << students[i].score
                  << " | 班级: " << static_cast<int>(students[i].class_type) << "\n";
    }
    std::cout << "---------------------------------\n";
}

void modifyStudent(Student* students, int count){
    if (count == 0) { std::cout << "没有学生可修改\n"; return; }

    char target_id[10];
    std::cout << "请输入要修改的学号: ";
    std::cin >> target_id;

    int target_index = -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(target_id, students[i].id) == 0) {
            target_index = i; break;
        }
    }
    if (target_index == -1) { std::cout << "找不到该学生\n"; return; }

    Student* s = &students[target_index];
    std::cout << "找到学生，当前姓名: " << s->name << "\n";

    // 嵌套函数1：输入校验年龄
    auto inputValidAge = [](int& age) {
        while (true) {
            std::cout << "请输入新年龄(1-100): ";
            std::cin >> age;
            if (age >= 1 && age <= 100) break;
            std::cout << "年龄不合法！\n";
        }
    };

    // 嵌套函数2：选择班级
    auto inputClass = [](ClassType& ct) {
        int choice;
        std::cout << "请选择新班级 (1-3): ";
        std::cin >> choice;
        if (choice == 1) {
            ct = ClassType::kClassA;
        } else if (choice == 2) {
            ct = ClassType::kClassB;
        } else if (choice == 3) {
            ct = ClassType::kClassC;
        } else {
            ct = ClassType::kClassNone;
        }
    };

    // 调用嵌套函数
    std::cout << "请输入新姓名: ";
    std::cin >> s->name;
    inputValidAge(s->age);
    std::cout << "请输入新成绩: ";
    std::cin >> s->score;
    inputClass(s->class_type);

    std::cout << "修改成功！\n";
}