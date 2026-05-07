#pragma once

#include <iostream>
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
    PremiumUser(int old_id, const char* username, const char* password, Song** liked_songs, int likedSongs_num, subsPlan planName, const Date& expirationDate);

    //!------------------- GETTERS ---------------------
    double getSubsFee() const { return subsFee; }
    const Date& getExpirationDate() const { return expirationDate; }
    bool getStudentSubs()  const { return isStudent; }
    const char* getPlanName() const;
    //!------------------- SETTERS ---------------------
    void setSubsPlan(subsPlan plan);

    //!------------------ OTHER METHODS -----------------
    void displayProfile() const override;
    //!------------------- DESTRUCTOR ---------------------
    ~PremiumUser();


};



/*
// The X-Ray Scanner: "Are you actually a PremiumUser underneath?"
PremiumUser* pUser = dynamic_cast<PremiumUser*>(currentUser);

if (pUser != nullptr) {
    // YES! The X-Ray found Premium data!
    totalRevenue += pUser->getSubsFee();
    
    // Now, let's check the Enum!
    if (pUser->getPlanName() == "Family Plan") {
        cout << "Found a Family Manager!" << endl;
        // If we wanted to, we could do another dynamic_cast here 
        // to turn them into a FamilyPremiumUser and read their array!
    }
}
*/