#include <iostream>
#include <sstream>
#include "family_premium_user.h"
#include "exceptions.h"

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

//!----------------------- GETTERS ---------------------
const string& FamilyPremiumUser::getMember(int idx) const {
    if (idx < 0  || idx >= connectedAccounts ) {
        throw OutOfRangeException(idx, connectedAccounts);
    }
    
    return usernames[idx];
} 

void FamilyPremiumUser::printMembers() const {
    if (connectedAccounts == 0) {
        throw InvalidActionException("There are no members yet!");
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

string FamilyPremiumUser::toString() const {
    string premium_profile = PremiumUser::toString();

    ostringstream os;
    os << "|               FAMILY MANAGEMENT                |\n";
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    os << "|  ACCOUNTS IN USE: " << connectedAccounts << " / " << FamilyPremiumUser::MAX_ACCOUNTS << "\n";
    
    if (usernames.empty()) {
        os << "|  [!] No members linked to this plan yet...     |\n";
    } else {
        os << "|  LINKED MEMBERS:                               |\n";
        for (int i = 0; i < connectedAccounts; i++) {
            os << "|    - " << usernames[i] << "\n"; 
        }
    }
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";

    return premium_profile + os.str();

}

bool FamilyPremiumUser::addMember(const string& new_member) {
    if(connectedAccounts >= MAX_ACCOUNTS) {
        throw LimitExceededException("Family Plan", MAX_ACCOUNTS);
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
        throw InvalidActionException("There are no members in this Family Plan yet!");
    }

    if (removed_member.empty()) {
        throw InvalidActionException("Your input is invalid!");
    }
    
    int found_idx = -1;
    for (int i = 0; i < connectedAccounts; i ++) {
        if (removed_member == usernames[i]){
            found_idx = i;
            break;
        }
    }

    if (found_idx == -1) {
        throw ObjectNotFoundException("Family Member", removed_member);
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