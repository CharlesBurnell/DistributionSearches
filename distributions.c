#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "searchFunctions.h"
#include "generateRVs.h"
struct test{
	int topFloor;
	int bottomFloor;
	int numRounds;
	struct typeOfRVstruct{
		int distro;
		int numberOfParameters;
	}typeOfRV;
	union firstParameterUnion{
		double lambda;
		double mu;
	} firstParameter;
	union secondParameterUnion{
		int sigma;
	} secondParameter;
	int argc;
	char **argv;
};
void testFunction(struct test thisTest, int (*searchToUse)(int balloonPop, int topFloor, int bottomFloor));
int getBalloonPop(struct test thisTest);
int getDistrubution(int argc, char **argv);
int getSearchFunction(int argc, char **argv);
void printUsage();

int main(int argc, char *argv[])
	{
	int searchFunction = getSearchFunction(argc, argv);
	struct test thisTest;
	thisTest.topFloor = atol(argv[3]);
	thisTest.bottomFloor = atol(argv[4]);
	thisTest.numRounds = atol(argv[5]);
	//printf("%d - %d - %d - %d \n",distro,userTopFloor,userBottomFloor,userNumRounds);
	thisTest.argc = argc;
	thisTest.argv = argv;
	int (*searchToUse)(int balloonPop, int topFloor, int bottomFloor);

	srand(time(NULL));
	switch(searchFunction)
	{
	case 1:
		searchToUse = binarySearchForFloor;
		break;
	case 2:
		searchToUse = linearSearchForFloor;
		break;
	}

	testFunction(thisTest,searchToUse);
	return 0;
}

/*
 * TODO finish this function
 */
void testFunction(struct test thisTest,int (*searchToUse)(int balloonPop, int topFloor, int bottomFloor))
{
	int numRounds = thisTest.numRounds;
	int topFloor = thisTest.topFloor;
	int bottomFloor = thisTest.bottomFloor;
	int passed;
	int totalRounds;
	int balloonPop;
	passed=0;
	totalRounds=0;
	int i;
	for(i=0;i<numRounds;i++)
	{
		int numFloors =topFloor-bottomFloor;
		balloonPop= rand() %numFloors+bottomFloor;
		//balloonPop=bottomFloor;
		//balloonPop=topFloor;
		int counter;
		counter = searchToUse(balloonPop,topFloor,bottomFloor);
		if (counter!=-1){
			passed++;
			totalRounds+=counter;
		}
	}
	printf("number passed : %d\nAverage number of rounds %f\n",passed,(double)totalRounds/(double)numRounds);
}


int getBalloonPop(struct test thisTest){
	int balloonPop;
	int topFloor = thisTest.topFloor;
	int bottomFloor = thisTest.bottomFloor;
	double probability;
	double firstParameter;
	double secondParameter;
	switch(thisTest.typeOfRV.distro)
	{
	//Uniform RV
	case 1:
		balloonPop = rand() % (topFloor-bottomFloor) + bottomFloor;
		break;
	//Exponential RV
	case 2:
		firstParameter= thisTest.firstParameter.lambda;
		probability = getUniformRV();
		balloonPop = (int) getExpFromUniform(probability,firstParameter)+bottomFloor;
		break;
	//Normal RV
	case 3:
		firstParameter = thisTest.firstParameter.mu;
		secondParameter= thisTest.secondParameter.sigma;
		balloonPop = (int) getNormalRV(firstParameter,secondParameter);
		break;
	}
	return balloonPop;
}

/*
 * parses command line argument for type of distribution to use to generate
 */
int getDistrubution(int argc, char **argv)
{
	if(argc<6)
	{
		printUsage();
	}
	int firstChar;
	firstChar = *++argv[1];
	switch(firstChar)
	{
	//Return Uniform RV
	case 'u':
		return 1;
		break;
	//Return Exponential RV
	case 'e':
		return 2;
		break;
	//Return Normal RV
	case 'n':
		return 3;
		break;
	default:
		printUsage();
		return 0;
	}
}
/*
 * parses command line arguments for the type of search function to sue
 */
int getSearchFunction(int argc, char **argv)
{
	if(argc<6)
	{
		printUsage();
	}
	int searchChar;
	searchChar = *++argv[2];
	switch(searchChar)
	{
	// Binary Search
	case 'b':
		return 1;
		break;
	// Linear Search
	case 'l':
		return 2;
		break;
	default:
		printUsage();
		return 0;
	}
}


void printUsage()
{
	printf("Arguments are -d -s topFloor bottomFloor numberOfRounds <mu|lambda> <sigma>\n");
	printf("where -d is -u for Unform -e for Exponential -n for Normal\n");
	printf("and where -s is -b for binary search -l for linear search\n");
	printf("no optional arguments passed for uniform\n");
	printf("for Exponential only lambda\n");
	printf("for Normal both mu and sigma\n");
	exit(0);
}

