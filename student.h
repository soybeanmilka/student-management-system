#pragma once

#define MAX_NAME_LEN 20

enum class ClassType {
    kClassA,
    kClassB,
    kClassC,
    kClassNone = -999
};

struct Student{
    char id[10];
    char name[MAX_NAME_LEN];
    int age;
    float score;
    ClassType class_type;
};

// 函数指针类型：排序策略
using CompareFunc = int (*) (const Student*, const Student*);
// 核心函数声明
void addStudent(Student** students, int& count, int& capacity);
void sortStudents(Student* students, int count, CompareFunc compare);
// ... 其他的你自己声明 ...
void deleteStudent(Student** students,int& count);
void printAllStudents(const Student* students, int count);
void modifyStudent(Student* students, int count);

int compareByScoreDesc(const Student* a, const Student* b);
int compareByAgeAsc(const Student* a, const Student* b);

void sortStudents(Student* students, int count, CompareFunc compare);

void saveToFile(const char* filename, const Student* students, int count);
void loadFromFile(const char* filename, Student** students, int& count, int& capacity);

void showStatistics(const Student* students, int count);
void writeLog(const char* action);