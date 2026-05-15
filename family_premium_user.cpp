#include <iostream>
#include <cstring>
#include "family_premium_user.h"

using namespace std;

//!----------------------- CONSTRUCTORS ---------------------
FamilyPremiumUser::FamilyPremiumUser()
:PremiumUser(FAMILY_PLAN), connectedAccounts(0), usernames(nullptr) {
    // cout << "FamilyPremiumUser()" << endl;
}

FamilyPremiumUser::FamilyPremiumUser(int old_id, const char* username, const char* password, Song** liked_songs, int likedSongs_num, const Date& expirationDate)
:PremiumUser(old_id, username, password, liked_songs, likedSongs_num,FAMILY_PLAN, expirationDate), connectedAccounts(0) {
    usernames = new char*[MAX_ACCOUNTS];

    for (int i = 0; i < MAX_ACCOUNTS; i ++) {
        usernames[i] = nullptr; 
    }

    // cout << "FamilyPremiumUser()" << endl;
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

void FamilyPremiumUser::printMembers() const {
    if (connectedAccounts == 0) {
        cout << " [!] There are no family members yet!";
        return;
    }

    cout << "  ~~~~~~~~~ Connected Family Members (" << this->connectedAccounts << " total) ~~~~~~~~~\n";
    for (int i = 0; i < connectedAccounts; i ++ ){
        if (usernames[i] != nullptr) {
            cout << "[ " << (i + 1) << " ]. " << usernames[i] << "\n";
        }
    }
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
}

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

bool FamilyPremiumUser::addMember(const char* new_member) {
    if(connectedAccounts >= MAX_ACCOUNTS) {
        cout <<" [!] Cannot add a new member. Family plan is already full.\n";
        return false;
    } 

    if (new_member == nullptr || !strlen(new_member)) {
        cout << "[!] Invalid name. Try again!\n";
        return false;
    }

    usernames[connectedAccounts] = new char[strlen(new_member) + 1];
    strcpy(usernames[connectedAccounts++], new_member);

    cout << "[SUCCESS] " << new_member << "is now part of the Family!\n";

    return true;
    
}

bool FamilyPremiumUser::removeMember(const char* removed_member) {
    if (connectedAccounts == 0) {
        cout << "[!] There is no accounts signed to this Family Plan.\n";
        return false;
    }

    if ( removed_member == nullptr || !strlen(removed_member)) {
        cout << "[!] Invalid name. Try again!\n";
        return false;
    }
    
    int found_idx = -1;
    for (int i = 0; i < connectedAccounts; i ++) {
        if (!strcmp(removed_member, usernames[i])){
            found_idx = i;
            break;
        }
    }

    if (found_idx == -1) {
        cout << "[!] Member " << removed_member << " is not part of this Family Plan!\n";
    }

    delete[] usernames[found_idx];
    usernames[found_idx] = nullptr;

    for (int i = found_idx; i < connectedAccounts - 1; i ++){
        usernames[i] = usernames[i + 1];
    }

    connectedAccounts--;
    cout << "[SUCCES] "<< removed_member << "Has been succesfully removed from the Family Plan!\n";
    return true;
}
//!----------------------- DESTRUCTOR ---------------------
FamilyPremiumUser::~FamilyPremiumUser() {
    for (int i = 0; i < connectedAccounts; i ++){
        delete[] usernames[i];
    }
    delete[] usernames;
    usernames = nullptr;
    // cout << "~FamilyPremiumUser()" << endl;
}