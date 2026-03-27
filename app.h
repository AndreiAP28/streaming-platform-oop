#pragma once

#include <iostream>
// #include "artist.h"
// #include "song.h"
// #include "playlist.h" 
// #include "user.h"

class Artist;
class Playlist; 
class Song;
class User;

class StreamingPlatformApp {
    Artist** artists_database;
    int artists_counter;

    Playlist** playlists_database;
    int playlists_counter;

    Song** songs_database;
    int songs_counter;

    User** users_database;
    int users_counter;
    User* current_user;

public:
    StreamingPlatformApp();
    ~StreamingPlatformApp();

    void run();

private:
    void connectingMenu();
    void signUp(); 
    void login();
    
    void mainMenu(bool& app_run_flag);
    void printMainMenu();
    void AddNewArtistinDB();
    void AddNewSonginDB();
    void CreatePlaylist();
    void ManagePlaylist();
    void ViewPlaylists();
    void ManageLikedSongs();
    void ViewArtistsDB();
    void ViewSongsDB();

    int find_artist_index_inDB_byID(int chosenID_by_user) const;
    int find_song_index_inDB_byID(int chosenID_by_user) const;
    int find_playlist_index_inDB_byID(int chosenID_by_user) const;
    bool duplicate_name_ignoreCase(const char* name1, const  char* name2);
};