 #include <iostream>
#include <sstream>


#include "utils.h"
#include "artist.h"
#include "app.h"
#include "exceptions.h"


using namespace std;


// int Artist::counter = 1000;
//!-------------------------------CONSTRUCTORS---------------------------------------------
Artist::Artist()
: name("N/A"), monthlyListeners(0), isVerified(false) { 
    // cout<<"CONSTRUCTORUL FARA PARAMETRII"<<endl;    
    StreamingPlatformApp::getInstance().systemLog(to_string(this->getID()) + " ] New Artist added.");
}


Artist::Artist(const string& name, int monthlyListeners, const vector<string>& topSongs , bool isVerified)
: name(name), monthlyListeners(monthlyListeners), isVerified(isVerified) {
    // cout<<"CONSTRUCTORUL CU TOTI PARAMETRII"<<endl;
    StreamingPlatformApp::getInstance().systemLog(to_string(this->getID()) + " ] New Artist added.");

    
}

Artist::Artist(const string& name)
: name(name), monthlyListeners(0), isVerified(false) { 
    // cout<<"CONSTRUCTORUL CU NUME"<<endl;
    StreamingPlatformApp::getInstance().systemLog(to_string(this->getID()) + " ] New Artist added.");

}


//!------------------------------- OVERWRITTING---------------------------------------------

bool Artist::operator==(const Artist& other_artist) const {
    if (this->monthlyListeners != other_artist.monthlyListeners) {
        return false;
    }

    if (this->isVerified != other_artist.isVerified) {
        return false;
    }

    if (this->name != other_artist.name) {
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
        throw InvalidActionException("There are not enough monthly listeners to be removed!");
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

string Artist::operator[](int index) const {
    if (this->topSongs.empty()) {
        throw InvalidActionException("This artist does not have any songs registered yet!");
    }

    if (index < 0 || index >= static_cast<int> (this->topSongs.size())) {
        throw OutOfRangeException(index, static_cast<int> (this->topSongs.size()));
    }
    
    return this->topSongs[index];
}

istream& operator>>(istream& in, Artist& artist) { 

    //! VERIFY IF IS WORKING TO NOT USE IN >> INSIDE OF VALIIDATION, JUST CIN
    artist.monthlyListeners = get_int_input_validation(" <--- Enter artist's monthly listeners number: ");

    artist.topSongs.clear();

    char is_verified_input = get_yesno_input_validation(" <--- Does the artist is verified?(y/n): ");

    artist.isVerified = ( is_verified_input == 'y' ? true : false);

    return in;
}

// ostream& operator<<(ostream& os, const Artist& artist) {
//    os << "\n|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
//     os << "|                ARTIST PROFILE                  |\n";
//     os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
//     os << "|  NAME: " << artist.name << (artist.isVerified ? " [VERIFIED]" : "") << "\n";
//     os << "|  MONTHLY LISTENERS: " << artist.monthlyListeners << "\n";
//     os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
//     if (artist.topSongs.empty()) {
//         os << "|  [!] This artist has no top songs yet...       |\n";
//     } else {
//         os << "|  TOP SONGS:                                    |\n";
//         for (int i = 0; i < static_cast<int>(artist.topSongs.size()); i++) {
//             os << "|    [" << (i + 1) << "]. " << artist.topSongs[i] << "\n"; 
//         }
//     }
//     os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";

//     return os;
// }

//!-------------------------------SETTERS------------------------------------------
void Artist::addTopSong(const string& songName) { 
    if (songName.empty()) return;

    this->topSongs.push_back(songName);
    // cout << "AM ADAUGAT PIESA " << songName << endl;
}

void Artist::setName(const string& artist_name) {
    if (artist_name.empty()) return;
    this->name = artist_name;
}


//!------------------------------- INHERITED METHODS---------------------------------------------
string Artist::toString() const {
    ostringstream os;

    os << "\n|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    os << "|                ARTIST PROFILE                  |\n";
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    os << "|  NAME: " << this->name << (this->isVerified ? " [VERIFIED]" : "") << "\n";
    os << "|  MONTHLY LISTENERS: " << this->monthlyListeners << "\n";
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    if (this->topSongs.empty()) {
        os << "|  [!] This artist has no top songs yet...       |\n";
    } else {
        os << "|  TOP SONGS:                                    |\n";
        for (int i = 0; i < static_cast<int>(this->topSongs.size()); i++) {
            os << "|    [" << (i + 1) << "]. " << this->topSongs[i] << "\n"; 
        }
    }
    os << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";

    return os.str();
}
//!------------------------------- DESTRUCTORS---------------------------------------------


