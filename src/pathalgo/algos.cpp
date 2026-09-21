/* 
 *   algosee; a algorithm visulizer
 *   Copyright (C) 2026  MrHunor, siryanni (as equals)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.(root/LICENSE)  If not, see <https://www.gnu.org/licenses/>.
 */
#include <utility>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>




std::vector<std::pair<int,int>> Dijkstra(const std::vector<std::vector<int>> & map,
std::pair<int,int> start,
std::pair<int,int> goal
)
{
const int rows = map.size();
const int collums = map[0].size();
const int INFINTY = std::numeric_limits<int>::max();

//dijkstra uses something similar to BFS but instead of vector<bool> visited, it uses a cost based vector<int>
//so this is effectivly just: "What is the cheapest possible distance to this point in the map"
std::vector<std::vector<int>> distance(rows,std::vector<int>(collums,INFINTY));//i dont think i'll ever unstand this contructor
std::vector<std::vector<std::pair<int,int>>> parent(rows, std::vector<std::pair<int,int>>(collums, {-1,-1}));
std::vector<std::pair<int,int>> path;

//in contrast to BFS we need a reverse "priority" queue, this means the first eleent of the queue is the one with the lowest "priority" which in Djkstras case is the distance
using Node = std::pair<int,std::pair<int,int>>;
std::priority_queue<Node,std::vector<Node>,std::greater<Node>> frontier;

distance[start.first][start.second]=0;

//push the start into the frontier, because its the lowest distance and also the only element it will be the first element
frontier.push({0,start});

std::vector<int> directionsRows = {-1,1,0,0};
std::vector<int> directionsCollums = {0,0,1,-1};

while(!frontier.empty())
{
//extract info from the first frontier element
int currentDistance = frontier.top().first;
int x = frontier.top().second.first;
int y = frontier.top().second.second;

//because the info fron the first frontier element is now stored in the above declared variables, we can delete the first element from frontier
frontier.pop();

if(currentDistance!= distance[x][y])continue; //already discovered

if(std::make_pair(x,y)==goal)break; //finished, goal discovered

//check all neighbours 
for(int i = 0; i<4; i++)
{
    int xNeighbour = x+directionsRows[i];
    int yNeighbour = y+directionsCollums[i];

    if(xNeighbour<0||xNeighbour>rows-1||yNeighbour<0||yNeighbour>collums-1)continue; //current neighboor out of map, skip this iteration

    if(map[xNeighbour][yNeighbour]==-1)continue;//this tile is blocked, skip

int newDistance = currentDistance + map[xNeighbour][yNeighbour];

//is the current path to this tile cheaper then the already known path from start to goal?
if(newDistance < distance[xNeighbour][yNeighbour])
{
    //yes, save all info about the new tile and add it to frontier to be explored next
    distance[xNeighbour][yNeighbour]=newDistance;
    parent[xNeighbour][yNeighbour]={x,y};
   frontier.push({newDistance,{xNeighbour,yNeighbour}});

}


}

}

//Explored all possible tiles

//checking if the goal was ever reached, be careful this could theoretically be a false positive if the addition of tiles matches exatly the intmax, but the likelyhood is very small
if(distance[goal.first][goal.second]==INFINTY) return {};


//copy paste from BFS, same logic
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




std::vector<std::pair<int,int>> BreadthFirstSearch(std::vector<std::vector<bool>> map, std::pair<int,int> start, std::pair<int,int> goal)
{
//Frontier means what is it going to explore next, hence it being a queue


const int rows = map.size();
const int collums = map[0].size();

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
int xNeighbour = x+directionsRows[i];
int yNeighbour = y+ directionsCollums[i];

if(xNeighbour<0||xNeighbour>rows-1||yNeighbour<0||yNeighbour>collums-1)continue; //current neighboor out of map, skip this iteration

if(map[xNeighbour][yNeighbour]==true)continue; //current Neighboor not a valid tile, skip 

if(visited[xNeighbour][yNeighbour]==true)continue; //been there done that, so skip

//if everything thus far was negative that means we have a valid unvisited tile
visited[xNeighbour][yNeighbour]=true;
parent[xNeighbour][yNeighbour]={x,y};
frontier.push({xNeighbour,yNeighbour});
}
}
//being out of the while loop means every possible tile was explored

if(!visited[goal.first][goal.second]) return {};//never once reached the goal means there was no path found to the goal

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


