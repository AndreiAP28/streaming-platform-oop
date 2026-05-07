#include <iostream>
#include <ctime>

#include "date.h"

using namespace std;

Date::Date() {
    cout << "Date()" << endl;

    time_t now = time(0);

    tm* ltm = localtime(&now);
    day = ltm->tm_mday;
    month = ltm->tm_mon + 1;
    year = ltm->tm_year + 1900;

    month ++;
    if (month > 12) {
        month = 1;
        year ++;
    }

}

Date::Date(int day, int month, int year)
    :day(day), month(month), year(year) {
        cout << "Date(int d, int m, int y)" << endl;
    };


void Date::setDate(int d, int m ,int y) {
        //todo instead of if do try/except
        if (d > 0 && d <= 31 && m >= 1 && m <= 12 && y >= 1900 && y < 2500 ) {
            day = d; month = m; year = y;
        } else {
            cout << "[!] Invalid date format!" << endl;
        }
    }


void Date::formattedDate() const {
    if (day < 10) {
        cout << "0";
    }
    cout << day << "/";

    if (month < 10) {
        cout << "0";
    }
    cout << month << "/" << year;

}


Date::~Date() {
    cout << "~Date()" << endl;
}