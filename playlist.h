#pragma once

#include <iostream>
// #include "song.h"
#include "playable_item.h"

class Song;


class Playlist: public playableItem {
    static int counter;
    const int PLAYLIST_ID;
    char* name;
    char* owner_username;
    Song** songs;
    int songsNum;
    int totalDurationSec;

public:
    //!     CONSTRUCTORS
    Playlist();
    
    //! song* const* -> lock the addresses but you can access the object 
    //! const Song* const* -> lock both addresses and the object from that address
    Playlist(const char* name, const char* owner_username, Song** songs, int songsNum, int totalDuration );
    Playlist(const char* name, const char* owner_username);
    Playlist(const Playlist& other_playlist);

    //!     OVERWRITTING OPERATORS
    Playlist& operator=(const Playlist& other_playlist);

    Playlist& operator-=(int removed_index);
    Playlist& operator+=(Song& new_song);

    bool operator==(const Playlist& other_playlist) const;
    bool operator>(const Playlist& other_playlist) const;
    bool operator<=(const Playlist& other_playlist) const;
    bool operator<(const Playlist& other_playlist) const;
    bool operator>=(const Playlist& other_playlist) const;

    const Song& operator[](int index) const;

    friend std::istream& operator>>(std::istream& in, Playlist& playlist);
    friend std::ostream& operator<<(std::ostream& os, const Playlist& playlist);
    
    //!     GETTERS
    static int getCounter() { return counter; }
    const char* getName() const { return name; }
    const char* getOwnerUsername() const { return owner_username; }
    const int getPlaylistID() const { return PLAYLIST_ID; }
    Song** getSongs() const { return songs; }
    int getSongsSize() const { return songsNum; } 
    int getTotalDuration() const { return totalDurationSec; }
    //!     SETTERS

    //todo const Song&?
    void addSong(Song& newSong);
    void removeSong(int removed_index);
    
    void setName(const char* name);
    void setOwnerUsername(const char* new_owner);
    
    //! INHERITED FUNCTIONS FROM ABSTRACT CLASS -> PLAYABLEITEM
    void simulatePlayItem() override;
    long long calculateTotalStreams() const override;

    //!     DESTRUCTORS
    ~Playlist();

};

