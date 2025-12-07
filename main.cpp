//
//
//

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Directions for DFS (students must use these)
int dr[4] = {-1, 0, 1, 0};
int dc[4] = {0, 1, 0, -1};

// ----------------------------------------------------------
// DO NOT MODIFY: Maze generation
// ----------------------------------------------------------
void generateMaze(vector<vector<int>>& maze, int N, int M) {
    srand(time(0));

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < M; c++) {
            int roll = rand() % 100;
            maze[r][c] = (roll < 70) ? 0 : 1;   // 0 = open, 1 = wall
        }
    }
}

// ----------------------------------------------------------
// DO NOT MODIFY: Choose a random boundary cell that is open
// ----------------------------------------------------------
pair<int,int> chooseBoundaryCell(const vector<vector<int>>& maze) {
    int N = maze.size();
    int M = maze[0].size();

    while (true) {
        int side = rand() % 4;
        int r, c;

        if (side == 0) {          // top row
            r = 0;
            c = rand() % M;
        } else if (side == 1) {   // bottom row
            r = N - 1;
            c = rand() % M;
        } else if (side == 2) {   // left column
            r = rand() % N;
            c = 0;
        } else {                  // right column
            r = rand() % N;
            c = M - 1;
        }

        if (maze[r][c] == 0) {
            return {r, c};
        }
    }
}

// ----------------------------------------------------------
// DO NOT MODIFY: Print the maze with S and E markers
// ----------------------------------------------------------
void printMaze(const vector<vector<int>>& maze,
               int ent_r, int ent_c,
               int exit_r, int exit_c)
{
    int N = maze.size();
    int M = maze[0].size();

    cout << "\nMaze:\n";
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < M; c++) {
            if (r == ent_r && c == ent_c) {
                cout << "S ";
            } else if (r == exit_r && c == exit_c) {
                cout << "E ";
            } else {
                cout << maze[r][c] << " ";
            }
        }
        cout << "\n";
    }
}

// ----------------------------------------------------------
// DO NOT MODIFY: Print the reconstructed path
// Students must fill parent[][] correctly during DFS
// ----------------------------------------------------------
void printPath(pair<int,int> exitcell,
               const vector<vector<int>>& parent_r,
               const vector<vector<int>>& parent_c,
               int ent_r, int ent_c)
{
    int r = exitcell.first;
    int c = exitcell.second;

    vector<pair<int,int>> path;

    // Walk backward from exit to entrance
    while (!(r == ent_r && c == ent_c)) {
        path.push_back({r, c});
        int pr = parent_r[r][c];
        int pc = parent_c[r][c];
        r = pr;
        c = pc;
    }
    path.push_back({ent_r, ent_c});

    cout << "\nPath from entrance to exit:\n";
    for (int i = (int)path.size() - 1; i >= 0; i--) {
        cout << "(" << path[i].first << ", " << path[i].second << ")\n";
    }
}

// ----------------------------------------------------------
// STUDENTS IMPLEMENT DFS HERE
// ----------------------------------------------------------
bool dfs(int r, int c,
         int exit_r, int exit_c,
         const vector<vector<int>>& maze,
         vector<vector<bool>>& visited,
         vector<vector<int>>& parent_r,
         vector<vector<int>>& parent_c)
{
    visited[r][c] = true;

    // Base case: reached exit
    if (r == exit_r && c == exit_c) {
        return true;
    }

    int N = maze.size();
    int M = maze[0].size();

    // Attempting all 4 directions
    for (int d = 0; d < 4; d++) {
        int nr = r + dr[d];
        int nc = c + dc[d];

        // Checking bounds
        if (nr < 0 || nr >= N || nc < 0 || nc >= M)
            continue;

        // Skipping walls
        if (maze[nr][nc] == 1)
            continue;

        // Skipping visited cells
        if (visited[nr][nc])
            continue;

        // Setting parent BEFORE recursing
        parent_r[nr][nc] = r;
        parent_c[nr][nc] = c;

        // Recurse
        if (dfs(nr, nc, exit_r, exit_c, maze, visited, parent_r, parent_c)) {
            return true;    // A path is available
        }
    }

    // No path can be found from this branch
    return false;
}

// ----------------------------------------------------------
// MAIN PROGRAM
// ----------------------------------------------------------
int main() {
    int N, M;

    cout << "Enter maze dimensions N M, follow format when typing: ";
    cin >> N >> M;

    vector<vector<int>> maze(N, vector<int>(M));
    generateMaze(maze, N, M);

    // choose entrance, (S) and exit,  (E) on boundary
    pair<int,int> entrance = chooseBoundaryCell(maze);
    pair<int,int> exitcell = chooseBoundaryCell(maze);

    int start_r = entrance.first;
    int start_c = entrance.second;
    int exit_r  = exitcell.first;
    int exit_c  = exitcell.second;

    // visited & parent arrays
    vector<vector<bool>> visited(N, vector<bool>(M, false));
    vector<vector<int>> parent_r(N, vector<int>(M, -1));
    vector<vector<int>> parent_c(N, vector<int>(M, -1));

    // print the maze
    printMaze(maze, start_r, start_c, exit_r, exit_c);

    // DFS
    bool found = dfs(start_r, start_c,
                     exit_r, exit_c,
                     maze, visited, parent_r, parent_c);

    if (found) {
        printPath(exitcell, parent_r, parent_c, start_r, start_c);
    } else {
        cout << "Maze cannot be completed" << endl;
    }

    return 0;
}
