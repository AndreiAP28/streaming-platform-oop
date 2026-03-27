#pragma once

#include <iostream>

class Artist {
    static int counter;
    const int ARTIST_ID;
    char *name;
    int monthlyListeners;
    char** topSongs;
    int topSongsSize;
    bool isVerified;


public:
    //!     CONSTRUCTORS
    Artist();
    Artist(const char* name, int monthlyListeners, int topSongsSize, const char** topSongs, bool isVerified);
    Artist(const char* name);
    // Artist(int monthlyListeners, bool isVerified);
    Artist(const Artist& other_artist);

    //!     OVERWRITTING OPERATORS
    Artist& operator=(const Artist& other_artist);
    
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

    const char* operator[](int index) const;

    friend std::istream& operator>>(std::istream& in, Artist& artist);
    friend std::ostream& operator<<(std::ostream& os, const Artist& artist);
    
    //! GETTERS
    static int getCounter() { return counter; }
    const char* getName() const { return name; }
    int getMonthlyListeners() const { return monthlyListeners; }
    bool getIsVerified() const { return isVerified; }
    const int getArtistID() const { return ARTIST_ID; }

    //! SETTERS
    
    void addTopSong(const char* songName);
    void setName(const char* artist_name);
    //!     DESTRUCTOR
    ~Artist();

};
