#include <chrono>
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <vector>
using json = nlohmann::json;

void afterSelfTestRun(std::string algorithmName,std::vector<int>& paramPassValues,
                      std::vector<int>& testvalues,
                      std::vector<int>& sortedValues,
                      std::vector<std::pair<int, int>>& moves,
                        std::vector<std::vector<int>>& tries,
                         json& response,
                      json& parampassTest, auto& startTime) {
  //end time measurement
  auto endTime = std::chrono::steady_clock::now();
      auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
          endTime - startTime);
           // cleanup
  moves.clear();
  tries.clear();
  parampassTest.clear();


  //check if sort worked
  if(paramPassValues==sortedValues)
   {
    response[algorithmName+" STATUS"]="SUCESS";
    response[algorithmName+" TIME"]=elapsed.count;
   }
   else {
       response[algorithmName+" STATUS"]="FAILED";
    response[algorithmName+" TIME"]=elapsed.count;
   }

   //reset passvalues to testvalues
   paramPassValues=testvalues;
}
