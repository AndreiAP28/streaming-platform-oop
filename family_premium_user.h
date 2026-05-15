#pragma once

#include <iostream>
#include "premium_user.h"


class FamilyPremiumUser: public PremiumUser {
    static const int MAX_ACCOUNTS = 6;
    int connectedAccounts;
    char** usernames;

public:
    //!----------------------- CONSTRUCTORS ---------------------
    FamilyPremiumUser();
    FamilyPremiumUser(int old_id, const char* username, const char* password, Song** liked_songs, int likedSongs_num, const Date& expirationDate);
    
    FamilyPremiumUser(const FamilyPremiumUser& other_user);

    //!----------------------- OPERATORS -------------------
    FamilyPremiumUser& operator=(const FamilyPremiumUser& other_user);
    
    //!----------------------- GETTERS ---------------------
    static int getMaxAccounts() { return MAX_ACCOUNTS;}
    int getCurrentAccountsNum() const { return connectedAccounts;}
    const char* getMember(int idx) const;    
    void printMembers() const; 
    //!----------------------- OTHER METHODS ---------------------
    void displayProfile() const override;
    bool addMember(const char* new_member);
    bool removeMember(const char* removed_member);
    //!----------------------- DESTRUCTOR ---------------------
    ~FamilyPremiumUser();
};