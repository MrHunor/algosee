/*
 * Copyright (C) 2026 MrHunor, siryanni
 * LICENSE:GNU General Public License v3 (GPLv3)
 */
#include <utility>
#include <vector>
#include <queue>
#include <algorithm>

std::vector<std::pair<int,int>> BreadthFirstSearch(std::vector<std::vector<bool>> map, std::pair<int,int> start, std::pair<int,int> goal)
{
//Frontier means what is it going to explore next, hence it being a queue


int rows = map.size();
int collums = map[0].size();

std::vector<std::vector<bool>> visited(rows, std::vector<bool>(collums, false)); //weird ass constructor
std::vector<std::vector<std::pair<int,int>>> parent(rows, std::vector<std::pair<int,int>>(collums, {-1,-1}));
std::vector<std::pair<int,int>> path;

std::queue<std::pair<int,int>> frontier;
frontier.push(start);
visited[start.first][start.second]=true;

std::vector<int> directionsRows = {-1,1,0,0};
std::vector<int> directionsCollums = {0,0,1,-1};

while(!frontier.empty())
{
int x = frontier.front().first;
int y = frontier.front().second;
//because the first frontier element is now in x,y the first element of frontier can be deleted
frontier.pop();

if(std::make_pair(x,y)==goal)break;//reached goal

for(int i = 0; i<4; i++)//check all possible neighbors
{
int xNeigbour = x+directionsRows[i];
int yNeigbour = y+ directionsCollums[i];

if(xNeigbour<0||xNeigbour>rows-1||yNeigbour<0||yNeigbour>collums-1)continue; //current neighboor out of map, skip this iteration

if(map[xNeigbour][yNeigbour]==true)continue; //current Neighboor not a valid tile, skip 

if(visited[xNeigbour][yNeigbour]==true)continue; //been there done that, so skip

//if everything thus far was negative that means we have a valid unvisited tile
visited[xNeigbour][yNeigbour]=true;
parent[xNeigbour][yNeigbour]={x,y};
frontier.push({xNeigbour,yNeigbour});
}
}
//being out of the while loop means every possible tile was explored

if(!visited[goal.first][goal.second]) return path;

//retrace the steps via parent
std::pair<int,int> current = goal;

while(current!=start)
{
path.push_back(current);
current= parent[current.first][current.second];
}

//push back start pour fini
path.push_back(start);


//due to starting with goal, the path must be reversed to start from start
std::reverse(path.begin(),path.end());

return path;

}


