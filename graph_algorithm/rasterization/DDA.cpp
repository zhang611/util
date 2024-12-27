#include <cmath>
#include <iostream>
using namespace std;
bool pic[50][50];
void DrawLine(int x0,int y0,int x1,int y1)
{
	double k = (y1-y0)*1.0/(x1-x0);
	double y = y0;
	for(auto x = x0 ; x <= x1 ; ++ x) {
		pic[(int)round(y)][x] = true;
		y += k;
	}
}
void ShowPic()
{
	for(auto i = 49 ; i >= 0 ; -- i) {
		for(auto j:pic[i]) {
			if(j) cout << 'x';
			else cout << ' ';
		}
		cout << endl;
	}
}
int main()
{
	DrawLine(2,2,48,30);
	ShowPic();
	return 0;
}
