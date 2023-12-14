#pragma once
#include "Utils.h"
std::string Encrypt(std::string str, int right_Shift, int left_shift) {
    std::string encrypted;
    std::vector<int> positions;
    encrypted = string_to_lower(str);
    for (int i = 0; i < encrypted.length(); i += 2) {
        if (isalpha(encrypted[i])) {
            encrypted[i] = encrypted[i] + right_Shift;
            if (encrypted[i] > 122) {
                encrypted[i] = encrypted[i] - 26;
            }
            else if (encrypted[i] < 97) {
                encrypted[i] = encrypted[i] + 26;
            }

        }
    }
    for (int i = 1; i < encrypted.length(); i += 2) {
        if (encrypted[i]>91 && encrypted[i]<126) {
            encrypted[i] = encrypted[i] - left_shift;
            if (encrypted[i] > 126) {
                encrypted[i] = encrypted[i] - 36;
            }
            else if (encrypted[i] < 91) {
                encrypted[i] = encrypted[i] + 36;
            }
        }
    }
    std::reverse(encrypted.begin(), encrypted.end());
    return encrypted;
}

