#pragma once
#include <math.h>
#include <random>
#include <iostream>
#include <boost/dynamic_bitset.hpp>
#include "Functions.h"
#include "Solution.h"
#include <time.h>
#include <fstream>
using namespace std;
typedef vector< boost::dynamic_bitset<> > bitset_vector;
//MIND THE DIFFERENCE BETWEEN BIT VECTOR AND BITSET VECTOR !!!!, (QUADRATIC)

class Problem
{
	float a;
	float b;
	int dimension;
	int log10eps;
	int nr_of_intervals;
	int one_argument_length;
	int full_length;
public:
	vector<Solution> Population;
	float (*function_used)(const vector<float>& args);

	Problem(float a, float b, int dimension, int log10eps)
		: a(a), b(b),
		dimension(dimension),
		log10eps(log10eps)
	{
		ComputeSpecs();
		Population.reserve(100);
		for (auto &chromosome: Population)
		{
			chromosome.bits_representation.reserve(full_length);
		}
	}
	boost::dynamic_bitset<> getRandomSequence()
	{
		random_device device;
		uniform_int_distribution<int> distribution(0, 1);
		boost::dynamic_bitset<> res(full_length);
		for (int i = 0; i < full_length; i++)
		{
			res[i] = distribution(device);
		}
		return res;
	}
	void SetFunction(float (*arg_function)(const std::vector<float>& args))
	{
		function_used = arg_function;
	}
	void GeneratePopulation()
	{
		for(auto &chromosome: Population)
		{
			chromosome.bits_representation = getRandomSequence();
			chromosome.solution_args = ConvertBitsetToVector(chromosome.bits_representation);
			chromosome.solution_value = function_used(chromosome.solution_args);
		}
	}
	void ComputeSpecs()
	{
		nr_of_intervals = (b - a) * pow(10, log10eps);
		one_argument_length = (int)abs(ceil(log2(nr_of_intervals)));
		full_length = one_argument_length * dimension;
		cout << "Specificatiile situatiei:\n";
		cout << "numar intervale: " << nr_of_intervals << "\n";
		cout << "lungimea in biti a unei valori: " << one_argument_length << " " << pow(2, one_argument_length) << "\n";
		cout << "lungimea in biti a vectorului solutii: " << full_length << "\n";
	}
	unsigned long NumberByIndex(int index, const boost::dynamic_bitset<>& bit_vector)
	{
		if (index >= dimension || index < 0)
		{
			cout << "Invalid Index";
			exit(2);
		}
		boost::dynamic_bitset<> atomic_number(one_argument_length);
		for (int i = 0; i < one_argument_length; i++)
		{
			atomic_number[one_argument_length - 1 - i] = bit_vector[full_length - 1 - (index * one_argument_length) - i];
		}

		return atomic_number.to_ulong(); // convert to unsigned long
	}
	vector<float> ConvertBitsetToVector(const boost::dynamic_bitset<>& bit_vector)//from binary to float arguments
	{
		vector<float> float_vector;
		float argument;
		//boost::dynamic_bitset<> atomic_number(one_argument_length);
		for (int i = 0; i < dimension; i++)
		{
			argument = (float)NumberByIndex(i, bit_vector);
			argument = (argument / (pow(2, (one_argument_length)) - 1));
			argument *= (b - a);
			argument += a;
			float_vector.push_back(argument);
		}
		return float_vector;
	}
	void crossover(Solution &chrm1, Solution &chrm2)
	{
		random_device device2;
		uniform_int_distribution<int> distribution2(1, one_argument_length-2);
		float first_point = distribution2(device2);
		float second_point = distribution2(device2);
		float aux;
		while (second_point == first_point)
			second_point = distribution2(device2);
		if (second_point < first_point)
		{
			aux = first_point;
			first_point = second_point;
			second_point = first_point;
		}
		for (int i = first_point; i <= second_point; i++)
		{
			aux = chrm1.bits_representation[i];
			chrm1.bits_representation[i] = chrm2.bits_representation[i];
			chrm2.bits_representation[i] = aux;
		}
	}
};
