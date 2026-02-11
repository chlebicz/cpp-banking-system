cmake_minimum_required(VERSION 3.4)
project(Library)

set(SOURCE_FILES
        include/model/Account.h
        src/model/Account.cpp
        include/managers/AccountManager.h
        src/managers/AccountManager.cpp
        include/model/Amount.h
        src/model/Amount.cpp
        include/model/Bank.h
        src/model/Bank.cpp
        include/model/Card.h
        src/model/Card.cpp
        include/model/Client.h
        src/model/Client.cpp
        include/managers/ClientManager.h
        src/managers/ClientManager.cpp
        include/model/CryptoAccount.h
        src/model/CryptoAccount.cpp
        include/model/Deposit.h
        src/model/Deposit.cpp
        include/model/DiamondCard.h
        src/model/DiamondCard.cpp
        include/model/GoldCard.h
        src/model/GoldCard.cpp
        include/model/InvestmentAccount.h
        src/model/InvestmentAccount.cpp
        include/model/Loan.h
        src/model/Loan.cpp
        include/model/MainAccount.h
        src/model/MainAccount.cpp
        include/Repository.h
        include/model/SavingsAccount.h
        src/model/SavingsAccount.cpp
        include/Serializable.h
        include/model/StandardCard.h
        src/model/StandardCard.cpp
        include/StorageHandler.h
        src/StorageHandler.cpp
        include/model/Transfer.h
        src/model/Transfer.cpp
        include/managers/TransferManager.h
        src/managers/TransferManager.cpp
        include/RandomDataGenerator.h
        src/RandomDataGenerator.cpp
        include/Entity.h
        src/Serializable.cpp
        include/Identifiable.h
        include/model/GoldCoins.h
        src/model/GoldCoins.cpp
)

add_library(Library ${SOURCE_FILES})

target_include_directories(Library PUBLIC include)

target_link_libraries(Library nlohmann_json::nlohmann_json)