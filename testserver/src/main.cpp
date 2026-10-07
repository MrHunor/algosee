#include "config.h"
#include "logger/logger.h"
#include "utils/utils.h"
#include <cmath>
#include <string>
#include <utility>

int main(int argc, char *argv[]) {

  stateClass state;
  state.out("Testing sorting algorithms", 0);
  struct run {
    std::string algoname;
    int size;
    std::pair<int, int> mapSize;
    bool failure;
  };
  std::vector<run> exitMessage;

  // sortalgos
  for (const auto &algo : implementedSortAlgos) {
    for (const auto &i : testSizesSort) {
      run currentrun;
      currentrun.failure = false;
      currentrun.algoname = algo;
      currentrun.size = i;
      currentrun.mapSize = {-1, -1};
      std::vector<int> testValues(i);
      std::iota(testValues.begin(), testValues.end(), 1);
      std::sort(testValues.begin(), testValues.end());
      std::vector<int> sorted = testValues;
      std::string command =
          "curl -v -X POST \"http://127.0.0.1:8080/sortalgo?algo=" + algo +
          "\" "
          "-H \"Content-Type: application/json\" "
          R"(-d "{\"values\":[$(seq 1 )" +
          std::to_string(i) + R"( | shuf | paste -sd, -)]}")";
      std::string output = executeCommand(command);
      json response = json::parse(output);
      state.out("recived:" + response.dump(), 0);
      std::vector<int> sortedByAlgosee = response["SORTED"];

      if (sortedByAlgosee != sorted) {

        currentrun.failure = true;
      }
      exitMessage.push_back(currentrun);
    }
  }

  for (const auto &algo : implementedPathAlgos) {
    for (const auto &size : testSizesPath) {
      std::vector<std::vector<int>> map(size.second,
                                        std::vector<int>(size.first));
      run currentrun;
      currentrun.failure = false;
      currentrun.algoname = algo;
      currentrun.size = -1;
      currentrun.mapSize = size;
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
      std::pair<int, int> goal = {size.first - 1, size.second - 1};
      std::string command =
          "curl -X POST \"http://127.0.0.1:8080/pathalgo?algo=" + algo + "\"" +
          " -H \"Content-Type: application/json\""
          " -d '{\"MAP\": " +
          mapString + ",\"START\": [" + std::to_string(start.first) + "," +
          std::to_string(start.second) + "],\"GOAL\": [" +
          std::to_string(goal.first) + "," + std::to_string(goal.second) +
          "]}'";
      std::string output = executeCommand(command);
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


  for(const auto& element : exitMessage)
  {
    if(element.failure==true)return -1;
  }
  return 0;
}