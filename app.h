#pragma once

#include <iostream>


class Artist;
class Playlist; 
class Song;
class User;
class playableItem;

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

    playableItem** music_queue;
    int queue_counter;
    static const int MAX_QUEUE_SIZE;
    

public:
    StreamingPlatformApp();
    ~StreamingPlatformApp();

    void run();
    void preloadedData();

private:
    void connectingMenu();
    void signUp(); 
    void login();
    
    void mainMenu(bool& app_run_flag);
    void printMainMenu();
    
    void addinDB();
    void AddNewArtistinDB();
    void AddNewSonginDB();
    
    void exploreLibrary();
    void CreatePlaylist();
    void ManagePlaylist();
    void ViewPlaylists();
    void ViewArtistsDB();
    void ViewSongsDB();

    void playQueue();

    void AccountSettings();
    void ManageSubscriptions();
    void ManageLikedSongs();

    int find_artist_index_inDB_byID(int chosenID_by_user) const;
    int find_song_index_inDB_byID(int chosenID_by_user) const;
    int find_playlist_index_inDB_byID(int chosenID_by_user) const;
    int find_user_index_inDB() const;
    bool duplicate_name_ignoreCase(const char* name1, const  char* name2);
};