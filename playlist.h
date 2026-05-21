#pragma once

#include <iostream>
#include <vector>
#include <string>


// #include "song.h"
#include "playable_item.h"

class Song;
//! CAND FOLOSIM UN POINTER CATRE O CLASA E DE AJUNS SA II PROMITEM COMPILATORULUI CA ACEA CLASA EXISTA
//! CAND FOLOSIM DIRECT PT MOSTENIRE O CLASA DIN ALT HEADER SAU ATRIBUT DIRECT ACEA CLASA TREBUIE INCLUS ACEL HEADER

class Playlist: public playableItem {
    // static int counter;
    // const int PLAYLIST_ID;
    std::string name;
    std::string owner_username;
    std::vector<Song*> songs;
    int totalDurationSec;

public:
    //!     CONSTRUCTORS
    Playlist();
    
    //! song* const* -> lock the addresses but you can access the object 
    //! const Song* const* -> lock both addresses and the object from that address
    Playlist(const std::string& name, const std::string& owner_username, const std::vector<Song*>& songs, int totalDuration );
    Playlist(const std::string& name, const std::string& owner_username);

    //!     OVERWRITTING OPERATORS

    Playlist& operator-=(int removed_index);
    Playlist& operator+=(Song& new_song);

    bool operator==(const Playlist& other_playlist) const;
    bool operator>(const Playlist& other_playlist) const;
    bool operator<=(const Playlist& other_playlist) const;
    bool operator<(const Playlist& other_playlist) const;
    bool operator>=(const Playlist& other_playlist) const;

    const Song& operator[](int index) const;

    friend std::istream& operator>>(std::istream& in, Playlist& playlist);
    // friend std::ostream& operator<<(std::ostream& os, const Playlist& playlist);
    
    //!     GETTERS
    // static int getCounter() { return counter; }
    std::string getName() const { return name; }
    std::string getOwnerUsername() const { return owner_username; }
    const int getPlaylistID() const { return this->getID(); }
    const std::vector<Song*>& getSongs() const { return songs; }
    int getSongsSize() const { return songs.size(); } 
    int getTotalDuration() const { return totalDurationSec; }
    //!     SETTERS

    //todo const Song&?
    void addSong(Song& newSong);
    void removeSong(int removed_index);
    
    void setName(const std::string& name);
    void setOwnerUsername(const std::string& new_owner);
    
    //! INHERITED FUNCTIONS FROM ABSTRACT CLASS -> PLAYABLEITEM
    void simulatePlayItem() override;
    long long calculateTotalStreams() const override;
    std::string toString() const override;
    //!     DESTRUCTORS
    // ~Playlist();

};

