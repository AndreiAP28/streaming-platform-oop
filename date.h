#pragma once

#include <iostream>

class Date {
    int day, month, year;

public: 
    //!-------------------- CONSTRUCTORS ----------------------
    Date();
    Date(int day, int month, int year);
    Date(const Date& other_day);

    //!------------------- DESTRUCTOR -----------------------   
    ~Date() noexcept;

    //! ------------------ GETTERS ----------------------------
    int getDay() const {return day; }
    int getMonth() const {return month; }
    int getYear() const {return year; }
    void formattedDate() const;

    //! ------------------ SETTERS ----------------------------
    void setDate(int d, int m, int y);

};