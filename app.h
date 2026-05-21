#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <list>
#include <vector>
#include <fstream>



class Artist;
class Playlist; 
class Song;
class User;
class playableItem;

class StreamingPlatformApp {

    std::unordered_map<int, Artist*> artists_database;
    std::vector<Playlist*> playlists_database;
    std::unordered_map<int, Song*> songs_database;
    std::unordered_map<std::string, User*> users_database;
    User* current_user;
    std::list<playableItem*> music_queue;
    static const int MAX_QUEUE_SIZE;
    // std::ofstream f;

    StreamingPlatformApp();
    

public:
    StreamingPlatformApp(const StreamingPlatformApp& other_app) = delete;
    StreamingPlatformApp& operator=(const StreamingPlatformApp& other_app) = delete;

    static StreamingPlatformApp& getInstance();
    void systemLog(const std::string& message);

    void run();
    ~StreamingPlatformApp() noexcept;
    
private:
    void preloadedData();

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
    void DisplayLogs();

    bool duplicate_name_ignoreCase(const std::string& name1, const std::string& name2);
};