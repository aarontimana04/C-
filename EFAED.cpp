/******************************************************************************

Welcome to GDB Online.
  GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
  C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, COBOL, HTML, CSS, JS
  Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

class Solution {
public:
    int comunidadMasExtensa(vector<vector<int>>& mapa) {
        if (mapa.empty()) return 0;
        int m = mapa.size();
        int n = mapa[0].size();
        int maxArea = 0;

        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (mapa[i][j] == 1) {
                    int area = dfs(mapa, i, j, m, n);
                    maxArea = max(maxArea, area);
                }
            }
        }
        return maxArea;
    }

private:
    int dfs(vector<vector<int>>& mapa, int i, int j, int m, int n) {
        if (i < 0 || j < 0 || i >= m || j >= n || mapa[i][j] == 0)
            return 0;

        mapa[i][j] = 0; // Marcar como visitado
        int area = 1;
        area += dfs(mapa, i + 1, j, m, n);
        area += dfs(mapa, i - 1, j, m, n);
        area += dfs(mapa, i, j + 1, m, n);
        area += dfs(mapa, i, j - 1, m, n);
        return area;
    }
};

int main() {
    Solution sol;

    vector<vector<int>> grid1 = {
        {1, 0, 0},
        {1, 0, 0},
        {1, 1, 1}
    };
    assert(sol.comunidadMasExtensa(grid1) == 5);

    vector<vector<int>> grid2 = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };
    assert(sol.comunidadMasExtensa(grid2) == 1);

    vector<vector<int>> grid3 = {
        {1,1,0,0,0},
        {1,1,0,1,1},
        {0,0,0,1,1}
    };
    assert(sol.comunidadMasExtensa(grid3) == 4);

    vector<vector<int>> grid4 = {
        {1,1,1},
        {1,0,1},
        {1,1,1}
    };
    assert(sol.comunidadMasExtensa(grid4) == 8);

    vector<vector<int>> grid5 = {
        {1,1,0,0,0,1,1},
        {1,1,0,1,0,1,1},
        {0,0,0,1,0,0,0},
        {1,1,1,1,1,1,1}
    };
    assert(sol.comunidadMasExtensa(grid5) == 9);

    vector<vector<int>> grid6 = {
        {1,0,1,0,1,1,0,1,1,1}
    };
    assert(sol.comunidadMasExtensa(grid6) == 3);

    vector<vector<int>> grid7(10, vector<int>(10, 0));
    for (int i = 0; i < 10; ++i) {
        grid7[0][i] = grid7[9][i] = 1;
        grid7[i][0] = grid7[i][9] = 1;
    }
    assert(sol.comunidadMasExtensa(grid7) == 36);

    vector<vector<int>> grid8(5, vector<int>(5, 0));
    assert(sol.comunidadMasExtensa(grid8) == 0);

    vector<vector<int>> grid9 = {
        {1,1,1,1,1},
        {1,0,0,0,1},
        {1,0,1,0,1},
        {1,0,0,0,1},
        {1,1,1,1,1}
    };
    assert(sol.comunidadMasExtensa(grid9) == 16);

    vector<vector<int>> grid10 = {
        {0,0,0},
        {0,1,0},
        {0,0,0}
    };
    assert(sol.comunidadMasExtensa(grid10) == 1);

    cout << "Todos los tests pasaron correctamente" << endl;
    return 0;
}