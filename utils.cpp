#include <iostream>
#include <cstring>
#include <cctype>

#include "utils.h"

using namespace std;

int get_int_input_validation(const char* prompt_message) {
    char input[256];
    while (true) {
        cout << prompt_message;
        cin.getline(input, 256);

        if (strlen(input) == 0) {
            cout << "[!] You MUST give an input!\n";
            continue;
        }

        bool isInt = true;
        int input_size = strlen(input);
        for (int i = 0; i < input_size ; i ++){
            if (input[i] < '0' || input[i] > '9') {
                isInt = false;
                break;
            }
        }

        if (isInt) {
            return atoi(input);
        } else {
            cout << "[!] Invalid input. Input MUST be INTEGER!\n";
        }

    }
}

char get_yesno_input_validation(const char* prompt_message) {
    char user_input[256];
    while (true) {
        cout << prompt_message;
        cin.getline(user_input,256);

        if (strlen(user_input) == 1) {
            char user_input_char = user_input[0];

            user_input_char = tolower(user_input_char);            
            if (user_input_char == 'y' || user_input_char == 'n') {
                return user_input_char;
            }
        }

        cout << "[!] Invalid choice. Please type 'y' or 'n'.\n";
    }
}

void get_name_input_validation(const char* prompt_message, char* input, int input_size) {
    while(true) {
        cout << prompt_message;
        cin.getline(input, input_size);
        if (strlen(input) != 0 && input[0] != ' ' && input != nullptr){
            break;
        } else {
            cout << "[!] Invalid name. Try again!\n";
        }
    }
}
