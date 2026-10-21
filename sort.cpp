#include "student.h"

#include <iostream>


int compareByScoreDesc(const Student* a, const Student* b){
    if (a->score < b->score) return 1;
    if (a->score > b->score) return -1;
    return 0;
}

int compareByAgeAsc(const Student* a, const Student* b){
    if (a->age > b->age) return 1;
    if (a->age < b->age) return -1;
    return 0;
}

void sortStudents(Student* students, int count, CompareFunc compare) {
    if (count <= 1) {
        return;
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - 1 - i; j++) {
            if (compare(&students[j], &students[j+1]) > 0 ) {
                Student temp = students[j];
                students[j] = students[j+1];
                students[j+1] = temp;
            }
        }
    }
    std::cout << "排序完成\n";
}