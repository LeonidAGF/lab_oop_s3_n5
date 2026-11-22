#include <bst.hpp>

int Book::count = 0;

Book::Book(std::string t, std::string a, int y, int p) :title(t), author(a), year(y), pages(p) {
    count++;
}
Book::~Book() {
    count--;
}

Book::Book(const Book& other) :title(other.title), author(other.author), year(other.year), pages(other.pages) {
    ++count;
}

Book::Book(Book&& other) : title(std::move(other.title)), author(std::move(other.author)), year(other.year), pages(other.pages) {
    ++count;
}

const std::string& Book::getTitle() const {
    return title;
}
const std::string& Book::getAuthor() const {
    return author;
}
int Book::getYear() const {
    return year;
}
int Book::getPages() const {
    return pages;
}

bool Book::operator<(const Book& other) const {
    return year < other.year;
}

bool Book::operator==(const Book& other) const {
    return year == other.year;
}

int Book::getCount() {
    return count;
}

BST::BST() {

}

bool BST::insert(const Book& book){
    
}
std::weak_ptr<Book> BST::find(const Book& key) const{

}
bool BST::remove(const Book& key){

}
Selection BST::select(const Criterion& c) const{
}

std::size_t BST::size() const{

}
int BST::height() const{

}