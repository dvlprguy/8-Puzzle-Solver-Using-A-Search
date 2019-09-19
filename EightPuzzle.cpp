#include <bits/stdc++.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>
#include <stack>
#include <map>
#include <unordered_map>
#include <climits>
#include <string>
#include <sstream>
#include <set>

using namespace std;

#define vii vector<int>
#define pii pair<int, int>

/* 
 * To check if moving blank to that point is safe or not
 */
#define IsSafe(x, y) (x >= 0 & x < 3 & y >= 0 & y < 3)

/*
    Final State
 */
vector<vii> goal_board(3, vii(3, 0));

/* 
    Solver Class for ease of calling different functions and using different attributes
*/
class Solver
{
public:
    Solver()
    {
        game_board = vector<vii>(3, vii(3));
        blank_pos = pii(-1, -1);
        tile_cost = 0;
        total_cost = 0;
        heuristic_kind = 0;
        parent_tile = NULL;
        // hash_board();
    }

    //current game board
    vector<vii> game_board;
    //current position of blank space
    pii blank_pos;
    //current cost of reaching this state ie g(n)
    int tile_cost;
    //total cost of reaching this state ie f(n)=g(n)+h(n)
    int total_cost;
    //kind of heuristic
    int heuristic_kind;
    //hash map for all tiles in this state
    pii game_hash[9];
    //hash map for goal state
    pii goal_hash[9];
    //Pointer to parent tile, Not used much
    Solver *parent_tile;

    //To search for blank space
    void SpaceSearch();
    //print current game_board
    void print_board();
    //function to hash the board
    void hash_board();
    //to get the heuristic value for current state
    int getHeuristic();
    //manhattan heuristic
    int manhattan();
    //tile mismatch heuristic
    int Tile_Mismatch();
    //Euclidian distance heuristic
    int EuclidianDistance();
    //to check if goal has been reached or not
    bool IsGoal();
    //Not using this function due to memory constraints
    void DisplayPath();
    //Compares if a state had been reached previously
    bool CompareSolver(Solver *S);
};

void Solver::SpaceSearch()
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (game_board[i][j] == 0)
                blank_pos = {i, j};
}

void Solver::print_board()
{
    for (int i = 0; i < game_board.size(); i++)
    {
        for (int j = 0; j < game_board[0].size(); j++)
        {
            cout << game_board[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

void Solver::hash_board()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            game_hash[game_board[i][j]] = make_pair(i, j);
            goal_hash[goal_board[i][j]] = make_pair(i, j);
        }
    }
}

int Solver::getHeuristic()
{
    if (heuristic_kind == 1)
    {
        return manhattan();
    }
    else if (heuristic_kind == 2)
    {
        return Tile_Mismatch();
    }
    else if (heuristic_kind == 3)
    {
        return EuclidianDistance();
    }
}

int Solver::manhattan()
{
    for (int i = 0; i < 9; i++)
    {
        total_cost += abs(game_hash[i].first - goal_hash[i].first);
        total_cost += abs(game_hash[i].second - goal_hash[i].second);
    }
    return total_cost;
}

int Solver::Tile_Mismatch()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (game_board[i][j] != goal_board[i][j])
            {
                total_cost += (i + j);
            }
        }
    }
    return total_cost;
}

int Solver::EuclidianDistance()
{
    for (int i = 0; i < 9; i++)
    {
        total_cost += sqrt((pow(game_hash[i].first - goal_hash[i].first, 2) + pow(game_hash[i].second - goal_hash[i].second, 2)));
    }
    return total_cost;
}

bool Solver::IsGoal()
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (game_board[i][j] != goal_board[i][j])
                return false;
    return true;
}

void Solver::DisplayPath()
{
    if (parent_tile != NULL)
        parent_tile->DisplayPath();
    print_board();
}

bool Solver::CompareSolver(Solver *S)
{
    if (S == NULL)
        return false;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (game_board[i][j] != S->game_board[i][j])
                return false;
    return true;
}

/*Normal Expansion function for a state.
 *Moves blank space only in 4 directions
 *namely left right up down
 *returns result in queue
 */
queue<Solver> NormalExpand(Solver S);
/*Modified Expansion function
 *Takes depth as input
 */
queue<Solver> LimitedDepthBFS(Solver S, int depth);
/*Modified A* funtion that calls LimitedDepthBFS*/
void ModifiedAStar(Solver ini, int depth);
/*
    To generate random testcases
    */
void GeneratePermutes(int in[]);
// to check if testcase is solvable or not
int getInvCount(int arr[]);
bool isSolvable(int puzzle[3][3]);

//operator for priority queue comparison as object is passed
bool operator<(const Solver &a, const Solver &b)
{
    return (a.tile_cost > b.tile_cost);
}

void ModifiedAStar(Solver ini, int depth = 1)
{
    int NodeCount = 0;
    int MaxLength = 0;
    map<vector<vii>, bool> SolverVisited;
    priority_queue<Solver, vector<Solver>> SolverFrontier;
    SolverFrontier.push(ini);
    Solver S;
    int x = 0;
    while (!SolverFrontier.empty())
    {
        S = (SolverFrontier.top());
        SolverFrontier.pop();
        if (S.IsGoal())
        {
            break;
        }
        SolverVisited[S.game_board] = true;
        MaxLength = max(MaxLength, (int)SolverFrontier.size());
        queue<Solver> SolverQueue = LimitedDepthBFS(S, depth);
        while (!SolverQueue.empty())
        {
            Solver ChildSolver;
            ChildSolver = (SolverQueue.front());
            SolverQueue.pop();
            if (!SolverVisited[ChildSolver.game_board])
            {
                SolverFrontier.push(ChildSolver);
                NodeCount++;
            }
        }
    }
    S.print_board();
    cout << "Number of Node: " << NodeCount << endl;
    cout << "Max Length of SolverFrontier: " << MaxLength << endl;
}

queue<Solver> LimitedDepthBFS(Solver S, int depth)
{
    queue<Solver> SolverQueue;
    queue<Solver> ArrQueues[depth + 1];
    ArrQueues[0].push(S);
    int goaldepth = depth;
    for (int i = 1; i <= depth; i++)
    {
        while (!ArrQueues[i - 1].empty() && goaldepth == depth)
        {
            queue<Solver> TempQueue = NormalExpand(ArrQueues[i - 1].front());
            ArrQueues[i - 1].pop();
            while (!TempQueue.empty())
            {

                ArrQueues[i].push(TempQueue.front());
                if (TempQueue.front().IsGoal())
                {
                    goaldepth = i;
                    break;
                }
                TempQueue.pop();
            }
        }
    }
    for (int i = 1; i <= goaldepth; i++)
    {
        while (!ArrQueues[i].empty())
        {
            SolverQueue.push(ArrQueues[i].front());
            ArrQueues[i].pop();
        }
    }
    return SolverQueue;
}

queue<Solver> NormalExpand(Solver S)
{
    pii operations[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    queue<Solver> ChildSolverQueue;
    int CurrentRow = S.blank_pos.first;
    int CurrentColumn = S.blank_pos.second;
    int NewRow;
    int NewColumn;
    for (int i = 0; i < 4; i++)
    {
        NewRow = CurrentRow + operations[i].first;
        NewColumn = CurrentColumn + operations[i].second;
        if (IsSafe(NewRow, NewColumn))
        {
            Solver ChildSolver;
            ChildSolver = S;
            swap(ChildSolver.game_board[NewRow][NewColumn], ChildSolver.game_board[CurrentRow][CurrentColumn]);
            ChildSolver.hash_board();
            ChildSolver.blank_pos = {NewRow, NewColumn};
            ChildSolver.total_cost = ChildSolver.total_cost + 1;
            ChildSolver.tile_cost = ChildSolver.getHeuristic();
            ChildSolver.parent_tile = &S;
            if (!ChildSolver.CompareSolver(S.parent_tile))
            {
                ChildSolverQueue.push(ChildSolver);
            }
        }
    }
    return ChildSolverQueue;
}

int getInvCount(int arr[])
{
    int inv_count = 0;
    for (int i = 0; i < 9 - 1; i++)
        for (int j = i + 1; j < 9; j++)
            if (arr[j] && arr[i] && arr[i] > arr[j])
                inv_count++;
    return inv_count;
}

bool isSolvable(int puzzle[3][3])
{
    int invCount = getInvCount((int *)puzzle);
    return (invCount % 2 == 0);
}

void GeneratePermutes(int in[])
{
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 0};
    while (true)
    {
        int x = rand() % 362880;
        for (int i = 0; i < x; i++)
        {
            next_permutation(arr, arr + 9);
        }
        int puzzle[3][3];
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                puzzle[i][j] = arr[i + j];
        if (isSolvable(puzzle))
        {
            break;
        }
    }
    for (int i = 0; i < 9; i++)
        in[i] = arr[i];
}

int main(int argc, char const *argv[])
{
    srand(time(0));
    goal_board = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};
    int depth;
    cin >> depth;
    int arr[9] = {1, 2, 3, 4, 5, 6, 7, 8, 0};
    for (int t = 0; t < 10; t++)
    {
        Solver solve1 = Solver();
        Solver solve2 = Solver();
        Solver solve3 = Solver();
        Solver solve4 = Solver();
        solve1.heuristic_kind = 1;
        solve2.heuristic_kind = 1;
        solve3.heuristic_kind = 2;
        solve4.heuristic_kind = 2;
        GeneratePermutes(arr);
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                solve1.game_board[i][j] = arr[i * 3 + j];
                solve2.game_board[i][j] = arr[i * 3 + j];
                solve3.game_board[i][j] = arr[i * 3 + j];
                solve4.game_board[i][j] = arr[i * 3 + j];
            }
        }
        solve1.SpaceSearch();
        solve1.hash_board();
        solve1.tile_cost = solve1.getHeuristic();
        cout << "----------------------------------------------------------------------\n";
        cout << "Starting State: \n";
        solve1.print_board();
        solve2.SpaceSearch();
        solve2.hash_board();
        solve2.tile_cost = solve2.getHeuristic();
        solve3.SpaceSearch();
        solve3.hash_board();
        solve3.tile_cost = solve3.getHeuristic();
        solve4.SpaceSearch();
        solve4.hash_board();
        solve4.tile_cost = solve4.getHeuristic();
        cout << "----------------------------------------------------------------------\n";
        cout << "Modified A* with depth and Manhattan Heuristic\n";
        ModifiedAStar(solve1, depth);
        cout << "----------------------------------------------------------------------\n";
        cout << "Normal A* with Manhattan Heuristic\n";
        ModifiedAStar(solve2);
        cout << "----------------------------------------------------------------------\n";
        cout << "Modified A* with depth and TileMismatch\n";
        ModifiedAStar(solve3, depth);
        cout << "----------------------------------------------------------------------\n";
        cout << "Normal A* with Tile Mismatch\n";
        ModifiedAStar(solve4);
        cout << "----------------------------------------------------------------------\n";
    }
    return 0;
}
