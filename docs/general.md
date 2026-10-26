## Project structure:
- `./assets` Contains images for this ReadME
- `./benchmarks` Contains the benchmark script and results
- `./frontend` Root folder of the Frontend side  
   -> `/assets/` Contains global images for `/frontend/` 
   -> `/css/` contains global css file  
   -> Global HTML files (loading;index/landing)
   -> `/path/`
      ->`/js` contains visualizer.js
      ->html files
   ->`/picsort/`
      ->`/js/` "
      -> html
   ->`/sorting/`
      ->`/js/`
         -> compare.js main.js visualizer.js 
      ->html files
     
- `./src` Root folder of the Backend side  
   -> `pathalgo` Contains pathfinding algorithms  
   -> `/server/` Contains server logic  
   -> `/sortalgo/` Contains sorting algorithms   
   -> `/utils/` Contains usefull tools  
   -> `/` Contains main.cpp
- `/` Contains all files needed for building the executable as well as this readME and the LICENSE file



## To build the Project:  
**DISCLAIMER: Technically, this project could build on windows but it has not been tested. The compilation has been verified on Linux Mint and CachyOS.**  
0. `cmake --version` >= **4.4.3**  
   `gcc --version` >= **16.2.1 20260810**  
   `conan --version` >= **2.32.0**
1. `conan install . --output-folder=build --build=missing` to install dependencies  
2. `cmake --preset conan-release` to automaticall configure the build with conan dependencies (-> **if this fails** and you have to rerun the command you HAVE to delete the build Folder and restart from scratch because cmakeCache has already been written)  
3. `cmake --build build` to build 

## Syntax  
`[SERVERIP]/status` **->** Responds with a json with STATUS and VERSION paramter, of which the status parameter should return 'ONLINE' if the server is online and reachable.    
`[SERVERIP]/sortalgo?algo=X` (where x is a search algorithm name, for valid names look in `src/utils/defs.h`) **->** Run a sorting algorithm of your choice on the values provided in the body of the request under the name 'values' (formatted in json).  
`[SERVERIP]/pathalgo?algo=X` (where x is a search algorithm name, for valid names look in `src/utils/defs.h`) **->** Run a pathfinding algorithm of your choice, you have to provide the MAP (in boolean or integer depending on algorithm) in a array(rows) format, a START in pair format, and a GOAL in the same format (view the examples under this section).  

## To test the backend sorting locally (linux):
Use curl to make a request while the server is running, e.g.: `curl -v -X POST "http://127.0.0.1:8080/sortalgo?algo=selection" -H "Content-Type: application/json" -d "{\"values\":[$(seq 1 n | shuf | paste -sd, -)]}" ` where n = number of values (be careful of the actual port when running through docker)  

## To test the backend sorting online (linux):
Use curl to make a request to the render server, e.g.: `curl -v -X POST "https://algosee.onrender.com/sortalgo?algo=quick" -H "Content-Type: application/json" -d "{\"values\":[$(seq 1 n | shuf | paste -sd, -)]}"` where n = number of values (be careful of the actual port when running through docker)

## To test the beckend pathfinding locally (linux):
Use curl to make a request while the server is runningm e.g.:`curl -X POST "http://127.0.0.1:8080/pathalgo?algo=BFS" -H "Content-Type: application/json" -d '{"MAP": [[false, false, true,  false],[true,  false, true,  false],[false, false, false, false],[false, true,  true,  false]],"START": [0, 0],"GOAL":   [3, 3]}'`

## To test the backend pathfinding online (linux):
Use curl to make request to the render server, e.g.: `curl -X POST "https://algosee.onrender.com/pathalgo?algo=BFS" -H "Content-Type: application/json" -d '{"MAP": [[false, false, true,  false],[true,  false, true,  false],[false, false, false, false],[false, true,  true,  false]],"START": [0, 0],"GOAL":   [3, 3]}'`



# AI usage disclosure:
I, **@MrHunor**, have used AI for explaining what do to when i didn't know anymore, as well as when i wanted to implement something but didn't know how or for scanning the codebase for bugs and possible enhancements. I've also used AI Audits, such as coderabbit on PRs.
I, **@siryanni**, main usage of AI was during major bughunts, as the codebase has slowly gotten pretty big.
                  other uses include: -image generation
                                      -help with concept mapping
                                      -help with code cleanup
---                                   -said coderabbit made Issues           
For anyone looking to contribute we would ask them to follow the line we provided 
