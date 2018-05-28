#ifndef FISHER_H
#define FISHER_H

#include<vector>

class Fisher
{
public:
    Fisher();
    static std::vector<std::vector<int>> getAllCombinations(int numberOfElements, int size);
private:
    static void combinations_recur(int offset, int k, std::vector<std::vector<int>> &result, std::vector<int> &workingCombination, std::vector<int> workingRange);
};

#endif // FISHER_H
