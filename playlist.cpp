#include <iostream>
#include <chrono>
#include <thread>
#include <algorithm>
#include <stdexcept>

// #include "classes.h"

#include "utils.h"
#include "playlist.h"
#include "song.h"
    
using namespace std;


int Playlist::counter = 5000;

//!--------------------------------CONSTRUCTORS------------------------------------------------
Playlist::Playlist(): PLAYLIST_ID(counter++), totalDurationSec(0) {
    // cout<<"CONSTRUCTORUL FARA PARAMETRII" << endl;
    // cout << "Playlist()" << endl;
}

Playlist::Playlist(const string& name, const string& owner_username, const vector<Song*>& songs, int totalDuration = 0)
:PLAYLIST_ID(counter++), name(name), owner_username(owner_username),songs(songs), totalDurationSec(0) {
    // cout << "Playlist()" << endl;

}

Playlist::Playlist(const string& name, const string& owner_username)
:PLAYLIST_ID(counter++), name(name), owner_username(owner_username), totalDurationSec(0) {
    // cout << "Playlist()" << endl;
}

// Playlist::Playlist(const Playlist& other_playlist)  
// : PLAYLIST_ID(other_playlist.PLAYLIST_ID), songsNum(other_playlist.songsNum), totalDurationSec(other_playlist.totalDurationSec) {
//     // cout << "CONSTRUCTOR DE COPIERE PLAYLIST" << endl;

//     if(other_playlist.name != nullptr) {
//         this->name = new char[strlen(other_playlist.name) + 1];
//         strcpy(this->name, other_playlist.name);
//     } else {
//         this->name = new char[4];
//         strcpy(this->name, "N/A");
//     }

//     if(other_playlist.owner_username != nullptr) {
//         this->owner_username = new char[strlen(other_playlist.owner_username) + 1];
//         strcpy(this->owner_username, other_playlist.owner_username);
//     } else {
//         this->owner_username = new char[4];
//         strcpy(this->owner_username, "N/A");
//     }

//     if (other_playlist.songs != nullptr && other_playlist.songsNum > 0) {
//         this->songs = new Song*[other_playlist.songsNum];
//         for(int i = 0; i < other_playlist.songsNum; i ++ ) {
//             this->songs[i] = other_playlist.songs[i];
//         }

//     } else {
//             this->songs = nullptr;
//             this->songsNum = 0;
//         }
// }

//!---------------------------------OVERWRITTING OPERATORS------------------------------------
// Playlist& Playlist::operator=(const Playlist& other_playlist) {
//     if (this == &other_playlist) return *this;

//     if (this->name != nullptr) {
//         delete[] this->name;
//         this->name = nullptr;
//     }

//     if (other_playlist.name != nullptr) {
//         this->name = new char[strlen(other_playlist.name) + 1];
//         strcpy(this->name, other_playlist.name);
//     } else {
//         this->name = new char[4];
//         strcpy(this->name, "N/A");
//     }

//     if (this->owner_username != nullptr) {
//         delete[] this->owner_username;
//         this->owner_username = nullptr;
//     }

//     if (other_playlist.owner_username != nullptr) {
//         this->owner_username = new char[strlen(other_playlist.owner_username) + 1];
//         strcpy(this->owner_username, other_playlist.owner_username);
//     } else {
//         this->owner_username = new char[4];
//         strcpy(this->owner_username, "N/A");
//     }

//     if (this->songs != nullptr && this->songsNum > 0) {
//         delete[] this->songs;
//         this->songs = nullptr;
//         this->songsNum = 0;
//     }

//     if (other_playlist.songsNum > 0 && other_playlist.songs != nullptr) {
//         this->songs = new Song*[other_playlist.songsNum];
//         this->songsNum = other_playlist.songsNum;

//         for(int i = 0; i < other_playlist.songsNum; i ++ ) { 
//             this->songs[i] = other_playlist.songs[i];
//         }

//     } else {
//         this->songs = nullptr;
//         this->songsNum = 0;
//     }

//     this->totalDurationSec = other_playlist.totalDurationSec;

//     return *this;
// }

Playlist& Playlist::operator+=(Song& new_song) { 
    this->addSong(new_song);

    return *this;
}

Playlist& Playlist::operator-=(int removed_index) {
        this->removeSong(removed_index);
        return *this;

    }

bool Playlist::operator==(const Playlist& other_playlist) const {
    return this->name == other_playlist.name;
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
    if (this->songs.empty()) {
        // cout << "Playlist " << this->name << " is empty!\n";
        // return;
        throw out_of_range("Error: Playlist is empty!");
    }

    if( index < 0 || index >= static_cast<int>(this->songs.size())) {
        // cout << "Error: Index out of range! Try a index between 0 and " << (this->songsNum - 1) << ".\n";
        // return;
        throw out_of_range("Error: Index out of range!");
    }

    return *(this->songs[index]);
}

istream& operator>>(istream& in, Playlist& playlist) { 
    playlist.name = get_name_input_validation(" <---Enter playlist name: ");
    playlist.owner_username = get_name_input_validation(" <----Enter playlist name: ");

    playlist.songs.clear();
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
    os << "|  TRACKS: " << playlist.songs.size() << "  |  DURATION: " << playlist.totalDurationSec << "s\n";
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";

    if (playlist.songs.empty()) { 
        os << "|  [!] This playlist is currently empty...       |\n";
    } else {
        os << "|  TRACKLIST:                                    |\n";
        for (int i = 0; i < static_cast<int>(playlist.songs.size()); i++) {
            os << "|    " << (i + 1) << ". " << playlist.songs[i]->getTitle() 
               << " (" << playlist.songs[i]->getDuration() << "s)\n";
        }
    }
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";

    return os;
}

//!---------------------------------SETTERS------------------------------------
void Playlist::addSong(Song& new_song) {
    this->totalDurationSec += new_song.getDuration();

    this->songs.push_back(&new_song);
}

void Playlist::removeSong(int removed_songID) { 
    if (this->songs.empty()) {
        return;
    }
    
    size_t before_removing_size = this->songs.size();
    this->songs.erase(
        remove_if(songs.begin(), songs.end(),
            [removed_songID](const Song* curr_song){
                return curr_song->getSongID() == removed_songID;
            }
        ),
        songs.end()
    );
    if (this->songs.size() < before_removing_size) {
        cout << "[SUCCESS] Song removed from playlist!\n";
    } else {
        cout << "[!] Invalid song selection.\n";
    }
}

void Playlist::setName(const string& new_name) {
    if (new_name.empty()) {
        return;
    }

    this->name = new_name;
}

void Playlist::setOwnerUsername(const string& new_owner) {
    if (new_owner.empty()) {
        return;
    }

    this->owner_username = new_owner;
}

//!-------------------- INHERITED METHODS FROM ABSTRACT CLASS -> PLAYABLEITEM ------------
void Playlist::simulatePlayItem() {
    cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    cout << "  [i] LOADING PLAYLIST: " << this->name << "\n";
    cout << "  [i] Total Tracks: " << this->songs.size() << "\n";
    cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

    this_thread::sleep_for(chrono::seconds(1));

    for(int i = 0; i < static_cast<int>(this->songs.size()); i ++ ){
        cout << "\n [ Track " << (i + 1) << " of" << this->songs.size() << " ]";
        songs[i]->simulatePlayItem();
    }

    cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    cout << "Playlist finished!\n";
    cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";


}

long long Playlist::calculateTotalStreams() const {
    long long total_streams = 0;
    for (const Song* s: this->songs) {
        total_streams += s->getStreams();
    }
    return total_streams;
}

//!---------------------------------DESTRUCTORS------------------------------------
// Playlist::~Playlist()  {
//     // cout << "DESTRUCTORUL PT PLAYLIST A AVUT LOC" << endl;
//     // cout << "~Playlist()" << endl;
    
//     if (this->name != nullptr) {
//         delete[] this->name;
//         this->name = nullptr;
//     }

//     if (this->owner_username != nullptr) {
//         delete[] this->owner_username;
//         this->owner_username = nullptr;
//     }

//     if (this->songs != nullptr) {
//         delete[] this->songs;
//         this->songs = nullptr;
//         this->songsNum = 0;
//     }
// }

