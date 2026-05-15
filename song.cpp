#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>


// #include "classes.h"

#include "utils.h"
#include "song.h"
// #include "date.h"

using namespace std;



int Song::counter = 3000;
//!-------------------------CONSTRUCTORS--------------------------------
Song::Song():
 SONG_ID(counter++), durationSec(0), artistID(0), featuresSize(0), artistsFeatures(nullptr) ,streams(0), releaseDate() {
    // cout<<"CONSTRUCTORUL SONG DEFAULT CREAT"<<endl;
    // cout << "Song()" << endl;
    this->title = new char[4];
    strcpy(this->title, "N/A");

    
}

Song::Song(const char* title, int durationSec, int artistID, int featuresSize,const char** artistsFeatures, long long streams, const Date& dateParam) 
: SONG_ID(counter++), durationSec(durationSec), artistID(artistID), featuresSize(featuresSize), streams(streams), releaseDate(dateParam) {
    // cout << "CONSTRUCTORUL SONG CU TOTI PARAM CREAT" << endl;
    // cout << "Song()" << endl;

    this->title = new char[strlen(title) + 1];
    strcpy(this->title, title);

    if (featuresSize > 0 && artistsFeatures != nullptr) {
        this->artistsFeatures = new char*[featuresSize];

        for ( int i = 0; i < featuresSize; i ++) {
            this->artistsFeatures[i] = new char[strlen(artistsFeatures[i]) + 1];
            strcpy(this->artistsFeatures[i], artistsFeatures[i]);
        }
    }  else {
        this->featuresSize = 0;
        this->artistsFeatures = nullptr;
    }


}

Song::Song(const char* title)
:SONG_ID(counter++), durationSec(0), artistID(0), featuresSize(0), artistsFeatures(nullptr), streams(0), releaseDate() {
    // cout << "CONSTRUCTOR SONG CU TITLU CREAT" << endl;
    // cout << "Song()" << endl;


    this->title = new char[strlen(title) + 1];
    strcpy(this->title, title);
}

Song::Song(const Song& other_song)
: SONG_ID(other_song.SONG_ID), durationSec(other_song.durationSec), artistID(other_song.artistID), streams(other_song.streams), releaseDate(other_song.releaseDate) {
    // cout << "CONSTRUCTOR SONG DE COPIERE " << endl;

    this->title = new char[strlen(other_song.title) + 1];
    strcpy(this->title, other_song.title);


    if (other_song.featuresSize > 0 && other_song.artistsFeatures != nullptr) {
        this->artistsFeatures = new char*[other_song.featuresSize];
        this->featuresSize = other_song.featuresSize;

        for ( int i = 0; i < other_song.featuresSize; i ++ ) {
            this->artistsFeatures[i] = new char[strlen(other_song.artistsFeatures[i]) + 1];
            strcpy(this->artistsFeatures[i], other_song.artistsFeatures[i]);
        }
    } else {
        this->artistsFeatures = nullptr;
        this->featuresSize = 0;
    }
}


//!------------------------------OVERWRITTING OPERATORS-------------------
Song& Song::operator=(const Song& other_song) {
    if(this == &other_song) return *this;


    if(this->title != nullptr) {
        delete[] title;
        title = nullptr;
    }    

    title = new char[ strlen(other_song.title) + 1];
    strcpy(title, other_song.title);

    if(this->artistsFeatures != nullptr) {
       for ( int i = 0; i < this->featuresSize; i ++) {
            delete[] this->artistsFeatures[i];
       }
       
        delete[] this->artistsFeatures;
        this->artistsFeatures = nullptr;
        this->featuresSize = 0;
    }


    if (other_song.featuresSize > 0 && other_song.artistsFeatures != nullptr) {
        artistsFeatures = new char*[other_song.featuresSize];
        this->featuresSize = other_song.featuresSize;
        for(int i = 0; i < other_song.featuresSize; i ++) {
            this->artistsFeatures[i] = new char[strlen(other_song.artistsFeatures[i]) + 1];
            strcpy(this->artistsFeatures[i], other_song.artistsFeatures[i]);
        }
    } 

    durationSec = other_song.durationSec;
    artistID = other_song.artistID;
    streams = other_song.streams;
    releaseDate = other_song.releaseDate;

    return *this;
}

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
        cout << "ERROR: There are not enough streams to be removed!\n";
        return *this;
    }

    Song temp = *this;
    temp.streams -= remove_streams_number;
    return temp;
}

Song& Song::operator+=(int add_streams_number) { 

    if (add_streams_number > 0) { 
        this->streams += add_streams_number;
    } else {
        cout << "Error: You can't add a negative number of stream (for removing streams use the operator '-') !";
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
    return !strcmp(this->title, other_song.title); 
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

const char* Song::operator[](int index) const {
    if (this->artistsFeatures == nullptr) {
        cout << "There are not features on this song!\n";
        return "N/A";
    }

    if (index < 0 || index >= this->featuresSize) {
        cout << "Error: There are " << this->featuresSize << "! Please introduce a number between 0 and " << (this->featuresSize - 1) << "." << endl;
        return "N/A";
    }

    return this->artistsFeatures[index];
}

//! VEZI DACA POTI LASA ASA FARA SA CITESTI DIN IN>>
istream& operator>>(istream& in, Song& song) {
    if ( song.artistsFeatures != nullptr ) {
        for (int i = 0; i < song.featuresSize; i ++ ) {
            delete[] song.artistsFeatures[i];
        }

        delete[] song.artistsFeatures;
        song.artistsFeatures = nullptr;
    } 
    
    song.featuresSize = 0;
    char userChoice = get_yesno_input_validation(" <--- There are features on this song (y/n): ");
    while ( userChoice == 'y' || userChoice == 'Y') {

        char tempFeatureName[256];
        get_name_input_validation(" <--- Enter the name of the song's feature: ", tempFeatureName, 256);
        song.addFeature(tempFeatureName);

        userChoice = get_yesno_input_validation( " <--- There are more features to be added? (y/n): ");
    }
   
    song.durationSec = get_int_input_validation( " <--- Enter the duration of the song (seconds): ");
    song.streams = get_int_input_validation(" <--- Enter song's streams: ");

    return in;
}

ostream& operator<<(ostream& os,const Song& song) {
   os << "\n|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    os << "|                 SONG PROFILE                   |\n";
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    
    os << "|  ID: [" << song.artistID << "]  TITLE: " << song.title << "\n";
    
    if (song.featuresSize > 0 && song.artistsFeatures != nullptr) {
        os << "|  FEATURING: ";
        for (int i = 0; i < song.featuresSize; i++) {
            os << song.artistsFeatures[i];
            if (i != song.featuresSize - 1) {
                os << ", ";
            }
        }
        os << " \n";
    }
    
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    os << "|  STREAMS: " << song.streams << "\n";
    os << "|  DURATION: " << song.durationSec << " seconds\n";
    os << "|  RELEASE DATE: " << song.releaseDate.getDay() << "/" <<song.releaseDate.getMonth() << "/" << song.releaseDate.getYear() << endl;
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    

    return os;
}


//!------------------------SETTERS----------------------------------------
void Song::addFeature(const char* featureName) {
    if (featureName == nullptr) return;

    char** tempFeatures = new char*[this->featuresSize + 1];

    if(this->artistsFeatures != nullptr){
        for ( int i = 0; i < this->featuresSize; i ++) {
            tempFeatures[i] = this->artistsFeatures[i];
        }
    }
    tempFeatures[this->featuresSize] = new char[strlen(featureName) + 1];
    strcpy(tempFeatures[this->featuresSize], featureName);
    
    if(this->artistsFeatures != nullptr) {
        delete[] this->artistsFeatures;
    }

    this->artistsFeatures = tempFeatures;
    this->featuresSize ++;

    // cout<< "ADAUGAT CU SUCCES " << featureName << endl;  

}

void Song::setTitle(const char* new_title) {
    if(this->title != nullptr) {
        delete[] this->title;
        this->title = nullptr;
    }

    this->title = new char[strlen(new_title) + 1];
    strcpy(this->title, new_title);
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

//!-------------------------DESTRUCTOR------------------------------------
Song::~Song() {
    // cout << "DESTRUCTOR SONG A AVUT LOC!!" << endl;
    // cout << "~Song()" << endl;

    if(this->title != nullptr) { 
        delete[] title;
        title = nullptr;
    }

    if (this->artistsFeatures != nullptr) {
        for ( int i = 0; i < this->featuresSize; i ++ ){
            delete[] artistsFeatures[i];
        }

        delete[] artistsFeatures;
        artistsFeatures = nullptr;
    }

}


