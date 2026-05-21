#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "premium_user.h"


class FamilyPremiumUser: public PremiumUser {
    static const int MAX_ACCOUNTS = 6;
    int connectedAccounts;
    std::vector<std::string> usernames;

public:
    //!----------------------- CONSTRUCTORS ---------------------
    FamilyPremiumUser();
    FamilyPremiumUser(int old_id, const std::string& username, const std::string& password, const std::vector<Song*>& liked_songs, const Date& expirationDate);
    
    // FamilyPremiumUser(const FamilyPremiumUser& other_user);

    //!----------------------- OPERATORS -------------------
    // FamilyPremiumUser& operator=(const FamilyPremiumUser& other_user);
    
    //!----------------------- GETTERS ---------------------
    static int getMaxAccounts() { return MAX_ACCOUNTS;}
    int getCurrentAccountsNum() const { return connectedAccounts;}
    const std::string& getMember(int idx) const;    
    void printMembers() const; 
    //!----------------------- OTHER METHODS ---------------------
    void displayProfile() const override;
    std::string toString() const override;
    bool addMember(const std::string& new_member);
    bool removeMember(const std::string& removed_member);
    //!----------------------- DESTRUCTOR ---------------------
    ~FamilyPremiumUser() noexcept;
};