#include <vector>
#include <utility>
#include <cstdlib>
#include <numeric>

#include "polyrand.hpp"
#include "CONSTANT.h"

std::vector<std::pair<int, int>> randPoly(int degree)
{
    std::vector<std::pair<int, int>> result;
    for (int i = 0; i < degree; i++)
    {
        int num = (std::rand() % (ROOT_RANGE_NUM * 2)) - ROOT_RANGE_NUM;
        int denom = (std::rand() % ROOT_RANGE_DENOM) + 1;
        if (std::gcd(num, denom) != 1)
        {
            num /= std::gcd(num, denom);
            denom /= std::gcd(num, denom);
        }
        result.push_back(std::make_pair(num, denom));
    }
    return result;
}
