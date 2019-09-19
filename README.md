# 8-Puzzle-Solver-Using-A-Search
Solving 8 Puzzle Problem Using A* Search with Modifications

This C++ program solves the classic 8 puzzle problem using A* with some modifications.

The Makefile is already included in the ZIP folder.
This program won’t run on any versions of gcc lower than gcc-7 as it uses c++17. 

sudo add-apt-repository ppa:jonathonf/gcc-7.1
sudo apt-get update
sudo apt-get install gcc-7 g++-7

I hope you upgrade to that before running this code.

The only input required is the value depth. It represents how deep you want to search during one call for the NormalExpand() Function. 
The goal state is: -
1 2 3
4 5 6
7 8 0
‘0’ specifying a blank tile.

The initial states are generated randomly using my GeneratePermutes() function. These are first checked for being solvable before being passed to the ModifiedAStar() function.

The Costs are calculated as: -
Total Number of Nodes visited.
Maximum size of Priority Queue(Frontier).

The default heuristics are: -
1: Manhattan Distance.
2: Tiles Mismatch
3: Euclidian Distance (Not Called but can be used if you want)

A sample output of the program is given in the results.txt file.

This sample output values are later used in the analysis.xlsx file for analytics.

It can be seen clearly from the graph that using the default depth=1 is the best way to do the A* search.
Also, the heuristics don’t play a very superior role in the determination of Number of Nodes etc.
