# include <iostream>
# include <vector>
using namespace std;

int main() {

    vector<vector<int>> mytest(vector<vector<int>> gray, vector<vector<float>> kernel) {
        int n = gray.size();
        int m = gray[0];
        int pd = kernel.size();

        vector<vector<int>> res(n, vector<int>(m, 0));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                for (int ki = -pd; ki < pd; ki++) {
                    for (int kj = -pd; kj < pd; kj++) {
                        int x = i + ki;
                        res[i][j] += gray[i + ki][j + kj] * kernel[k][l];
                    }
                }
            }
        }
        
    }
}