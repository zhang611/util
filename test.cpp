# include <iostream>
# include <random>
# include <vector>
# include <algorithm>
using namespace std;

vector<int> nums = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
vector<bool> st(nums.size(), false);
int res = 0;
int s = 5;
int sum = 0;

void dfs(int u) {
    if (sum > s) return;
    if (s == sum) {
        res++;
        return;
    }

    for (int i = u; i < nums.size(); i++) {
        if (st[i]) continue;
        st[i] = true;
        sum += nums[i];
        dfs(i + 1);
        sum -= nums[i];
        st[i] = false;
    }
}

int main () {
    dfs(0);
    cout << res;
}