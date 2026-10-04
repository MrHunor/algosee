#include "config.h"
#include "logger/logger.h"
#include "utils/utils.h"
#include <cmath>
#include <string>
#include <utility>


  struct run {
    std::string algoname;
    int size;
    std::pair<int, int> mapSize;
    bool failure;
  };
  
int main(int argc, char *argv[]) {
  remove("log.txt");
  stateClass state;
  state.out("Testing sorting algorithms", 0);

  std::vector<run> exitMessage;

  // sortalgos
  for (const auto &testcase : testCasesSorting) {

    run currentrun;
    currentrun.failure = false;
    currentrun.algoname = testcase.first;
    currentrun.size = testcase.second;
    currentrun.mapSize = {-1, -1};
    
    state.out("Testing Sorting validness with: " + currentrun.algoname +
                  " size:" + std::to_string(currentrun.size),
              0);
    try {

      std::vector<int> testValues(currentrun.size);
      std::iota(testValues.begin(), testValues.end(), 1);
      std::sort(testValues.begin(), testValues.end());
      std::vector<int> sorted = testValues;
      std::string command =
          "curl -v -X POST \"http://127.0.0.1:8080/sortalgo?algo=" +
          currentrun.algoname +
          "\" "
          "-H \"Content-Type: application/json\" "
          R"(-d "{\"values\":[$(seq 1 )" +
          std::to_string(currentrun.size) + R"( | shuf | paste -sd, -)]}")";
      std::string output = executeCommand(command);
      state.out("Recived (raw):" + output, 0);
      json response = json::parse(output);
      state.out("recived (json):" + response.dump(), 0);
      std::vector<int> sortedByAlgosee = response["SORTED"];

      if (sortedByAlgosee != sorted) {

        currentrun.failure = true;
      }
      exitMessage.push_back(currentrun);
    } catch (const std::exception &e) {
      currentrun.failure = true;
      exitMessage.push_back(currentrun);
    }
  }

  for (const auto &testcase : testCasesPathfinding) {

    run currentrun;
    currentrun.failure = false;
    currentrun.algoname = testcase.first;
    currentrun.size = -1;
    currentrun.mapSize = testcase.second;
    state.out("Testing path validness for:"+currentrun.algoname+" on Mapsize:"+std::to_string(currentrun.mapSize.first)+"x"+std::to_string(currentrun.mapSize.second),0);
    std::vector<std::vector<int>> map(testcase.second.second,
                                      std::vector<int>(testcase.second.first));
    try {
      std::string mapString;
      mapString += "[";
      for (const auto &row : map) {
        mapString += "[";
        for (const auto &element : row) {
          mapString += std::to_string(element);
          mapString += ",";
        }
        mapString.erase(mapString.size() - 1, 1);
        mapString += "]";
        mapString += ",";
      }
      mapString.erase(mapString.size() - 1, 1);
      mapString += "]";
      std::pair<int, int> start = {0, 0};
      std::pair<int, int> goal = {testcase.second.first - 1,
                                  testcase.second.second - 1};
      std::string command =
          "curl -X POST \"http://127.0.0.1:8080/pathalgo?algo=" +
          currentrun.algoname + "\"" +
          " -H \"Content-Type: application/json\""
          " -d '{\"MAP\": " +
          mapString + ",\"START\": [" + std::to_string(start.first) + "," +
          std::to_string(start.second) + "],\"GOAL\": [" +
          std::to_string(goal.first) + "," + std::to_string(goal.second) +
          "]}'";
      std::string output = executeCommand(command);
      state.out("Recived (raw):" + output, 0);
      json response = json::parse(output);
      state.out("recived:" + response.dump(), 0);
      std::vector<std::pair<int, int>> path = response["PATH"];

      for (int i = 0; i < path.size(); i++) {
        if (i == 0) {
          if (path[i] != start) {
            currentrun.failure = true;
            i = path.size();
          }
          continue;
        }
        if (i == path.size() - 1) {
          if (path[i] != goal) {
            currentrun.failure = true;
            i = path.size();
            continue;
          }
        }
        if (std::abs(path[i].first - path[i - 1].first) > 1 ||
            std::abs(path[i].second - path[i - 1].second) > 1) {
          currentrun.failure = true;
          i = path.size();
          continue;
        }
      }
      exitMessage.push_back(currentrun);
    } catch (const std::exception &e) {
      state.out("Path test failed for " + currentrun.algoname + ": " + e.what(),
                0);
      currentrun.failure = true;
      exitMessage.push_back(currentrun);
    }
  }

state.out("Finished", 0);
state.out("Succededed for:", 0);
for (const auto &element : exitMessage) {
  if (element.failure == false)
    state.out("Algorithm:" + element.algoname +
                  ", Size:" + std::to_string(element.size) +
                  "Map Size:" + std::to_string(element.mapSize.first) + "," +
                  std::to_string(element.mapSize.second),
              0);
}

state.out("Failed for:", 0);
for (const auto &element : exitMessage) {
  if (element.failure == true)
    state.out("Algorithm:" + element.algoname +
                  ", Size:" + std::to_string(element.size) +
                  "Map Size:" + std::to_string(element.mapSize.first) + "," +
                  std::to_string(element.mapSize.second),
              0);
}

for (const auto &element : exitMessage) {
  if (element.failure == true)
    return -1;
}
return 0;
}