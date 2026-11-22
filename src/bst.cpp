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

Node::Node(std::shared_ptr<Book> data): data(std::move(data)) {
}

std::unique_ptr<Node> BST::createNode(const Book& book) {
    return std::make_unique<Node>(std::make_shared<Book>(book));
}

bool BST::insert(const Book& book) {
    if (!root) {
        root = createNode(book);
        count++;
        return true;
    }

    Node* aim = root.get();
    Node* parent = nullptr;

    while (aim) {
        parent = aim;

        if (book < *aim->data) {
            aim = aim->left.get();
        }
        else if (*aim->data < book) {
            aim = aim->right.get();
        }
        else {
            return false;
        }
    }

    auto nn = createNode(book);
    if (book < *parent->data) {
        parent->left = std::move(nn);
    }
    else {
        parent->right = std::move(nn);
    }

    count++;
    return true;
}

std::weak_ptr<Book> BST::find(const Book& key) const{

    if (!root) {
        return std::weak_ptr<Book>();
    }

    Node* aim = root.get();
    Node* parent = nullptr;

    while (aim) {
        parent = aim;

        if (key < *aim->data) {
            aim = aim->left.get();
        }
        else if (*aim->data < key) {
            aim = aim->right.get();
        }
        else {
            return std::weak_ptr<Book>(aim->data);
        }
    }
    return std::weak_ptr<Book>();
}
bool BST::remove(const Book& key) {
    std::unique_ptr<Node>* aim = &root;

    while (*aim) {
        if (key < *(*aim)->data) {
            aim = &(*aim)->left;
        }
        else if (*(*aim)->data < key) {
            aim = &(*aim)->right;
        }
        else {
            break;
        }
    }

    if (!*aim) {
        return false;
    }

    if (!(*aim)->left && !(*aim)->right) {
        aim->reset();
        --count;
        return true;
    }

    if ((*aim)->left && !(*aim)->right) {
        *aim = std::move((*aim)->left);
        --count;
        return true;
    }

    if (!(*aim)->left && (*aim)->right) {
        *aim = std::move((*aim)->right);
        --count;
        return true;
    }

 
    std::unique_ptr<Node>* zam = &(*aim)->right;

    while ((*zam)->left) {
        zam = &(*zam)->left;
    }
    (*aim)->data = (*zam)->data;
    *zam = std::move((*zam)->right);

    --count;
    return true;
}

void BST::selectDfs(const Criterion& c, const Node* v, Selection& res) const{
    if (!v) {
        return;
    }

    selectDfs(c,v->left.get(),res);

    if (c.matches(*v->data)) {
        res.add(v->data);
    }

    selectDfs(c,v->right.get(),res);
}

Selection BST::select(const Criterion& c) const {
    Selection res;
    selectDfs(c,root.get(),res);
    return res;
}

std::size_t BST::size() const {
    return count;
}

int BST::height(Node* n) const {
    if (!n)
        return 0;
    return std::max(height(n->left.get()), height(n->right.get())) + 1;
}

int BST::height() const{
    return height(root.get());
}

BST::BST(BST&& o): root(std::move(o.root)),count(o.count) {
    o.count = 0;
}

BST& BST::operator=(BST&& o)  {
    if (this != &o) {
        root = std::move(o.root);
        count = o.count;

        o.count = 0;
    }

    return *this;
}


Selection::Selection(): data(nullptr), size(0), capacity(0) {
}


Selection::Selection(const Selection& other): data(nullptr),size(other.size),capacity(other.capacity) {
    if (capacity > 0) {
        data = std::make_unique<std::weak_ptr<Book>[]>(capacity);
        for (std::size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }
}

Selection& Selection::operator=(const Selection& other) {
    if (this == &other) {
        return *this;
    }

    std::unique_ptr<std::weak_ptr<Book>[]> newData;

    if (other.capacity > 0) {
        newData = std::make_unique<std::weak_ptr<Book>[]>(other.capacity);
        for (std::size_t i = 0; i < other.size; ++i) {
            newData[i] = other.data[i];
        }
    }

    data = std::move(newData);
    size = other.size;
    capacity = other.capacity;

    return *this;
}

Selection::Selection(Selection&& other): data(std::move(other.data)),size(other.size),capacity(other.capacity) {

    other.size = 0;
    other.capacity = 0;
}

Selection& Selection::operator=(Selection&& other) {
    if (this == &other) {
        return *this;
    }

    data = std::move(other.data);
    size = other.size;
    capacity = other.capacity;

    other.size = 0;
    other.capacity = 0;

    return *this;
}

void Selection::add(const std::weak_ptr<Book>& book) {
    if (size == capacity) {
        std::size_t newcap = (capacity == 0) ? 1 : capacity * 2;

        auto newData = std::make_unique<std::weak_ptr<Book>[]>(newcap);

        for (std::size_t i = 0; i < size; ++i) {
            newData[i] = data[i];
        }

        data = std::move(newData);
        capacity = newcap;
    }

    data[size] = book;
    ++size;
}

std::size_t Selection::getSize() const {
    return size;
}
bool Selection::empty() const {
    return size == 0;
}

std::weak_ptr<Book> Selection::operator[](std::size_t index) const {
    if (index >= size) {
        return std::weak_ptr<Book>();
    }
    return data[index];
}