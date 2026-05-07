#include <iostream>
#include <cstring>
#include "family_premium_user.h"
//! TO NOT FORGET TO CALL THE CONSTRUCTORS FOR THE BASE CLASSES FOR BOTH FAMILYPREMUMUSER AND PREMIUM_USER 
using namespace std;

//!----------------------- CONSTRUCTORS ---------------------
FamilyPremiumUser::FamilyPremiumUser()
:PremiumUser(FAMILY_PLAN), connectedAccounts(0), usernames(nullptr) {
    cout << "FamilyPremiumUser()" << endl;
}

FamilyPremiumUser::FamilyPremiumUser(int old_id, const char* username, const char* password, Song** liked_songs, int likedSongs_num, const Date& expirationDate)
:PremiumUser(old_id, username, password, liked_songs, likedSongs_num,FAMILY_PLAN, expirationDate), connectedAccounts(0) {
    usernames = new char*[MAX_ACCOUNTS];

    for (int i = 0; i < MAX_ACCOUNTS; i ++) {
        usernames[i] = nullptr; 
    }

    cout << "FamilyPremiumUser()" << endl;
}

FamilyPremiumUser::FamilyPremiumUser(const FamilyPremiumUser& other_user):
 PremiumUser(other_user), connectedAccounts(other_user.connectedAccounts) {

    if (other_user.MAX_ACCOUNTS && other_user.usernames != nullptr) {
        usernames = new char*[MAX_ACCOUNTS];
        for (int i = 0; i < MAX_ACCOUNTS; i ++) {
            usernames[i] = new char[strlen(other_user.usernames[i]) + 1];
            strcpy(usernames[i], other_user.usernames[i]);
        }
    } else {
        usernames = nullptr;
    }

}

FamilyPremiumUser& FamilyPremiumUser::operator=(const FamilyPremiumUser& other_user) {
    if (this == &other_user) return *this;

    PremiumUser::operator=(other_user);
    
    if (this->usernames != nullptr && MAX_ACCOUNTS) {
        for(int i = 0; i < MAX_ACCOUNTS; i ++) {
            delete[] this->usernames[i];
        }
        delete[] this->usernames;
    }

    connectedAccounts = other_user.connectedAccounts;

    if (other_user.MAX_ACCOUNTS && other_user.usernames != nullptr) {
        this->usernames = new char*[MAX_ACCOUNTS];

        for (int i = 0;i < MAX_ACCOUNTS; i ++) {
            if (other_user.usernames[i] != nullptr) {
                this->usernames[i] = new char[strlen(other_user.usernames[i]) + 1];
                strcpy(this->usernames[i], other_user.usernames[i]);
            } else {
                usernames[i] = nullptr;
            }
        }
    } else {
        this->usernames = nullptr;
    }

    return *this;
}
    
//!----------------------- GETTERS ---------------------
const char* FamilyPremiumUser::getMember(int idx) const {
    if (idx < 0  || idx >= connectedAccounts ) {
        return nullptr;
    }
    
    return usernames[idx];
} 

//!-------------------------- SETTERS ------------------------
//!----------------------- OTHER METHODS ---------------------
void FamilyPremiumUser::displayProfile() const {
    PremiumUser::displayProfile();

    cout << "|               FAMILY MANAGEMENT                |\n";
    cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    cout << "|  ACCOUNTS IN USE: " << connectedAccounts << " / " << FamilyPremiumUser::MAX_ACCOUNTS << "\n";
    
    if (connectedAccounts == 0 || usernames == nullptr) {
        cout << "|  [!] No members linked to this plan yet...     |\n";
    } else {
        cout << "|  LINKED MEMBERS:                               |\n";
        for (int i = 0; i < connectedAccounts; i++) {
            cout << "|    - " << usernames[i] << "\n"; 
        }
    }
    cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
}
//!----------------------- DESTRUCTOR ---------------------
FamilyPremiumUser::~FamilyPremiumUser() {
    for (int i = 0; i < connectedAccounts; i ++){
        delete[] usernames[i];
    }
    delete[] usernames;
    usernames = nullptr;
    cout << "~FamilyPremiumUser()" << endl;
}