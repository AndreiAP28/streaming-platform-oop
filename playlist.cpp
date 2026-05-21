#include <iostream>
#include <chrono>
#include <thread>
#include <algorithm>
#include <stdexcept>
#include <sstream>

// #include "classes.h"

#include "utils.h"
#include "playlist.h"
#include "song.h"
#include "app.h"
#include "exceptions.h"

using namespace std;


// int Playlist::counter = 5000;

//!--------------------------------CONSTRUCTORS------------------------------------------------
Playlist::Playlist(): totalDurationSec(0) {
    // cout<<"CONSTRUCTORUL FARA PARAMETRII" << endl;
    // cout << "Playlist()" << endl;
    StreamingPlatformApp::getInstance().systemLog(to_string(this->getID()) + " ] New Playlist created.");

}

Playlist::Playlist(const string& name, const string& owner_username, const vector<Song*>& songs, int totalDuration = 0)
:name(name), owner_username(owner_username),songs(songs), totalDurationSec(0) {
    // cout << "Playlist()" << endl;
    StreamingPlatformApp::getInstance().systemLog(to_string(this->getID()) + " ] New Playlist created.");

}

Playlist::Playlist(const string& name, const string& owner_username)
:name(name), owner_username(owner_username), totalDurationSec(0) {
    // cout << "Playlist()" << endl;
    StreamingPlatformApp::getInstance().systemLog(to_string(this->getID()) + " ] New Playlist created.");

}



//!---------------------------------OVERWRITTING OPERATORS------------------------------------

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
        throw InvalidActionException("Playlist is empty");
    }

    if( index < 0 || index >= static_cast<int>(this->songs.size())) {
        throw OutOfRangeException(index, static_cast<int>(this->songs.size()));
        
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

// ostream& operator<<(ostream& os, const Playlist& playlist) {
//     os << "\n|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
//     os << "|               PLAYLIST PROFILE                 |\n";
//     os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
//     os << "|  NAME: " << playlist.name << "\n";
//     os << "|  OWNER: " << playlist.owner_username << "\n";
//     os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
//     os << "|  TRACKS: " << playlist.songs.size() << "  |  DURATION: " << playlist.totalDurationSec << "s\n";
//     os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";

//     if (playlist.songs.empty()) { 
//         os << "|  [!] This playlist is currently empty...       |\n";
//     } else {
//         os << "|  TRACKLIST:                                    |\n";
//         for (int i = 0; i < static_cast<int>(playlist.songs.size()); i++) {
//             os << "|    " << (i + 1) << ". " << playlist.songs[i]->getTitle() 
//                << " (" << playlist.songs[i]->getDuration() << "s)\n";
//         }
//     }
//     os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";

//     return os;
// }

//!---------------------------------SETTERS------------------------------------
void Playlist::addSong(Song& new_song) {
    this->totalDurationSec += new_song.getDuration();

    this->songs.push_back(&new_song);
}

void Playlist::removeSong(int removed_songID) { 
    if (this->songs.empty()) {
        throw InvalidActionException("Playlist is already empty!");
    }
    
    size_t before_removing_size = this->songs.size();
    this->songs.erase(
        remove_if(songs.begin(), songs.end(),
            [removed_songID](const Song* curr_song){
                return curr_song->getID() == removed_songID;
            }
        ),
        songs.end()
    );
    if (this->songs.size() < before_removing_size) {
        cout << "[SUCCESS] Song removed from playlist!\n";
    } else {
        throw ObjectNotFoundException("Song ID", to_string(removed_songID));
    }
}

void Playlist::setName(const string& new_name) {
    if (new_name.empty()) {
        throw InvalidActionException("Playlist name cannot be empty!");
    }

    this->name = new_name;
}

void Playlist::setOwnerUsername(const string& new_owner) {
    if (new_owner.empty()) {
        throw InvalidActionException("Owner username cannot be empty!");
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

string Playlist::toString() const {
    ostringstream os; 
    os << "\n|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    os << "|               PLAYLIST PROFILE                 |\n";
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    os << "|  PLAYLIST ID: " << this->getID() << "\n";
    os << "|  NAME: " << this->name << "\n";
    os << "|  OWNER: " << this->owner_username << "\n";
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    os << "|  TRACKS: " << this->songs.size() << "  |  DURATION: " << this->totalDurationSec << "s\n";
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";

    if (this->songs.empty()) { 
        os << "|  [!] This playlist is currently empty...       |\n";
    } else {
        os << "|  TRACKLIST:                                    |\n";
        for (int i = 0; i < static_cast<int>(this->songs.size()); i++) {
            os << "|    " << (i + 1) << ". " << this->songs[i]->getTitle() 
               << " (" << this->songs[i]->getDuration() << "s)\n";
        }
    }
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";

    return os.str();
}

//!---------------------------------DESTRUCTORS------------------------------------


