#include "Problem.h"
#include "Functions.h"
using namespace std;

int main()
{
	Problem* dj0;
	dj0 = new Problem(*DeJong, -5.12, 5.12, 1);

	dj0->GeneratePopulation();
	dj0->CrossoverPopulation(0.5);

	return 0;
}
