#include <fstream>
#include <boost/test/unit_test.hpp>

#include "StorageHandler.h"

BOOST_AUTO_TEST_SUITE(StorageHandlerTest)

/**
 * @test Sprawdza, czy StorageHandler tworzy katalog na pliki obiektów,
 *       jeśli takowy jeszcze nie istnieje
 */
BOOST_AUTO_TEST_CASE(StorageHandlerConstructorCreatesDirIfDoesNotExist) {
    std::filesystem::path path = std::filesystem::current_path() / "katalog";
    if (exists(path))
        remove_all(path);

    StorageHandler sh{path};
    BOOST_TEST(std::filesystem::is_directory(path));
    BOOST_TEST(sh.getAllObjects().empty());
}

/**
 * @test Sprawdza, czy jeśli katalog z plikami obiketów istnieje, to
 *       StorageHandler odczytuje ich stan
 */
BOOST_AUTO_TEST_CASE(StorageHandlerConstructorLoadsExistingDirContents) {
    std::filesystem::path path = std::filesystem::current_path() / "katalog";
    if (exists(path))
        remove_all(path);

    create_directory(path);

    std::string id1 = "plik1";
    json data1 = {{"a", 1}};
    std::ofstream file1{path / id1};
    BOOST_REQUIRE(file1.good());
    file1 << data1;
    file1.close();

    std::string id2 = "plik2";
    json data2 = {{"b", 2}};
    std::ofstream file2{path / id2};
    BOOST_REQUIRE(file2.good());
    file2 << data2;
    file2.close();

    StorageHandler sh{path};
    BOOST_TEST(sh.getAllObjects().size() == 2);
    BOOST_TEST(sh.getObjectData(id1) == data1);
    BOOST_TEST(sh.getObjectData(id2) == data2);
}

/**
 * Klasa do testów, obsługiwana przez StorageHandler
 */
class TestObject : public Entity {
public:
    /**
     * Konstruktor
     * @param id Identyfikator
     * @param data Dane obiektu
     */
    TestObject(std::string id, std::string data)
        : id(std::move(id)), data(std::move(data)) {}

    /**
     * @returns Dane obiektu
     */
    std::string toString() const override {
        return data;
    }

    /**
     * @returns Dane zapisane w obiekcie w formacie JSON
     */
    json toJSON() const override {
        return {
            {"id", id},
            {"data", data}
        };
    }

    /**
     * @returns Identyfikator obiektu
     */
    std::string getId() const override {
        return id;
    }
private:
    std::string id, data;
};

/**
 * @test Sprawdza poprawne zapisywanie obiektów przez StorageHandler
 */
BOOST_AUTO_TEST_CASE(StorageHandlerSaveObjectTest) {
    std::filesystem::path path = std::filesystem::current_path() / "katalog";
    if (exists(path))
        remove_all(path);

    StorageHandler sh{path};

    auto testObj1 = std::make_shared<TestObject>("id1", "data");
    sh.saveObject(testObj1);

    auto testObj2 = std::make_shared<TestObject>("id2", "data");
    sh.saveObject(testObj2);

    // sprawdzenie czy odpowiednie pliki dla obiektow sa tworzone
    std::ifstream file1{path / testObj1->getId()};
    BOOST_TEST(file1.good());

    std::ifstream file2{path / testObj2->getId()};
    BOOST_TEST(file2.good());

    // sprawdzenie czy dane obiektow sa zapisywane
    BOOST_TEST(sh.getAllObjects().size() == 2);
    BOOST_TEST(sh.getObjectData(testObj1->getId()) == testObj1->toJSON());
    BOOST_TEST(sh.getObjectData(testObj2->getId()) == testObj2->toJSON());
}

/**
 * @test Sprawdza poprawne nadpisywanie zawartości istniejących plików z
 *       obiektami przez StorageHandler
 */
BOOST_AUTO_TEST_CASE(StorageHandlerSaveObjectGivenExistingIdTest) {
    std::filesystem::path path = std::filesystem::current_path() / "katalog";
    if (exists(path))
        remove_all(path);

    StorageHandler sh{path};

    std::string id = "IDENTYFIKATOOOOOR";
    auto testObj1 = std::make_shared<TestObject>(id, "data");
    sh.saveObject(testObj1);

    auto testObj2 = std::make_shared<TestObject>(id, "dejta");
    sh.saveObject(testObj2);

    BOOST_TEST(sh.getAllObjects().size() == 1);
    BOOST_TEST(sh.getObjectData(testObj1->getId()) == testObj2->toJSON());

    std::ifstream file1{path / testObj1->getId()};
    BOOST_TEST(file1.good());

    int fileCount = 0;
    for (const auto& _: std::filesystem::directory_iterator(path))
        fileCount++;

    BOOST_TEST(fileCount == 1);
}

/**
 * @test Sprawdza poprawne usuwanie istniejących obiektów przez StorageHandler
 */
BOOST_AUTO_TEST_CASE(StorageHandlerRemoveObjectTest) {
    std::filesystem::path path = std::filesystem::current_path() / "katalog";
    if (exists(path))
        remove_all(path);

    StorageHandler sh{path};

    auto testObj = std::make_shared<TestObject>("id1", "data");
    sh.saveObject(testObj);

    sh.removeObject(testObj->getId());

    BOOST_TEST(sh.getAllObjects().empty());

    std::ifstream file{path / testObj->getId()};
    BOOST_TEST(!file.good());
}

/**
 * @test Sprawdza, czy removeObject nie robi nic jeśli podano identyfikator,
 *       który nie jest zapisany w plikach
 */
BOOST_AUTO_TEST_CASE(StorageHandlerRemoveObjectDoesNothingGivenNonExistingIdTest) {
    std::filesystem::path path = std::filesystem::current_path() / "katalog";
    if (exists(path))
        remove_all(path);

    StorageHandler sh{path};

    auto testObj = std::make_shared<TestObject>("id1", "data");
    sh.saveObject(testObj);

    sh.removeObject("id31");

    BOOST_TEST(sh.getAllObjects().size() == 1);
}

BOOST_AUTO_TEST_SUITE_END()