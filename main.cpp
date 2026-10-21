#include <iostream>

#include "student.h"


int main() {
    // 初始化动态数组(堆内存)
    int capacity = 10;
    int count = 0;
    Student* students = new Student[capacity]{};
    int choice;


    loadFromFile("students.dat", &students, count, capacity);

    while (true) {
        std::cout << "\n1. 添加学生\n2. 打印所有学生\n3.删除一个学生\n4. "
                     "按成绩降序\n5. 按年龄升序\n6. 修改信息\n7. 数据统计\n0. 退出\n请选择: ";
        std::cin >> choice;

        if (choice == 0) {
            break;
        } else if (choice == 1) {
            addStudent(&students, count, capacity);
            writeLog("添加了学生数据");
        } else if (choice == 2) {
            printAllStudents(students, count);
        } else if (choice == 3) {
            deleteStudent(&students, count);
            writeLog("删除了一条学生数据");
        } else if (choice == 4) {
            sortStudents(students, count, compareByScoreDesc); // 传函数指针
            writeLog("执行了按成绩降序排序");
        } else if (choice == 5) {
            sortStudents(students, count, compareByAgeAsc);    // 传函数指针
            writeLog("执行了按年龄升序排序");
        } else if (choice == 6) {
            modifyStudent(students, count);
            writeLog("修改了学生信息");
        } else if (choice == 7) {
            showStatistics(students, count);
        }
    }

    saveToFile("students.dat", students, count);


    delete[] students;
    students = nullptr;
    std::cout << "内存释放成功\n" << std::endl;

    return 0;
}