#include <functional>
#include <iostream>
using namespace std;

class RandomIterator {
   public:
    RandomIterator(int* a) : p(a) {}
    RandomIterator(const RandomIterator& r) { p = r.p; }
    RandomIterator& operator=(const RandomIterator& r) {
        p = r.p;
        return *this;
    }
    int& operator*() const { return *p; }
    RandomIterator operator+(int n) const { return RandomIterator(p + n); }
    RandomIterator operator-(int n) const { return RandomIterator(p - n); }
    int operator-(RandomIterator& r) { return p - r.p; }
    RandomIterator& operator++() {
        ++p;
        return *this;
    }
    RandomIterator& operator--() {
        --p;
        return *this;
    }
    RandomIterator& operator++(int) {
        RandomIterator temp = *this;
        ++p;
        return temp;
    }
    RandomIterator& operator--(int) {
        RandomIterator temp = *this;
        --p;
        return temp;
    }
    bool operator<(const RandomIterator& r) const { return p < r.p; }
    bool operator>(const RandomIterator& r) const { return r < *this; }
    bool operator>=(const RandomIterator& r) const { return !(*this < r); }
    bool operator<=(const RandomIterator& r) const { return !(*this > r); }
    bool operator==(const RandomIterator& r) const {
        return !(*this > r) && !(*this < r);
    }
    bool operator!=(const RandomIterator& r) const { return !(*this == r); }

   private:
    int* p;
};

const int threthold = 16;
int get_mid(int a, int b, int c) {
    if (a > b) swap(a, b);
    if (a > c) swap(a, c);
    if (b > c) swap(b, c);
    return b;
}

void quick_sort(RandomIterator first, RandomIterator last,
                function<bool(int, int)> cmp = less<int>()) {
    while (last - first > threthold) {
        RandomIterator x = first, y = last - 1;
        int pivot = get_mid(*x, *y, *(first + (last - first) / 2));
        while (x <= y) {
            while (cmp(*x, pivot)) x++;
            while (cmp(pivot, *y)) y--;
            if (x <= y) {
                swap(*x, *y);
                x++, y--;
            }
        }
        quick_sort(x, last, cmp);
        last = y + 1;
    }
    return;
}

void insertion_sort(RandomIterator first, RandomIterator last,
                    function<bool(int, int)> cmp = less<int>()) {
    RandomIterator ind = first;
    for (RandomIterator j = first + 1; j < last; ++j) {
        if (cmp(*j, *ind)) ind = j;
    }
    for (RandomIterator j = ind; j > first; --j) {
        swap(*j, *(j - 1));
    }
    for (RandomIterator j = first + 1; j < last; ++j) {
        ind = j;
        while (cmp(*ind, *(ind - 1))) {
            swap(*ind, *(ind - 1));
            --ind;
        }
    }
    return;
}

void my_sort(RandomIterator first, RandomIterator last,
             function<bool(int, int)> cmp = less<int>()) {
    quick_sort(first, last, cmp);
    insertion_sort(first, last, cmp);
    return;
}

// 一定要用指针的引用，否则里面的修改，对外面无效
// 指针的本质是地址，开辟一个地址存储指向对象的地址，通过->来修改指向对象，如果一个指针指向空，不会影响另一个指针
// void mergeX(ListNode*& p, ListNode*& q) {
//     ListNode ret, *pr = &ret;
//     while (p && q) {
//         if (p->val < q->val) {
//             pr->next = p;
//             pr = pr->next;
//             p = p->next;
//         } else {
//             pr->next = q;
//             pr = pr->next;
//             q = q->next;
//         }
//     }
//     if (p) {
//         pr->next = p;
//     } else {
//         pr->next = q;
//     }
//     p = ret.next;
//     q = nullptr;
//     return;
// }

// ListNode* sortList(ListNode* head) {
//     if (head == nullptr || head->next == nullptr) return head;
//     ListNode *counter[64], *carry;
//     int fill = 0;
//     while (head) {
//         carry = head;
//         head = head->next;
//         carry->next = nullptr;
//         int i = 0;
//         for (; i < fill && counter[i]; i++) {
//             merge(carry, counter[i]);
//         }
//         swap(carry, counter[i]);
//         if (i == fill) {
//             fill++;
//         }
//     }
//     for (int i = 1; i < fill; i++) merge(counter[i], counter[i - 1]);
//     return counter[fill - 1];
// }