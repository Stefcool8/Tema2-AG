#include "Problem.h"
#include "Functions.h"
using namespace std;

int main()
{
	Problem* dj0;
	dj0 = new Problem(*DeJong, -5.12, 5.12, 5);

	dj0->GeneratePopulation();

	return 0;
}
