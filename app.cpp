#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cctype>

// #include "classes.h"

#include "utils.h"
#include "app.h"
#include "artist.h"
#include "song.h"
#include "playlist.h" 
#include "user.h"

using namespace std;

StreamingPlatformApp::StreamingPlatformApp() 
: artists_database(nullptr), artists_counter(0), playlists_database(nullptr), playlists_counter(0), songs_database(nullptr), songs_counter(0), users_database(nullptr), users_counter(0), current_user(nullptr) {}

bool StreamingPlatformApp::duplicate_name_ignoreCase(const char* name1, const char* name2) {
    if (name1 == nullptr || name2 == nullptr) {
        return false;
    }

    if (strlen(name1) != strlen(name2)) {
        return false;
    }

    int i = 0;
    while(name1[i] != '\0') { 
        if (tolower(name1[i]) != tolower(name2[i])) {
            return false;
        }
        i ++;
    }

    return true;
    
}

int StreamingPlatformApp::find_artist_index_inDB_byID(int chosenID_by_user) const {
    for (int i = 0; i < this->artists_counter; i ++) {
        if (artists_database[i]->getArtistID() == chosenID_by_user) {
            return i;
        }
    }

    return -1;
}

int StreamingPlatformApp::find_song_index_inDB_byID(int chosenID_by_user) const {
    for (int i = 0; i < this->songs_counter; i ++) {
        if (songs_database[i]->getSongID() == chosenID_by_user) {
            return i;
        }
    }

    return -1;
}

int StreamingPlatformApp::find_playlist_index_inDB_byID(int chosenID_by_user) const {
    for (int i = 0; i < this->playlists_counter; i ++ ){ 
        if (this->playlists_database[i]->getPlaylistID() == chosenID_by_user) {
            if (!strcmp(this->playlists_database[i]->getOwnerUsername(), this->current_user->getUsername())) {
                return i;
            }
        }
    }

    return -1;
}

void StreamingPlatformApp::printMainMenu() {
    cout << "\n|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    cout << "|           STREAMING MUSIC PLATFORM APP         |\n";
    cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    cout << "|  Hello, " << this->current_user->getUsername() << "! How was your day?              |\n";
    cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    cout << "|  [1]. Add a New Artist in Database             |\n";
    cout << "|  [2]. Add a New Song in Database               |\n";
    cout << "|  [3]. Create a Playlist                        |\n";
    cout << "|  [4]. Manage a Playlist (Add/Remove songs)     |\n";
    cout << "|  [5]. View Playlists                           |\n";
    cout << "|  [6]. View Registered Artists                  |\n";
    cout << "|  [7]. View Registered Songs                    |\n";
    cout << "|  [8]. Manage Liked Songs                       |\n";
    cout << "|  [9]. View My Profile                          |\n";
    cout << "|  [10]. Sign out                                |\n";
    cout << "|  [0]. EXIT                                     |\n";
    cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";

}

void StreamingPlatformApp::AddNewArtistinDB() {
    char input_artistName[256];
    get_name_input_validation(" <--- Enter artist name: ",input_artistName, 256);

    bool already_exist = false;
    for (int i = 0; i < this->artists_counter && !already_exist; i ++ ){
        if (duplicate_name_ignoreCase((*this->artists_database[i]).getName(), input_artistName)) {
            already_exist = true;
        }
    }

    if (already_exist) {
        cout << "\n[!] Error: An artist named '" << input_artistName << "' already exists!\n";
        return;     
    }
    Artist* tempNewArtist = new Artist();
    tempNewArtist->setName(input_artistName);
    cin >> *tempNewArtist;

    Artist** tempArtistsDB = new Artist*[this->artists_counter + 1];
    for ( int i = 0; i < this->artists_counter; i ++ ){ 
        tempArtistsDB[i] = this->artists_database[i];
    }
    tempArtistsDB[this->artists_counter] = tempNewArtist;

    if (this->artists_database != nullptr ) {
        delete[] this->artists_database;
        this->artists_database = nullptr;
    }

    this->artists_database = new Artist*[this->artists_counter ++];
    for (int i = 0;i < this->artists_counter; i ++) {
        this->artists_database[i] = tempArtistsDB[i];
    }

    delete[] tempArtistsDB;

    //! this->artists_database = tempArtistsDB;

    cout << "\n  Artist " << tempNewArtist->getName() << " added with ID: " << (*tempNewArtist).getArtistID() << endl;
}

void StreamingPlatformApp::AddNewSonginDB() {

    if (artists_counter == 0) {
        cout << "\n [!] Error: No artists in database. Add an artist first!\n";
        return;
    }

 
    char input_artistName[256];
    get_name_input_validation("\n [?] Enter the name of the Artist for the new song: ", input_artistName, 256);

    int artistIndex = -1;
    int artistID = -1;
    for (int i = 0; i < artists_counter && artistIndex == -1; i ++) {
        if (duplicate_name_ignoreCase(artists_database[i]->getName(), input_artistName)) {
            artistIndex = i;
            artistID = (artists_database[i])->getArtistID();
        }
    }
    

    if (artistIndex == -1) {
        cout << " [!] Error: Artist '" << input_artistName << "' not found. \n";
        cout << " [i] Tip: Check spelling or add the artist to the database first.\n";
        return;
    }
    
    char input_songTitle[256];
    get_name_input_validation(" <--- Enter song name: ", input_songTitle, 256);
    
    bool already_exist = false;
    for (int i = 0; i < this->songs_counter && !already_exist; i ++){
        if (duplicate_name_ignoreCase(this->songs_database[i]->getTitle(), input_songTitle)
            && (*this->songs_database)->getSongsArtistID() == artistID) {
                already_exist = true;
            }
    }
     
    if (already_exist) {
        cout << "\n[!] Error: A song with the title '" << input_songTitle << "' already exists!\n";
        return;
    }
    
    Song* new_song = new Song();
    new_song->setTitle(input_songTitle);
    new_song->setArtistID(artistID);
    cin >> *new_song;

    Song** tempSongsDB = new Song*[this->songs_counter + 1];
    for ( int i = 0;i < this->songs_counter; i ++ ){
        tempSongsDB[i] = this->songs_database[i];
        // cout <<"\tempUsersDB---------- SONG ID in TEMP: -> " << tempSongsDB[i]->getSongID() << " NAME: "<< new_song->getTitle() << "-----------\n";
    }

    tempSongsDB[this->songs_counter] = new_song;
    // cout <<"\n---------- SONG ID: -> " << new_song->getSongID() << " NAME: "<< new_song->getTitle()<< "-----------\n";
    if (this->songs_database != nullptr) {
        delete[] this->songs_database;
        songs_database = nullptr;
    }

    this->songs_database = new Song*[++(this->songs_counter)];
    for (int i = 0;i < songs_counter; i ++ ){
        this->songs_database[i] = tempSongsDB[i];
    }

    delete[] tempSongsDB;

    //! this->songs_database = tempSongsDB; 2nd method???
    //! gen de ce e gresit ca practic dupa ce copiaza toate piesele vechi in temporar + piesa noua, songs_database ia adresa lui temp

    this->artists_database[artistIndex]->addTopSong(new_song->getTitle());

    cout <<"\nSong '" << new_song->getTitle() << "' added to the Database!\n";


}

void StreamingPlatformApp::CreatePlaylist() {
    cout << "\n~~~~~~~~~~~~~~~~~~~~ CREATE NEW PLAYLIST ~~~~~~~~~~~~~~~~~~~~\n";
    
    char temp_playlistName[256];
    get_name_input_validation(" <--- Enter a name for your new playlist: ", temp_playlistName, 256);

    Playlist* newPlaylist = new Playlist();
    newPlaylist->setName(temp_playlistName);
    newPlaylist->setOwnerUsername(this->current_user->getUsername());

    Playlist** temp_PlaylistDB = new Playlist*[playlists_counter + 1];

    for (int i = 0; i < playlists_counter; i ++) {
        temp_PlaylistDB[i] = playlists_database[i];
    }

    temp_PlaylistDB[playlists_counter] = newPlaylist;
    if ( playlists_database != nullptr) {
        delete[] playlists_database;
        playlists_database = nullptr;
    }
    //! playlists_database = temp_PlaylistDB;
    playlists_database = new Playlist*[++playlists_counter];
    for (int i = 0; i < playlists_counter; i ++) {
        playlists_database[i] = temp_PlaylistDB[i];
    }

    delete[] temp_PlaylistDB;
       

    cout << " [SUCCESS] Playlist '" << temp_playlistName << "' has been created!\n";
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

}

void StreamingPlatformApp::ManagePlaylist() {

    if (playlists_counter == 0 ){
        cout <<"\n[!] No playlist exist. Please create one first!\n";
        return;
    }

    bool user_has_playlists = false;
    for (int i = 0; i < playlists_counter; i ++ ){
        if (!strcmp(this->playlists_database[i]->getOwnerUsername(), this->current_user->getUsername())) {
            user_has_playlists = true;
        }
    }

    if (!user_has_playlists) {
        cout << "\n[!] You haven't created any playlist yet!";
        return;
    }

    cout << "\n ~~~~~~~~~~~~~~~~~~~~~~~~~ MANAGE PLAYLISTS ~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    for ( int i = 0; i < playlists_counter; i ++) {
        if(!strcmp(playlists_database[i]->getOwnerUsername(), current_user->getUsername())) {
            cout << "[ ID: " << playlists_database[i]->getPlaylistID() << "] " << playlists_database[i]->getName() << "\n";
        }
    }
    cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";


    int playlistID_input = get_int_input_validation("\n <--- Enter the ID of the playlist you want to manage: ");

    int playlist_index_input = this->find_playlist_index_inDB_byID(playlistID_input);
    if (playlist_index_input == -1) {
        cout << " [!] Invalid ID or you do not own this playlist!\n";
        return;
    }

    int user_choice = -1;
    while (user_choice != 0) {
        cout << "\n|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    cout << "|  MANAGING: " << playlists_database[playlist_index_input]->getName() << "                            |\n";
    cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    cout << "|  [1]. Add a Song to Playlist                   |\n";
    cout << "|  [2]. Remove a Song from Playlist              |\n";
    cout << "|  [3]. View Playlist Tracks                     |\n";
    cout << "|  [0]. Go Back to Main Menu                     |\n";
    cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
        user_choice = get_int_input_validation("Choose an action: ");

        switch (user_choice) {
            case 1: {
                if (songs_counter == 0) {
                    cout << "[!] There are no songs in database!\n";
                    break;
                }

                cout << "\n ~~~~~~~~~~~~~~~~~ REGISTERED SONGS ~~~~~~~~~~~~~~~~~\n";
                for ( int i = 0; i < songs_counter; i ++ ){
                    cout << "[" << i + 1 << "] " << songs_database[i]->getTitle() << endl;
                }
                cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

                int song_index_input = get_int_input_validation(" <--- Enter the index of the song you want to add: ");
                song_index_input --;
               
                if ( song_index_input >= 0 && song_index_input < songs_counter) {

                    bool already_exist = false;
                    int current_playlist_size = playlists_database[playlist_index_input]->getSongsSize();

                    for (int i = 0;i < current_playlist_size && !already_exist; i ++) {
                        if (songs_database[song_index_input]->getSongID() == (*playlists_database[playlist_index_input])[i].getSongID()){
                            already_exist = true;
                        }
                    }

                    if (already_exist) {
                        cout << "[!] This song is already in playlist!\n";
                    } else {
                        playlists_database[playlist_index_input]->addSong(*songs_database[song_index_input]);
                        cout << "[SUCCESS] Song added to playlist!\n";
                    }
                
                }  else {
                    cout << "[!] Invalid selection!\n";
                }
                break;
            }
            
            case 2: {
                int playlist_size = (*playlists_database[playlist_index_input]).getSongsSize();
                if (playlist_size == 0) {
                    cout <<" [!] This playlist is already empty!\n";
                    break;
                }

                cout << "\n~~~~~~~~~~~~~~~~ TRACKS IN PLAYLIST ~~~~~~~~~~~~~~~~\n";
                for ( int i = 0; i < playlist_size;  i ++) {
                        cout << "[" << i + 1 << "] -> " << (*playlists_database[playlist_index_input])[i].getTitle() << endl;
                }
                cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

                int remove_index_input = get_int_input_validation("\n <--- Enter the index of the song that you want to remove: ");
                remove_index_input --;

                if (remove_index_input >= 0 && remove_index_input < (*playlists_database[playlist_index_input]).getSongsSize()) {
                    (*playlists_database[playlist_index_input]).removeSong(remove_index_input); 
                    cout << "[SUCCESS] Song removed from playlist!\n";
                } else {
                    cout << "[!] Invalid song selection.\n";
                }
                break;
            }

            case 3:
                cout << *playlists_database[playlist_index_input];
                break;

            case 0: 
                cout << "Returning to Main Menu ! .... \n";
                break;
            default:
                cout << "[!] Invalid Choice. Try again!\n";
        }

    }


}

void StreamingPlatformApp::ViewPlaylists() {
    if (playlists_counter == 0) {
        cout << "[!] There are no playlists registered yet.\n";
        return;
    }

    cout << "\n~~~~~~~~~~~~~~~ " << this->current_user->getUsername() << " PLAYLISTS ~~~~~~~~~~~~~~~\n";

    for (int i = 0; i < playlists_counter; i ++) {
        if (!strcmp(this->playlists_database[i]->getOwnerUsername(), this->current_user->getUsername())) {
            cout << "   <- [ ID: " << playlists_database[i]->getPlaylistID() << "] - " << playlists_database[i]->getName() << " (" << playlists_database[i]->getSongsSize() << " tracks)\n"; 
        }
    }
    cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

    char user_input = get_yesno_input_validation("\n[?] Would you like to view more about a playlist? (y/n): ");

    if (user_input == 'y' || user_input == 'Y') {
        int chosenIndex = get_int_input_validation(" <--- Enter the Playlist ID: ");

        int playlist_index_input = this->find_playlist_index_inDB_byID(chosenIndex);
        if (playlist_index_input == -1) {
            cout << " [!] Invalid ID or you do not own this playlist!\n";
            return;
        }

        cout << *playlists_database[chosenIndex];
    }
}

void StreamingPlatformApp::ManageLikedSongs() {
    int user_choice = -1;
    bool show_menu = true, liked_songs_looping = true;
    while (liked_songs_looping) {
        if (show_menu) {
            cout << "\n|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
            cout << "|        LIKED SONGS MENU        |\n";
            cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
            cout << "|  [1]. View My Liked Songs      |\n";
            cout << "|  [2]. Add a Song to Liked      |\n";
            cout << "|  [3]. Remove a Song from Liked |\n";
            cout << "|  [0]. Go Back to Main Menu     |\n";
            cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
        }

        user_choice = get_int_input_validation("Choose an action: ");
    
        switch (user_choice) {
            case 1: {
                int liked_songs_num = current_user->getSongsArrSize();
                if (liked_songs_num) {
                    cout <<"\n~~~~~~~~~~~~~~~~~ Your Liked Songs ~~~~~~~~~~~~~~~~~\n";

                    for (int i = 0;i < liked_songs_num; i ++) {
                        cout << "[ ID: " << current_user->getLikedSongs()[i]->getSongID() << "] " << current_user->getLikedSongs()[i]->getTitle() << "\n"; 
                    }
                    cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

                } else {
                    cout << "\n[!] You don't have any liked songs yet!\n";
                }
                show_menu = true;
                break;
            }
        
            case 2: {

                if (songs_counter == 0 ) {
                    cout << "[!] There are no songs registered yet.\n";
                    show_menu = true;
                    break;
                }

                cout << "\n~~~~~~~~~~~~~~~~~~ REGISTERED SONGS ~~~~~~~~~~~~~~~~~~\n";
                for (int idx_songsdb = 0; idx_songsdb < songs_counter; idx_songsdb ++) {
                    int current_artistID = songs_database[idx_songsdb]->getSongsArtistID();

                    char current_artistName[256];
                    int artistIndex_inDB = find_artist_index_inDB_byID(current_artistID); 

                    if (artistIndex_inDB != -1) {
                        strcpy(current_artistName, artists_database[artistIndex_inDB]->getName());
                    } else {
                        strcpy(current_artistName, "Unknown");
                    }

                    cout << " <- ID: [" << songs_database[idx_songsdb]->getSongID() << "] -> "<< songs_database[idx_songsdb]->getTitle() << " - " << current_artistName << "\n";
                }
                cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

                int songID_input = get_int_input_validation("\n <--- What song do you like (type Song ID): ");
   

                int index_in_db = this->find_song_index_inDB_byID(songID_input);

                if (index_in_db == -1) {
                    cout << " [!] Invalid ID. Going back to menu!\n";
                    show_menu = true;
                    break;
                }
                

                // bool already_liked = false;
                // for (int i = 0; i < current_user->getSongsArrSize() && !already_liked; i++ ) {
                //     if (songs_database[index_in_db]->getSongID() == current_user->getLikedSongs()[i]->getSongID()) {
                //         cout << "[!] You have already liked this song!\n";
                //         show_menu = true;
                //         already_liked = true;
                //     }  
                // }
                // if (!already_liked) {
                *current_user += *songs_database[index_in_db];
                // }

                
                show_menu = true;
                break;
            }
            
            case 3: {
                int liked_songs_num = current_user->getSongsArrSize();
                if (!liked_songs_num) {
                    cout << "\n[!] You don't have any liked songs to remove!\n";
                    show_menu = true;
                    break;
                }

                cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~ YOUR LIKED SONGS ~~~~~~~~~~~~~~~~~~~~~\n";
                for (int i = 0;i < liked_songs_num; i ++ ){
                    cout << " <- ID: [" << current_user->getLikedSongs()[i]->getSongID() << "] -> " << current_user->getLikedSongs()[i]->getTitle() << "\n";
                }

                int songID_input = get_int_input_validation("\n[?] What song do you like (type Song ID): ");
                int index_in_db = this->find_song_index_inDB_byID(songID_input);
                if (index_in_db == -1) {
                    cout << " <--- [!] Invalid ID. Going back to menu!\n";
                    show_menu = true;
                    break;
                }

                *current_user -= index_in_db;

                show_menu = true;
                break;

            }
            
                case 0:
                cout << "\nReturning to Main Menu...\n";
                liked_songs_looping = false;
                break;
            
            default:
                cout << "\n[!] Invalid choice. Try again!\n";
                show_menu = false;
        }   

    }

}

void StreamingPlatformApp::ViewArtistsDB() {
    if (artists_counter == 0) {
        cout << "[!] There are no artists registered yet.\n";
        return;
    }

    cout << "\n~~~~~~~~~~~~~~~~~~~ REGISTERED ARTISTS ~~~~~~~~~~~~~~~~~~~\n";
    for (int i = 0;i < artists_counter; i ++) {
        cout << "<- ID: [" << artists_database[i]->getArtistID() << "] -> " << artists_database[i]->getName() << " " << (artists_database[i]->getIsVerified() ? " (VERIFIED)" : "") << "\n";
    }
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

    char user_input = get_yesno_input_validation("\n[?] Would you like to view a specific artist's full profile? (y/n): ");

    if (user_input == 'y' || user_input == 'Y') {
        int chosenID = get_int_input_validation(" <--- Enter the Artist ID: ");
       

        int index_in_db = find_artist_index_inDB_byID(chosenID);
    
        // if exist in database print it and increase the monthlylisteners like you listened to it
        if (index_in_db != -1) {
            ++(*artists_database[index_in_db]);
            cout << *artists_database[index_in_db];
        } else {
            cout <<" <--- [!] Invalid ID. Going back to menu!\n";
        }
    }
}

void StreamingPlatformApp::ViewSongsDB() { 
    if (songs_counter == 0 ) {
        cout << "[!] There are no songs registered yet.\n";
        return;
    }

        cout << "\n~~~~~~~~~~~~~~~~~~~ REGISTERED SONGS ~~~~~~~~~~~~~~~~~~~\n";
    for (int idx_songsdb = 0; idx_songsdb < songs_counter; idx_songsdb ++) {
        int current_artistID = songs_database[idx_songsdb]->getSongsArtistID();

        char current_artistName[256];
        int artistIndex_inDB = find_artist_index_inDB_byID(current_artistID); 

        if (artistIndex_inDB != -1) {
            strcpy(current_artistName, artists_database[artistIndex_inDB]->getName());
        } else {
            strcpy(current_artistName, "Unknown");
        }

        cout << " <- ID: [" <<songs_database[idx_songsdb]->getSongID() << "] -> "<< songs_database[idx_songsdb]->getTitle() << " - " << current_artistName << "\n";
    }
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

    char user_input = get_yesno_input_validation("\n[?] Would you like to view a specific song's full profile? (y/n): ");
   
    if (user_input == 'y' || user_input == 'Y') {
        int chosenID = get_int_input_validation(" <--- Enter the Song ID: ");

        int index_in_db = this->find_song_index_inDB_byID(chosenID);

        // if exist in database print it and increase the streams like you listened to it
        if (index_in_db != -1) {
            ++(*songs_database[index_in_db]);
            cout << *songs_database[index_in_db];
        } else {
            cout << " <--- [!] Invalid ID. Going back to menu!\n";
        }
    }

}

void StreamingPlatformApp::connectingMenu() {
    bool connectingMenu_looping = true, show_menu = true;
    while (this->current_user == nullptr && connectingMenu_looping) {
        if (show_menu) {
            cout << "\n|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
            cout << "|      WELCOME TO THE APP        |\n";
            cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
            cout << "|  [1]. Sign Up                  |\n";
            cout << "|  [2]. Log In                   |\n";
            cout << "|  [0]. EXIT                     |\n";
            cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
            show_menu = false;
        }

        int user_choice = get_int_input_validation("\n <--- Choose an option: ");
        switch(user_choice) {
            case 1:
                this->signUp();
                show_menu = true;
                break;
            case 2:
                this->login();
                show_menu = true;
                break;
            case 0:
                cout << "\nTHE APP IS SHUTTING DOWN..... SEE YOU SOON!\n";
                connectingMenu_looping = false;
                break;
            default:
                cout << "\n[!] Invalid choice. Please try again. \n";
        }
    }

}

void StreamingPlatformApp::mainMenu(bool& app_run_flag) {   
        int user_input;
        bool main_looping = true, show_menu = true;
        while (main_looping) {
            if (show_menu) {
                this->printMainMenu();
                show_menu = false;
            }
    
            user_input = get_int_input_validation("\n<--- Enter an action's number: ");
    
            switch(user_input) {
                case 1:
                    this->AddNewArtistinDB();
                    show_menu = true;
                    break;
                case 2:
                    this->AddNewSonginDB();
                    show_menu = true;
                    break;
                case 3:
                    this->CreatePlaylist();
                    show_menu = true;
                    break;
                case 4:
                    this->ManagePlaylist();
                    show_menu = true;
                    break;
                case 5:
                    this->ViewPlaylists();
                    show_menu = true;
                    break;
                case 6:
                    this->ViewArtistsDB();
                    show_menu = true;
                    break;
                case 7:
                    this->ViewSongsDB();
                    show_menu = true;
                    break;    
                case 8:
                    this->ManageLikedSongs();
                    show_menu = true;
                    break; 
                case 9:
                    cout << *current_user;
                    show_menu = true;
                    break;
                case 10:
                    cout << "\nLogging out... Goodbye, " << this->current_user->getUsername() << "!\n";
                    this->current_user = nullptr;
                    main_looping = false;
                    break;
                case 0:
                    cout << "\nTHE APP IS SHUTTING DOWN..... SEE YOU SOON!\n";
                    main_looping = false;
                    app_run_flag = false;

                    break;
                default:
                    cout << "\n[!] Invalid choice. Please try again. \n";
                
            }
    
        }
}

void StreamingPlatformApp::signUp() {
    cout << "\n~~~~~~~~~~~~ CREATE A NEW ACCOUNT ~~~~~~~~~~~~\n";

    char input_username[256];
    get_name_input_validation(" [?] Enter a username: ", input_username, 256);

    bool already_exist = false;
    for (int i = 0;i < this->users_counter && !already_exist; i ++) {
        if (duplicate_name_ignoreCase(input_username, (*this->users_database[i]).getUsername())) {
            already_exist = true;
        }
    }

    if (already_exist) {
        cout << "\n[!] Error: The username '" << input_username << "' is already taken!\n";
        return;
    }

    char input_password[256];
    get_name_input_validation(" [?] Enter a password: ", input_password, 256);
    //! snippet for subscription type attribute if i add it

    User* new_user = new User();
    new_user->setUsername(input_username);
    new_user->setPassword(input_password);

    User** tempUsersDB = new User*[this->users_counter + 1];
    for(int i = 0;i < this->users_counter; i ++ ){
        tempUsersDB[i] = this->users_database[i];
    }
    tempUsersDB[this->users_counter] = new_user;
    
    if(this->users_database != nullptr) {
        delete[] this->users_database;
        this->users_database = nullptr;
    }

    this->users_database = new User*[++(this->users_counter)];
    for (int i = 0;i < this->users_counter; i ++){
        this->users_database[i] = tempUsersDB[i];
    }

    delete[] tempUsersDB;
    this->current_user = new_user;

    cout << "\n[SUCCESS] Account created! Welcome, " << this->current_user->getUsername() << "!\n";
    cout << "CONNECTING TO THE SERVERS............\n";    

}

void StreamingPlatformApp::login() {
    cout << "\n~~~~~~~~~~~~~ LOG IN ~~~~~~~~~~~~~\n";

    if (this->users_counter == 0) {
        cout << "[!] No accounts exist yet. Please Sign Up first!\n";
        return;
    }

    char input_username[256];
    get_name_input_validation(" [?] Enter your username: ", input_username, 256);

    bool user_found = false;
    int current_user_index = -1;
    for (int i = 0; i < this->users_counter && !user_found; i ++) {
        if (duplicate_name_ignoreCase(this->users_database[i]->getUsername(), input_username)) {
            user_found = true;
            current_user_index = i;
        }
    }

    if (!user_found) {
        cout << "\n[!] Error: Username '" << input_username << "' not found.\n";
        return;
    }

    char input_password[256];
    get_name_input_validation(" [?] Enter your password: ", input_password, 256);
    if (!strcmp(input_password, (*this->users_database[current_user_index]).getPassword())) {
        this->current_user = this->users_database[current_user_index];
        cout << "\n[SUCCESS] Logged in successfully! Welcome back, " << this->current_user->getUsername() << "!\n";
    } else {
        cout <<"\n[!] Wrong password!\n";
    }


}

void StreamingPlatformApp::run() {
    bool app_run_flag = true;
    while (app_run_flag) {
        this->connectingMenu();
    
        if (this->current_user != nullptr) {
            this->mainMenu(app_run_flag);
        } else {
            app_run_flag = false;
        }
    }
}

StreamingPlatformApp::~StreamingPlatformApp() {

    // cout << "DESTRUCTOR PT APP A AVUT LOC GOODBYE RETURN 0 PAPA S A TERMINAT HAHA DAI RUN IAR PAPA\n";

    if (this->artists_database != nullptr) {
        for (int i = 0; i < this->artists_counter; i++) {
            delete this->artists_database[i];
        }
        delete[] this->artists_database;      
        this->artists_counter = 0;
    }

    if (this->playlists_database != nullptr) {
        for (int i = 0; i < this->playlists_counter; i++) {
            delete this->playlists_database[i];
        }
        delete[] this->playlists_database;
        this->playlists_counter = 0;
    }

    if (this->songs_database != nullptr) { 
        for (int i = 0; i < this->songs_counter; i++) {
            delete this->songs_database[i];
        }
        delete[] this->songs_database;
        this->songs_counter = 0;
    }

    if (this->users_database != nullptr) {
        for (int i = 0; i < this->users_counter; i++) {
            delete this->users_database[i];
        }
        delete[] this->users_database;
        this->users_counter = 0;
    }
}


int main() {    
    StreamingPlatformApp SpotifyApp;
    SpotifyApp.run();
    return 0;
}