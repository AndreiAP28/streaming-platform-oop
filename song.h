#pragma once

#include <iostream>

class Song { 
    static int counter;
    const int SONG_ID;
    char* title;
    int durationSec;
    int artistID;
    int featuresSize;
    char** artistsFeatures;
    long long streams;

public:

    //!     CONSTRUCTORS
    Song();
    Song(const char* title, int durationSec, int artistID,int featuresSize, const char** artistFeatures, long long streams);
    Song(const char* title);
    Song(const Song& other_song);

    //!     OVERWRITTING OPERATORS

    Song& operator=(const Song& other_song);

    
    Song& operator++(); //? ++ song
    Song& operator--(); //? -- song
    Song operator++(int dummy); //? song ++
    Song operator--(int dummy); //? song --
    
    Song operator+(int add_new_streams) const;
    friend Song operator+(int add_new_streams, const Song& song);
    Song operator-(int remove_streams_number) const;
    
    Song& operator+=(int add_streams_number);
    Song& operator-=(int remove_streams_number);
    
    bool operator==(const Song& other_song) const;
    bool operator>(const Song& other_song) const;
    bool operator<=(const Song& other_song) const;
    bool operator<(const Song& other_song) const;
    bool operator>=(const Song& other_song) const;
    
    const char* operator[](int index) const;

    friend std::istream& operator>>(std::istream& in, Song& song);
    friend std::ostream& operator<<(std::ostream& os, const Song& song);
    
    //!     GETTERS
    const char* getTitle() const {
        return this->title;
    }

    int getDuration() const {
        return this->durationSec;
    }

    int getSongsArtistID() const{ 
        return this->artistID;
    }

    const int getSongID() const {
        return this->SONG_ID;
    }

    long long getStreams() const {
        return this->streams;
    }

    char** getFeatures() const {
        return this->artistsFeatures;
    }

    int getFeaturesNum() const {
        return this->featuresSize;
    }

    //!     SETTERS

    void addFeature(const char* featureName);
    void setArtistID(int id) { this->artistID = id;};
    void setTitle(const char* song_title);

    //!     DESTRUCTOR
    ~Song();

};
