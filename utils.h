#pragma once

#include <iostream>
#include <string>

int get_int_input_validation(const std::string& prompt_message);
char get_yesno_input_validation(const std::string& prompt_message);
std::string get_name_input_validation(const std::string& prompt_message);