#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <chrono>
char generateRandDigit() {
    srand(time(0));
    return ((rand() % 10) + '0');
}
bool isVowel(char ch) {
    switch (ch) {
    case 97:
        return 1;
        break;
    case 101:
        return 1;
        break;
    case 105:
        return 1;
    case 111:
        return 1;
        break;
    case 117:
        return 1;
        break;
    default:
        return 0;

    }
}
std::string string_to_lower(std::string& given) {
    std::string lower = given;
    for (short i = 0; i < given.size(); i++) {
        lower[i] = tolower(given[i]);
    }
    return lower;
}
std::string removeDigits(std::string str) {
    std::string result = "";
    for (int i = 0; i < str.length(); i++) {
        if (isdigit(str[i])) {
            continue;
        }
        result += str[i];
    }

    return result;
}
