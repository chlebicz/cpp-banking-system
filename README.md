# Bank
Application implementing bank functionalities - handling accounts, cards, clients, transfers.

## Building (CMake)
The application requires a compiler supporting the C++17 standard and the [Boost](https://www.boost.org/) library installed on the system (version at least 1.60.0). It can be installed using the command (Ubuntu/Debian):
```
sudo apt-get install libboost-all-dev
```

When building the application using CMake, the external [json](https://github.com/nlohmann/json) library is automatically downloaded. Example usage of CMake and make to build the application (Linux):
```
mkdir build
cd build
cmake ..
make
```

## Running (Unit Tests)
Unit tests for the application built as above can be run with the command:
```
./LibraryTester
```

## Running (User Interface)
An example user interface has been implemented in the text console. For the application built as above, it can be run with the commands:
```
cd program
./Program
```

## UML Diagram
The UML diagram of object dependencies is available in the file [Bank.pdf](Bank.pdf)

## Authors
- Mikołaj Chlebicz
- Bartosz Horna

## License
The project uses the [MIT license](LICENSE)
