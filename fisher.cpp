#include "fisher.h"

Fisher::Fisher()
{

}

std::vector<std::vector<int>> Fisher::getAllCombinations(int numberOfElements, int size) {
    //Making working range to check if 0 or 1 is needed to be start
    std::vector<int> workingRange;
    for(int i = 0; i < numberOfElements; i++) {
        workingRange.push_back(i);
    }
    std::vector<std::vector<int>> result;
    std::vector<int> workingCombination;
    combinations_recur(0, size, result, workingCombination, workingRange);
    return result;
}

void Fisher::combinations_recur(int offset, int k, std::vector<std::vector<int>> &result, std::vector<int> &workingCombination, std::vector<int> workingRange) {
  if (k == 0) {
    result.push_back(workingCombination);
    return;
  }
  for (int i = offset; i <= workingRange.size() - k; ++i) {
    workingCombination.push_back(workingRange[i]);
    combinations_recur(i+1, k-1, result, workingCombination, workingRange);
    workingCombination.pop_back();
  }
}
