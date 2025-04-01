# include <iostream>
# include <vector>
# include <algorithm>
using namespace std;


struct Point {
    double x, y, z;
    Point() {}
    Point(double x, double y, double z): x(x), y(y), z(z) {}
};

struct Triangle {
    Point a, b, c;
    Triangle() {}
    Triangle(Point a, Point b, Point c): a(a), b(b), c(c) {}
};  


bool isInersection(Triangle t1, Triangle t2) {
    vector<Point> p1 = {t1.a, t1.b, t1.c};
    vector<Point> p2 = {t2.a, t2.b, t2.c};

    for (int i = 0; i < 3; i++) {
        Point p = p1[i];
        for (int j = 0; j < 3; j++) {
            Point q = p2[j];
            if (p.x == q.x && p.y == q.y && p.z == q.z) return true;
        }
    }

    return false;
}



int main () {
    Point a(0, 0, 0), b(0, 0, 1), c(0, 1, 0);
    Point d(0, 0, 0), e(-0.5, 0.5, 0.5), f(0.5, 0.5, 0.5);
    Triangle t1(a, b, c), t2(d, e, f);

    cout << isInersection(t1, t2);
    return 0;

}