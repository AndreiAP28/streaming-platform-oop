#include <iostream>
#include <ctime>

#include "user.h"
#include "premium_user.h"
#include "family_premium_user.h"
using namespace std;



// int main() {

    // User* premiumUser = new PremiumUser();  
    // delete premiumUser; 
    // cout << "\n--------------------------\n";
    // User* FamilyUser = new FamilyPremiumUser();
    // delete FamilyUser;
    // cout << "\n--------------------------\n";

    // User* FamUser2 = new FamilyPremiumUser(9000, "Andrei", "1234", nullptr, 0, Date());
    // FamUser2->displayProfile();

    // FamilyPremiumUser* validate = dynamic_cast<FamilyPremiumUser*>(FamUser2);
    // if(validate != nullptr) {
    //     cout <<"\n daaaaaa ma daaa \n";
    // } else {
    //     cout <<"\n fail ba\n";
    // }

    // cout << "\n---------------------\n";
    // FamilyPremiumUser original(9012, "abc", "123", nullptr, 0, Date());
    // // original.displayProfile();
    // // original.setSubsPlan(STANDARD_MONTHLY);
    // FamilyPremiumUser clone(original);
    // FamilyPremiumUser clone2;
    // cout <<clone2.getUserID()<< endl;
    // clone2 = original;

    // cout <<original.getUserID()<< " Original Name: " << original.getUsername() << " | Fee: $" << original.getSubsFee() << "\n";
    // cout <<clone.getUserID()<< " Clone 1 Name: " << clone.getUsername() << " | Fee: $" << clone.getSubsFee() << "\n";
    // cout <<clone2.getUserID()<< " Clone 2 Name: " << clone2.getUsername() << " | Fee: $" << clone2.getSubsFee() << "\n";
    
    // User* array[3] = {
    //     new User(),
    //     new PremiumUser(),
    //     new FamilyPremiumUser()
    // };

    // for (int i = 0; i < 3; i ++ ){
    //     array[i]->displayProfile();
    //     cout << "-------------------------------------------------------- \n\n\n\n";
    // }

    // User** array = new User*[3];
    // array[0] = new User();
    // array[1] = new PremiumUser();
    // array[2] = new FamilyPremiumUser();
    // for(int i = 0; i < 3; i ++) {
    //     array[i]->displayProfile();
    // }

    // User array2[3] = {
    //     User(),
    //     PremiumUser(),
    //     PremiumUser()
    // };
    
    // array2[2].displayProfile();
    // cout << "========================================\n";

    // User* test = new PremiumUser[3];
    // for (int i = 0; i < 3;  i ++)
    //     test->displayProfile();

//     Date today;
//     today.formattedDate();

//     return 0;   
// }
