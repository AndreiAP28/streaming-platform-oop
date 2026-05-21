#pragma once 
#include <iostream>
#include <vector>
#include "exceptions.h"
#include "song.h"
#include "playlist.h"

using namespace std;

//!BONUS
template <typename T, int N>
class PlaybackHistory {
    T history[N];
    int count;
public:
    explicit PlaybackHistory(): count(0) {}
    void add(const T& item) {
        // if (count < N) {
        //     history[count++] = item;
        // } else {
        //     for (int i = 0; i < N - 1; i ++) {
        //         history[i] = history[i + 1];
        //     }

        //     history[N - 1] = item;
        // }
        if (count >= N){
            throw std::overflow_error("Playback history is full! Maximum capacity reached.");
        }
        history[count++] = item;
    }

    T getLastPlayed() const {
        if (count == 0) {
            // throw InvalidActionException("Playback history is empty!");
            throw std::underflow_error("Playback history is empty!");
        }
        return history[count - 1];
    }

    void clear() {
        count = 0;
    }

    template<typename Iterator>
    void migrateTo(Iterator dest) const {
        for (int i = 0; i < count; i ++) {
            *dest = history[i];
            dest++;
        }
    }
};

template<typename Key, typename Value>
std::vector<Value> extractMapValues(const std::unordered_map<Key, Value>& source) {
    std::vector<Value> result;
    for (const auto& pair: source) {
        result.push_back(pair.second);
    }
    return result;
}

template<typename T>
void printItem(const T& item) {
    cout << item << "\n";
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
}

// template <typename Iterator, typename T>
// bool searchItem(Iterator start, Iterator end, const T& value) {

// }


//! specializare in cazul in care in istoric este un playlist sau o piesa
template<>
inline void printItem<Song*>(Song* const& song) {
    cout << "\n  [SONG]\n";
    cout << "   Title: " << song->getTitle() << "\n";
    cout << "   Artist: " << song->getArtistName() << "\n";
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

    

};

template<>
inline void printItem<Playlist*>(Playlist* const& playlist) {
    cout << "\n [PLAYED PLAYLIST] \n";
    cout << "   Name: " << playlist->getName() << "\n";
    cout << "   Owner: " << playlist->getOwnerUsername() << "\n";
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
};