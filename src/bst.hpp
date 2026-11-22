#pragma once

#include <memory>
#include <string>

class Book final{
private:
    std::string title;
    std::string author;
    int year;
    int pages;
    static int count;
public:
    Book(std::string t, std::string a, int y, int p);
    ~Book();
    Book(const Book& other);
    Book(Book&& other);
    const std::string& getTitle() const;
    const std::string& getAuthor() const;
    int getYear() const;
    int getPages() const;
    bool operator<(const Book& other) const;
    bool operator==(const Book& other) const;
    static int getCount();
};

class Criterion {
public:
    virtual ~Criterion() = default;
    virtual bool matches(const Book& book) const = 0;
};

class Selection final {
private:
    std::unique_ptr<std::weak_ptr<Book>[]> data;
    std::size_t size;
    std::size_t capacity;

public:
    Selection();
    Selection(const Selection& other);
    Selection& operator=(const Selection& other);
    Selection(Selection&& other);
    Selection& operator=(Selection&& other);
    void add(const std::weak_ptr<Book>& book);
    size_t getSize() const;
    bool empty() const;
    std::weak_ptr<Book> operator[](size_t index) const;
};

class Node final {
private:
    std::shared_ptr<Book> data;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
public:
    explicit Node(std::shared_ptr<Book> data);
    friend class BST;
};

class BST final {
private:
    std::unique_ptr<Node> root;
    int count = 0;
    std::unique_ptr<Node> createNode(const Book& book);
    int height(Node* n) const;
    void selectDfs(const Criterion& c, const Node* v, Selection& res) const;
public:
    BST(const BST&) = delete;
    BST& operator=(const BST&) = delete;
    BST(BST&& other);
    BST& operator=(BST&& other);
    bool insert(const Book& book);
    std::weak_ptr<Book> find(const Book& key) const;
    bool remove(const Book& key);
    Selection select(const Criterion& c) const;

    std::size_t size() const;
    int height() const;
};

class BookSelector final : public Criterion {
private:
    int l;
    int h;
    int minPagesNum;
public:
    BookSelector(int lo, int hi, int minPages): l(lo), h(hi), minPagesNum(minPages) {}
    bool matches(const Book& book) const override {
        return book.getYear() >= l && book.getYear() < h &&book.getPages() >= minPagesNum;
    }
};

class MinPagesSelector final : public Criterion {
private:
    int minPagesNum;
public:
    explicit MinPagesSelector(int minPages): minPagesNum(minPages) {
    }

    bool matches(const Book& book) const override {
        return book.getPages() >= minPagesNum;
    }
};