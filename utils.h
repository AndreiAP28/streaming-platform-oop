#pragma once

#include <iostream>


int get_int_input_validation(const char* prompt_message);
char get_yesno_input_validation(const char* prompt_message);
void get_name_input_validation(const char* prompt_message, char* input, int input_size);