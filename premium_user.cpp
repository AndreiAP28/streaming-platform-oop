#include <iostream>
#include <sstream>
#include "premium_user.h"


//! UPDATE THE CONSTRUCTORS TO ADD THE LIKED SONGS LIST ARRAY TOO 
using namespace std;

//!------------------- CONSTRUCTORS ---------------------

PremiumUser::PremiumUser()
:User(), expirationDate() {
    // cout << "PremiumUser()" << endl;
    setSubsPlan(STANDARD_MONTHLY);
}

PremiumUser::PremiumUser(subsPlan planName)
:User(), expirationDate() {
    // cout<< "PremiumUser()" << endl;
    setSubsPlan(planName);
}

PremiumUser::PremiumUser(int old_id, const string& username, const string& password, const vector<Song*>& liked_songs, subsPlan planName, const Date& expirationDate)
:User(old_id, username, password, liked_songs), expirationDate(expirationDate) {
    // cout << "PremiumUser()" << endl;
    setSubsPlan(planName);
}
//!------------------- GETTERS ---------------------
string PremiumUser::getPlanName() const {
    switch(planName) {
        case STANDARD_MONTHLY:
            return "Standard Monthly";
        case STUDENT_PLAN:
            return "Student Monthly";
        case ANNUAL_DISCOUNT:
            return "Annual Discount Plan";
        case FAMILY_PLAN:
            return "Family Plan";
        default:
            return "Unknown";
    }
}
//!------------------- SETTERS ---------------------
void PremiumUser::setSubsPlan(subsPlan tier) {
    planName = tier;
    int d = expirationDate.getDay();
    int m = expirationDate.getMonth();
    int y = expirationDate.getYear();
    switch(tier){


        case STANDARD_MONTHLY:
            subsFee = 4.99;
            isStudent = false;

            m++;
            if ( m > 12) {
                m = 1;
                y ++;
            }

            expirationDate.setDate(d, m, y);

            cout << "Activated Standard Monthly Plan ($4.99/month)." << endl;
            break;
        
        case ANNUAL_DISCOUNT:
            subsFee = 49.99;
            isStudent = false;
            expirationDate.setDate(d, m, ++y);

            cout << "Activated Annual Plan ($49.99/year)." << endl;
            break;

        case STUDENT_PLAN:
            subsFee = 2.99;
            isStudent = true;

            m++;
            if ( m > 12) {
                m = 1;
                y ++;
            }
            
            expirationDate.setDate(d, m, y);

            cout << "Activated Student Plan ($4.99/mo)." << endl;
            break;
        case FAMILY_PLAN:
            subsFee = 14.99;
            isStudent = false;

            m++;
            if ( m > 12) {
                m = 1;
                y ++;
            }

            cout << "Activated Family Plan ($14.99/mo)." << endl;


    }
}
//!------------------ OTHER METHODS -----------------
void PremiumUser::displayProfile() const {

    User::displayProfile();

    cout << "|                  PREMIUM PLAN                  |\n";
    cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    
    cout << "|  CURRENT PLAN: " << this->getPlanName() << "\n";
    cout << "|  MONTHLY FEE : $" << subsFee << "\n";
    
    cout << "|  STUDENT     : " << (isStudent ? "YES" : "NO") << "\n";
    
    cout << "|  EXPIRES ON  : ";
    expirationDate.formattedDate(); 
    cout << "\n";
    cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
}

string PremiumUser::toString() const {
    string user_standard_profile = User::toString();

    ostringstream os;
    os << "|                  PREMIUM PLAN                  |\n";
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    
    os << "|  CURRENT PLAN: " << this->getPlanName() << "\n";
    os << "|  MONTHLY FEE : $" << subsFee << "\n";
    
    os << "|  STUDENT     : " << (isStudent ? "YES" : "NO") << "\n";
    
    os << "|  EXPIRES ON  : ";
    expirationDate.formattedDate(); 
    os << "\n";
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";

    return user_standard_profile + os.str();
}
//!------------------- DESTRUCTOR ---------------------
PremiumUser::~PremiumUser() {
    // cout << "~PremiumUser()" << endl;
}
