#include <functional>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Node {
   public:
    Node() = default;
    Node(string, int, Node*);
    string get_key();
    int& get_value();  // 返回引用，确保[]运算符 可以修改值 h[2] = 1;
    Node* next();
    void set_next(Node*);  // 私有变量 使用这个设置next
    void insert_next(Node*);
    void erase_next();

   private:
    string __key;
    int __value;
    Node* __next;
};

class Hash_Table {
   public:
    typedef function<int(string)> HASH_FUNC_T;
    Hash_Table(int, HASH_FUNC_T);
    ~Hash_Table();
    bool insert(string, int);  // 调用私有的__insert， 控制扩容函数
    bool find(string);         // 调用私有的__find
    bool erase(string);
    int& operator[](string);  // 调用__find,没找到就先插入__insert(key, 0)，
                              // 然后返回__find(key)->get_value()
    int capacity();
    void swap(Hash_Table&);

   private:
    int data_cnt, size;
    vector<Node> data;
    HASH_FUNC_T hash_func;
    void __expand();
    Node* __insert(string, int);
    Node* __find(string);
};

Node::Node(string key, int value = 0, Node* node = nullptr)
    : __key(key), __value(value), __next(node) {}

string Node::get_key() { return __key; }
int& Node::get_value() { return __value; }
Node* Node::next() { return __next; }

void Node::set_next(Node* node) {
    this->__next = node;
    return;
}
void Node::insert_next(Node* node) {
    node->set_next(this->next());
    this->set_next(node);
    return;
}
void Node::erase_next() {
    Node* temp = this->next();
    if (temp == nullptr) return;
    this->set_next(temp->next());
    delete temp;
    return;
}

Hash_Table::Hash_Table(int n, HASH_FUNC_T hash_func)
    : data_cnt(0), size(n), data(n), hash_func(hash_func) {}

Hash_Table::~Hash_Table() {
    for (int i = 0; i < size; i++) {
        Node *p = &data[i], *temp;
        while (p->next()) {
            temp = p->next();
            p->set_next(temp->next());
            delete temp;
        }
    }
}
bool Hash_Table::insert(string key, int value = 0) {
    if (data_cnt == 2 * size) __expand();
    return __insert(key, value);
}
bool Hash_Table::find(string key) { return __find(key); }
bool Hash_Table::erase(string key) {
    int ind = hash_func(key) % size;
    Node* p = &data[ind];
    while (p->next() && p->next()->get_key() != key) p = p->next();
    if (p->next() == nullptr) return false;
    p->erase_next();
    data_cnt -= 1;
    return true;
}
int& Hash_Table::operator[](string key) {
    if (!__find(key)) insert(key, 0);
    return __find(key)->get_value();
}
int Hash_Table::capacity() { return data_cnt; }
void swap(Hash_Table&);

Node* Hash_Table::__insert(string key, int value) {
    if (__find(key)) return nullptr;
    data_cnt += 1;
    int ind = hash_func(key) % size;
    Node* p = &data[ind];
    Node* node = new Node(key, value);
    p->insert_next(node);
    return node;
}
Node* Hash_Table::__find(string key) {
    int ind = hash_func(key) % size;
    Node* p = data[ind].next();
    while (p && p->get_key() != key) p = p->next();
    return p;
}

void Hash_Table::swap(Hash_Table& h) {
    std::swap(this->data_cnt, h.data_cnt);
    std::swap(this->size, h.size);
    std::swap(this->data, h.data);
    std::swap(this->hash_func, h.hash_func);
}

void Hash_Table::__expand() {
    Hash_Table h(2 * size, hash_func);
    for (int i = 0; i < size; i++) {
        Node* p = data[i].next();
        while (p) {
            h.insert(p->get_key(), p->get_value());
            p = p->next();
        }
    }
    this->swap(h);
}