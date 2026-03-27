#include <iostream>
#include <cstring>

// #include "classes.h"

#include "utils.h"
#include "artist.h"

using namespace std;


int Artist::counter = 1000;
//!-------------------------------CONSTRUCTORS---------------------------------------------
Artist::Artist()
: ARTIST_ID(counter++), monthlyListeners(0),  topSongs(nullptr), topSongsSize(0), isVerified(false) { 
    // cout<<"CONSTRUCTORUL FARA PARAMETRII"<<endl;
    
    this->name = new char[4];
    strcpy(this->name, "N/A");
}

Artist::Artist(const char* name, int monthlyListeners, int topSongsSize, const char** topSongs , bool isVerified)
: ARTIST_ID(counter++),monthlyListeners(monthlyListeners), topSongsSize(topSongsSize), isVerified(isVerified) {
    // cout<<"CONSTRUCTORUL CU TOTI PARAMETRII"<<endl;
    
    if(name != nullptr) {
        this->name = new char[strlen(name) + 1];
        strcpy(this->name, name);
    } else {
        this->name = new char[4];
        strcpy(this->name, "N/A");
    }

    if (topSongsSize > 0 && topSongs != nullptr) {
        this->topSongs = new char*[topSongsSize];

        for (int i = 0; i < topSongsSize; i ++ ){ 
            this->topSongs[i] = new char[strlen(topSongs[i]) + 1];

            strcpy(this->topSongs[i], topSongs[i]);
        }

    } else {
        this->topSongs = nullptr;
        this->topSongsSize = 0;
    }

}

Artist::Artist(const char* name)
: ARTIST_ID(counter++), topSongs(nullptr), topSongsSize(0) { 
    // cout<<"CONSTRUCTORUL CU NUME"<<endl;
    if (name != nullptr) {
        this->name = new char[strlen(name) + 1];
        strcpy(this->name, name);
    } else {
        this->name = new char[4];
        strcpy(this->name, "N/A");
    }

    
}

Artist::Artist(const Artist& other_artist)
: ARTIST_ID(other_artist.ARTIST_ID), monthlyListeners(other_artist.monthlyListeners),topSongsSize(other_artist.topSongsSize), isVerified(other_artist.isVerified) {
    // cout<<"CONSTRUCTOR DE COPIERE ARTIST"<<endl;
    
    if (other_artist.name != nullptr) {
        this->name = new char[strlen(other_artist.name) + 1];
        strcpy(this->name, other_artist.name);
    } else {
        this->name = new char[4];
        strcpy(this->name, "N/A");
    }

    if (other_artist.topSongs != nullptr && other_artist.topSongsSize > 0){
        this->topSongs = new char*[other_artist.topSongsSize];
        for(int i = 0; i < other_artist.topSongsSize; i ++ ){ 
            this->topSongs[i] = new char[strlen(other_artist.topSongs[i]) + 1];
            strcpy(this->topSongs[i], other_artist.topSongs[i]);
        }

    } else {
        this->topSongs = nullptr;
        this->topSongsSize = 0;
    }
}

//!------------------------------- OVERWRITTING---------------------------------------------
Artist& Artist::operator=(const Artist& other_artist) { 
    if (this == &other_artist) return *this;

    if (this->name != nullptr) {
        delete[] this->name;
        this->name = nullptr;
    }
    
    if (other_artist.name != nullptr) {
        this->name = new char[strlen(other_artist.name) + 1];
        strcpy(this->name, other_artist.name);
    } else {
        this->name = new char[4];
        strcpy(this->name, "N/A");
    }
        

    if (this->topSongs != nullptr && this->topSongsSize > 0) { 
        for( int i = 0; i < this->topSongsSize; i ++) {
            delete[] this->topSongs[i];
        }
        delete[] this->topSongs;
        this->topSongs = nullptr;
        this->topSongsSize = 0;
    }

    if (other_artist.topSongs != nullptr && other_artist.topSongsSize > 0) {
        
        this->topSongs = new char*[other_artist.topSongsSize];
        this->topSongsSize = other_artist.topSongsSize;
        for(int i = 0; i < other_artist.topSongsSize; i ++ ) {
            this->topSongs[i] = new char[strlen(other_artist.topSongs[i]) + 1];
            strcpy(this->topSongs[i], other_artist.topSongs[i]);
        }
    } else {
        this->topSongs = nullptr;
        this->topSongsSize = 0;
    }


    this->monthlyListeners = other_artist.monthlyListeners;
    this->isVerified = other_artist.isVerified;

    return *this;
}

bool Artist::operator==(const Artist& other_artist) const {
    if (this->monthlyListeners != other_artist.monthlyListeners) {
        return false;
    }

    if (this->isVerified != other_artist.isVerified) {
        return false;
    }

    if (strcmp(this->name, other_artist.name) != 0){
        return false;
    }

    return true;
}

bool Artist::operator>(const Artist& other_artist) const {
    return this->monthlyListeners > other_artist.monthlyListeners;
}

bool Artist::operator<=(const Artist& other_artist) const {
    return !(*this > other_artist);
}

bool Artist::operator<(const Artist& other_artist) const {
    return this->monthlyListeners < other_artist.monthlyListeners;
}

bool Artist::operator>=(const Artist& other_artist) const {
    return !(*this < other_artist);
}

Artist& Artist:: operator++() {
        this->monthlyListeners++;
        return *this;
}

Artist& Artist:: operator--() { 
    this->monthlyListeners--;
    return *this;
}

Artist Artist:: operator++(int dummy) {
    Artist copy_artist = *this;
    ++(*this);
    return copy_artist; 
}

Artist Artist:: operator--(int dummy) {
    Artist copy_artist = *this;
    --(*this);
    return copy_artist; 
}

Artist Artist::operator+(int new_mListeners) const {
    Artist temp = *this;
    temp.monthlyListeners += new_mListeners;

    return temp;
}

Artist operator+(int new_mListeners, const Artist& artist) {
    return artist + new_mListeners;
}

Artist Artist::operator-(int remove_mListeners) const {
    if (remove_mListeners > this->monthlyListeners) {
        cout<<"Error: There is not enough monthly listeners to be removed!" << endl;
        return *this;
    }

    Artist temp = *this;
    temp.monthlyListeners -= remove_mListeners;
    return temp;
}

Artist& Artist::operator+=(int new_mListeners) {

    if (new_mListeners > 0 ) {
        this->monthlyListeners += new_mListeners;
    } else {
        cout << "Error: You can't add a negative number of monthly listeners (for removing monthly listeners use the operator '-') !";
    }

    return *this;

}

Artist& Artist::operator-=(int remove_mListeners) { 
    if (remove_mListeners > 0) {
        if (remove_mListeners < this->monthlyListeners) {
            this->monthlyListeners -= remove_mListeners;
        } else {
            this->monthlyListeners = 0;
        }
    }

    return *this;

}

const char* Artist::operator[](int index) const {
    if (this->topSongs == nullptr) {
        cout<<"Error: This artist does not have at least 1 song yet!" << endl;
        return "N/A";
    }

    if (index < 0 || index >= this->topSongsSize) {
        cout << "Error: Index out of range! try a index between 0 and " << (this->topSongsSize - 1) << "." << endl;
        return "N/A";
    }
    
    return this->topSongs[index];
}

istream& operator>>(istream& in, Artist& artist) { 

    //! VERIFY IF IS WORKING TO NOT USE IN >> INSIDE OF VALIIDATION, JUST CIN
    artist.monthlyListeners = get_int_input_validation(" <--- Enter artist's monthly listeners number: ");

    if ( artist.topSongs != nullptr ) { 
        for (int i = 0; i < artist.topSongsSize; i ++ ) {
            delete[] artist.topSongs[i];
        }
        delete[] artist.topSongs;
        artist.topSongs = nullptr;
    }

    artist.topSongsSize = 0;
    char is_verified_input = get_yesno_input_validation(" <--- Does the artist is verified?(y/n): ");

    artist.isVerified = ( is_verified_input == 'y' ? true : false);

    return in;
}

ostream& operator<<(ostream& os, const Artist& artist) {
   os << "\n|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    os << "|                ARTIST PROFILE                  |\n";
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    os << "|  NAME: " << artist.name << (artist.isVerified ? " [VERIFIED]" : "") << "\n";
    os << "|  MONTHLY LISTENERS: " << artist.monthlyListeners << "\n";
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    if (artist.topSongsSize == 0 || artist.topSongs == nullptr) {
        os << "|  [!] This artist has no top songs yet...       |\n";
    } else {
        os << "|  TOP SONGS:                                    |\n";
        for (int i = 0; i < artist.topSongsSize; i++) {
            os << "|    [" << (i + 1) << "]. " << artist.topSongs[i] << "\n"; 
        }
    }
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";

    return os;
}

//!-------------------------------SETTERS------------------------------------------
void Artist::addTopSong(const char* songName) { 
    if (songName == nullptr) return;

    char** tempTopSongs = new char*[this->topSongsSize + 1];

    if(this->topSongs != nullptr && this->topSongsSize > 0) {
        for (int i = 0; i < this->topSongsSize; i ++) {
            tempTopSongs[i] = this->topSongs[i];
        }
    }

    tempTopSongs[this->topSongsSize] = new char[strlen(songName) + 1];
    strcpy(tempTopSongs[this->topSongsSize], songName);

    if (this->topSongs != nullptr) {
        delete[] topSongs;
    }

    this->topSongs = tempTopSongs;
    this->topSongsSize ++;

    // cout << "AM ADAUGAT PIESA " << songName << endl;
}

void Artist::setName(const char* artist_name) {
    if (artist_name == nullptr) {
        return;
    }

    if(this->name != nullptr) {
        delete[] this->name;
        this->name = nullptr; 
    }

    this->name = new char[strlen(artist_name) + 1];
    strcpy(this->name, artist_name);
}
//!------------------------------- DESTRUCTORS---------------------------------------------
Artist::~Artist() { 
    // cout<<"DESTRUCTORUL PT ARTIST A AVUT LOC TEST!!"<<endl;
    if(this->name != nullptr) { 
        delete[] name;
        name = nullptr;
    }

    if(this->topSongs != nullptr) {
        for(int i = 0; i < topSongsSize; i ++ ){ 
            delete[] topSongs[i];
        }

        delete[] this->topSongs;
        this->topSongs = nullptr;
        this->topSongsSize = 0;
    }
}

