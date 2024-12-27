#include <cmath>
#include <iostream>
using namespace std;

const int N = 50;
bool pic[N][N];
void DrawLine(int x0, int y0, int x1, int y1)
{
	double k = (y1 - y0) * 1.0 / (x1 - x0);
	double y = y0;
	for (auto x = x0; x <= x1; ++x)
	{
		pic[(int)round(y)][x] = true;
		y += k;
	}
}

void ShowPic()
{
	for (int x = 0; x < N; x++)
	{
		for (int y = 0; y < N; y++)
		{
			if (pic[x][y])
				cout << 'x';
			else
				cout << ' ';
		}
		cout << endl;
	}
}

int main()
{
	DrawLine(2, 2, 48, 30);
	ShowPic();
	return 0;
}
