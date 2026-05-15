#include <iostream>
#include <cctype>
#include <cstring>

// #include "classes.h" 

#include "utils.h"
#include "user.h"
#include "song.h"

using namespace std;

int User::counter = 9000;
//!--------------------------- CONSTRUCTORS ----------------------
User::User()
:USER_ID(counter++),username(nullptr), password(nullptr), liked_songs(nullptr), likedSongs_num(0) {
    // cout << "CONSTRUCTOR USER FARA PARAMETRII" << endl;
    // cout << "User()" << endl;
}

User::User(const char* username, const char* password, Song** liked_songs, int likedSongs_num)
: USER_ID(counter++), likedSongs_num(likedSongs_num) {

    // cout << "User()" << endl;

    if (username != nullptr) {
        this->username = new char[strlen(username) + 1];
        strcpy(this->username, username);
    } else {
        this->username = nullptr;
    }

    if (password != nullptr) {
        this->password = new char[strlen(password) + 1];
        strcpy(this->password, password);
    } else {
        this->password = nullptr;
    }

    if (likedSongs_num && liked_songs != nullptr) {
        this->liked_songs = new Song*[likedSongs_num];

        for (int i = 0; i < this->likedSongs_num; i ++) {
            this->liked_songs[i] = liked_songs[i];
        }
    } else {
        this->liked_songs = nullptr;
        this->likedSongs_num = 0;
    }
}

User::User(const char* username, const char* password)
:USER_ID(counter++),liked_songs(nullptr), likedSongs_num(0) {

    // cout << "User()" << endl;

    if (username != nullptr) {
        this->username = new char[strlen(username) + 1];
        strcpy(this->username, username);
    } else {
        this->username = nullptr;
    }

    if (password != nullptr) {
        this->password = new char[strlen(password) + 1];
        strcpy(this->password, password);
    } else {
        this->password = nullptr;
    }
}

User::User(int old_id, const char* username, const char* password, Song** liked_songs, int likedSongs_num)
:USER_ID(old_id) {

    // cout << "User()" << endl;

    if (username != nullptr) {
        this->username = new char[strlen(username) + 1];
        strcpy(this->username, username);
    } else {
        this->username = nullptr;
    }

    if (password != nullptr) {
        this->password = new char[strlen(password) + 1];
        strcpy(this->password, password);
    } else {
        this->password = nullptr;
    }

    if (likedSongs_num && liked_songs != nullptr) {
        this->liked_songs = new Song*[likedSongs_num];

        for (int i = 0; i < this->likedSongs_num; i ++) {
            this->liked_songs[i] = liked_songs[i];
        }
    } else {
        this->liked_songs = nullptr;
        this->likedSongs_num = 0;
    }

}

User::User(const User& other_user):
USER_ID(other_user.USER_ID), likedSongs_num(other_user.likedSongs_num) {
    if (other_user.username != nullptr) {
        this->username = new char[strlen(other_user.username) + 1];
        strcpy(this->username, other_user.username);
    } else {
        this->username = nullptr;
    }

    if (other_user.password != nullptr) {
        this->password = new char[strlen(other_user.password) + 1];
        strcpy(this->password, other_user.password);
    } else {
        this->password = nullptr;
    }

    if (other_user.liked_songs != nullptr && other_user.likedSongs_num > 0) {
        this->liked_songs = new Song*[other_user.likedSongs_num];
        for (int i = 0; i < other_user.likedSongs_num; i ++) {
            this->liked_songs[i] = other_user.liked_songs[i]; 
        }   
    } else {
        this->liked_songs =  nullptr;
        this->likedSongs_num = 0;
    }
}

//!------------------- OVERWRITTING OPERATORS --------------------
User& User::operator=(const User& other_user) {
    if (this == &other_user) return *this;

    if (this->username != nullptr) {
        delete[] this->username;
        this->username = nullptr;
    }

    if (other_user.username != nullptr) {
        this->username = new char[strlen(other_user.username) + 1];
        strcpy(this->username, other_user.username);
    } else {
        this->username = nullptr;
    }

    if (this->password != nullptr) {
        delete[] this->password;
        this->password = nullptr;
    }

    if (other_user.password != nullptr) {
        this->password = new char[strlen(other_user.password) + 1];
        strcpy(this->password, other_user.password);
    } else {
        this->password = nullptr;
    }

     if (this->liked_songs != nullptr && this->likedSongs_num> 0) {
        delete[] this->liked_songs;
        this->liked_songs = nullptr;
        this->likedSongs_num= 0;
    }

    if (other_user.likedSongs_num > 0 && other_user.liked_songs != nullptr) {
        this->liked_songs = new Song*[other_user.likedSongs_num];
        this->likedSongs_num = other_user.likedSongs_num;

        for(int i = 0; i < other_user.likedSongs_num; i ++ ) { 
            this->liked_songs[i] = other_user.liked_songs[i];
        }

    } else {
        this->liked_songs = nullptr;
        this->likedSongs_num = 0;
    }

    return *this;
}

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

User& User::operator-=(int removed_index) {
    if (this->likedSongs_num == 0 || removed_index < 0 || removed_index >= this->likedSongs_num) {
            return *this;
        }

        if ( this->likedSongs_num == 1) {
            delete[] this->liked_songs;
            this->liked_songs = nullptr;
            this->likedSongs_num = 0;
            return *this;
        }

        Song** temp_likedSongs = new Song*[this->likedSongs_num - 1];
        int current_counter = 0;
        for ( int i = 0; i < this->likedSongs_num; i ++ ) {
            if ( i == removed_index ) {
                continue;
            }

            temp_likedSongs[current_counter] = this->liked_songs[i];
            current_counter ++;
        }
        if(this->liked_songs != nullptr) {
            delete[] this->liked_songs;
            this->liked_songs = nullptr;
        }   

        this->liked_songs = new Song*[-- (this->likedSongs_num)];
        for (int i = 0; i < this->likedSongs_num; i ++) {
            this->liked_songs[i] =temp_likedSongs[i];
        } 
        
        //!this->songs =temp_likedSongs;
        delete[]temp_likedSongs;

        cout << "[SUCCES] You have removed the selected song from liked.\n";

        return *this;
}

bool User::operator==(const User& other_user) const {
    if (this->username != nullptr && other_user.username) {
        return this->username == other_user.username;
    }

    return false;   
}

bool User::operator>(const User& other_user) const {
    if (this->username != nullptr && other_user.username != nullptr) {
        return strcmp(this->username, other_user.username) > 0;
    }
    return false;
}

bool User::operator<=(const User& other_user) const {
    return !(*this > other_user);
}

const Song& User::operator[](int index) const {
    if (this->liked_songs == nullptr) {
        throw out_of_range("Error: Playlist is empty!");
    }

    if (index < 0 || index >= this->likedSongs_num) {
        throw out_of_range("Error: Index out of range!");
    }

    return *(this->liked_songs[index]);
}

istream& operator>>(istream& in, User& user) {
    char temp_username[256];
    get_name_input_validation(" <--- [?] Enter New Username: ", temp_username, 256);
    user.setUsername(temp_username);

    char temp_password[256];
    get_name_input_validation(" <--- [?] Enter New Username: ", temp_password, 256);
    user.setUsername(temp_password);

    return in;


}
 
ostream& operator<<(ostream& os, const User& user) {
    os << "\n|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    os << "|                  USER PROFILE                  |\n";
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    os << "|  ID: [" << user.USER_ID << "]  USERNAME: " << user.username << "\n";
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    os << "|  LIKED SONGS TOTAL: " << user.likedSongs_num << "\n";
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    if (user.likedSongs_num == 0 || user.liked_songs == nullptr) {
        os << "|  [!] This user hasn't liked any songs yet...   |\n";
    } else {
        os << "|  LIKED SONGS:                                  |\n";
        for (int i = 0; i < user.likedSongs_num; i++) {
            os << "|    [" << (i + 1) << "]. " << user.liked_songs[i]->getTitle() << "\n";
        }
    }
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";

    return os;
}
//!---------------------- SETTERS --------------------------------
void User::setUsername(const char* new_username) {
    if (new_username == nullptr) {
        return;
    }

    if (this->username != nullptr) {
        delete[] this->username;
        this->username = nullptr;
    }

    this->username = new char[strlen(new_username) + 1];
    strcpy(this->username, new_username);
}

void User::setPassword(const char* new_password) {

    if (new_password == nullptr) {
        return;
    }

    if (this->password != nullptr) {
        delete[] this->password;
        this->password = nullptr;
    }

    this->password = new char[strlen(new_password) + 1];
    strcpy(this->password, new_password);
}

void User::add_likedSong(Song& new_song) {
    // if (new_song == nullptr) {
    //     return;
    // }

    Song** temp_likedSongs = new Song*[this->likedSongs_num + 1];

    if (this->liked_songs != nullptr) {
        for (int i = 0; i < this->likedSongs_num; i ++ ) {
            temp_likedSongs[i] = this->liked_songs[i];
        }
    }

    temp_likedSongs[this->likedSongs_num] = &new_song;

    if (this->liked_songs != nullptr) {
        delete[] this->liked_songs;
    }
    this->liked_songs = new Song*[++(this->likedSongs_num)];
    for (int i = 0; i < likedSongs_num; i ++) {
        liked_songs[i] = temp_likedSongs[i];
    }
    //! this->songs = temp_likedSongs;
    delete[] temp_likedSongs;

    // cout << "AM ADAUGAT PIESA: " << new_song.getTitle() << endl;
}
//!------------------------------------- ---------------------------------
void User::displayProfile() const {
    cout << "\n|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    cout << "|                  USER PROFILE                  |\n";
    cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    cout << "|  ID: [" << USER_ID << "]  USERNAME: " << (username != nullptr ? username : "N/A") << "\n";
    cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    cout << "|  LIKED SONGS TOTAL: " << likedSongs_num << "\n";
    cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    if (likedSongs_num == 0 || liked_songs == nullptr) {
        cout << "|  [!] This user hasn't liked any songs yet...   |\n";
    } else {
        cout << "|  LIKED SONGS:                                  |\n";
        for (int i = 0; i < likedSongs_num; i++) {
            cout << "|    [" << (i + 1) << "]. " << liked_songs[i]->getTitle() << "\n";
        }
    }
    cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
}
//!------------------------- DESTRUCTOR ---------------------------------

User::~User() {
    if (this->username != nullptr) {
        delete[] this->username;
        this->username = nullptr;
    }

    if (this->password != nullptr) {
        delete[] this->password;
        this->password = nullptr;
    }

    if (this->liked_songs != nullptr) {
        delete[] this->liked_songs;
        this->liked_songs = nullptr;
    }

    // cout << "~User()" << endl;
}