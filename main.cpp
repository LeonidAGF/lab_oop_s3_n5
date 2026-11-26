#include "src/bst.hpp"
#include <iostream>

int main()
{
    BST tree;
    tree.insert(Book("1", "1", 2000, 100));
    tree.insert(Book("2", "2", 2005, 200));
    tree.insert(Book("3", "3", 2000, 300));

    std::cout << tree.size() << std::endl;


    Book key("", "", 2000, 0);

    std::weak_ptr<Book> result = tree.find(key);
    std::cout << result.lock().get()->getAuthor() << std::endl;

    AllSelector criterion;
    Selection res = tree.select(criterion);

	return 0;
}
