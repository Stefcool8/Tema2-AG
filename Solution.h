#pragma once
#include "Problem.h"
using namespace std;
typedef vector< vector<float> > float_matrix;
class Solution
{
public:
    boost::dynamic_bitset<> bits_representation;
    vector<float> solution_args;
    float solution_value;
    vector<Solution> all_neighbors;
    Solution operator=(const Solution rvalue)
    {
        bits_representation = rvalue.bits_representation;
        solution_args = rvalue.solution_args;
        solution_value = rvalue.solution_value;
        all_neighbors = rvalue.all_neighbors;
        return *this;
    }
};

