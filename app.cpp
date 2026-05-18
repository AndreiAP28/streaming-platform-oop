#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <algorithm>

// #include "classes.h"

#include "utils.h"
#include "app.h"
#include "artist.h"
#include "song.h"
#include "playlist.h" 
#include "user.h"
#include "premium_user.h"
#include "family_premium_user.h"
#include "playable_item.h"

//! TESTEAZA AIA CU CHRONO SI THREAD

using namespace std;

const int StreamingPlatformApp::MAX_QUEUE_SIZE = 50;

StreamingPlatformApp::StreamingPlatformApp() 
: artists_database(nullptr), artists_counter(0), playlists_database(nullptr), playlists_counter(0), songs_database(nullptr), songs_counter(0), users_database(nullptr), users_counter(0), current_user(nullptr), queue_counter(0) {
    this->music_queue = new playableItem*[MAX_QUEUE_SIZE];

    for (int i = 0;i < MAX_QUEUE_SIZE; i ++) {
        this->music_queue[i] = nullptr; 
    }
}

bool StreamingPlatformApp::duplicate_name_ignoreCase(const string& name1, const string& name2) {
    if (name1.empty() || name2.empty()) {
        return false;
    }

    if (name1.size() != name2.size()) {
        return false;
    }

    
    return equal(name1.begin(), name1.end(), name2.begin(), name2.end(),
        [](char a, char b) {
            return tolower(a) == tolower(b);
        }
    );
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
            if (this->playlists_database[i]->getOwnerUsername() == this->current_user->getUsername()) {
                return i;
            }
        }
    }

    return -1;
}

int StreamingPlatformApp::find_user_index_inDB() const {
    for (int i = 0; i < this->users_counter; i ++) {
        if (users_database[i] == current_user) {
            return i;
        }
    }

    return -1;
}


//!---------------- ADD IN DB ---------------------------------
void StreamingPlatformApp::addinDB() {
    bool looping = true, show_second_menu = true; 
    while(looping) {
        if (show_second_menu)  {
            cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
            cout << "|  [1]. Add a New Artist in Database             |\n";
            cout << "|  [2]. Add a New Song in Database               |\n";
            cout << "|  [0]. Go Back to Main Menu                     |\n";
            cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
            show_second_menu = false;
        }
        int user_choice = get_int_input_validation("Choose an action: ");
        switch(user_choice) {
            case 1:
                this->AddNewArtistinDB();
                show_second_menu = true;
                break;
            case 2:
                this->AddNewSonginDB();
                show_second_menu = true;
                break;
            case 0:
                looping = false;
                break;

            default:
                cout << "[!] Invalid option. Try again!\n";
        }   
    }
}

void StreamingPlatformApp::AddNewArtistinDB() {
    string input_artistName = get_name_input_validation(" <--- Enter artist name: ");

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

 
    string input_artistName = get_name_input_validation("\n [?] Enter the name of the Artist for the new song: ");

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
    
    string input_songTitle = get_name_input_validation(" <--- Enter song name: ");
    
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

//! ------------------- EXPLORE LIBRARY ------------------------
void StreamingPlatformApp::exploreLibrary() {
    bool looping = true;
    bool show_second_menu = true; 

    while(looping) {
        if (show_second_menu) {
            cout << "\n|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
            cout << "|  [1]. Create a New Playlist                    |\n";
            cout << "|  [2]. Manage an Existing Playlist              |\n";
            cout << "|  [3]. View All Playlists                       |\n";
            cout << "|  [4]. View All Artists in Database             |\n";
            cout << "|  [5]. View All Songs in Database               |\n";
            cout << "|  [0]. Go Back to Main Menu                     |\n";
            cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
            show_second_menu = false;
        }

        int user_choice = get_int_input_validation("Choose an action: ");

        switch(user_choice) {
            case 1:
                this->CreatePlaylist();
                show_second_menu = true;
                break;
            case 2:
                this->ManagePlaylist();
                show_second_menu = true;
                break;
            case 3:
                this->ViewPlaylists();
                show_second_menu = true;
                break;
            case 4:
                this->ViewArtistsDB();
                show_second_menu = true;
                break;
            case 5:
                this->ViewSongsDB();
                show_second_menu = true;
                break;
            case 0:
                looping = false;
                break;
            default:
                cout << " [!] Invalid option. Try again!\n";
        }   
    }
}

void StreamingPlatformApp::CreatePlaylist() {
    cout << "\n~~~~~~~~~~~~~~~~~~~~ CREATE NEW PLAYLIST ~~~~~~~~~~~~~~~~~~~~\n";
    
    string temp_playlistName = get_name_input_validation(" <--- Enter a name for your new playlist: ");

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
        if (this->playlists_database[i]->getOwnerUsername() == this->current_user->getUsername()) {
            user_has_playlists = true;
        }
    }

    if (!user_has_playlists) {
        cout << "\n[!] You haven't created any playlist yet!";
        return;
    }

    cout << "\n ~~~~~~~~~~~~~~~~~~~~~~~~~ MANAGE PLAYLISTS ~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    for ( int i = 0; i < playlists_counter; i ++) {
        if(playlists_database[i]->getOwnerUsername() == current_user->getUsername()) {
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
        if (this->playlists_database[i]->getOwnerUsername() == this->current_user->getUsername()) {
            cout << "   <- [ ID: " << playlists_database[i]->getPlaylistID() << "] - " << playlists_database[i]->getName() << " (" << playlists_database[i]->getSongsSize() << " tracks)\n"; 
        }
    }
    cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    
    char user_input = get_yesno_input_validation("\n[?] Would you like to view more about a playlist? (y/n): ");
    
    if (user_input == 'y' || user_input == 'Y') {
        int chosenID = get_int_input_validation(" <--- Enter the Playlist ID: ");
        
        int playlist_index_input = this->find_playlist_index_inDB_byID(chosenID);
        if (playlist_index_input == -1) {
            cout << " [!] Invalid ID or you do not own this playlist!\n";
            return;
        }
        
        cout << *playlists_database[playlist_index_input];
        if ((*playlists_database[playlist_index_input]).getSongsSize()){
            cout << "Total Songs' Streams in this playlist: " << playlists_database[playlist_index_input]->calculateTotalStreams() << " streams.\n";
        }

        int user_choice = get_int_input_validation("\n[1] Play Now  ~  [2] Add to Queue  ~  [0] Go Back: ");
        bool looping = true;
            while (looping) {
                switch(user_choice) {
                    case 1: 
                        playlists_database[playlist_index_input]->simulatePlayItem();
                        looping = false;
                        break;
                    case 2:
                        if (this->queue_counter < this->MAX_QUEUE_SIZE - 1) {
                            music_queue[queue_counter ++] = playlists_database[playlist_index_input];
                            cout << " [SUCCES] Playlist " << playlists_database[playlist_index_input]->getName() << " Added in the queue!\n";
                        } else {
                            cout << "[!] Queue is already full!\n";
                        }
                        looping = false;
                        break;
                    default:
                    cout << "\n[!] Invalid choice. Please try again. \n";
                        

                }
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

        string current_artistName;
        int artistIndex_inDB = find_artist_index_inDB_byID(current_artistID); 

        if (artistIndex_inDB != -1) {
            current_artistName = artists_database[artistIndex_inDB]->getName();
        } else {
            current_artistName = "Unknown";
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
            cout << "\n [1] Play Now ~ [2] Add to Queue ~ [0] Go Back: \n";
            int user_choice = get_int_input_validation("Choose an action: ");
            
            bool looping = true;
            while (looping) {
                switch(user_choice) {
                    case 1: 
                        songs_database[index_in_db]->simulatePlayItem();
                        looping = false;
                        break;
                    case 2:
                        if (this->queue_counter < this->MAX_QUEUE_SIZE - 1) {
                            music_queue[queue_counter ++] = songs_database[index_in_db];
                            cout << " [SUCCES] Song " << songs_database[index_in_db]->getTitle() << " Added in the queue!\n";
                        } else {
                            cout << "[!] Queue is already full!\n";
                        }
                        looping = false;
                        break;
                    case 0:
                        looping = false;
                        break;
                    default:
                    cout << "\n[!] Invalid choice. Please try again. \n";
                        

                }
            }

        } else {
            cout << " <--- [!] Invalid ID. Going back to menu!\n";
        }
    }

}

//!------------------- ACCOUNT SETTINGS -----------------------
void StreamingPlatformApp::AccountSettings() {
    bool looping = true, show_second_menu = true; 
    while(looping) {
        if (show_second_menu)  {
            cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
            cout << "|  [1]. View My Profile                          |\n";
            cout << "|  [2]. Manage Liked Songs                       |\n";
            cout << "|  [3]. Manage Subscriptions                     |\n";
            cout << "|  [4]. Sign Out                                 |\n";
            cout << "|  [0]. Go Back to Main Menu                     |\n";
            cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
            show_second_menu = false;
        }
        int user_choice = get_int_input_validation("Choose an action: ");
        switch(user_choice) {
            case 1:
                // cout << *current_user;
                current_user->displayProfile();
                show_second_menu = true;
                break;
            case 2:
                this->ManageLikedSongs();
                show_second_menu = true;
                break;
            case 3:
                this->ManageSubscriptions();
                show_second_menu = true;
                break;
            case 4:
                cout << "\nLogging out... Goodbye, " << this->current_user->getUsername() << "!\n";
                this->current_user = nullptr;
                looping = false;
                break;
            case 0:
                looping = false;
                break;

            default:
                cout << "[!] Invalid option. Try again!\n";
        }   
    }
};

void StreamingPlatformApp::ManageLikedSongs() {
    // int user_choice;
    // int user_choice = -1;
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

       int user_choice = get_int_input_validation("Choose an action: ");
    
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

                    string current_artistName;
                    int artistIndex_inDB = find_artist_index_inDB_byID(current_artistID); 

                    if (artistIndex_inDB != -1) {
                        current_artistName = artists_database[artistIndex_inDB]->getName();
                    } else {
                        current_artistName = "Unknown";
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

                int songID_input = get_int_input_validation("\n[?] What song do you want to remove (type Song ID): ");
                
                int index_in_liked = -1;
                for (int i = 0; i < liked_songs_num; i++) {
                    if (current_user->getLikedSongs()[i]->getSongID() == songID_input) {
                        index_in_liked = i; 
                        break;
                    }
                }
                if (index_in_liked == -1) {
                    cout << " <--- [!] Invalid ID. Going back to menu!\n";
                    show_menu = true;
                    break;
                }

                *current_user -= index_in_liked;

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

void StreamingPlatformApp::ManageSubscriptions() {

    FamilyPremiumUser* family_user = dynamic_cast<FamilyPremiumUser*>(this->current_user);
    PremiumUser* premium_user = dynamic_cast<PremiumUser*>(this->current_user);
    
    bool is_family = (family_user != nullptr);
    bool is_premium = (premium_user != nullptr);
    
    if (is_family) {
        bool show_menu = true, subscription_looping  = true;
        while (subscription_looping) {
            if (show_menu) {
                cout << "\n|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
                cout << "|             SUBSCRIPTION MANAGEMENT            |\n";
                cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
                cout << "|  STATUS: FAMILY PLAN MANAGER                   |\n";
                cout << "|  [1]. Add a Family Member                      |\n";
                cout << "|  [2]. Remove a Family Member                   |\n";
                cout << "|  [0]. Go Back                                  |\n";
                cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
            }

            int user_choice = get_int_input_validation("Choose an action: ");
            switch(user_choice) {
                case 1: {
                    string new_member = get_name_input_validation("Enter new member's name: ");
                    family_user->addMember(new_member);
                    show_menu = true;
                    break;
                }

                case 2: {
                    //! CONTINUE HERE
                    string removed_member = get_name_input_validation("Enter the member's name you want to remove: ");
                    family_user->removeMember(removed_member);
                    show_menu = true;
                    break;
                }

                case 0:
                    cout << "\nReturning to Main Menu...\n";
                    return;

                default:
                    cout << "\n[!] Invalid Choice. Try again!\n";
                    show_menu = false;

            }
        }
    
    } else if (is_premium){
        while(true) {
            cout << "|  STATUS: PREMIUM PLAN ACTIVE                   |\n";
            cout << "|  [1]. Upgrade to FAMILY PLAN ($14.99/mo)       |\n";
            cout << "|  [0]. Go Back                                  |\n";
            cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
            int user_choice = get_int_input_validation("Choose an action: ");
            if (user_choice == 0) {
                return;
            } else if (user_choice == 1) {
                break;
            } 
            else {
                cout << "[!] Invalid option. Try again!";
            }
            

        }
    } else {
        cout << "|  STATUS: FREE TIER (AD-SUPPORTED)              |\n";
        cout << "|  [1]. Buy Standard Plan ($4.99/mo)             |\n";
        cout << "|  [2]. Buy Student Plan ($2.99/mo)              |\n";
        cout << "|  [3]. Buy Annual Plan ($49.99/yr)              |\n";
        cout << "|  [4]. Buy FAMILY PLAN ($14.99/mo) - 6 Accounts |\n";
        cout << "|  [0]. Go Back                                  |\n";
        cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    }

    int user_choice = (premium_user != nullptr) ? 4 : get_int_input_validation("Choose a plan: ");
    if (!user_choice) return;

    int db_index = find_user_index_inDB();
    int saved_id = current_user->getUserID();
    string saved_name = current_user->getUsername();
    string saved_password = current_user->getPassword();

    User* upgraded_subs_user = nullptr;
    Date today;
    // Here the normal User it will be replaced with the class for PremiumUser/FamilyPremiumUser so it will activate the polymorphism
    switch(user_choice) {
        case 1:
            upgraded_subs_user = new PremiumUser(saved_id, saved_name, saved_password, current_user->getLikedSongs(), STANDARD_MONTHLY, today);            
            break;
        case 2:
            upgraded_subs_user = new PremiumUser(saved_id, saved_name, saved_password, current_user->getLikedSongs(), STUDENT_PLAN, today);            
            break;
        case 3:
            upgraded_subs_user = new PremiumUser(saved_id, saved_name, saved_password, current_user->getLikedSongs(), ANNUAL_DISCOUNT, today);            
            break;
        case 4: {
            upgraded_subs_user = new FamilyPremiumUser(saved_id, saved_name, saved_password, current_user->getLikedSongs(), today);            
            
            FamilyPremiumUser* family_plan_user = dynamic_cast<FamilyPremiumUser*>(upgraded_subs_user);
            string currentMemberName = get_name_input_validation("Enter your name that will be display as member: ");
            family_plan_user->addMember(currentMemberName);
            break;
        }
        default:
            cout << "[!] Invalid choice./n";
            return;        
    }

    delete current_user;


    users_database[db_index] = upgraded_subs_user;
    current_user = upgraded_subs_user;

    cout << "[SUCCES] Congratulations! Subscription Activated!\n";
}
//!-----------------------------------------------------------

void StreamingPlatformApp::playQueue() {
    if(this->queue_counter == 0) {
        cout << "\n############## [!] Your queue is empty! ##############  \n";
    } else {
        cout << "\n ############## STARTING PLAYBACK QUEUE ############## \n";
        for (int i = 0;i < this->queue_counter; i ++) {
            this->music_queue[i]->simulatePlayItem();

            this->music_queue[i] = nullptr;
        }

        this->queue_counter = 0;
        cout << "\n [!] ############## Queue finished and cleared! ##############\n";
    }
}

void StreamingPlatformApp::printMainMenu() {
    cout << "\n|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    cout << "|           STREAMING MUSIC PLATFORM APP         |\n";
    cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    cout << "|  Hello, " << this->current_user->getUsername() << "! How was your day?              |\n";
    cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    cout << "|  [1]. Add a New Artist/Song in Database        |\n";
    cout << "|  [2]. Explore Library                          |\n";
    cout << "|  [3]. Play Queue                               |\n";
    cout << "|  [4]. Account Settings                         |\n";
    cout << "|  [0]. EXIT                                     |\n";
    cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    
    
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

void StreamingPlatformApp::signUp() {
    cout << "\n~~~~~~~~~~~~ CREATE A NEW ACCOUNT ~~~~~~~~~~~~\n";

    string input_username = get_name_input_validation(" [?] Enter a username: ");

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

    string input_password = get_name_input_validation(" [?] Enter a password: ");
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

    string input_username = get_name_input_validation(" [?] Enter your username: ");

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

    string input_password = get_name_input_validation(" [?] Enter your password: ");
    if (input_password == (*this->users_database[current_user_index]).getPassword()) {
        this->current_user = this->users_database[current_user_index];
        cout << "\n[SUCCESS] Logged in successfully! Welcome back, " << this->current_user->getUsername() << "!\n";
    } else {
        cout <<"\n[!] Wrong password!\n";
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
                    this->addinDB();
                    show_menu = true;
                    break;

                case 2:
                    this->exploreLibrary();
                    show_menu = true;
                    break;
                case 3:
                    this->playQueue();
                    show_menu = true;
                    break;
                case 4:
                    this->AccountSettings();
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

void StreamingPlatformApp::preloadedData() {
    cout << "[i] Booting Database... Injecting Mock Data...\n";

    this->artists_counter = 10;
    this->artists_database = new Artist*[10];

    artists_database[0] = new Artist("Travis Scott", 60000000, {}, true);
    artists_database[1] = new Artist("Lil Uzi Vert", 35000000, {}, true);
    artists_database[2] = new Artist("A$AP Rocky", 30000000, {}, true);
    artists_database[3] = new Artist("Drake", 80000000, {},true);
    artists_database[4] = new Artist("Kendrick Lamar", 55000000, {},true);
    artists_database[5] = new Artist("The Weeknd", 100000000, {},true);
    artists_database[6] = new Artist("Playboi Carti", 25000000, {},true);
    artists_database[7] = new Artist("Future", 45000000, {},true);
    artists_database[8] = new Artist("Tyler, The Creator", 32000000, {},true);
    artists_database[9] = new Artist("Metro Boomin", 50000000, {},true);

    this->songs_counter = 50;
    this->songs_database = new Song*[50];

    const char* travis_songs[] = {"SICKO MODE", "goosebumps", "STARGAZING", "HIGHEST IN THE ROOM", "BUTTERFLY EFFECT"};
    const char* uzi_songs[] = {"XO Tour Llif3", "20 Min", "Money Longer", "The Way Life Goes", "Just Wanna Rock"};
    const char* rocky_songs[] = {"Praise The Lord", "Sundress", "L$D", "Everyday", "F**kin' Problems"};
    const char* drake_songs[] = {"God's Plan", "One Dance", "Hotline Bling", "Passionfruit", "Jimmy Cooks"};
    const char* kendrick_songs[] = {"HUMBLE.", "DNA.", "Alright", "Swimming Pools", "Money Trees"};
    const char* weeknd_songs[] = {"Blinding Lights", "Starboy", "The Hills", "Save Your Tears", "Die For You"};
    const char* carti_songs[] = {"Magnolia", "Sky", "Shoota", "wokeuplikethis*", "R.I.P."};
    const char* future_songs[] = {"Mask Off", "March Madness", "Codeine Crazy", "Life Is Good", "Stick Talk"};
    const char* tyler_songs[] = {"EARFQUAKE", "See You Again", "NEW MAGIC WAND", "WUSYANAME", "Yonkers"};
    const char* metro_songs[] = {"Space Cadet", "No Complaints", "10 Freaky Girls", "Overdue", "Too Many Nights"};

    const char** all_songs[] = {travis_songs, uzi_songs, rocky_songs, drake_songs, kendrick_songs, weeknd_songs, carti_songs, future_songs, tyler_songs, metro_songs};

    int s_idx = 0;

    for (int a_idx = 0; a_idx < 10; a_idx++) {
        int current_artist_id = artists_database[a_idx]->getArtistID();
        
        for (int i = 0; i < 5; i++) {
            // Giving random duration between 2m and 3m to the songs
            int random_duration = 120 + (rand() % 60); 
            long long random_streams = 500000 + (rand() % 50000000); 

            int random_year = 2000 + (rand() % 20);
            int random_month = 1 + (rand() % 12);
            int random_day = 1 + (rand() % 28); // safety for feb

            songs_database[s_idx] = new Song(all_songs[a_idx][i], random_duration, current_artist_id, {}, random_streams, Date(random_day, random_month, random_year));
            
            artists_database[a_idx]->addTopSong(all_songs[a_idx][i]);
            
            s_idx++;
        }
    }
    
    cout << "[SUCCESS] " << artists_counter << " Artists and " << songs_counter << " Songs successfully loaded into the matrix!\n";

}

StreamingPlatformApp::~StreamingPlatformApp() {
    // cout << "DESTRUCTOR PT APP A AVUT LOC GOODBYE RETURN 0 PAPA S A TERMINAT HAHA DAI RUN IAR PAPA\n";

    if (this->music_queue != nullptr) {
        delete[] this->music_queue;
    }

    if (this->users_database != nullptr) {
        for (int i = 0; i < this->users_counter; i++) {
            delete this->users_database[i];
        }
        delete[] this->users_database;
        this->users_counter = 0;
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

    if (this->artists_database != nullptr) {
        for (int i = 0; i < this->artists_counter; i++) {
            delete this->artists_database[i];
        }
        delete[] this->artists_database;      
        this->artists_counter = 0;
    }

    

    

    
}


int main() {    
    // playableItem test; eroare 

    StreamingPlatformApp SpotifyApp;
    SpotifyApp.preloadedData();
    SpotifyApp.run();
    return 0;
}