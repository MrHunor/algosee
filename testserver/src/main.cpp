#include "config.h"
#include "logger/logger.h"
#include "utils/utils.h"
#include <string>

int main(int argc, char *argv[]) {

  stateClass state;
  state.out("Testing sorting algorithms", 0);

  for (const auto &algo : implementedSortAlgos) {
    for (const auto &i : testSizes) {
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
      json response  = json::parse(output);
      state.out("recived:"+response.dump(),0);
      std::vector<int> sortedByAlgosee = response["SORTED"];
      if (sortedByAlgosee != sorted) {
        state.out("Failed for algo:" + algo + " with size:" + std::to_string(i),
                  0);
                  return -1;
      }
    }
  }
  return 0;
}