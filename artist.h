#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "IObject.h"

class Artist: public IObject {
    std::string name;
    int monthlyListeners;
    std::vector<std::string> topSongs;
    bool isVerified;


public:
    //!     CONSTRUCTORS
    Artist();
    Artist(const std::string& name, int monthlyListeners, const std::vector<std::string>& topSongs, bool isVerified);
    Artist(const std::string& name);

    //!     OVERWRITTING OPERATORS
    
    Artist& operator++(); //? ++artist
    Artist& operator--(); //? --artist

    Artist operator++(int dummy);
    Artist operator--(int dummy);
        
    Artist operator+(int new_mListeners) const;
    friend Artist operator+(int new_mListeners, const Artist& artist);
    
    Artist operator-(int remove_mListeners) const;

    Artist& operator-=(int remove_mListeners);
    Artist& operator+=(int new_mListeners);

    bool operator==(const Artist& other_artist) const;
    bool operator>(const Artist& other_artist) const;
    bool operator<=(const Artist& other_artist) const;
    bool operator<(const Artist& other_artist) const;
    bool operator>=(const Artist& other_artist) const;

    std::string operator[](int index) const;

    friend std::istream& operator>>(std::istream& in, Artist& artist);
    // friend std::ostream& operator<<(std::ostream& os, const Artist& artist);
    
    //! GETTERS
    std::string getName() const { return name; }
    int getMonthlyListeners() const { return monthlyListeners; }
    bool getIsVerified() const { return isVerified; }
    const int getArtistID() const { return this->getID(); }

    //! SETTERS
    
    void addTopSong(const std::string& songName);
    void setName(const std::string& artist_name);

    //! INHERITED METHODS
    std::string toString() const override;
    //!     DESTRUCTOR
    // ~Artist();

};
