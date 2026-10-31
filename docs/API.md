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

*Tip: You can view the defintion of these keys in src/defs/implemented*

### 1. /status 
The status endpoint can be used to check if the server is online.  
It takes no URL arguments or Body, it is a GET request.  
It will return a json object along the lines of 0.  

### 2. /limits  
The limits endpoint can be used to get the maximum Size of elements that a algorithm will accept.   
It takes not URL arguments or Body, as it is also a GET request.  
It returns all keys documented in 0. and:  
- 'MAX_ELEMENT_COUNT_SORT'
- 'MAX_ELEMENT_COUNT_SELECTION'
- 'MAX_ELEMENT_COUNT_BOGO'
- 'MAX_ELEMENT_COUNT_BUBBLE'

**TO BE FINISHED**
