#include <iostream>
#include <cmath>
using namespace std;

const int N = 50;
bool pic[N][N];
void DrawALine(int x0, int y0, int x1, int y1)
{
	int dx = fabs(x0-x1);
	int dy = fabs(y0-y1);
	int y = y0;
	int e = -2*dx;
	for(auto x = x0 ; x <= x1 ; ++ x) {
		pic[y][x] = true;
		e += 2*dy;
		if(e > 0) ++y;
		if(e >= dx) e -= 2*dx;
	}
}

void ShowPic()
{
	for(auto i = 0; i < N; i++) {
		for(auto j : pic[i]) {
			if(j) cout << 'x';
			else cout << ' ';
		}
		cout << endl;
	}
}

int main()
{
	DrawALine(2,2,40,10);
	ShowPic();
	return 0;
}