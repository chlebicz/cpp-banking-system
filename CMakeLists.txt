cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS_DEBUG "-g3 -gdwarf-4")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -gdwarf-4")

project(Project)
enable_language(CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_VERBOSE_MAKEFILE TRUE)

add_subdirectory(library)
add_subdirectory(program)

enable_testing()

find_package(Boost 1.60.0 COMPONENTS "unit_test_framework" "date_time" REQUIRED)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
)

set(SOURCE_TEST_FILES library/test/master.cpp
    library/test/AmountTest.cpp
        library/test/StorageHandlerTest.cpp
        library/test/AccountTest.cpp
        library/test/CardTest.cpp
        library/test/ClientTest.cpp
        library/test/DepositTest.cpp
        library/test/TransferTest.cpp
        library/test/LoanTest.cpp
        library/test/ClientManagerTest.cpp
        library/test/AccountManagerTest.cpp
        library/test/TransferManagerTest.cpp
        library/test/GoldCoinsTest.cpp
        library/test/RepositoryTest.cpp
)

add_executable(LibraryTester ${SOURCE_TEST_FILES})
target_link_libraries(LibraryTester Library ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(LibraryTest LibraryTester
    --report_level=detailed
    --log_level=all
    --color_output=yes
)

