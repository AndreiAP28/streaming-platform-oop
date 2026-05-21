#include <iostream>
#include <algorithm>
#include <sstream>

#include "utils.h"
#include "user.h"
#include "song.h"
#include "app.h"
#include "exceptions.h"


using namespace std;

// int User::counter = 9000;
//!--------------------------- CONSTRUCTORS ----------------------
User::User(){
    // cout << "CONSTRUCTOR USER FARA PARAMETRII" << endl;
    // cout << "User()" << endl;
    StreamingPlatformApp::getInstance().systemLog(to_string(this->getID()) + " ] New User profile registered.");
}

User::User(const string& username, const string& password, const vector<Song*>& liked_songs)
:  username(username), password(password), liked_songs(liked_songs) {
    // cout << "User()" << endl;
    StreamingPlatformApp::getInstance().systemLog(to_string(this->getID()) + " ] New User profile registered.");
}

User::User(const string& username, const string& password)
: username(username), password(password) {
    // cout << "User()" << endl;
    StreamingPlatformApp::getInstance().systemLog(to_string(this->getID()) + " ] New User profile registered.");
}

User::User(int old_id, const string& username, const string& password, const vector<Song*>& liked_songs)
:IObject(old_id), username(username), password(password), liked_songs(liked_songs) {
    // cout << "User()" << endl;
    StreamingPlatformApp::getInstance().systemLog(to_string(this->getID()) + "New User profile registered.");
}



//!------------------- OVERWRITTING OPERATORS --------------------

User& User::operator+=(Song& new_song) {
    for (int i = 0;i < this->getSongsArrSize(); i++ ){
        if (new_song.getID() == this->liked_songs[i]->getID()) {
            cout << "[!] You have already liked this song before!\n"; 
            return *this;
        }
    }

    this->add_likedSong(new_song);
    cout << "[SUCCESS] Song added to liked songs!\n";

    return *this;
}

User& User::operator-=(int removed_songID) {
    if (this->liked_songs.empty() ) {
        throw InvalidActionException("There are no songs in Liked Songs");
    }

    size_t before_removing_size = this->liked_songs.size();
    this->liked_songs.erase(
        remove_if(this->liked_songs.begin(), this->liked_songs.end(),
            [removed_songID](const Song* song){
                return song->getID() == removed_songID;
            }),
        this->liked_songs.end()
    );

    if (this->liked_songs.size() < before_removing_size) {
        cout << "[SUCCES] You have removed the selected song from liked.\n";
    } 

    return *this;
}

bool User::operator==(const User& other_user) const {
    return this->username == other_user.username;
}

bool User::operator>(const User& other_user) const {
    return this->username > other_user.username;
}

bool User::operator<=(const User& other_user) const {
    return !(*this > other_user);
}

const Song& User::operator[](int index) const {
    if (this->liked_songs.empty()) {
        throw InvalidActionException("Liked songs playlist is empty");
    }

    if (index < 0 || index >= static_cast<int>(this->liked_songs.size())) {
        throw OutOfRangeException(index, static_cast<int>(this->liked_songs.size()));
       }

    return *(this->liked_songs[index]);
}

istream& operator>>(istream& in, User& user) {
    string temp_username = get_name_input_validation(" <--- [?] Enter New Username: ");
    user.setUsername(temp_username);

    string temp_password = get_name_input_validation(" <--- [?] Enter New Password: ");
    user.setPassword(temp_password);

    return in;
}
 
// ostream& operator<<(ostream& os, const User& user) {
//     os << "\n|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
//     os << "|                  USER PROFILE                  |\n";
//     os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
//     os << "|  ID: [" << user.USER_ID << "]  USERNAME: " << user.username << "\n";
//     os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
//     os << "|  LIKED SONGS TOTAL: " << static_cast<int>(user.liked_songs.size()) << "\n";
//     os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
//     if (static_cast<int>(user.liked_songs.size()) == 0 || user.liked_songs.empty()) {
//         os << "|  [!] This user hasn't liked any songs yet...   |\n";
//     } else {
//         os << "|  LIKED SONGS:                                  |\n";
//         for (int i = 0; i < static_cast<int>(user.liked_songs.size()); i++) {
//             os << "|    [" << (i + 1) << "]. " << user.liked_songs[i]->getTitle() << "\n";
//         }
//     }
//     os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";

//     return os;
// }
//!---------------------- SETTERS --------------------------------
void User::setUsername(const string& new_username) {
    this->username = new_username;
}

void User::setPassword(const string& new_password) {
    this->password = new_password;
}

void User::add_likedSong(Song& new_song) {
    this->liked_songs.push_back(&new_song);
}
//!------------------------------------- ---------------------------------
void User::displayProfile() const {
    cout << "\n|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    cout << "|                  USER PROFILE                  |\n";
    cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    cout << "|  System ID: [" << getID()<< "]  USERNAME: " << username << "\n";
    cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    cout << "|  LIKED SONGS TOTAL: " << static_cast<int>(liked_songs.size()) << "\n";
    cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    if (static_cast<int>(liked_songs.size()) == 0 || liked_songs.empty()) {
        cout << "|  [!] This user hasn't liked any songs yet...   |\n";
    } else {
        cout << "|  LIKED SONGS:                                  |\n";
        for (int i = 0; i < static_cast<int>(liked_songs.size()); i++) {
            cout << "|    [" << (i + 1) << "]. " << liked_songs[i]->getTitle() << "\n";
        }
    }
    cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
}

//!-------------------------------INHERITED METHODS -----------------------------
string User::toString() const {
    ostringstream os;
    os << "\n|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    os << "|                  USER PROFILE                  |\n";
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    os << "|  ID: [" << this->getID() << "]  USERNAME: " << this->username << "\n";
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    os << "|  LIKED SONGS TOTAL: " << static_cast<int>(this->liked_songs.size()) << "\n";
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    if (static_cast<int>(this->liked_songs.size()) == 0 || this->liked_songs.empty()) {
        os << "|  [!] This user hasn't liked any songs yet...   |\n";
    } else {
        os << "|  LIKED SONGS:                                  |\n";
        for (int i = 0; i < static_cast<int>(this->liked_songs.size()); i++) {
            os << "|    [" << (i + 1) << "]. " << this->liked_songs[i]->getTitle() << "\n";
        }
    }
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";

    return os.str();
}
//!------------------------- DESTRUCTOR ---------------------------------
