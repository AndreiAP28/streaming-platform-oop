#include <iostream>

#include "utils.h"

using namespace std;

int get_int_input_validation(const string& prompt_message) {
    string input;
    while (true) {
        cout << prompt_message;
        getline(cin, input);

        if (input.empty()) {
            cout << "[!] You MUST give an input!\n";
            continue;
        }

        bool isInt = true;
        for (int i = 0; i < static_cast<int>(input.size()) ; i ++){
            if (input[i] < '0' || input[i] > '9') {
                isInt = false;
                break;
            }
        }

        if (isInt) {
            return stoi(input);
        } else {
            cout << "[!] Invalid input. Input MUST be INTEGER!\n";
        }

    }
}

char get_yesno_input_validation(const string& prompt_message) {
    string user_input;
    while (true) {
        cout << prompt_message;
        getline(cin, user_input);

        if (user_input.size() == 1) {
            char user_input_char = user_input[0];

            user_input_char = tolower(user_input_char);            
            if (user_input_char == 'y' || user_input_char == 'n') {
                return user_input_char;
            }
        }

        cout << "[!] Invalid choice. Please type 'y' or 'n'.\n";
    }
}

string get_name_input_validation(const string& prompt_message) {
    string input;
    while(true) {
        cout << prompt_message;
        getline(cin, input);

        if (!input.empty() && input[0] != ' ') {
            return input;
        }

        cout << "[!] Invalid name. Try again!\n";
    }
}
