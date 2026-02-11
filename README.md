# Bank
Aplikacja realizująca funkcjonalności banku - obsługa kont, karty, klienci, przelewy.

## Budowanie (CMake)
Aplikacja wymaga kompilatora obsługującego standard C++17 oraz zainstalowanej w systemie biblioteki [Boost](https://www.boost.org/) w wersji co najmniej 1.60.0. Można ją zainstalować przy użyciu polecenia (Ubuntu/Debian):
```
sudo apt-get install libboost-all-dev
```

Podczas budowania aplikacji przy użyciu CMake, automatycznie pobierana jest biblioteka zewnętrzna [json](https://github.com/nlohmann/json). Przykład użycia CMake i make do zbudowania aplikacji (Linux):
```
mkdir build
cd build
cmake ..
make
```

## Uruchomienie (testy jednostkowe)
Testy jednostkowe dla aplikacji zbudowanej jak wyżej można uruchomić poleceniem:
```
./LibraryTester
```

## Uruchomienie (interfejs użytkownika)
Przykładowy interfejs użytkownika został zaimplementowany w konsoli tekstowej, dla aplikacji zbudowanej jak wyżej można uruchomić go poleceniami:
```
cd program
./Program
```

## Diagram UML
Diagram UML zależności obiektowych jest dostępny w pliku [Bank.pdf](Bank.pdf)

## Autorzy
- Mikołaj Chlebicz
- Bartosz Horna

## Licencja
Projekt używa [licencji MIT](LICENSE)