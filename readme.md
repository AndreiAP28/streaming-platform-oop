# Aplicație CLI pentru o Platformă de Streaming în C++

## Descrierea Proiectului

Acest proiect este o aplicație complet , bazată pe o Interfață în Linie de Comandă (CLI), care recreează arhitectura complexă de backend a marilor platforme de streaming muzical, precum **Spotify** sau **Apple Music**. Construită în întregime în **C++**, aplicația se concentrează puternic pe principiile avansate ale Programării Orientate pe Obiecte (POO/OOP), incluzând ierarhii profunde de moștenire, polimorfism, clase abstracte și o gestionare riguroasă a memoriei dinamice.

Toate datele, inclusiv melodiile, artiștii, playlist-urile și profilurile utilizatorilor, sunt gestionate activ în memoria dinamică în timpul rulării (runtime), simulând un mediu de baze de date live. 

### Structura Proiectului (Fișiere de Bază):

- `app.cpp` / `app.h` 
- `playable_item.h` 
- `song.cpp` / `song.h`
- `playlist.cpp` / `playlist.h` 
- `artist.cpp` / `artist.h`
- `user.cpp` / `user.h` 
- `premium_user.cpp` / `premium_user.h`
- `family_premium_user.cpp` / `family_premium_user.h` 
- `utils.cpp` / `utils.h`
- `Makefile`

---

## Funcționalitățile Aplicației

- **Sistem de Autentificare**: Utilizatorii pot crea un cont nou sau se pot conecta la unul existent pentru a-și accesa biblioteca personalizată.
- **Gestionarea Bazei de Date**: Adăugarea de noi artiști și asocierea dinamică a melodiilor noi la aceștia.
- **Vizualizare Librarie**: Vizualizarea întregii baze de date de melodii, artiști și playlist-uri globale. 
- **Generare de Playlist-uri**: Crearea de playlist-uri personalizate, adăugarea sau eliminarea de piese și vizualizarea duratelor totale și a numărului de ascultări acumulate.
- **Coadă de Redare Dinamică**: Adăugarea de melodii individuale sau playlist-uri întregi într-o coadă. Aplicația simulează redarea în timp real folosind întârzieri prin `std::thread` și `std::chrono`
- **Niveluri Avansate de Abonament**: Utilizând Polimorfismul din C++, utilizatorii pot face upgrade de la nivelul Gratuit la Standard, Student, Anual sau un **Plan de Familie**, care deblochează capacitatea de a adăuga și elimina membri ai familiei asociați contului.
- **Integrarea Melodiilor Apreciate (Liked Songs)**: Utilizatorii pot aprecia melodii și le pot salva dinamic în profilul lor.

---

* **Compilator:** Compilator GNU C++ (g++)
* **Versiune Compilator:** 13.1.0
* **Standard C++:** C++17

## Flag-uri de Compilare Utilizate
* `-std=c++17`
* `-g` 
* `-Wall`



