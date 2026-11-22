#include <gtest/gtest.h>
#include "bst.hpp"

TEST(Test, t1) {
    BST tree;

    EXPECT_EQ(tree.size(), 0);

    EXPECT_EQ(tree.insert(Book("1", "1", 2000, 100)), true);
    EXPECT_EQ(tree.size(), 1);

    EXPECT_EQ(tree.insert(Book("2", "2", 2005, 200)), true);
    EXPECT_EQ(tree.size(), 2);

    EXPECT_EQ(tree.insert(Book("3", "3", 2000, 300)), false);
    EXPECT_EQ(tree.size(), 2);
}

TEST(Test, t2) {
    BST tree;

    tree.insert(Book("1", "2", 2000, 300));

    Book key("", "", 2000, 0);

    std::weak_ptr<Book> result = tree.find(key);

    EXPECT_EQ(result.expired(), false);
    EXPECT_EQ(result.lock()->getTitle(), "1");
    EXPECT_EQ(result.lock()->getAuthor(), "2");
    EXPECT_EQ(result.lock()->getPages(), 300);
}

TEST(Test, t3) {
    BST tree;

    tree.insert(Book("A", "1", 2000, 100));

    Book key("", "", 2010, 0);

    std::weak_ptr<Book> result = tree.find(key);

    EXPECT_EQ(result.expired(), true);
}


TEST(Test, t4) {
    BST tree;

    tree.insert(Book("A", "1", 2000, 100));

    Book key("", "", 2000, 0);

    EXPECT_EQ(tree.remove(key), true);
    EXPECT_EQ(tree.find(key).expired(), true);
}


TEST(Test, t5) {
    BST tree;

    EXPECT_EQ(tree.height(), 0);

    tree.insert(Book("A", "1", 2000, 100));

    EXPECT_EQ(tree.height(), 1);

    tree.insert(Book("B", "1", 2010, 100));
    tree.insert(Book("C", "1", 2005, 100));

    EXPECT_EQ(tree.height(), 3);
}


TEST(Test, t6) {
    BST tree;

    tree.insert(Book("A", "1", 2000, 100));
    tree.insert(Book("B", "1", 2010, 100));
    tree.insert(Book("C", "1", 2005, 100));
    tree.insert(Book("D", "1", 2020, 100));

    AllSelector criterion;
    Selection result = tree.select(criterion);

    EXPECT_EQ(result.getSize(), 4);
    EXPECT_EQ(result[0].lock()->getYear(), 2000);
    EXPECT_EQ(result[1].lock()->getYear(), 2005);
    EXPECT_EQ(result[2].lock()->getYear(), 2010);
    EXPECT_EQ(result[3].lock()->getYear(), 2020);

    EXPECT_EQ(result[0].lock()->getYear() < result[1].lock()->getYear(), true);
    EXPECT_EQ(result[1].lock()->getYear() < result[2].lock()->getYear(), true);
    EXPECT_EQ(result[2].lock()->getYear() < result[3].lock()->getYear(), true);
}


TEST(Test, t7) {

    BST tree;

    tree.insert(Book("A", "1", 2000, 100));
    tree.insert(Book("B", "1", 2010, 100));
    tree.insert(Book("C", "1", 2005, 100));

    Book key("", "", 2005, 0);

    EXPECT_EQ(tree.remove(key), true);
    EXPECT_EQ(tree.size(), 2);

    AllSelector criterion;
    Selection result = tree.select(criterion);

    EXPECT_EQ(result[0].lock()->getYear(), 2000);
    EXPECT_EQ(result[1].lock()->getYear(), 2010);

    BST tree2;

    tree2.insert(Book("A", "1", 2000, 100));
    tree2.insert(Book("B", "1", 2010, 100));

    Book key2("", "", 2000, 0);

    EXPECT_EQ(tree2.remove(key2), true);
    EXPECT_EQ(tree2.size(), 1);

    result = tree2.select(criterion);

    EXPECT_EQ(result[0].lock()->getYear(), 2010);
    
    BST tree3;

    tree3.insert(Book("A", "1", 2000, 100));
    tree3.insert(Book("B", "1", 2010, 100));
    tree3.insert(Book("C", "1", 1990, 100));
    tree3.insert(Book("D", "1", 2005, 100));
    tree3.insert(Book("E", "1", 2020, 100));

    Book key3("", "", 2000, 0);

    EXPECT_EQ(tree3.remove(key3), true);
    EXPECT_EQ(tree3.size(), 4);

    result = tree.select(criterion);

    EXPECT_EQ(result[0].lock()->getYear(), 1990);
    EXPECT_EQ(result[1].lock()->getYear(), 2005);
    EXPECT_EQ(result[2].lock()->getYear(), 2010);
    EXPECT_EQ(result[3].lock()->getYear(), 2020);
}


TEST(Test, t8) {
    BST tree;

    tree.insert(Book("A", "1", 2000, 100));
    tree.insert(Book("B", "1", 2010, 100));

    Book key("", "", 2005, 0);

    EXPECT_EQ(tree.remove(key), false);
    EXPECT_EQ(tree.size(), 2);
}

TEST(Test, t9) {
    BST tree;

    tree.insert(Book("A", "1", 2000, 100));
    tree.insert(Book("B", "1", 2005, 300));
    tree.insert(Book("C", "1", 2010, 500));
    tree.insert(Book("D", "1", 2015, 250));

    BookSelector criterion(2000, 2015, 250);

    Selection result = tree.select(criterion);

    EXPECT_EQ(result.getSize(), 2);
    EXPECT_EQ(result[0].lock()->getYear(), 2005);
    EXPECT_EQ(result[1].lock()->getYear(), 2010);
    EXPECT_EQ(result[0].lock()->getPages(), 300);
    EXPECT_EQ(result[1].lock()->getPages(), 500);
}


TEST(Test, t10) {
    BST tree;

    tree.insert(Book("A", "1", 2000, 100));
    tree.insert(Book("B", "1", 2010, 300));

    BookSelector criterion(2100, 2200, 500);

    Selection result = tree.select(criterion);

    EXPECT_EQ(result.empty(), true);
    EXPECT_EQ(result.getSize(), 0);
}


TEST(Test, t11) {
    BST tree;

    tree.insert(Book("A", "1", 2000, 100));
    tree.insert(Book("B", "1", 2005, 300));
    tree.insert(Book("C", "1", 2010, 500));

    BookSelector criterion1(2000, 2010, 0);
    AllSelector criterion2;

    Selection result1 = tree.select(criterion1);
    Selection result2 = tree.select(criterion2);

    EXPECT_EQ(result1.getSize(), 2);
    EXPECT_EQ(result2.getSize(), 3);

    EXPECT_EQ(result1[0].lock()->getYear(), 2000);
    EXPECT_EQ(result1[1].lock()->getYear(), 2005);
}


TEST(Test, t12) {
    BST tree;

    tree.insert(Book("A", "1", 2000, 100));
    tree.insert(Book("B", "1", 2010, 200));

    AllSelector criterion;

    Selection first = tree.select(criterion);
    Selection second = first;

    EXPECT_EQ(first.getSize(), 2);
    EXPECT_EQ(second.getSize(), 2);

    Book key("", "", 2000, 0);

    EXPECT_EQ(tree.remove(key), true);

    EXPECT_EQ(first.getSize(), 2);
    EXPECT_EQ(second.getSize(), 2);

    EXPECT_EQ(first[0].expired(), true);
    EXPECT_EQ(second[0].expired(), true);

    EXPECT_EQ(first[1].expired(), false);
    EXPECT_EQ(second[1].expired(), false);
}

TEST(Test, t13) {

    BST tree;

    tree.insert(Book("A", "1", 2000, 100));
    tree.insert(Book("B", "1", 2010, 200));

    EXPECT_EQ(Book::getCount(), 2);

    AllSelector criterion;

    Selection first = tree.select(criterion);
    Selection second = std::move(first);

    EXPECT_EQ(first.empty(), true);
    EXPECT_EQ(second.getSize(), 2);
    EXPECT_EQ(second[0].lock()->getYear(), 2000);
    EXPECT_EQ(second[1].lock()->getYear(), 2010);

    Book key("", "", 2000, 0);

    EXPECT_EQ(tree.remove(key), true);
    EXPECT_EQ(Book::getCount(), 1);

    EXPECT_EQ(Book::getCount(), 0);
}