#include <iostream>
#include <cstring>
#include <stdexcept>

// #include "classes.h"

#include "utils.h"
#include "playlist.h"
#include "song.h"
    
using namespace std;


int Playlist::counter = 5000;

//!--------------------------------CONSTRUCTORS------------------------------------------------
Playlist::Playlist(): PLAYLIST_ID(counter++), songs(nullptr), songsNum(0), totalDurationSec(0) {
    // cout<<"CONSTRUCTORUL FARA PARAMETRII" << endl;
    this->name = new char[4];
    strcpy(this->name, "N/A");

    this->owner_username = new char[4];
    strcpy(this->owner_username, "N/A");
}

Playlist::Playlist(const char* name, const char* owner_username, Song** songs, int songsNum, int totalDuration = 0)
:PLAYLIST_ID(counter++), songsNum(songsNum), totalDurationSec(0) {

    if(name != nullptr) {
        this->name = new char[strlen(name) + 1];
        strcpy(this->name, name);
    } else {
        this->name = new char[4];
        strcpy(this->name, "N/A");
    }

    if(owner_username != nullptr) {
        this->owner_username = new char[strlen(owner_username) + 1];
        strcpy(this->owner_username, owner_username);
    } else {
        this->owner_username = new char[4];
        strcpy(this->owner_username, "N/A");
    }

    if(songsNum > 0 && songs != nullptr) {
        this->songs = new Song*[songsNum];

        for ( int i = 0; i < songsNum; i ++) {
            this->songs[i] = songs[i];
            this->totalDurationSec += songs[i]->getDuration();
        }
    } else {
        this->songs = nullptr;
        this->totalDurationSec = 0;
        this->songsNum = 0;
    }

}

Playlist::Playlist(const char* name, const char* owner_username)
:PLAYLIST_ID(counter++), songs(nullptr), songsNum(0), totalDurationSec(0) {

    if (name != nullptr) {
        this->name = new char[strlen(name) + 1];
        strcpy(this->name, name);
    } else {
        this->name = new char[4];
        strcpy(this->name, "N/A");
    }

    if(owner_username != nullptr) {
        this->owner_username = new char[strlen(owner_username) + 1];
        strcpy(this->owner_username, owner_username);
    } else {
        this->owner_username = new char[4];
        strcpy(this->owner_username, "N/A");
    }
}

Playlist::Playlist(const Playlist& other_playlist)  
: PLAYLIST_ID(other_playlist.PLAYLIST_ID), songsNum(other_playlist.songsNum), totalDurationSec(other_playlist.totalDurationSec) {
    // cout << "CONSTRUCTOR DE COPIERE PLAYLIST" << endl;

    if(other_playlist.name != nullptr) {
        this->name = new char[strlen(other_playlist.name) + 1];
        strcpy(this->name, other_playlist.name);
    } else {
        this->name = new char[4];
        strcpy(this->name, "N/A");
    }

    if(other_playlist.owner_username != nullptr) {
        this->owner_username = new char[strlen(other_playlist.owner_username) + 1];
        strcpy(this->owner_username, other_playlist.owner_username);
    } else {
        this->owner_username = new char[4];
        strcpy(this->owner_username, "N/A");
    }

    if (other_playlist.songs != nullptr && other_playlist.songsNum > 0) {
        this->songs = new Song*[other_playlist.songsNum];
        for(int i = 0; i < other_playlist.songsNum; i ++ ) {
            this->songs[i] = other_playlist.songs[i];
        }

    } else {
            this->songs = nullptr;
            this->songsNum = 0;
        }
}

//!---------------------------------OVERWRITTING OPERATORS------------------------------------
Playlist& Playlist::operator=(const Playlist& other_playlist) {
    if (this == &other_playlist) return *this;

    if (this->name != nullptr) {
        delete[] this->name;
        this->name = nullptr;
    }

    if (other_playlist.name != nullptr) {
        this->name = new char[strlen(other_playlist.name) + 1];
        strcpy(this->name, other_playlist.name);
    } else {
        this->name = new char[4];
        strcpy(this->name, "N/A");
    }

    if (this->owner_username != nullptr) {
        delete[] this->owner_username;
        this->owner_username = nullptr;
    }

    if (other_playlist.owner_username != nullptr) {
        this->owner_username = new char[strlen(other_playlist.owner_username) + 1];
        strcpy(this->owner_username, other_playlist.owner_username);
    } else {
        this->owner_username = new char[4];
        strcpy(this->owner_username, "N/A");
    }

    if (this->songs != nullptr && this->songsNum > 0) {
        delete[] this->songs;
        this->songs = nullptr;
        this->songsNum = 0;
    }

    if (other_playlist.songsNum > 0 && other_playlist.songs != nullptr) {
        this->songs = new Song*[other_playlist.songsNum];
        this->songsNum = other_playlist.songsNum;

        for(int i = 0; i < other_playlist.songsNum; i ++ ) { 
            this->songs[i] = other_playlist.songs[i];
        }

    } else {
        this->songs = nullptr;
        this->songsNum = 0;
    }

    this->totalDurationSec = other_playlist.totalDurationSec;

    return *this;
}

Playlist& Playlist::operator+=(Song& new_song) { 
    this->addSong(new_song);

    return *this;
}

Playlist& Playlist::operator-=(int removed_index) {
        if (this->songsNum == 0 || removed_index < 0 || removed_index >= this->songsNum) {
            return *this;
        }

        if ( this->songsNum == 1) {
            delete[] this->songs;
            this->songs = nullptr;
            this->songsNum = 0;
            this->totalDurationSec = 0;
            return *this;
        }

        Song** tempSongs = new Song*[this->songsNum - 1];
        int current_counter = 0;
        for ( int i = 0; i < this->songsNum; i ++ ) {
            if ( i == removed_index ) {
                this->totalDurationSec -= this->songs[i]->getDuration();
                continue;
            }
            tempSongs[current_counter] = this->songs[i];
            current_counter ++;
        }
        if(this->songs != nullptr) {
            delete[] this->songs;
            this->songs = nullptr;
        }   

        this->songs = new Song*[-- (this->songsNum)];
        for (int i = 0; i < this->songsNum; i ++) {
            songs[i] = tempSongs[i];
        } 
        
        //!this->songs = tempSongs;
        delete[] tempSongs;

        return *this;

    }

bool Playlist::operator==(const Playlist& other_playlist) const {
    return !strcmp(this->name, other_playlist.name);
}

bool Playlist::operator>(const Playlist& other_playlist) const {
    return this->totalDurationSec > other_playlist.totalDurationSec;
}

bool Playlist::operator<=(const Playlist& other_playlist) const {
    return !(*this>other_playlist);
}

bool Playlist::operator<(const Playlist& other_playlist) const {
    return this->totalDurationSec < other_playlist.totalDurationSec;
}

bool Playlist::operator>=(const Playlist& other_playlist) const {
    return !(*this < other_playlist);
}

const Song& Playlist::operator[](int index) const {
    if (this->songs == nullptr) {
        // cout << "Playlist " << this->name << " is empty!\n";
        // return;
        throw out_of_range("Error: Playlist is empty!");
    }

    if( index < 0 || index >= this->songsNum) {
        // cout << "Error: Index out of range! Try a index between 0 and " << (this->songsNum - 1) << ".\n";
        // return;
        throw out_of_range("Error: Index out of range!");
    }

    return *(this->songs[index]);
}

istream& operator>>(istream& in, Playlist& playlist) { 
    char tempName[256];
    get_name_input_validation(" <---Enter playlist name: ", tempName, 256);

    if (playlist.name != nullptr) {
        delete[] playlist.name;
        playlist.name = nullptr;
    }

    playlist.name = new char[strlen(tempName) + 1];
    strcpy(playlist.name, tempName);

    
    char temp_ownerUsername[256];
    get_name_input_validation(" <----Enter playlist name: ", temp_ownerUsername, 256);

    if (playlist.owner_username != nullptr) {
        delete[] playlist.owner_username;
        playlist.owner_username = nullptr;
    }

    playlist.owner_username = new char[strlen(temp_ownerUsername) + 1];
    strcpy(playlist.owner_username, temp_ownerUsername);



    if (playlist.songs != nullptr) {
        delete[] playlist.songs;
        playlist.songs = nullptr;
    }

    playlist.songsNum = 0;
    playlist.totalDurationSec = 0;

    return in;
}

ostream& operator<<(ostream& os, const Playlist& playlist) {
    os << "\n|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    os << "|               PLAYLIST PROFILE                 |\n";
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    os << "|  NAME: " << playlist.name << "\n";
    os << "|  OWNER: " << playlist.owner_username << "\n";
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    os << "|  TRACKS: " << playlist.songsNum << "  |  DURATION: " << playlist.totalDurationSec << "s\n";
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";

    if (playlist.songsNum == 0 || playlist.songs == nullptr) { 
        os << "|  [!] This playlist is currently empty...       |\n";
    } else {
        os << "|  TRACKLIST:                                    |\n";
        for (int i = 0; i < playlist.songsNum; i++) {
            os << "|    " << (i + 1) << ". " << playlist.songs[i]->getTitle() 
               << " (" << playlist.songs[i]->getDuration() << "s)\n";
        }
    }
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";

    return os;
}

//!---------------------------------SETTERS------------------------------------
void Playlist::addSong(Song& new_song) {
    Song** tempSongs = new Song*[this->songsNum + 1];
    
    if (this->songs != nullptr) {
        for (int i = 0; i < this->songsNum; i ++ ) {
            tempSongs[i] = this->songs[i];
        }
    }

    tempSongs[this->songsNum] = &new_song;
    this->totalDurationSec += new_song.getDuration();

    if (this->songs != nullptr) {
        delete[] this->songs;
    }
    this->songs = new Song*[++(this->songsNum) ];
    for (int i = 0; i < songsNum; i ++) {
        songs[i] = tempSongs[i];
    }
    //! this->songs = tempSongs;
    delete[] tempSongs;

    // cout << "AM ADAUGAT PIESA: " << new_song.getTitle() << endl;
}

void Playlist::removeSong(int removed_index) { 
    if (this->songsNum == 0 || removed_index < 0 || removed_index >= this->songsNum) {
        return;
    }

    if ( this->songsNum == 1) {
        delete[] this->songs;
        this->songs = nullptr;
        this->songsNum = 0;
        this->totalDurationSec = 0;
        return;
    }

    Song** tempSongs = new Song*[this->songsNum - 1];
    int current_counter = 0;
    for ( int i = 0; i < this->songsNum; i ++ ) {
        if ( i == removed_index ) {
            this->totalDurationSec -= this->songs[i]->getDuration();
            continue;
        }

        tempSongs[current_counter] = songs[i];
        current_counter ++;
    }
    if (this->songs != nullptr) {
        delete[] this->songs;
        this->songs = nullptr;
    }

    this->songs = new Song*[--(this->songsNum)];
    for (int i = 0; i < songsNum; i ++) {
        this->songs[i] = tempSongs[i];
    }

    delete[] tempSongs;
}

void Playlist::setName(const char* new_name) {
    if (new_name == nullptr) {
        return;
    }

    if (name != nullptr) {
        delete[] name;
    }

    name = new char[strlen(new_name) + 1];
    strcpy(name, new_name);
}

void Playlist::setOwnerUsername(const char* new_owner) {
    if (new_owner == nullptr) {
        return;
    }

    if (this->owner_username != nullptr) {
        delete[] this->owner_username;
    }

    this->owner_username = new char[strlen(new_owner) + 1];
    strcpy(this->owner_username, new_owner);
}
//!---------------------------------DESTRUCTORS------------------------------------
Playlist::~Playlist()  {
    // cout << "DESTRUCTORUL PT PLAYLIST A AVUT LOC" << endl;
    if (this->name != nullptr) {
        delete[] this->name;
        this->name = nullptr;
    }

    if (this->owner_username != nullptr) {
        delete[] this->owner_username;
        this->owner_username = nullptr;
    }

    if (this->songs != nullptr) {
        delete[] this->songs;
        this->songs = nullptr;
        this->songsNum = 0;
    }
}

