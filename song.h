#pragma once

#include <iostream>
#include <string>
#include <vector>


#include "date.h"
#include "playable_item.h"

class Song: public playableItem { 
    // static int counter;
    // const int SONG_ID;
    std::string title;
    std::string artistName;
    int durationSec;
    int artistID;
    std::vector<std::string> artistsFeatures;
    long long streams;
    Date releaseDate;

public:

    //!     CONSTRUCTORS
    Song();
    Song(const std::string& title, const std::string& artistName, int durationSec, int artistID, const std::vector<std::string>& artistFeatures, long long streams, const Date& dateParam);
    Song(const std::string& title);

    //!     OVERWRITTING OPERATORS
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
    
    const std::string& operator[](int index) const;

    friend std::istream& operator>>(std::istream& in, Song& song);
    // friend std::ostream& operator<<(std::ostream& os, const Song& song);
    
    //!     GETTERS
    const std::string& getTitle() const { return this->title; }
    int getDuration() const { return this->durationSec; }
    int getSongsArtistID() const{return this->artistID; }
    const int getSongID() const { return this->getID(); }
    long long getStreams() const { return this->streams; }
    const std::vector<std::string>& getFeatures() const { return this->artistsFeatures; }
    const std::string& getArtistName() const { return this->artistName; }
    int getFeaturesNum() const { return this->artistsFeatures.size(); }
    //!     SETTERS

    void addFeature(const std::string& featureName);
    void setArtistID(int id) { this->artistID = id;};
    void setTitle(const std::string& song_title);
    void setArtistName(const std::string& artist_name);

    //! INHERITED FUNCTIONS FROM ABSTRACT CLASS -> PLAYABLEITEM
    void simulatePlayItem() override;
    long long calculateTotalStreams() const override;
    std::string toString() const override;

    //!     DESTRUCTOR
    // ~Song();

};
