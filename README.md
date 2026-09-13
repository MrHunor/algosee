# algosee © 2026 MrHunor, siryanni (as equals) GPLv3

## To compile the Project:  
0. `cmake --version` >= **4.4.3**  
   `gcc --version` >= **16.2.1 20260810**  
   `conan --version` >= **2.32.0**
1. `conan install . --output-folder=build --build=missing` to install dependencies  
2. `cmake --preset conan-release` to automaticall configure the build with conan dependencies (-> **if this fails** and you have to rerun the command you HAVE to delete the build Folder and restart from scratch because cmakeCache has already been written)  
3. `cmake --build build` to build 

## To test the backend:
Use curl to make a request while the server is running, e.g. `curl -X POST "http://127.0.0.1:8080/sortalgo?algo=selection" -H "Content-Type: application/json" -d '{"values":[5,3,8,1,2]}'`