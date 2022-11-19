#pragma once
#include <math.h>
#include <random>
#include <iostream>
#include <boost/dynamic_bitset.hpp>
#include "Functions.h"
#include "Solution.h"
#include <time.h>
#include <algorithm>
using namespace std;
typedef vector< boost::dynamic_bitset<> > bitset_vector;
#define precision 5
//MIND THE DIFFERENCE BETWEEN BIT VECTOR AND BITSET VECTOR !!!! (QUADRATIC)

class Problem
{
	float a;
	float b;
	int dimension;
	int nr_of_intervals;
	int one_argument_length;
	int full_length;
	float (*function)(const vector<float>& args);
	vector<Solution*> Population;

public:
	Problem(float (*function)(const vector<float>& args), float a, float b, int dimension)
		: a(a), b(b),
		dimension(dimension),
		function(function)
	{
		ComputeSpecs();
		Population.reserve(100);

		for (int i = 0; i < 100; i++)
		{
			Solution* chromosome = new Solution(full_length, dimension);
			Population.emplace_back(chromosome);
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
	void GeneratePopulation()
	{
		for(auto chromosome: Population)
		{
			chromosome->bits_representation = getRandomSequence();
			chromosome->solution_args = ConvertBitsetToVector(chromosome->bits_representation);
			chromosome->solution_value = function(chromosome->solution_args);
			//cout << chromosome->bits_representation << endl;
		}
	}
	void ComputeSpecs()
	{
		nr_of_intervals = (b - a) * pow(10, precision);
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
			float_vector.emplace_back(argument);
		}
		return float_vector;
	}
	void crossover_two_cromosomes(Solution *&chrm1, Solution *&chrm2)
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
			aux = chrm1->bits_representation[i];
			chrm1->bits_representation[i] = chrm2->bits_representation[i];
			chrm2->bits_representation[i] = aux;
		}
	}
	void CrossoverPopulation(float cross_probability)
	{
		random_device device3;
		uniform_int_distribution<int> distribution3(0, 1);
		vector<int> chosen_chromosomes;
		auto rng = std::default_random_engine{};
		int i = 0;
		
		for (i = 0; i < 100; i++)
		{
			if (distribution3(device3) < cross_probability)
			{
				chosen_chromosomes.emplace_back(i);	// add indices of chromosomes from population for crossover
			}
		}
		shuffle(begin(chosen_chromosomes), end(chosen_chromosomes), rng); // choose pairs randomly
		for (i = 0; i < chosen_chromosomes.size() / 2; i += 2)
		{
			cout << Population[i]->bits_representation << endl << Population[i + 1]->bits_representation << endl << endl;
			crossover_two_cromosomes(Population[i], Population[i + 1]);
			cout << Population[i]->bits_representation << endl << Population[i + 1]->bits_representation << endl << endl;
		}
	}
};
