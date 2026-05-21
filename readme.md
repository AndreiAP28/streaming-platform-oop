# Aplicație CLI pentru o Platformă de Streaming în C++

## Descrierea Proiectului

Acest proiect este o aplicație complet , bazată pe o Interfață în Linie de Comandă (CLI), care recreează arhitectura complexă de backend a marilor platforme de streaming muzical, precum **Spotify** sau **Apple Music**. Construită în întregime în **C++**, aplicația se concentrează puternic pe principiile avansate ale Programării Orientate pe Obiecte (POO/OOP), incluzând ierarhii profunde de moștenire, polimorfism, clase abstracte și o gestionare riguroasă a memoriei dinamice.



### Structura Proiectului (Fișiere de Bază):

- `app.cpp` / `app.h` (Aplicația principală, baze de date și meniul)
- `IObject.h` (Interfața de bază a tuturor entităților)
- `playable_item.h` (Clasa abstractă pentru redare)
- `song.cpp` / `song.h`
- `playlist.cpp` / `playlist.h` 
- `artist.cpp` / `artist.h`
- `user.cpp` / `user.h` 
- `premium_user.cpp` / `premium_user.h`
- `family_premium_user.cpp` / `family_premium_user.h` 
- `utils.cpp` / `utils.h`
- `templates.h` (Funcții și clase template / PlaybackHistory)
- `exceptions.h` (Ierarhia custom de excepții)
- `Makefile`
---

## Funcționalitățile Aplicației

- **Sistem de Autentificare**: Utilizatorii pot crea un cont nou sau se pot conecta la unul existent pentru a-și accesa biblioteca personalizată.
- **Gestionarea Bazei de Date**: Adăugarea de noi artiști și asocierea dinamică a melodiilor noi la aceștia.
- **Vizualizare Librarie**: Vizualizarea întregii baze de date de melodii, artiști și playlist-uri globale. 
- **Generare de Playlist-uri**: Crearea de playlist-uri personalizate, adăugarea sau eliminarea de piese și vizualizarea duratelor totale și a numărului de ascultări acumulate.
- **Coadă de Redare Dinamică**: Adăugarea de melodii individuale sau playlist-uri întregi într-o coadă. Aplicația simulează redarea în timp real folosind întârzieri prin `std::thread` și `std::chrono`
- **Vizualizare Istoric de Redare (Playback History)**: Utilizatorii își pot verifica oricând istoricul pieselor și playlist-urilor ascultate recent.
- **Niveluri Avansate de Abonament**: Utilizând Polimorfismul din C++, utilizatorii pot face upgrade de la nivelul Gratuit la Standard, Student, Anual sau un **Plan de Familie**, care deblochează capacitatea de a adăuga și elimina membri ai familiei asociați contului.
- **Integrarea Melodiilor Apreciate (Liked Songs)**: Utilizatorii pot aprecia melodii și le pot salva dinamic în profilul lor.

---

## Ierarhia de Clase

Aplicația respectă urmatoarea ierarhie de clase:
- **`IObject`**: Clasă interfață / bază abstractă care generează ID-uri unice pentru fiecare instanță și impune implementarea funcției `toString()`. [cite_start]Toate clasele de domeniu derivă de aici. [cite: 62, 63]
  - **`playableItem`**: Clasă abstractă (derivată din `IObject`) care reprezintă orice element ce poate fi introdus în coada de redare (`simulatePlayItem()`).
    - **`Song`**: Reprezintă o melodie cu date despre durată, artiști colaboratori și număr de ascultări.
    - **`Playlist`**: Reprezintă o colecție de obiecte de tip `Song` (agregare).
  - **`Artist`**: Reprezintă un artist cu statistici, melodii de top și un statut de cont verificat.
  - **`User`**: Reprezintă utilizatorul standard (Free-Tier), ce poate da like la melodii.
    - **`PremiumUser`**: Moștenește din `User` și deblochează un abonament platit și o dată de expirare (`Date`).
      - **`FamilyPremiumUser`**: Moștenește din `PremiumUser` și deblochează capacitatea de a adăuga mai mulți membri sub același abonament.

---

---

## Justificare alegere containere STL

1. **`std::unordered_map`** (`artists_database`, `songs_database`, `users_database`)
   - **Justificare:** Folosit pentru a stoca baza de date simulată din `StreamingPlatformApp`. Deoarece căutările utilizatorilor după nume (login) sau a melodiilor după ID se fac constant, `unordered_map` garantează un timp mediu de execuție de **O(1)** pentru operațiile de căutare, inserare și ștergere. 
2. **`std::vector`** (`playlists_database`, `liked_songs`, `topSongs`, `artistsFeatures`)
   - **Justificare:** Folosit pentru liste finite de date asociate unui cont (ex. melodiile apreciate ale unui utilizator). Permite indexare directă în timp constant **O(1)** și inserare rapidă la final (amortizat **O(1)**). Elementele ocupă memorie contiguă, fiind optim pentru cache.
3. **`std::list`** (`music_queue`)
   - **Justificare:** Folosit pentru coada de redare dinamică (Queue). Deoarece o coadă necesită constant eliminarea elementului de pe prima poziție (după ce melodia a fost ascultată) sau inserarea în interiorul cozii, lista dublu înlănțuită asigură un timp garantat de **O(1)** pentru aceste modificări.

   ### Idiomul `Erase-Remove`
- **`std::remove_if`**: Nu șterge fizic nimic din memorie. În schimb, rearanjează elementele containerului, mutând elementele valide la final și returneaza un iterator la finalul elementelor invalide al containerului.
- **`container.erase(...)`**: Preia iteratorul furnizat de `remove_if` și șterge efectiv elementele "reziduale" rămase la coada vectorului, redimensionând containerul în mod corect.

---

* **Compilator:** Compilator GNU C++ (g++)
* **Versiune Compilator:** 13.1.0
* **Standard C++:** C++17

## Flag-uri de Compilare Utilizate
* `-std=c++17`
* `-g` 
* `-Wall`

    ### Conținutul `Makefile`-ului utilizat:
```makefile
CXX = g++
CXXFLAGS = -std=c++17 -g -Wall
SRC_DIR = oop_project
BIN_DIR = bin
TARGET = $(BIN_DIR)/app.exe

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

all: $(TARGET)

$(TARGET): $(SOURCES)
    $(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
    ./$(TARGET)

clean:
    rm $(BIN_DIR)/*.exe
```
- În cazul alegerii metodei cu `Makefile` aveti urmatoarele 3 comenzi:
```bash
make
make run
make clean
```
- `make` doar generează executabilul `app.exe`, `make run` atât îl generează, cât și îl ruleaza, iar `make clean` șterge fișierele `.exe` din folderul `bin/`



