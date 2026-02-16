#include <boost/test/unit_test.hpp>

#include "Repository.h"

BOOST_AUTO_TEST_SUITE(RepositoryTest)

class TestObject {
public:
    /**
     * Constructor
     * @param test Test parameter
     */
    TestObject(int test)
        : test(test) {}

    /**
     * @returns Test parameter
     */
    int getTest() const {
        return test;
    }

    /**
     * Sets new value of test parameter
     * @param value New value
     */
    void setTest(int value) {
        test = value;
    }
private:
    int test;
};

BOOST_AUTO_TEST_CASE(RepositoryConstructorTest) {
    Repository<std::shared_ptr<TestObject>> repo;
    BOOST_TEST(repo.size() == 0);
    BOOST_TEST(repo.getAll().empty());
}

BOOST_AUTO_TEST_CASE(GetReturnsNullptrGivenOutOfBoundIndexTest) {
    Repository<std::shared_ptr<TestObject>> repo;
    BOOST_TEST(repo.get(1) == nullptr);
}

BOOST_AUTO_TEST_CASE(RepoAddTest) {
    Repository<std::shared_ptr<TestObject>> repo;
    int test = 420;
    repo.add(std::make_shared<TestObject>(test));

    BOOST_TEST(repo.get(0)->getTest() == test);
}

BOOST_AUTO_TEST_CASE(RepoRemoveTest) {
    Repository<std::shared_ptr<TestObject>> repo;
    auto element = std::make_shared<TestObject>(2);
    repo.add(element);

    repo.remove(element);

    BOOST_TEST(repo.size() == 0);
}

BOOST_AUTO_TEST_CASE(RepoFindFirstTest) {
    Repository<std::shared_ptr<TestObject>> repo;
    repo.add(std::make_shared<TestObject>(2));
    repo.add(std::make_shared<TestObject>(4));
    repo.add(std::make_shared<TestObject>(5));
    repo.add(std::make_shared<TestObject>(6));

    auto found = repo.findFirst([](const auto& element) {
        return element->getTest() > 3;
    });
    BOOST_TEST(found->getTest() == 4);
}

BOOST_AUTO_TEST_CASE(FindAllTest) {
    Repository<std::shared_ptr<TestObject>> repo;
    repo.add(std::make_shared<TestObject>(2));
    repo.add(std::make_shared<TestObject>(4));
    repo.add(std::make_shared<TestObject>(5));
    repo.add(std::make_shared<TestObject>(6));

    auto found = repo.findAll([](const auto& element) {
        return element->getTest() > 3;
    });
    BOOST_TEST(found.size() == 3);
}

BOOST_AUTO_TEST_CASE(RepoRemoveAllTest) {
    Repository<std::shared_ptr<TestObject>> repo;
    repo.add(std::make_shared<TestObject>(2));
    repo.add(std::make_shared<TestObject>(4));
    repo.add(std::make_shared<TestObject>(5));
    repo.add(std::make_shared<TestObject>(6));

    repo.removeAll();
    BOOST_TEST(repo.size() == 0);
}

BOOST_AUTO_TEST_SUITE_END()
