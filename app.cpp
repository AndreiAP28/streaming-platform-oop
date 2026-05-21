#include <iostream>
#include <cstdlib>
#include <cctype>
#include <algorithm>


#include "utils.h"
#include "IObject.h"
#include "app.h"
#include "artist.h"
#include "song.h"
#include "playlist.h" 
#include "user.h"
#include "premium_user.h"
#include "family_premium_user.h"
#include "playable_item.h"
#include "exceptions.h"


int IObject::counter = 0;

using namespace std;

const int StreamingPlatformApp::MAX_QUEUE_SIZE = 50;

StreamingPlatformApp::StreamingPlatformApp() : current_user(nullptr) {
    ofstream log_file("oop_project/logger.txt", ios::trunc);
    if (log_file.is_open()) {
        log_file <<" ~~~~~~~~ PROGRAM JUST STARTED ~~~~~~~~" << endl;
        log_file.close();
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
        try {
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
                    throw InvalidActionException("Invalid menu choice. Please select a valid number.");
            }   

        } catch(const StreamingAppException& e) {
            cout << "\n" << e.what() << "\n";
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
        throw DuplicateObjectException("Artist", input_artistName); 
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
        throw InvalidActionException("No artists in database. Add an artist first!");
    }
 
    string input_artistName = get_name_input_validation("\n [?] Enter the name of the Artist for the new song: ");

    auto artist_it = find_if(artists_database.begin(), artists_database.end(),
    [&](const auto& pair) {
        return duplicate_name_ignoreCase(pair.second->getName(), input_artistName);
    });
    
    
    if (artist_it == artists_database.end()) {
        throw ObjectNotFoundException("Artist", input_artistName);
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
        throw DuplicateObjectException("Song", input_songTitle);
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
        try {
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
                    throw InvalidActionException("Invalid menu choice. Please select a valid number.");
            }   

        } catch( const StreamingAppException& e) {
            cout << "\n" << e.what() << "\n";
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
        throw InvalidActionException("There are no playlists created yet.");
    }

   bool user_hasno_playlists = none_of(playlists_database.begin(), playlists_database.end(), 
        [&](const Playlist* curr_p) {
            return curr_p->getOwnerUsername() == current_user->getUsername();     
        }    
    );

    if (user_hasno_playlists) {
        throw InvalidActionException("You haven't created any platlist yet!");
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
        throw ObjectNotFoundException("Playlist ID", to_string(playlistID_input));
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
        try  {
            switch (user_choice) {
                case 1: {
                    if (songs_database.empty()) {
                        throw InvalidActionException("There are no songs in database. Add songs first!");
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
                    
                    int display_index = 1;
                    for ( const Song* current_song: sorted_songs) {
                        int current_artistID = current_song->getSongsArtistID();
                        auto current_artist_it = artists_database.find(current_artistID);

                        string current_artistName = "Unknown";
                        if (current_artist_it != artists_database.end()) {
                            current_artistName = current_artist_it->second->getName();
                        }

                        cout << " <- No. " <<display_index++ <<" "<< current_song->getTitle() << " - " << current_artistName << "\n";
                    }
                    cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    
                    int chosen_songNumber = get_int_input_validation(" <--- Enter the number of the song you want to add: ");
                    
                    // auto adding_song_it = songs_database.find(songID_input);
                    // if (adding_song_it == songs_database.end()) {
                        // throw ObjectNotFoundException("Song ID", to_string(songID_input));
                    // }  
                    // Song* adding_song = adding_song_it->second;
                    if (chosen_songNumber < 1 || chosen_songNumber > static_cast<int>(sorted_songs.size())) {
                        throw OutOfRangeException(chosen_songNumber,(int) sorted_songs.size());
                    }

                    Song* adding_song = sorted_songs[chosen_songNumber - 1];

                    const vector<Song*>& playlist_songs = managing_playlist->getSongs();
                    bool already_exist = any_of(playlist_songs.begin(), playlist_songs.end(),
                        [&](const Song* curr_song) {
                            return curr_song->getSongID() == adding_song->getSongID();
                        }
                    );
    
                    if (already_exist) {
                        throw DuplicateObjectException("Song", adding_song->getTitle());
                    } else {
                        managing_playlist->addSong(*adding_song);
                        cout << "[SUCCESS] Song added to playlist!\n";
                    }
                    break;
                }
                
                case 2: {
                    const vector<Song*>& playlist_songs = managing_playlist->getSongs();
                    if (playlist_songs.empty()) {
                        throw InvalidActionException("This playlist is already empty");
                    }
    
                    cout << "\n~~~~~~~~~~~~~~~~ TRACKS IN PLAYLIST ~~~~~~~~~~~~~~~~\n";
                    int display_index = 1;
                    for ( const Song* song: playlist_songs) {
                            cout << " <- No. " << display_index++ << " -> " << song->getTitle() << "\n";
                    }
                    cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    
                    // int removeID_input = get_int_input_validation("\n <--- Enter the ID of the song that you want to remove: ");
                    // managing_playlist->removeSong(removeID_input); 
                    
                    int chosen_songNumber = get_int_input_validation("\n[?] What song do you want to remove (Enter No.): ");
                    
                    if (!(chosen_songNumber >= 1 && chosen_songNumber <= static_cast<int>(playlist_songs.size()))) {
                       throw InvalidActionException("Invalid Number. Going back to menu!");
                    } 
                    
                    int real_song_id = playlist_songs[chosen_songNumber - 1]->getID();
                    managing_playlist->removeSong(real_song_id);
                    
                    break;
                }
    
                case 3:
                    cout << *managing_playlist;
                    break;
    
                case 0: 
                    cout << "Returning to Main Menu ! .... \n";
                    break;
                default:
                    throw InvalidActionException("Invalid menu choice. Please select a valid number.");
            }

        } catch(const ObjectNotFoundException& e) {
            cout << "\n" << e.what() << "\n";
            cout << "[i] HINT: The ID you entered (" << e.getMissingObj() << ") does not exist. Please check the list above.\n";
        } catch( const OutOfRangeException& e) {
            cout << "\n" << e.what() << "\n";
            cout << "[i] HINT: " << e.getInvalidIndex() <<" is your chosen index, but this is out of range. Choose a valid index.\n";   
        }
        catch (const DuplicateObjectException& e) {
            cout << "\n" << e.what() << "\n";
            cout << "[i] HINT: The song' " << e.getDuplicateValue() << "' is already in your playlist! Try adding something else.\n";
        }
        catch (const StreamingAppException& e) {
            cout << "\n" << e.what() << "\n";
        }

    }


}

void StreamingPlatformApp::ViewPlaylists() {
    if (playlists_database.empty()) {
        throw  InvalidActionException("There are no playlists registered yet");
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
        throw InvalidActionException("User has no playlist created.");
    }
    char user_input = get_yesno_input_validation("\n[?] Would you like to view more about a playlist? (y/n): ");
    
    if (user_input == 'y' || user_input == 'Y') {
        int chosenID = get_int_input_validation(" <--- Enter the Playlist ID: ");
        
        auto chosen_playlist_it = find_if(playlists_database.begin(), playlists_database.end(), 
            [&](const Playlist* curr_p) {
                return curr_p->getPlaylistID() == chosenID && curr_p->getOwnerUsername() == current_user->getUsername();
            });
        if (chosen_playlist_it == playlists_database.end()) {
            throw ObjectNotFoundException("Playlist ID", to_string(chosenID));
        }

        Playlist* chosen_playlist = *chosen_playlist_it;
        
        cout << *chosen_playlist;
        if (chosen_playlist->getSongsSize()){
            cout << "Total Songs' Streams in this playlist: " << chosen_playlist->calculateTotalStreams() << " streams.\n";
        }

        bool looping = true;
        while (looping) {
                int user_choice = get_int_input_validation("\n[1] Play Now  ~  [2] Add to Queue  ~  [0] Go Back: ");
                try {
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
                                throw LimitExceededException("Music Queue", this->MAX_QUEUE_SIZE);
                            }
                            looping = false;
                            break;
                        default:
                            throw InvalidActionException("Invalid menu choice. Please select a valid number.");
                            
                    }

                } catch (const StreamingAppException& e) {
                    cout << "\n" << e.what() << "\n";
                }
            }
        
        
    }
}

void StreamingPlatformApp::ViewArtistsDB() {
    if (artists_database.empty()) {
        throw InvalidActionException("There are no artists registered yet.");
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

    // for (Artist* current_artist: sorted_artists) {
    //     cout << "<- ID: [" << current_artist->getArtistID() << "] -> " << current_artist->getName() << " " << (current_artist->getIsVerified() ? " (VERIFIED)" : "") << "\n";
    // }

    int display_index = 1;
    for(const Artist* current_artist: sorted_artists) {
        cout << "<- No. " << display_index++ << " " << current_artist->getName() << " " << (current_artist->getIsVerified() ? " (VERIFIED)" : "") << "\n";
    }
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

    char user_input = get_yesno_input_validation("\n[?] Would you like to view a specific artist's full profile? (y/n): ");

    if (user_input == 'y' || user_input == 'Y') {
        // int chosenID = get_int_input_validation(" <--- Enter the Artist ID: ");
        // auto chosen_artist_it = artists_database.find(chosenID);
        // if (chosen_artist_it != artists_database.end()) {
        int chosen_artistNumber = get_int_input_validation(" <--- Enter the artist number (1 - " + to_string(sorted_artists.size()) + "): ");

        
        if (chosen_artistNumber >= 1 && chosen_artistNumber <= static_cast<int>(sorted_artists.size())) {
            // Artist* chosen_artist = chosen_artist_it->second;
            Artist* chosen_artist = sorted_artists[chosen_artistNumber - 1];
            ++(*chosen_artist);
            cout << *chosen_artist;
        } else {
            throw InvalidActionException("Invalid Number. Going back to menu!");
        }
    }
}

void StreamingPlatformApp::ViewSongsDB() { 
    if (songs_database.empty()) {
        throw InvalidActionException("There are no songs registered yet.");
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
    
    // for (const Song* current_song: sorted_songs) {
    //     int current_artistID = current_song->getSongsArtistID();
    //     auto current_artist_it = artists_database.find(current_artistID);

    //     string current_artistName;
    //     if (current_artist_it != artists_database.end()) {
    //         current_artistName = current_artist_it->second->getName();
    //     }

    //     cout << " <- ID: [" <<current_song->getSongID() << "] -> "<< current_song->getTitle() << " - " << current_artistName << "\n";
    // }

    //! VARIANTA FOLOSITA PT ID URI GENERALE IN IOBJECT NU PT FIECARE CLASA IN PARTE
    int display_index = 1;
    for ( const Song* current_song: sorted_songs) {
        int current_artistID = current_song->getSongsArtistID();
        auto current_artist_it = artists_database.find(current_artistID);

        string current_artistName = "Unknown";
        if (current_artist_it != artists_database.end()) {
            current_artistName = current_artist_it->second->getName();
        }

        cout << " <- No. " <<display_index++ <<" "<< current_song->getTitle() << " - " << current_artistName << "\n";
    }

    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

    char user_input = get_yesno_input_validation("\n[?] Would you like to view a specific song's full profile? (y/n): ");
   
    if (user_input == 'y' || user_input == 'Y') {
        int chosen_songNumber = get_int_input_validation(" <--- Enter the song number (1 - " + to_string(sorted_songs.size()) + "): ");
        
        // int chosenID = get_int_input_validation(" <--- Enter the Song ID: ");
        // auto chosen_song_it = songs_database.find(chosenID);
        // if (chosen_song_it != songs_database.end()) {

        if (chosen_songNumber >= 1 && chosen_songNumber <= static_cast<int>(sorted_songs.size())) {
            // Song* chosen_song = chosen_song_it->second;
            Song* chosen_song = sorted_songs[chosen_songNumber - 1];
            ++(*chosen_song);
            cout << *chosen_song;
            
            bool looping = true;
            while (looping) {
                cout << "\n [1] Play Now ~ [2] Add to Queue ~ [0] Go Back: \n";
                int user_choice = get_int_input_validation("Choose an action: ");
                try {
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
                                throw LimitExceededException("Music Queue", this->MAX_QUEUE_SIZE);
                            }
                            looping = false;
                            break;
                        case 0:
                            looping = false;
                            break;
                        default:
                            throw InvalidActionException("Invalid menu choice. Please select a valid number.");
                    }

                } catch (const LimitExceededException& e) {
                    cout << "\n" << e.what() << "\n";
                    cout << "[i] HINT: Your queue can only hold up to " << e.getLimit() << "items. Please, first play the queue to clear it, then add it again.\n";
                } 
                catch(const StreamingAppException& e) {
                    cout << "\n" << e.what() << "\n";
                }
            }

        } else {
            throw InvalidActionException("Invalid Number. Going back to menu!");
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
            cout << "|  [4]. Display Logs                             |\n";
            cout << "|  [5]. Sign Out                                 |\n";
            cout << "|  [0]. Go Back to Main Menu                     |\n";
            cout << "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n";
            show_second_menu = false;
        }
        int user_choice = get_int_input_validation("Choose an action: ");
        try {
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
                    this->DisplayLogs();
                    show_second_menu = true;
                    break;
                case 5:
                    cout << "\nLogging out... Goodbye, " << this->current_user->getUsername() << "!\n";
                    this->current_user = nullptr;
                    looping = false;
                    break;
                case 0:
                    looping = false;
                    break;
    
                default:
                    throw InvalidActionException("Invalid menu choice. Please select a valid number.");
            }   

        } catch(const StreamingAppException& e) {
            cout << "\n" << e.what() << "\n";
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
        try {
            switch (user_choice) {
               case 1: {
                    const vector<Song*>& liked_songs = current_user->getLikedSongs();
                    if (!liked_songs.empty()) {
                        cout <<"\n~~~~~~~~~~~~~~~~~ Your Liked Songs ~~~~~~~~~~~~~~~~~\n";
                        int display_index = 1; 
                        for (const Song* song : liked_songs) {
                            // cout << "[ ID: " << song->getSongID() << "] " << song->getTitle() << "\n"; 
                            cout << "No. " <<display_index++ << " -> " << song->getTitle() << "\n"; 
                        }
                        cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    
                    } else {
                        throw InvalidActionException("You don't have any liked songs yet");
                    }
                    show_menu = true;
                    break;
                }
            
                case 2: {
    
                    if (songs_database.empty()) {
                        throw InvalidActionException("There are no songs registered yet");
                        // show_menu = true;
                        // break;
                    }
    
                    cout << "\n~~~~~~~~~~~~~~~~~~ REGISTERED SONGS ~~~~~~~~~~~~~~~~~~\n";
                    vector<Song*> sorted_songs;
                    for (const auto& [current_id, current_song] : songs_database) {
                        sorted_songs.push_back(current_song);
                    }
    
                    sort(sorted_songs.begin(), sorted_songs.end(),
                        [](Song* first, Song* second) {
                            return first->getSongID() < second->getSongID();
                        });
                    
                    int display_index = 1; 
                    for (Song* current_song: sorted_songs) {
                        int current_artistID = current_song->getSongsArtistID();
                        auto current_artist_it = artists_database.find(current_artistID);
    
                        string current_artistName;
                        if (current_artist_it != artists_database.end()) {
                            current_artistName = current_artist_it->second->getName();
                        }
                        cout << "No. " <<display_index++ << " -> " << current_song->getTitle() << "\n"; 
                        // cout << " <- ID: [" <<current_song->getSongID() << "] -> "<< current_song->getTitle() << " - " << current_artistName << "\n";
                    }
                    
                    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    
                    int chosen_songNumber = get_int_input_validation("\n <--- Enter the song number (1 - " + to_string(sorted_songs.size()) + "): ");
                    // int songID_input = get_int_input_validation("\n <--- What song do you like (type Song ID): ");
       
    
                    // auto liked_song_it =songs_database.find(songID_input);
    
                    // if (liked_song_it == songs_database.end()) {
                    if (!(chosen_songNumber >= 1 && chosen_songNumber <= static_cast<int>(sorted_songs.size()))) {
                        throw InvalidActionException("Invalid Number. Going back to menu!");
                        // show_menu = true;
                        // break;
                    }
                    
                    // *current_user += *(liked_song_it->second);
                    *current_user += *(sorted_songs[chosen_songNumber - 1]);
                    
                    
                    show_menu = true;
                    break;
                }
                
                case 3: {
                    const vector<Song*>& liked_songs = current_user->getLikedSongs();
                    if (liked_songs.empty()) {
                        throw InvalidActionException("You don't have any liked songs to remove");
                        // show_menu = true;
                        // break;
                    }
    
                    cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~ YOUR LIKED SONGS ~~~~~~~~~~~~~~~~~~~~~\n";
                    int display_index = 1;
                    for (const Song* song : liked_songs ){
                        cout << " <- No. " << display_index++ << " -> " << song->getTitle() << "\n";
                        // cout << " <- ID: [" << song->getSongID() << "] -> " << song->getTitle() << "\n";
                    }
    
                    // int songID_input = get_int_input_validation("\n[?] What song do you want to remove (type Song ID): ");
                    // *current_user -= songID_input;
                    int chosen_songNumber = get_int_input_validation("\n[?] What song do you want to remove (Enter No.): ");
                    
                    if (chosen_songNumber >= 1 && chosen_songNumber <= static_cast<int>(liked_songs.size())) {
                        int real_song_id = liked_songs[chosen_songNumber - 1]->getID();
                        *current_user -= real_song_id;
                        
                        cout << " [SUCCESS] Song removed from your liked list!\n";
                    } else {
                       throw InvalidActionException("Invalid Number. Going back to menu!");
                    }
                    show_menu = true;
                    break;
    
                }
                
                    case 0:
                        cout << "\nReturning to Main Menu...\n";
                        liked_songs_looping = false;
                        break;
                
                default:
                    throw InvalidActionException("Invalid menu choice. Please select a valid number.");
            }   
        } catch (const StreamingAppException& e) {
            cout << "\n" << e.what() << "\n";
            show_menu = true;
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
            try {
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
                            throw InvalidActionException("Invalid menu choice. Please select a valid number.");
        
                    }
            } catch (const StreamingAppException& e) {
                cout << "\n" << e.what() << "\n";
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
            try {
                if (user_choice == 0) {
                    return;
                } else if (user_choice == 1) {
                    break;
                } 
                else {
                    throw InvalidActionException("Invalid menu choice. Please select a valid number.");
                }
            } catch ( const StreamingAppException& e) {
                cout << "\n" << e.what() << "\n";
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
        case 0:
            return;
        default:
            throw InvalidActionException("Invalid menu choice. Please select a valid number.");
    }

    delete current_user;


    db_current_user_it->second  = upgraded_subs_user;
    current_user = upgraded_subs_user;

    cout << "[SUCCES] Congratulations! Subscription Activated!\n";
}

void StreamingPlatformApp::DisplayLogs() {
    try {
        string filepath = "oop_project/logger.txt" ;
        ifstream log_file(filepath);
        cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
        cout << "\n       SYSTEM LOG VISUALIZER (UX)       ";
        cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    
        if (log_file.is_open()) {
            string line;
            int line_count = 0;

            getline(log_file, line);
    
            while (getline(log_file, line)) {
                cout << line << "\n";
                line_count++;
            }
    
            if (line_count == 0) {
                cout << "[i] Jurnalul este gol momentan.\n";
            }
            log_file.close();
        } else {
            throw ObjectNotFoundException("File" , filepath);
        }
    
        cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        cout << "Press ENTER to get to the menu...";
        string ENTER_DUMMY;
        getline(cin, ENTER_DUMMY);
    } catch (const ObjectNotFoundException& e)  {
        cout << "\n" << e.what() << "\n";
        cout << "[i] HINT: Make sure the file exist, and is in the right DIRECTORY.\n";
    } catch( const StreamingAppException& e) {
        cout << "\n [!] Other error: "<< e.what() << "\n";
    }
}
//!-----------------------------------------------------------
void StreamingPlatformApp::playQueue() {
    if(this->music_queue.empty()) {
        throw InvalidActionException("Your queue is empty");
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
        try {
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
                    throw InvalidActionException("Invalid menu choice. Please select a valid number.");
            }

        } catch(const StreamingAppException& e) {
            cout << "\n" << e.what() << "\n";
            show_menu = true;
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
        throw DuplicateObjectException("User", input_username);
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
        throw InvalidActionException("No accounts exist yet. Please Sign Up first!");
    }

    string input_username = get_name_input_validation(" [?] Enter your username: ");
    string lowercase_username = input_username;
    transform(lowercase_username.begin(), lowercase_username.end(), lowercase_username.begin(), ::tolower);
    auto user_found_it = users_database.find(lowercase_username);
    
    if (user_found_it == users_database.end()) {
        throw ObjectNotFoundException("User", input_username);
    }
    User* found_user = user_found_it->second;
    string input_password = get_name_input_validation(" [?] Enter your password: ");
    if (input_password == found_user->getPassword()) {
        this->current_user = found_user;
        cout << "\n[SUCCESS] Logged in successfully! Welcome back, " << this->current_user->getUsername() << "!\n";
    } else {
        throw InvalidActionException("Wrong password! Try again.");
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
    
            try {
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
                        show_menu = true;
                        break;
                    case 0:
                        cout << "\nTHE APP IS SHUTTING DOWN..... SEE YOU SOON!\n";
                        main_looping = false;
                        app_run_flag = false;
    
                        break;
                    default:
                        throw InvalidActionException("Invalid menu choice. Please select a valid number.");   
                }

            } catch( const StreamingAppException& e) {
                cout << "\n" << e.what() << "\n";
                show_menu = true;
            }
    
        }
}

void StreamingPlatformApp::run() {
    this->preloadedData();

    bool app_run_flag = true;
    while (app_run_flag) {
        try {
            this->connectingMenu();
        
            if (this->current_user != nullptr) {
                this->mainMenu(app_run_flag);
            } else {
                app_run_flag = false;
            }

        } catch( const StreamingAppException& e){
            cout << "\n" << e.what() << "\n";
        } 
        catch(...){
            cout << "\n [!!] UNKNOWN ERROR: The app is closing....\n";
            app_run_flag = false;
        }
    }
}

void StreamingPlatformApp::preloadedData() {
    cout << "[i] Booting Database... Injecting Mock Data...\n";

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

StreamingPlatformApp& StreamingPlatformApp::getInstance() {
    static StreamingPlatformApp instance;
    return instance;
}

void StreamingPlatformApp::systemLog(const string& message) {
        ofstream log_file("oop_project/logger.txt", ios::app);
        if (log_file.is_open()){
            log_file << "[ LOG SYSTEM ID#" + message << endl;
            log_file.close();
        }
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
    
    // if (f.is_open()) {
    //     f.close();    
    // }

}


int main() {    
    // playableItem test; eroare 

    // StreamingPlatformApp SpotifyApp; eroare compilare pt ca avem constructor privat
    // StreamingPlatformApp copie = StreamingPlatformApp::getInstance(); // operator = este sters

    StreamingPlatformApp::getInstance().run();
    return 0;
}

//valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./bin/app.exe
