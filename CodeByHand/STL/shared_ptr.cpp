#include <iostream>
#include <atomic>
using namespace std;

template <typename T>
class ptr_data {
   public:
    ptr_data() : ptr(nullptr), cnt(0) {}
    ptr_data(T* p) : ptr(p), cnt(1) {}
    ptr_data(nullptr_t) : ptr(nullptr), cnt(0) {cout << "nullptr" << endl;}

    void increase_one() { this->cnt++; }
    void decrease_one() {
        this->cnt--;
        if (cnt == 0) {
            delete ptr;
        }
    }

    T* ptr;
    std::atomic<int> cnt;
};

template <typename T>
class my_shared_ptr {
   public:
    my_shared_ptr() : p_data(new ptr_data<T>()) {}
    my_shared_ptr(T* ptr) : p_data(new ptr_data<T>(ptr)) {}
    my_shared_ptr(nullptr_t) : p_data(new ptr_data<T>(nullptr)) {}
    my_shared_ptr(const my_shared_ptr<T>& p) : p_data(p.p_data) {
        this->p_data->increase_one();
    }
    my_shared_ptr& operator=(const my_shared_ptr<T>& p) {
        if (p_data == p.p_data) return *this;
        this->p_data->decrease_one();
        this->p_data = p.p_data;
        p_data->increase_one();
        return *this;
    }

    T& operator*() { return *p_data->ptr; }
    T* operator->() { return p_data->ptr; }

    int use_count() const { return p_data->cnt.load(); }
    ~my_shared_ptr() { p_data->decrease_one(); }

   private:
    ptr_data<T>* p_data;
};


// 完美转发 (std::forward<Args>(args)...) 传递可变参数，避免不必要的拷贝
template <class T, class... Args>
inline my_shared_ptr<T> my_make_shared(Args&&... args) {
    return my_shared_ptr<T>(new T(std::forward<Args>(args)...));
}

class A {
   public:
    A(int a, int b, int c) : a(a), b(b), c(c) {}
    void func() { cout << a << " " << b << " " << c << endl; }
    ~A() { cout << "dptr" << endl; }
    int a, b, c;
};

int main() {
    my_shared_ptr<A> p = my_make_shared<A>(1, 2, 3);
    cout << p.use_count() << endl;
    my_shared_ptr<A> p2 = p;
    cout << p.use_count() << endl;
    p2->func();
    p2 = nullptr;  // 会调用 ptr_data(nullptr_t)
    cout << p.use_count() << endl;
    cout << p2.use_count() << endl;
}