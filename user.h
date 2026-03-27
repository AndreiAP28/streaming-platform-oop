#pragma once

#include <iostream>
// #include "song.h"

class Song;

class User {
    static int counter;
    const int USER_ID;
    char* username;
    char* password;
    Song** liked_songs;
    int likedSongs_num;

public:
    //!------------------------------CONSTRUCTORS--------------------------
    User();
    User(const char* username, const char* password, Song** liked_songs, int likedSongs_num);
    User(const char* username, const char* password);
    User(const User& other_user);

    //!--------------------------- OVERWRITTING OPERATORS--------------------
    User& operator=(const User& other_user);
    User& operator+=(Song& new_song);
    User& operator-=(int removed_index);

    bool operator==(const User& other_user) const;
    bool operator>(const User& other_user) const;
    bool operator<=(const User& other_user) const;

    const Song& operator[](int index) const;
    
    friend std::istream& operator>>(std::istream& in, User& user);
    friend std::ostream& operator<<(std::ostream& os, const User& user);
    //!--------------------------------GETTERS--------------------------
    const char* getUsername() const { return username;}
    const char* getPassword() const { return password;}
    const int getUserID() const {return USER_ID;}
    Song** getLikedSongs() const {return liked_songs;}
    int getSongsArrSize() const{return likedSongs_num;}
    //!--------------------------------SETTERS--------------------------
    
    void setUsername(const char* new_username);
    void setPassword(const char* new_password);
    void add_likedSong(Song& new_song);
    //!--------------------------------DESTRUCTORS--------------------------
    ~User();

};
