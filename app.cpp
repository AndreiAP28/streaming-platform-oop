#include <iostream>
#include <cstdlib>
#include <cctype>
#include <algorithm>


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

StreamingPlatformApp::StreamingPlatformApp() : current_user(nullptr) {
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

// int StreamingPlatformApp::find_artist_index_inDB_byID(int chosenID_by_user) const {
//     for (int i = 0; i < this->artists_counter; i ++) {
//         if (artists_database[i]->getArtistID() == chosenID_by_user) {
//             return i;
//         }
//     }
//
//     return -1;
// }

// int StreamingPlatformApp::find_song_index_inDB_byID(int chosenID_by_user) const {
//     for (int i = 0; i < this->songs_counter; i ++) {
//         if (songs_database[i]->getSongID() == chosenID_by_user) {
//             return i;
//         }
//     }
//
//     return -1;
// }

// int StreamingPlatformApp::find_playlist_index_inDB_byID(int chosenID_by_user) const {
//     for (int i = 0; i < this->playlists_counter; i ++ ){ 
//         if (this->playlists_database[i]->getPlaylistID() == chosenID_by_user) {
//             if (this->playlists_database[i]->getOwnerUsername() == this->current_user->getUsername()) {
//                 return i;
//             }
//         }
//     }
//
//     return -1;
// }

// int StreamingPlatformApp::find_user_index_inDB() const {
//     for (int i = 0; i < this->users_counter; i ++) {
//         if (users_database[i] == current_user) {
//             return i;
//         }
//     }
//
//     return -1;
// }

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

    bool not_found = all_of(artists_database.begin(), artists_database.end(),
    [&](const auto& pair) {
        return !duplicate_name_ignoreCase(pair.second->getName(), input_artistName);
    });

    if (!not_found) {
        cout << "\n[!] Error: An artist named '" << input_artistName << "' already exists!\n";
        return;     
    }
    Artist* tempNewArtist = new Artist();
    tempNewArtist->setName(input_artistName);
    cin >> *tempNewArtist;

    artists_database[tempNewArtist->getArtistID()] = tempNewArtist;

    //! this->artists_database = tempArtistsDB;

    cout << "\n  Artist " << tempNewArtist->getName() << " added with ID: " << (*tempNewArtist).getArtistID() << endl;
}

void StreamingPlatformApp::AddNewSonginDB() {

    if (artists_database.empty()) {
        cout << "\n [!] Error: No artists in database. Add an artist first!\n";
        return;
    }
 
    string input_artistName = get_name_input_validation("\n [?] Enter the name of the Artist for the new song: ");

    auto artist_it = find_if(artists_database.begin(), artists_database.end(),
    [&](const auto& pair) {
        return duplicate_name_ignoreCase(pair.second->getName(), input_artistName);
    });
    
    
    if (artist_it == artists_database.end()) {
        cout << " [!] Error: Artist '" << input_artistName << "' not found. \n";
        cout << " [i] Tip: Check spelling or add the artist to the database first.\n";
        return;
    }
    
    Artist* found_artist = artist_it->second;
    int artistID = found_artist->getArtistID();

    string input_songTitle = get_name_input_validation(" <--- Enter song name: ");
    
    bool already_exist = any_of(songs_database.begin(), songs_database.end(),
        [&](const auto& pair){
            Song* current_song = pair.second;
            return duplicate_name_ignoreCase(current_song->getTitle(), input_songTitle) && current_song->getSongsArtistID() == artistID;
        });
     
    if (already_exist) {
        cout << "\n[!] Error: A song with the title '" << input_songTitle << "' already exists!\n";
        return;
    }
    
    Song* new_song = new Song();
    new_song->setTitle(input_songTitle);
    new_song->setArtistID(artistID);
    cin >> *new_song;

    songs_database[new_song->getSongID()] = new_song;
    found_artist->addTopSong(new_song->getTitle());   

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

    this->playlists_database.push_back(newPlaylist);       

    cout << " [SUCCESS] Playlist '" << temp_playlistName << "' has been created!\n";
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

}

void StreamingPlatformApp::ManagePlaylist() {

    if (playlists_database.empty()){
        cout <<"\n[!] No playlist exist. Please create one first!\n";
        return;
    }

   bool user_hasno_playlists = none_of(playlists_database.begin(), playlists_database.end(), 
        [&](const Playlist* curr_p) {
            return curr_p->getOwnerUsername() == current_user->getUsername();     
        }    
    );

    if (user_hasno_playlists) {
        cout << "\n[!] You haven't created any playlist yet!";
        return;
    }

    cout << "\n ~~~~~~~~~~~~~~~~~~~~~~~~~ MANAGE PLAYLISTS ~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    for (const Playlist* curr_p: playlists_database) {
        if(curr_p->getOwnerUsername() == current_user->getUsername()) {
            cout << "[ ID: " << curr_p->getPlaylistID() << "] " << curr_p->getName() << "\n";
        }
    }
    cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";


    int playlistID_input = get_int_input_validation("\n <--- Enter the ID of the playlist you want to manage: ");


    auto managing_playlist_it = find_if (playlists_database.begin(), playlists_database.end(),
        [&](const Playlist* curr_p) {
                return curr_p->getPlaylistID() == playlistID_input && curr_p->getOwnerUsername() == current_user->getUsername();
        }
    );
    if (managing_playlist_it == playlists_database.end()) {
        cout << " [!] Invalid ID or you do not own this playlist!\n";
        return;
    }

    Playlist* managing_playlist = *managing_playlist_it;
    int user_choice = -1;
    while (user_choice != 0) {
        cout << "\n|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    cout << "|  MANAGING: " << managing_playlist->getName() << "                            |\n";
    cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
    cout << "|  [1]. Add a Song to Playlist                   |\n";
    cout << "|  [2]. Remove a Song from Playlist              |\n";
    cout << "|  [3]. View Playlist Tracks                     |\n";
    cout << "|  [0]. Go Back to Main Menu                     |\n";
    cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
        user_choice = get_int_input_validation("Choose an action: ");

        switch (user_choice) {
            case 1: {
                if (songs_database.empty()) {
                    cout << "[!] There are no songs in database!\n";
                    break;
                }

                cout << "\n ~~~~~~~~~~~~~~~~~ REGISTERED SONGS ~~~~~~~~~~~~~~~~~\n";
                vector<Song*> sorted_songs;
                for (const auto& pair : songs_database) {
                    sorted_songs.push_back(pair.second);
                }

                sort(sorted_songs.begin(), sorted_songs.end(),
                    [](Song* first, Song* second) {
                        return first->getSongID() < second->getSongID();
                    });
                
                for (const Song* current_song: sorted_songs) {
                    cout << "[ ID: " << current_song->getSongID() << " ] " << current_song->getTitle() << "\n";
                }
                cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

                int songID_input = get_int_input_validation(" <--- Enter the ID of the song you want to add: ");
                
                auto adding_song_it = songs_database.find(songID_input);
                if (adding_song_it == songs_database.end()) {
                    cout << "[!] Invalid selection!\n";
                    break;
                }  

                Song* adding_song = adding_song_it->second;
                const vector<Song*>& playlist_songs = managing_playlist->getSongs();
                bool already_exist = any_of(playlist_songs.begin(), playlist_songs.end(),
                    [&](const Song* curr_song) {
                        return curr_song->getSongID() == songID_input;
                    }
                );

                if (already_exist) {
                    cout << "[!] This song is already in playlist!\n";
                } else {
                    managing_playlist->addSong(*adding_song);
                    cout << "[SUCCESS] Song added to playlist!\n";
                }
                break;
            }
            
            case 2: {
                const vector<Song*>& playlist_songs = managing_playlist->getSongs();
                if (playlist_songs.empty()) {
                    cout <<" [!] This playlist is already empty!\n";
                    break;
                }

                cout << "\n~~~~~~~~~~~~~~~~ TRACKS IN PLAYLIST ~~~~~~~~~~~~~~~~\n";
                for ( const Song* curr_song: playlist_songs) {
                        cout << "[" << curr_song->getSongID() << "] -> " << curr_song->getTitle() << endl;
                }
                cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

                int removeID_input = get_int_input_validation("\n <--- Enter the ID of the song that you want to remove: ");

                 
                managing_playlist->removeSong(removeID_input); 
                break;
            }

            case 3:
                cout << *managing_playlist;
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
    if (playlists_database.empty()) {
        cout << "[!] There are no playlists registered yet.\n";
        return;
    }

    bool user_has_playlist = false;
    cout << "\n~~~~~~~~~~~~~~~ " << this->current_user->getUsername() << " PLAYLISTS ~~~~~~~~~~~~~~~\n";
    for (const Playlist* playlist: playlists_database) {
        if (playlist->getOwnerUsername() == this->current_user->getUsername()) {
            cout << "   <- [ ID: " << playlist->getPlaylistID() << "] - " << playlist->getName() << " (" << playlist->getSongsSize() << " tracks)\n"; 
            user_has_playlist = true;
        }
    }
    cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    
    if (!user_has_playlist) {
        return;
    }
    char user_input = get_yesno_input_validation("\n[?] Would you like to view more about a playlist? (y/n): ");
    
    if (user_input == 'y' || user_input == 'Y') {
        int chosenID = get_int_input_validation(" <--- Enter the Playlist ID: ");
        
        auto chosen_playlist_it = find_if(playlists_database.begin(), playlists_database.end(), 
            [&](const Playlist* curr_p) {
                return curr_p->getPlaylistID() == chosenID && curr_p->getOwnerUsername() == current_user->getUsername();
            });
        if (chosen_playlist_it == playlists_database.end()) {
            cout << " [!] Invalid ID or you do not own this playlist!\n";
            return;
        }

        Playlist* chosen_playlist = *chosen_playlist_it;
        
        cout << *chosen_playlist;
        if (chosen_playlist->getSongsSize()){
            cout << "Total Songs' Streams in this playlist: " << chosen_playlist->calculateTotalStreams() << " streams.\n";
        }

        bool looping = true;
        while (looping) {
                int user_choice = get_int_input_validation("\n[1] Play Now  ~  [2] Add to Queue  ~  [0] Go Back: ");
                switch(user_choice) {
                    case 1: 
                        chosen_playlist->simulatePlayItem();
                        looping = false;
                        break;
                    case 2:
                        if (music_queue.size() < this->MAX_QUEUE_SIZE) {
                            music_queue.push_back(chosen_playlist);
                            cout << " [SUCCES] Playlist " << chosen_playlist->getName() << " Added in the queue!\n";
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
    if (artists_database.empty()) {
        cout << "[!] There are no songs registered yet.\n";
        return;
    }

    cout << "\n~~~~~~~~~~~~~~~~~~~ REGISTERED ARTISTS ~~~~~~~~~~~~~~~~~~~\n";
    vector<Artist*> sorted_artists;
    for (const auto& pair: artists_database) {
        sorted_artists.push_back(pair.second);
    }
    
    sort(sorted_artists.begin(), sorted_artists.end(),
        [](Artist* first, Artist* second) {
            return first->getArtistID() < second->getArtistID();
    });

    for (Artist* current_artist: sorted_artists) {
        cout << "<- ID: [" << current_artist->getArtistID() << "] -> " << current_artist->getName() << " " << (current_artist->getIsVerified() ? " (VERIFIED)" : "") << "\n";
    }
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

    char user_input = get_yesno_input_validation("\n[?] Would you like to view a specific artist's full profile? (y/n): ");

    if (user_input == 'y' || user_input == 'Y') {
        int chosenID = get_int_input_validation(" <--- Enter the Artist ID: ");
       

        auto chosen_artist_it = artists_database.find(chosenID);
        
        // if exist in database print it and increase the monthlylisteners like you listened to it
        if (chosen_artist_it != artists_database.end()) {
            Artist* chosen_artist = chosen_artist_it->second;
            ++(*chosen_artist);
            cout << *chosen_artist;
        } else {
            cout <<" <--- [!] Invalid ID. Going back to menu!\n";
        }
    }
}

void StreamingPlatformApp::ViewSongsDB() { 
    if (songs_database.empty()) {
        cout << "[!] There are no songs registered yet.\n";
        return;
    }

    cout << "\n~~~~~~~~~~~~~~~~~~~ REGISTERED SONGS ~~~~~~~~~~~~~~~~~~~\n";
    vector<Song*> sorted_songs;
    for (const auto& pair : songs_database) {
        sorted_songs.push_back(pair.second);
    }

    sort(sorted_songs.begin(), sorted_songs.end(),
        [](Song* first, Song* second) {
            return first->getSongID() < second->getSongID();
        });
    
    for (const Song* current_song: sorted_songs) {
        int current_artistID = current_song->getSongsArtistID();
        auto current_artist_it = artists_database.find(current_artistID);

        string current_artistName;
        if (current_artist_it != artists_database.end()) {
            current_artistName = current_artist_it->second->getName();
        }

        cout << " <- ID: [" <<current_song->getSongID() << "] -> "<< current_song->getTitle() << " - " << current_artistName << "\n";
    }
    
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

    char user_input = get_yesno_input_validation("\n[?] Would you like to view a specific song's full profile? (y/n): ");
   
    if (user_input == 'y' || user_input == 'Y') {
        int chosenID = get_int_input_validation(" <--- Enter the Song ID: ");

        auto chosen_song_it = songs_database.find(chosenID);

        // if exist in database print it and increase the streams like you listened to it
        if (chosen_song_it != songs_database.end()) {
            Song* chosen_song = chosen_song_it->second;
            ++(*chosen_song);
            cout << *chosen_song;
            cout << "\n [1] Play Now ~ [2] Add to Queue ~ [0] Go Back: \n";
            int user_choice = get_int_input_validation("Choose an action: ");
            
            bool looping = true;
            while (looping) {
                switch(user_choice) {
                    case 1: 
                        chosen_song->simulatePlayItem();
                        looping = false;
                        break;
                    case 2:
                        if (music_queue.size() < this->MAX_QUEUE_SIZE) {
                            music_queue.push_back(chosen_song);
                            cout << " [SUCCES] Song " << chosen_song->getTitle() << " Added in the queue!\n";
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
}

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
                const vector<Song*>& liked_songs = current_user->getLikedSongs();
                if (!liked_songs.empty()) {
                    cout <<"\n~~~~~~~~~~~~~~~~~ Your Liked Songs ~~~~~~~~~~~~~~~~~\n";

                    for (const Song* song : liked_songs) {
                        cout << "[ ID: " << song->getSongID() << "] " << song->getTitle() << "\n"; 
                    }
                    cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

                } else {
                    cout << "\n[!] You don't have any liked songs yet!\n";
                }
                show_menu = true;
                break;
            }
        
            case 2: {

                if (songs_database.empty()) {
                    cout << "[!] There are no songs registered yet.\n";
                    show_menu = true;
                    break;
                }

                cout << "\n~~~~~~~~~~~~~~~~~~ REGISTERED SONGS ~~~~~~~~~~~~~~~~~~\n";
                vector<Song*> sorted_songs;
                for (const auto& pair : songs_database) {
                    sorted_songs.push_back(pair.second);
                }

                sort(sorted_songs.begin(), sorted_songs.end(),
                    [](Song* first, Song* second) {
                        return first->getSongID() < second->getSongID();
                    });
                
                for (Song* current_song: sorted_songs) {
                    int current_artistID = current_song->getSongsArtistID();
                    auto current_artist_it = artists_database.find(current_artistID);

                    string current_artistName;
                    if (current_artist_it != artists_database.end()) {
                        current_artistName = current_artist_it->second->getName();
                    }

                    cout << " <- ID: [" <<current_song->getSongID() << "] -> "<< current_song->getTitle() << " - " << current_artistName << "\n";
                }
                
                cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

                int songID_input = get_int_input_validation("\n <--- What song do you like (type Song ID): ");
   

                auto liked_song_it =songs_database.find(songID_input);

                if (liked_song_it == songs_database.end()) {
                    cout << " [!] Invalid ID. Going back to menu!\n";
                    show_menu = true;
                    break;
                }
                
                *current_user += *(liked_song_it->second);
                
                show_menu = true;
                break;
            }
            
            case 3: {
                const vector<Song*>& liked_songs = current_user->getLikedSongs();
                if (liked_songs.empty()) {
                    cout << "\n[!] You don't have any liked songs to remove!\n";
                    show_menu = true;
                    break;
                }

                cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~ YOUR LIKED SONGS ~~~~~~~~~~~~~~~~~~~~~\n";
                for (const Song* song : liked_songs ){
                    cout << " <- ID: [" << song->getSongID() << "] -> " << song->getTitle() << "\n";
                }

                int songID_input = get_int_input_validation("\n[?] What song do you want to remove (type Song ID): ");
                
                *current_user -= songID_input;

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

    auto db_current_user_it = users_database.find(current_user->getUsername());    
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


    db_current_user_it->second  = upgraded_subs_user;
    current_user = upgraded_subs_user;

    cout << "[SUCCES] Congratulations! Subscription Activated!\n";
}
//!-----------------------------------------------------------
void StreamingPlatformApp::playQueue() {
    if(this->music_queue.empty()) {
        cout << "\n############## [!] Your queue is empty! ##############  \n";
        return;
    } else {
        cout << "\n ############## STARTING PLAYBACK QUEUE ############## \n";
        for (playableItem* item: this->music_queue) {
            item->simulatePlayItem();
        }

        this->music_queue.clear();
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

    string lowercase_username = input_username;
    transform(lowercase_username.begin(), lowercase_username.end(), lowercase_username.begin(), ::tolower);
    auto new_user_it = users_database.find(lowercase_username);
    

    if (new_user_it != users_database.end()) {
        cout << "\n[!] Error: The username '" << input_username << "' is already taken!\n";
        return;
    }

    string input_password = get_name_input_validation(" [?] Enter a password: ");

    User* new_user = new User();
    new_user->setUsername(input_username);
    new_user->setPassword(input_password);

    //! LAS ASA CA KEY DE LA HASH SA AIBA numele lower si sa permita ca numele sa fie diferit sau?
    users_database[lowercase_username] = new_user;
    this->current_user = new_user;

    cout << "\n[SUCCESS] Account created! Welcome, " << this->current_user->getUsername() << "!\n";
    cout << "CONNECTING TO THE SERVERS............\n";    

}

void StreamingPlatformApp::login() {
    cout << "\n~~~~~~~~~~~~~ LOG IN ~~~~~~~~~~~~~\n";

    if (users_database.empty()) {
        cout << "[!] No accounts exist yet. Please Sign Up first!\n";
        return;
    }

    string input_username = get_name_input_validation(" [?] Enter your username: ");
    string lowercase_username = input_username;
    transform(lowercase_username.begin(), lowercase_username.end(), lowercase_username.begin(), ::tolower);
    auto user_found_it = users_database.find(lowercase_username);
    
    if (user_found_it == users_database.end()) {
        cout << "\n[!] Error: Username '" << input_username << "' not found.\n";
        return;
    }
    User* found_user = user_found_it->second;
    string input_password = get_name_input_validation(" [?] Enter your password: ");
    if (input_password == found_user->getPassword()) {
        this->current_user = found_user;
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

    // Create artists and add them to the database
    vector<Artist*> artists;
    artists.push_back(new Artist("Travis Scott", 60000000, {}, true));
    artists.push_back(new Artist("Lil Uzi Vert", 35000000, {}, true));
    artists.push_back(new Artist("A$AP Rocky", 30000000, {}, true));
    artists.push_back(new Artist("Drake", 80000000, {}, true));
    artists.push_back(new Artist("Kendrick Lamar", 55000000, {}, true));
    artists.push_back(new Artist("The Weeknd", 100000000, {}, true));
    artists.push_back(new Artist("Playboi Carti", 25000000, {}, true));
    artists.push_back(new Artist("Future", 45000000, {}, true));
    artists.push_back(new Artist("Tyler, The Creator", 32000000, {}, true));
    artists.push_back(new Artist("Metro Boomin", 50000000, {}, true));

    for (Artist* artist : artists) {
        artists_database[artist->getArtistID()] = artist;
    }

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


    for (int a_idx = 0; a_idx < static_cast<int>(artists.size()); a_idx++) {
        Artist* current_artist = artists[a_idx];
        int current_artist_id = current_artist->getArtistID();
        
        for (int i = 0; i < 5; i++) {
            // Giving random duration between 2de m and 3m to the songs
            int random_duration = 120 + (rand() % 60); 
            long long random_streams = 500000 + (rand() % 50000000); 

            int random_year = 2000 + (rand() % 20);
            int random_month = 1 + (rand() % 12);
            int random_day = 1 + (rand() % 28); // safety for feb

            Song* new_song =  new Song(all_songs[a_idx][i], random_duration, current_artist_id, {}, random_streams, Date(random_day, random_month, random_year));
            //? cout << *current_artist <<endl; DEBUGGING
            songs_database[new_song->getSongID()] = new_song;
            
            current_artist->addTopSong(all_songs[a_idx][i]);
            
        }
    }
    
    cout << "[SUCCESS] " << artists_database.size() << " Artists and " << songs_database.size() << " Songs successfully loaded into the matrix!\n";

}

StreamingPlatformApp::~StreamingPlatformApp() {
    // cout << "DESTRUCTOR PT APP A AVUT LOC GOODBYE RETURN 0 PAPA S A TERMINAT HAHA DAI RUN IAR PAPA\n";

    for (auto& pair : users_database) {
        delete pair.second;
    }

    for (auto& pair : songs_database ) {
        delete pair.second;
    }
    
    for (auto& pair : artists_database) {
        delete pair.second;
    }

    for (Playlist* p : playlists_database) {
        delete p;
    }
    
}


int main() {    
    // playableItem test; eroare 

    StreamingPlatformApp SpotifyApp;
    SpotifyApp.preloadedData();
    SpotifyApp.run();
    return 0;
}

//valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./app