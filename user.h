#pragma once

#include <iostream>
#include <string>
#include <vector>



class Song;

class User {
protected:
// displayProfile function for polimorphism
    static int counter;
    const int USER_ID;
    std::string username;
    std::string password;
    std::vector<Song*> liked_songs;

public:
    //!------------------------------CONSTRUCTORS--------------------------
    User();
    User(const std::string& username, const std::string& password, const std::vector<Song*>& liked_songs);
    User(const std::string& username, const std::string& password);

    //used for inheritance 
    User(int old_id, const std::string& username, const std::string& password, const std::vector<Song*>& liked_songs);

    // User(const User& other_user);

    //!--------------------------- OVERWRITTING OPERATORS--------------------
    // User& operator=(const User& other_user);
    User& operator+=(Song& new_song);
    User& operator-=(int removed_index);

    bool operator==(const User& other_user) const;
    bool operator>(const User& other_user) const;
    bool operator<=(const User& other_user) const;

    const Song& operator[](int index) const;
    
    friend std::istream& operator>>(std::istream& in, User& user);
    friend std::ostream& operator<<(std::ostream& os, const User& user);
    //!--------------------------------GETTERS--------------------------
    const std::string& getUsername() const { return username;}
    const std::string& getPassword() const { return password;}
    const int getUserID() const {return USER_ID;}
    const std::vector<Song*>& getLikedSongs() const {return liked_songs;}
    int getSongsArrSize() const{return liked_songs.size();}
    //!--------------------------------SETTERS--------------------------
    
    void setUsername(const std::string& new_username);
    void setPassword(const std::string& new_password);
    void add_likedSong(Song& new_song);
    //!--------------------------------------------------------------------
    virtual void displayProfile() const;
    //!--------------------------------DESTRUCTORS--------------------------
    virtual ~User() = default;

};
