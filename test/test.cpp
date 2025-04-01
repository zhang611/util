# include <iostream>
# include <mutex>
# include <thread>
using namespace std;



class Singleton {
private:
    Singleton() {}
    Singleton(const Singleton&);
    Singleton& operator=(const Singleton&);
public:
    static Singleton* instance;
    static Singleton* getInstance() {
        if (instance == nullptr) {
            instance = new Singleton();
        }
        return instance;
    }
};
Singleton* Singleton::instance = nullptr;





int main() {
    thread t1(fun1, 1);
    thread t2(fun2, 1);
    t1.join();
    t2.join();
    cout << "sharenum = " << sharenum << endl;
    return 0;
}