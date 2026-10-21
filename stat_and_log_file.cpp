#include "student.h"

#include <ctime>

#include <iostream>
#include <fstream>

void showStatistics(const Student* students, int count) {
    if (count == 0) {
        std::cout << "没有数据可统计。\n";
        return;
    }
    float sum = 0;
    float max_score = students[0].score;
    float min_score = students[0].score;

    for (int i = 0; i < count; i++) {
        sum += students[i].score;
        if (students[i].score > max_score) {
            max_score = students[i].score;
        }
        if (students[i].score < min_score) {
            min_score = students[i].score;
        }
    }

    std::cout << "--- 成绩统计 ---\n";
    std::cout << "平均分: " << sum / count << "\n";
    std::cout << "最高分: " << max_score << "\n";
    std::cout << "最低分: " << min_score << "\n";
    std::cout << "----------------\n";
}

void writeLog(const char* action) {
    std::time_t t = std::time(nullptr);

    std::tm* now = std::localtime(&t);
    char time_str[26];
    std::strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", now);
    std::ofstream log("system.log", std::ios::app); // app = 追加模式
    if (log) {
        log << "[" << time_str << "]" <<  action << "\n";
        log.close();
    }
}