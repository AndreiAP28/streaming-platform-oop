#include <iostream>
#include <cstring>
#include "family_premium_user.h"

using namespace std;

//!----------------------- CONSTRUCTORS ---------------------
FamilyPremiumUser::FamilyPremiumUser()
:PremiumUser(FAMILY_PLAN), connectedAccounts(0) {
    // cout << "FamilyPremiumUser()" << endl;
}

FamilyPremiumUser::FamilyPremiumUser(int old_id, const string& username, const string& password, const vector<Song*>& liked_songs, const Date& expirationDate)
:PremiumUser(old_id, username, password, liked_songs,FAMILY_PLAN, expirationDate), connectedAccounts(0) {
    // cout << "FamilyPremiumUser()" << endl;
}

// FamilyPremiumUser::FamilyPremiumUser(const FamilyPremiumUser& other_user):
//  PremiumUser(other_user), connectedAccounts(other_user.connectedAccounts) {

//     if (other_user.MAX_ACCOUNTS && other_user.usernames != nullptr) {
//         usernames = new char*[MAX_ACCOUNTS];
//         for (int i = 0; i < MAX_ACCOUNTS; i ++) {
//             usernames[i] = new char[strlen(other_user.usernames[i]) + 1];
//             strcpy(usernames[i], other_user.usernames[i]);
//         }
//     } else {
//         usernames = nullptr;
//     }

// }

// FamilyPremiumUser& FamilyPremiumUser::operator=(const FamilyPremiumUser& other_user) {
//     if (this == &other_user) return *this;

//     PremiumUser::operator=(other_user);
    
//     if (this->usernames != nullptr && MAX_ACCOUNTS) {
//         for(int i = 0; i < MAX_ACCOUNTS; i ++) {
//             delete[] this->usernames[i];
//         }
//         delete[] this->usernames;
//     }

//     connectedAccounts = other_user.connectedAccounts;

//     if (other_user.MAX_ACCOUNTS && other_user.usernames != nullptr) {
//         this->usernames = new char*[MAX_ACCOUNTS];

//         for (int i = 0;i < MAX_ACCOUNTS; i ++) {
//             if (other_user.usernames[i] != nullptr) {
//                 this->usernames[i] = new char[strlen(other_user.usernames[i]) + 1];
//                 strcpy(this->usernames[i], other_user.usernames[i]);
//             } else {
//                 usernames[i] = nullptr;
//             }
//         }
//     } else {
//         this->usernames = nullptr;
//     }

//     return *this;
// }
    
//!----------------------- GETTERS ---------------------
const string& FamilyPremiumUser::getMember(int idx) const {
    if (idx < 0  || idx >= connectedAccounts ) {
        throw out_of_range("Error: Index out of range!");
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
        if (!(usernames[i].empty())) {
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
    
    if (usernames.empty()) {
        cout << "|  [!] No members linked to this plan yet...     |\n";
    } else {
        cout << "|  LINKED MEMBERS:                               |\n";
        for (int i = 0; i < connectedAccounts; i++) {
            cout << "|    - " << usernames[i] << "\n"; 
        }
    }
    cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
}

bool FamilyPremiumUser::addMember(const string& new_member) {
    if(connectedAccounts >= MAX_ACCOUNTS) {
        cout <<" [!] Cannot add a new member. Family plan is already full.\n";
        return false;
    } 

    if (new_member.empty()) {
        cout << "[!] Invalid name. Try again!\n";
        return false;
    }

    connectedAccounts++;
    usernames.push_back(new_member);

    cout << "[SUCCESS] " << new_member << "is now part of the Family!\n";

    return true;
    
}

bool FamilyPremiumUser::removeMember(const string& removed_member) {
    if (connectedAccounts == 0) {
        cout << "[!] There is no accounts signed to this Family Plan.\n";
        return false;
    }

    if (removed_member.empty()) {
        cout << "[!] Invalid name. Try again!\n";
        return false;
    }
    
    int found_idx = -1;
    for (int i = 0; i < connectedAccounts; i ++) {
        if (removed_member == usernames[i]){
            found_idx = i;
            break;
        }
    }

    if (found_idx == -1) {
        cout << "[!] Member " << removed_member << " is not part of this Family Plan!\n";
            return false;
    }

    usernames.erase(usernames.begin() + found_idx);
    connectedAccounts--;
    cout << "[SUCCES] "<< removed_member << "Has been succesfully removed from the Family Plan!\n";
    return true;
}
//!----------------------- DESTRUCTOR ---------------------
FamilyPremiumUser::~FamilyPremiumUser() {
    // cout << "~FamilyPremiumUser()" << endl;
}