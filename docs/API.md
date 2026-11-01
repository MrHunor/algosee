# API Documentation 

## Interface 
Currently there are six diffrent API endpoints:

### 0. Default Response Keys  
Any sucessfull (and most unsucessful) requests will returns a array containing all of these **uninitilised** keys, only those keys that are actually used by the response will be filled, the rest will be empty:  
***Array types:*** 
- 'MOVES'
- 'TRIES'
- 'COUNT ARRAY'
- 'SWAPED'
- 'SORTED (LEFT,RIGHT)'
- 'MERGED (LEFT,RIGHT,RESULT)'
- 'SPLIT (ORIGIN,LEFT,RIGHT)'
- 'SORTED'
- 'PATH'
- 'DISTANCE'
- 'VISITED' 
- 'SEARCHMAP'
- 'VISITEDFROMSTART'
- 'VISITEDFROMEND'

***Non-Array types:***
- 'STATUS' -> typically filled with "ONLINE"
- 'VERSION' -> typically filled with the current server version
- 'BUILDTIME' -> " " " the time of compilation
- 'TIME' -> " " " the time it took for the algorithm to execute (and some json parsing overhead)
- 'ERROR' -> " " " an errormessage if the server threw one and is streaming its response, if not it will return its errormessage with an non 200 status code  
- 'ERRORCODE' -> " " " an errorcode if the server threw one, only when streaming its response (born because after you accept the request you must set the response status), **ALWAYS** check if this is 0 when gettig a streamed response, if it isn't an error occured  

*Tip: You can view the defintion of these keys in [src/defs/implemented.h](./../src/defs/implemented.h)*

### 1. /status 
The status endpoint can be used to check if the server is online.  
It takes no URL arguments or Body, it is a GET request.  
It will return a json object along the lines of 0.  

### 2. /limits  
The limits endpoint can be used to get the maximum Size of elements that a algorithm will accept.   
It takes not URL arguments or Body, as it is also a GET request.  
It returns all keys documented in 0. and:  
***Sorting:***  
- 'MAX_ELEMENT_COUNT_SORT'
- 'MAX_ELEMENT_COUNT_SELECTION'
- 'MAX_ELEMENT_COUNT_BOGO'
- 'MAX_ELEMENT_COUNT_BUBBLE'
- 'MAX_ELEMENT_COUNT_QUICK'
- 'MAX_ELEMENT_COUNT_MERGE'
- 'MAX_ELEMENT_COUNT_COUNTING'
- 'MAX_ELEMENT_COUNT_CYCLE'
- 'HIGHEST_ELEMENT_COUNTING'  
***Pathfinding:***
- 'MAX_ELEMENT_COUNT_BFS'
- 'MAX_ELEMENT_COUNT_DIJKSTRA'
- 'MAX_ELEMENT_COUNT_DFS'
- 'MAX_ELEMENT_COUNT_BIBFS'
- 'MAX_ELEMENT_COUNT_ASTAR'
- 'MAX_ELEMENT_COUNT_GBFS'  
***Image:***
- 'MAX_PIXEL_COUNT_IMAGE'  

*Tip: You can view the defintion of these keys in [src/routing/limits.cpp](../src/routing/limits.cpp)*

### 3. /sortalgo
The sortalgo endpoint can be used to sort an array of integer elements.  
It expects the URL argument algo being equal to one of the implemented sorting algorithms. (see [docs/Implemented_Algorithms.md](./Implemented_Algorithms.md))  
Furthermore it requires the body to have a array of 'values'.  
It will return the default keys defined in 0. and some other keys depending on the chosen alorithm:
- ***Selection***:
  - 'MOVES' containing a array of integer pairs representing the indexes of the values array to swap  
- ***Cycle***:
  - 'SWAPED' contains an array of inegers paris represeting the number thats being swaped INTO the array and the index of the element in the values array thats being swaped OUT (and being used as the next element to sort)  
- ***Bubble***:
  - 'MOVES' [*see selection*](#L72)  
- ***Quick***:
  - 'MOVES' [*see selection*](#L72)  
- ***Merge***:
  - 'SPLIT (ORIGIN,LEFT,RIGHT)' contains an array of three elements, first being the original array, then the left area of the split array, and finally the right area  
  - 'SORTED (LEFT,RIGHT)' contains an array of two elements, first being the sorted left part of the array, and secondly the sorted right part of the array  
  - 'MERGED (LEFT,RIGHT,RESULT)' contains an array of three elements, first being the left area of the soon to be merged array, secondly the right area and finally the result of the merge  
- ***Counting***:
  - 'COUNT ARRAY' contains an array which represents the values array in the format of countarr[n]=number of occurences of the number n in the values array  
- ***Bogo***:
  - 'TRIES' contains an array of arrays, the inner part being some random variation of the original values array  


### 4. /pathalgoMAP
The pathfinding endpoint for maps can be used to find a path throught a (un-)weighted "Map" or more precisely a 2 dimensional array.  
Similiar to the [sorting endpoint](#3-sortalgo) it requires a URL arugment named algo being equal to one of the implemented Pathfinding algorithms (see [docs/Implemented_Algorithms.md](./Implemented_Algorithms.md)).  
It also requires the following body values:
  - 'MAP' containing either an 2D array of positive integers (except 0 for A*) and -1 (being blocked), which would be a weighted map or a 2D array of boolean with 0 being traversable and 1 being blocked, which would be a unweighted map, the map MUST have equal row lengths
  - 'START' A pair of integers containing the start in the format of x,y (so inner, outer)
  - 'GOAL' See above  

It will returns a json object containing the keys defined in 0. and some other (non algorithm specific like [sorting](#3-sortalgo)):
  - 'VISITED' A 2D array of positive integers showcasing which tile was explored (1) or not (-1)  
  - 'SEARCHMAP' A 2D array of positive integers showcasing which tile was explored at which iteration or not at all (-1)  
  - 'PATH' A array of pairs in the format x,y representing the path step by step, from start to goal