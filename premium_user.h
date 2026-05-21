#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "user.h"
#include "date.h"

enum subsPlan {
    STUDENT_PLAN,
    STANDARD_MONTHLY,
    ANNUAL_DISCOUNT,
    FAMILY_PLAN
};
 

class PremiumUser: public User {
protected:
    subsPlan planName;
    double subsFee;
    Date expirationDate;
    bool isStudent;

public:  
    //!------------------- CONSTRUCTORS ---------------------
    PremiumUser();
    PremiumUser(subsPlan planName);
    PremiumUser(int old_id, const std::string& username, const std::string& password, const std::vector<Song*>& liked_songs, subsPlan planName, const Date& expirationDate);

    //!------------------- GETTERS ---------------------
    double getSubsFee() const { return subsFee; }
    const Date& getExpirationDate() const { return expirationDate; }
    bool getStudentSubs()  const { return isStudent; }
    std::string getPlanName() const;
    //!------------------- SETTERS ---------------------
    void setSubsPlan(subsPlan plan);

    //!------------------ OTHER METHODS -----------------
    void displayProfile() const override;
    std::string toString() const override;
    //!------------------- DESTRUCTOR ---------------------
    ~PremiumUser() noexcept;


};



