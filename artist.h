#pragma once

#include <iostream>
#include <string>
#include <vector>

class Artist {
    static int counter;
    const int ARTIST_ID;
    std::string name;
    int monthlyListeners;
    std::vector<std::string> topSongs;
    // int topSongsSize;
    bool isVerified;


public:
    //!     CONSTRUCTORS
    Artist();
    Artist(const std::string& name, int monthlyListeners, const std::vector<std::string>& topSongs, bool isVerified);
    Artist(const std::string& name);
    // Artist(int monthlyListeners, bool isVerified);
    // Artist(const Artist& other_artist);

    //!     OVERWRITTING OPERATORS
    // Artist& operator=(const Artist& other_artist);
    
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
    friend std::ostream& operator<<(std::ostream& os, const Artist& artist);
    
    //! GETTERS
    static int getCounter() { return counter; }
    std::string getName() const { return name; }
    int getMonthlyListeners() const { return monthlyListeners; }
    bool getIsVerified() const { return isVerified; }
    const int getArtistID() const { return ARTIST_ID; }

    //! SETTERS
    
    void addTopSong(const std::string& songName);
    void setName(const std::string& artist_name);
    //!     DESTRUCTOR
    // ~Artist();

};
