#include <iostream>
#include <algorithm>

#include "utils.h"
#include "user.h"
#include "song.h"

using namespace std;

int User::counter = 9000;
//!--------------------------- CONSTRUCTORS ----------------------
User::User()
:USER_ID(counter++) {
    // cout << "CONSTRUCTOR USER FARA PARAMETRII" << endl;
    // cout << "User()" << endl;
}

User::User(const string& username, const string& password, const vector<Song*>& liked_songs)
: USER_ID(counter++), username(username), password(password), liked_songs(liked_songs) {
    // cout << "User()" << endl;
}

User::User(const string& username, const string& password)
:USER_ID(counter++), username(username), password(password) {
    // cout << "User()" << endl;
}

User::User(int old_id, const string& username, const string& password, const vector<Song*>& liked_songs)
:USER_ID(old_id), username(username), password(password), liked_songs(liked_songs) {
    // cout << "User()" << endl;
}

// User::User(const User& other_user):
// USER_ID(other_user.USER_ID), likedSongs_num(other_user.likedSongs_num) {
//     if (other_user.username != nullptr) {
//         this->username = new char[strlen(other_user.username) + 1];
//         strcpy(this->username, other_user.username);
//     } else {
//         this->username = nullptr;
//     }

//     if (other_user.password != nullptr) {
//         this->password = new char[strlen(other_user.password) + 1];
//         strcpy(this->password, other_user.password);
//     } else {
//         this->password = nullptr;
//     }

//     if (other_user.liked_songs != nullptr && other_user.likedSongs_num > 0) {
//         this->liked_songs = new Song*[other_user.likedSongs_num];
//         for (int i = 0; i < other_user.likedSongs_num; i ++) {
//             this->liked_songs[i] = other_user.liked_songs[i]; 
//         }   
//     } else {
//         this->liked_songs =  nullptr;
//         this->likedSongs_num = 0;
//     }
// }

//!------------------- OVERWRITTING OPERATORS --------------------
// User& User::operator=(const User& other_user) {
//     if (this == &other_user) return *this;

//     this->username = other_user.username;
//     this->password = other_user.password;
//     this->liked_songs = other_user.liked_songs;
//     this->likedSongs_num = other_user.likedSongs_num;

//     return *this;
// }

User& User::operator+=(Song& new_song) {
    for (int i = 0;i < this->getSongsArrSize(); i++ ){
        if (new_song.getSongID() == this->liked_songs[i]->getSongID()) {
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
        return *this;
    }

    size_t before_removing_size = this->liked_songs.size();
    this->liked_songs.erase(
        remove_if(this->liked_songs.begin(), this->liked_songs.end(),
            [removed_songID](const Song* song){
                return song->getSongID() == removed_songID;
            }),
        this->liked_songs.end()
    );

    if (this->liked_songs.size() < before_removing_size) {
        cout << "[SUCCES] You have removed the selected song from liked.\n";
    } else {
        cout << " <--- [!] Invalid ID. Going back to menu!\n";
        
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
        throw out_of_range("Error: Playlist is empty!");
    }

    if (index < 0 || index >= static_cast<int>(this->liked_songs.size())) {
        throw out_of_range("Error: Index out of range!");
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
 
ostream& operator<<(ostream& os, const User& user) {
    os << "\n|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    os << "|                  USER PROFILE                  |\n";
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    os << "|  ID: [" << user.USER_ID << "]  USERNAME: " << user.username << "\n";
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    os << "|  LIKED SONGS TOTAL: " << static_cast<int>(user.liked_songs.size()) << "\n";
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    if (static_cast<int>(user.liked_songs.size()) == 0 || user.liked_songs.empty()) {
        os << "|  [!] This user hasn't liked any songs yet...   |\n";
    } else {
        os << "|  LIKED SONGS:                                  |\n";
        for (int i = 0; i < static_cast<int>(user.liked_songs.size()); i++) {
            os << "|    [" << (i + 1) << "]. " << user.liked_songs[i]->getTitle() << "\n";
        }
    }
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";

    return os;
}
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
    cout << "|  ID: [" << USER_ID << "]  USERNAME: " << username << "\n";
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
//!------------------------- DESTRUCTOR ---------------------------------
