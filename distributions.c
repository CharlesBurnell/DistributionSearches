#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "searchFunctions.h"
#include "generateRVs.h"
//#include "testStructure.h"

void testFunction(struct test *thisTest);
int getBalloonPop(struct test *thisTest);
void getDistrubution(int argc, char **argv, struct test *thisTest);
void getSearchFunction(int argc, char **argv, struct test *thisTest);
void printUsage();

int main(int argc, char *argv[])
{
	srand(time(NULL));
	struct test* thisTest=createTest();
	thisTest->topFloor = atol(argv[3]);
	thisTest->bottomFloor = atol(argv[4]);
	thisTest->numRounds = atol(argv[5]);
	getSearchFunction(argc, argv, thisTest);
	getDistrubution(argc, argv, thisTest);
	//printf("%d - %d - %d - %d \n",distro,userTopFloor,userBottomFloor,userNumRounds);
	//thisTest.argc = argc;
	//thisTest.argv = argv;
	//int (*searchToUse)(int balloonPop, int topFloor, int bottomFloor);
	//double test = thisTest.

	testFunction(thisTest);
	return 0;
}

/*
 * TODO finish this function
 */
void testFunction(struct test *thisTest)
{
	int numRounds = thisTest->numRounds;
	int topFloor = thisTest->topFloor;
	int bottomFloor = thisTest->bottomFloor;
	int passed;
	int totalRounds;
	int balloonPop;
	passed=0;
	totalRounds=0;
	int i;
	for(i=0;i<numRounds;i++)
	{
		balloonPop= (int) getBalloonPop(thisTest);
		//printf("getBalloonPop result %d\n",balloonPop);
		//balloonPop=bottomFloor;
		//balloonPop=topFloor;
		int counter;
		counter = thisTest->searchToUse(balloonPop,topFloor,bottomFloor);
		//printf("got this far %d\n",i);
		if (counter!=-1){
			passed++;
			totalRounds+=counter;
		}
	}
	printf("number passed : %d\nAverage number of rounds %f\n",passed,(double)totalRounds/(double)numRounds);
}

/*
 * This takes a struct test extracts the parameters
 * of the RV and then generates the floor that the
 * balloonPop will be. This needs to be updated
 * when RVs are added.
 */
int getBalloonPop(struct test *thisTest){
	//printf("In getBalloonPop \n");
	int balloonPop;
	int topFloor = thisTest->topFloor;
	int bottomFloor = thisTest->bottomFloor;
	//printf("thisTest.typeOfRV.distro: %d\n", thisTest->typeOfRV.distro);
	switch(thisTest->typeOfRV.distro)
	{
	//Uniform RV
	case 1:
		balloonPop = rand() % (topFloor-bottomFloor) + bottomFloor;
		break;
	//Exponential RV
	case 2:
		//firstParameter= thisTest.firstParameter.lambda;
		balloonPop =  (int) getExponentialRV(thisTest);
		break;
	//Normal RV
	case 3:
		//firstParameter = thisTest.firstParameter.mu;
		//secondParameter= thisTest.secondParameter.sigma;
		balloonPop = (int) getNormalRV(thisTest);
		break;
	}
	//printf("%d\n",balloonPop);
	return balloonPop;
}

/*
 * parses command line argument for type of distribution to use to generate
 */
void getDistrubution(int argc, char **argv, struct test *thisTest)
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
		printf("Uniform Random Variable\n");
		thisTest->typeOfRV.distro = 1;
		thisTest->generateRVFunc = getUniformRV;
		return;
		break;
	//Return Exponential RV
	case 'e':
		printf("Exponential Random Variable\n");
		thisTest->typeOfRV.distro = 2;
		thisTest->generateRVFunc = getExponentialRV;
		thisTest->firstParameter = atof(argv[6]);
		return;
		break;
	//Return Normal RV
	case 'n':
		printf("Normal Random Variable\n");
		thisTest->typeOfRV.distro = 3;
		thisTest->generateRVFunc = getNormalRV;
		thisTest->firstParameter = atof(argv[6]);
		thisTest->secondParameter = atof(argv[7]);
		return;
		break;
	default:
		printUsage();
		return;
	}
}


/*
 * parses command line arguments for the type of search function to sue
 */
void getSearchFunction(int argc, char **argv, struct test *thisTest)
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
		thisTest->searchToUse = binarySearchForFloor;
		break;
	// Linear Search
	case 'l':
		thisTest->searchToUse = linearSearchForFloor;
		break;
	// Random Search
	case 'r':
		thisTest->searchToUse = randomSearchForFloor;
		break;
	default:
		printUsage();
	}
}
/*
 * This should be called when a input isn't correct
 * TODO format it so its better and easier to read
 */

void printUsage()
{
	printf("Arguments are -d -s topFloor bottomFloor numberOfRounds <mu|lambda> <sigma>\n");
	printf("where -d is -u for Unform -e for Exponential -n for Normal\n");
	printf("and where -s is -b for binary search -l for linear search\n");
	printf("   and -r is for a random search \n");
	printf("no optional arguments passed for uniform\n");
	printf("for Exponential only lambda\n");
	printf("for Normal both mu and sigma\n");
	exit(0);
}

