#include <iostream>
#include <random>
#include <cmath>
using namespace std;

const float MY_PI = 3.1415926;

int main() {
    srand(time(0));
    float theta = ((float)rand() / RAND_MAX) * 2 * MY_PI;
    float r = sqrtf(((float)rand() / RAND_MAX));
    cout << r * cos(theta) << "  " << r * sin(theta) << endl;
}