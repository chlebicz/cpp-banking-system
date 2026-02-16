#include <fstream>
#include <boost/test/unit_test.hpp>

#include "StorageHandler.h"

BOOST_AUTO_TEST_SUITE(StorageHandlerTest)

/**
 * @test Checks if StorageHandler creates a directory for object files
 *       if one does not exist yet
 */
BOOST_AUTO_TEST_CASE(StorageHandlerConstructorCreatesDirIfDoesNotExist) {
    std::filesystem::path path = std::filesystem::current_path() / "directory";
    if (exists(path))
        remove_all(path);

    StorageHandler sh{path};
    BOOST_TEST(std::filesystem::is_directory(path));
    BOOST_TEST(sh.getAllObjects().empty());
}

/**
 * @test Checks if StorageHandler reads their state if a directory with object
 *       files exists
 */
BOOST_AUTO_TEST_CASE(StorageHandlerConstructorLoadsExistingDirContents) {
    std::filesystem::path path = std::filesystem::current_path() / "directory";
    if (exists(path))
        remove_all(path);

    create_directory(path);

    std::string id1 = "file1";
    json data1 = {{"a", 1}};
    std::ofstream file1{path / id1};
    BOOST_REQUIRE(file1.good());
    file1 << data1;
    file1.close();

    std::string id2 = "file2";
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
 * Class for tests, handled by StorageHandler
 */
class TestObject : public Entity {
public:
    /**
     * Constructor
     * @param id Identifier
     * @param data Object data
     */
    TestObject(std::string id, std::string data)
        : id(std::move(id)), data(std::move(data)) {}

    /**
     * @returns Object data
     */
    std::string toString() const override {
        return data;
    }

    /**
     * @returns Data saved in the object in JSON format
     */
    json toJSON() const override {
        return {
            {"id", id},
            {"data", data}
        };
    }

    /**
     * @returns Object identifier
     */
    std::string getId() const override {
        return id;
    }
private:
    std::string id, data;
};

/**
 * @test Checks correct saving of objects by StorageHandler
 */
BOOST_AUTO_TEST_CASE(StorageHandlerSaveObjectTest) {
    std::filesystem::path path = std::filesystem::current_path() / "directory";
    if (exists(path))
        remove_all(path);

    StorageHandler sh{path};

    auto testObj1 = std::make_shared<TestObject>("id1", "data");
    sh.saveObject(testObj1);

    auto testObj2 = std::make_shared<TestObject>("id2", "data");
    sh.saveObject(testObj2);

    // check if appropriate files for objects are created
    std::ifstream file1{path / testObj1->getId()};
    BOOST_TEST(file1.good());

    std::ifstream file2{path / testObj2->getId()};
    BOOST_TEST(file2.good());

    // check if object data is saved
    BOOST_TEST(sh.getAllObjects().size() == 2);
    BOOST_TEST(sh.getObjectData(testObj1->getId()) == testObj1->toJSON());
    BOOST_TEST(sh.getObjectData(testObj2->getId()) == testObj2->toJSON());
}

/**
 * @test Checks correct overwriting of existing object files content by
 *       StorageHandler
 */
BOOST_AUTO_TEST_CASE(StorageHandlerSaveObjectGivenExistingIdTest) {
    std::filesystem::path path = std::filesystem::current_path() / "directory";
    if (exists(path))
        remove_all(path);

    StorageHandler sh{path};

    std::string id = "IDENTIFIEEEER";
    auto testObj1 = std::make_shared<TestObject>(id, "data");
    sh.saveObject(testObj1);

    auto testObj2 = std::make_shared<TestObject>(id, "dayta");
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
 * @test Checks correct removal of existing objects by StorageHandler
 */
BOOST_AUTO_TEST_CASE(StorageHandlerRemoveObjectTest) {
    std::filesystem::path path = std::filesystem::current_path() / "directory";
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
 * @test Checks if removeObject does nothing if an identifier not saved
 *       in files is given
 */
BOOST_AUTO_TEST_CASE(StorageHandlerRemoveObjectDoesNothingGivenNonExistingIdTest) {
    std::filesystem::path path = std::filesystem::current_path() / "directory";
    if (exists(path))
        remove_all(path);

    StorageHandler sh{path};

    auto testObj = std::make_shared<TestObject>("id1", "data");
    sh.saveObject(testObj);

    sh.removeObject("id31");

    BOOST_TEST(sh.getAllObjects().size() == 1);
}

BOOST_AUTO_TEST_SUITE_END()
