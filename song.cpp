#include <iostream>
#include <thread>
#include <chrono>
#include <stdexcept>    
#include <sstream>
// #include "classes.h"

#include "utils.h"
#include "song.h"
#include "app.h"
#include "exceptions.h"

using namespace std;



// int Song::counter = 3000;
//!-------------------------CONSTRUCTORS--------------------------------
Song::Song():
durationSec(0), artistID(0), streams(0), releaseDate() {
    // cout<<"CONSTRUCTORUL SONG DEFAULT CREAT"<<endl;
    // cout << "Song()" << endl;
    StreamingPlatformApp::getInstance().systemLog(to_string(this->getID()) + " ] New Song added to the database.");
}

Song::Song(const string& title, const string& artistName, int durationSec, int artistID, const vector<string>& artistsFeatures, long long streams, const Date& dateParam) 
:title(title), artistName(artistName), durationSec(durationSec), artistID(artistID), streams(streams), releaseDate(dateParam) {
    // cout << "CONSTRUCTORUL SONG CU TOTI PARAM CREAT" << endl;
    // cout << "Song()" << endl;
    StreamingPlatformApp::getInstance().systemLog(to_string(this->getID()) + " ] New Song added to the database.");

}

Song::Song(const string& title)
:title(title), durationSec(0), artistID(0), streams(0), releaseDate() {
    // cout << "CONSTRUCTOR SONG CU TITLU CREAT" << endl;
    // cout << "Song()" << endl;
    StreamingPlatformApp::getInstance().systemLog(to_string(this->getID()) + " ] New Song added to the database.");

}



//!------------------------------OVERWRITTING OPERATORS-------------------

Song& Song::operator++() {
    this->streams ++;
    return *this;
}

Song& Song::operator--() {
    this->streams --;
    return *this;


}

Song Song::operator++(int dummy) {
    Song copy_song = *this;
    ++(*this);
    return copy_song;
}

Song Song::operator--(int dummy) {
    Song copy_song = *this;
    --(*this);
    return copy_song;
}

Song Song::operator+(int add_new_streams) const {
    Song temp = *this;
    temp.streams += add_new_streams;
    
    return temp;
}

Song operator+(int add_new_streams, const Song& song) {
    return song + add_new_streams;
}

Song Song::operator-(int remove_streams_number) const {
    if (remove_streams_number > this->streams) { 
        throw InvalidActionException("There are not enough streams to be removed!");
    }

    Song temp = *this;
    temp.streams -= remove_streams_number;
    return temp;
}

Song& Song::operator+=(int add_streams_number) { 

    if (add_streams_number > 0) { 
        this->streams += add_streams_number;
    } else {
        throw InvalidActionException("You can't add a negative number of stream (for removing streams use the operator '-')");
    }

    return *this;

}

Song& Song::operator-=(int remove_streams_number) {

    if (remove_streams_number > 0) {
        if (remove_streams_number < this->streams) {
            this->streams -= remove_streams_number;
        } else {
            this->streams = 0;
        }
    }

    return *this;


}

bool Song::operator==(const Song& other_song) const {
    return this->title == other_song.title; 
}

bool Song:: operator>(const Song& other_song) const {
    return this->streams > other_song.streams;
}

bool Song::operator<=(const Song& other_song) const {
    return !(*this > other_song);
}

bool Song::operator<(const Song& other_song) const {
    return this->streams < other_song.streams;
}

bool Song::operator>=(const Song& other_song) const {
    return !(*this < other_song);
}

const string& Song::operator[](int index) const {
    if (this->artistsFeatures.empty()) {
        throw InvalidActionException("There are not features on this song!");
    }

    if (index < 0 || index >= static_cast<int>(this->artistsFeatures.size())) {
        throw OutOfRangeException(index, static_cast<int>(this->artistsFeatures.size()));
    }

    return this->artistsFeatures[index];
}

istream& operator>>(istream& in, Song& song) {
    song.artistsFeatures.clear();

    char userChoice = get_yesno_input_validation(" <--- There are features on this song (y/n): ");
    while ( userChoice == 'y' || userChoice == 'Y') {

        string tempFeatureName = get_name_input_validation(" <--- Enter the name of the song's feature: ");
        song.addFeature(tempFeatureName);

        userChoice = get_yesno_input_validation( " <--- There are more features to be added? (y/n): ");
    }
   
    song.durationSec = get_int_input_validation( " <--- Enter the duration of the song (seconds): ");
    song.streams = get_int_input_validation(" <--- Enter song's streams: ");

    return in;
}

// ostream& operator<<(ostream& os,const Song& song) {
//    os << "\n|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
//     os << "|                 SONG PROFILE                   |\n";
//     os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    
//     os << "|  ID: [" << song.artistID << "]  TITLE: " << song.title << "\n";
    
//     if (!song.artistsFeatures.empty()) {
//         os << "|  FEATURING: ";
//         for (int i = 0; i < static_cast<int>(song.artistsFeatures.size()); i++) {
//             os << song.artistsFeatures[i];
//             if (i != static_cast<int>(song.artistsFeatures.size()) - 1) {
//                 os << ", ";
//             }
//         }
//         os << " \n";
//     }
    
//     os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
//     os << "|  STREAMS: " << song.streams << "\n";
//     os << "|  DURATION: " << song.durationSec << " seconds\n";
//     os << "|  RELEASE DATE: " << song.releaseDate.getDay() << "/" <<song.releaseDate.getMonth() << "/" << song.releaseDate.getYear() << endl;
//     os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    

//     return os;
// }


//!------------------------SETTERS----------------------------------------
void Song::addFeature(const string& featureName) {
    if (featureName.empty()) 
        throw InvalidActionException("Artist name can't be empty!");

    this->artistsFeatures.push_back(featureName);
    // cout<< "ADAUGAT CU SUCCES " << featureName << endl;  

}

void Song::setTitle(const string& new_title) {
   this->title = new_title;
}

void Song::setArtistName(const string& artist_name){
    this->artistName = artist_name;
}

//!------------------ INHERITED FUNCTIONS FROM ABSTRACT CLAS PLAYABLEITEM
void Song::simulatePlayItem() {
    cout << "\n [|>] Now Playing Audio Track: " << this->title << "...\n";
    cout << "  ----------------------------------------\n";
    cout << "   [===||===|||======||======|||||===||]  \n"; 
    cout << "  ----------------------------------------\n";

    this_thread::sleep_for(chrono::seconds(1));

    ++(*this);

}

long long Song::calculateTotalStreams() const {
    return this->streams;
}

string Song::toString() const {
    ostringstream os;
    os << "\n|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    os << "|                 SONG PROFILE                   |\n";
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    
    os << "| System ID: [" << this->getID() << "]  TITLE: " << this->title << "\n";
    os << "| BY: " << this->artistName <<"\n";
    if (!this->artistsFeatures.empty()) {
        os << "|  FEATURING: ";
        for (int i = 0; i < static_cast<int>(this->artistsFeatures.size()); i++) {
            os << this->artistsFeatures[i];
            if (i != static_cast<int>(this->artistsFeatures.size()) - 1) {
                os << ", ";
            }
        }
        os << " \n";
    }
    
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    os << "|  STREAMS: " << this->streams << "\n";
    os << "|  DURATION: " << this->durationSec << " seconds\n";
    os << "|  RELEASE DATE: " << this->releaseDate.getDay() << "/" <<this->releaseDate.getMonth() << "/" << this->releaseDate.getYear() << endl;
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    

    return os.str();
}

//!-------------------------DESTRUCTOR------------------------------------
// Song::~Song() {
//     // cout << "DESTRUCTOR SONG A AVUT LOC!!" << endl;
//     // cout << "~Song()" << endl;

//     if(this->title != nullptr) { 
//         delete[] title;
//         title = nullptr;
//     }

//     if (this->artistsFeatures != nullptr) {
//         for ( int i = 0; i < this->artistsFeatures.size(); i ++ ){
//             delete[] artistsFeatures[i];
//         }

//         delete[] artistsFeatures;
//         artistsFeatures = nullptr;
//     }

// }


