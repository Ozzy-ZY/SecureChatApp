#include "Utils.h"
std::string Decrypt(std::string str,int shift_right,int shift_left) {
	std::reverse(str.begin(), str.end());
	for (int i = 0; i < str.length(); i += 2) {
		if (isalpha(str[i])) {
			str[i] = str[i] - shift_right;
			if (str[i] > 122) {
				str[i] = str[i] - 26;
			}
			else if (str[i] < 97) {
				str[i] = str[i] + 26;
			}
		}
		else
			continue;
	}
	for(int i =1;i<str.length();i+=2){
		if (str[i] > 91 && str[i] < 126) {
			str[i] = str[i] + shift_left;
			if (str[i] > 126) {
				str[i] = str[i] - 36;
			}
			else if (str[i] < 91) {
				str[i] = str[i] + 36;
			}
		}
		else 
			continue;
	}
	return str;


}
