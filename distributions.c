#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

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
int binarySearchForFloor(int balloonPop, int topFloor, int bottomFloor);
int linearSearchForFloor(int balloonPop, int topFloor, int bottomFloor);
double getExpFromUniform(double probability, double lambda);
double getNormalRV(double mu, double sigma);
double getUniformRV();
void testFunction(struct test thisTest, int (*searchToUse)(int balloonPop, int topFloor, int bottomFloor));
int getBalloonPop(struct test thisTest);
// Right now pointers are not being used to return values.
// This was originally going to be used to pass results of the test
// But with the newest restructure it isn't done yet
int* uniformTest(struct test thisTest, int (*searchToUse)(int balloonPop, int topFloor, int bottomFloor));
int* expTest(struct test thisTest, double lambda, int (*searchToUse)(int balloonPop, int topFloor, int bottomFloor));
int* normTest(struct test thisTest, double mu, double sigma, int (*searchToUse)(int balloonPop, int topFloor, int bottomFloor));
int getDistrubution(int argc, char **argv);
int getSearchFunction(int argc, char **argv);
void printUsage();

int main(int argc, char *argv[])
{
	int distro = getDistrubution(argc, argv);
	int searchFunction = getSearchFunction(argc, argv);
	int startingArg = 3;
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
 * This does a linear search from bottom floor to top
 * If you for some reason think that the average is high
 * if you are doing a search with the average kind of low
 * and a variance high it will be above the average because
 * for it to be drug down to the mean you need the negatives.
 */
int linearSearchForFloor(int balloonPop, int topFloor, int bottomFloor)
{
	if(balloonPop>topFloor){
		balloonPop=topFloor;
	}
	if(balloonPop<bottomFloor){
		balloonPop=bottomFloor;
	}
	int check;
	int counter = 1;
	for (check=bottomFloor;check<=topFloor;check++){
		if(check>=balloonPop){
			break;
		}
		counter++;
	}
	//printf("%d\n",counter);
	return counter;
}

/**
 * This performs a binary search on the building between
 * top and bottom floor
 */
int binarySearchForFloor(int balloonPop, int topFloor, int bottomFloor)
{
	if(balloonPop>topFloor){
		balloonPop=topFloor;
	}
	if(balloonPop<bottomFloor){
		balloonPop=bottomFloor;
	}
	int lowerBound=bottomFloor;
	int upperBound=topFloor;
	int counter=1;
	int floorGuess=(topFloor-bottomFloor)/2+bottomFloor;
	/**
	 * This checks that the search finds the correct position
	 * starting at floor(numFloors/2) due to integer division
	 * then if the search is above it checks the
	 * guess +(upperbound - guess)/2
	 * and similarly checks guess-(guess-lowerbound)/2 otherwise
	 * at the very upper cases it has the top two if statements
	 * for corner cases I can't get to work without them.
	 */
	while(floorGuess!=balloonPop)
	{
		//printf("balloonPop: %d,floorGuess %d \n",balloonPop,floorGuess);
		if(floorGuess==topFloor-1 && floorGuess<balloonPop)
		{
			floorGuess++;
			//	printf("top");
			//	break;
		}else if(floorGuess==bottomFloor+1 && floorGuess>balloonPop)
		{
			//printf("bottom");
			floorGuess--;
			//printf("%d\n",floorGuess);
			//	break;
		}else if(balloonPop>floorGuess)
		{
			lowerBound=floorGuess;
			floorGuess=floorGuess+(upperBound-floorGuess)/2;
			/*
			 * The +1 is used so that it can hit the upper
			 * bound.
			 */
		}else if(balloonPop<floorGuess)
		{
			upperBound=floorGuess;
			floorGuess=floorGuess-(floorGuess-lowerBound)/2;
		}
		counter++;
	}
	if(floorGuess!=balloonPop){
		printf(" %d and %d and %d\n",floorGuess,balloonPop,lowerBound);
		exit(1);
		return -1;
	}
	//printf("%d\n",counter);
	return counter;
}


/*
 * This uses 2 variables in U(0,1) to generate a RV ~ N(mu,sigma)
 */

double getNormalRV(double mu, double sigma)
{
	double Unif1 = getUniformRV();
	double Unif2 = getUniformRV();
	double Norm1 = sqrt(-2*log(Unif1))*cos(2*M_PI*Unif2);
	Norm1 = Norm1*sigma+mu;
	return Norm1;
}
/**
 * This uses the quantile passed in as the probability to generate
 * a value in an expontial RV with lamba as the scale vector
 */
double getExpFromUniform(double probability, double lambda)
{
	double expRV;
	expRV=-log(1.0-probability)/lambda;
	return expRV;
}

/**
 * This returns a uniform rv on (0,1)
 */
double getUniformRV()
{
	double uniformRV;
	// consider adding 1 to rand and 2 to maxrand to make it
	// guarenteed (0,1).
	uniformRV = (double)rand()/(double)RAND_MAX;
	return uniformRV;
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
	int numFloors = topFloor-bottomFloor;
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
	case 1:
		balloonPop = rand() % (topFloor-bottomFloor) + bottomFloor;
		break;
	case 2:
		firstParameter= thisTest.firstParameter.lambda;
		probability = getUniformRV();
		balloonPop = (int) getExpFromUniform(probability,firstParameter)+bottomFloor;
		break;
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
	case 'u':
		return 1;
		break;
	case 'e':
		return 2;
		break;
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
	case 'b':
		return 1;
		break;
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

