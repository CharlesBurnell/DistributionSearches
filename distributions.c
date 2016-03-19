#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int binarySearchForFloor(int balloonPop, int topFloor, int bottomFloor);
int linearSearchForFloor(int balloonPop, int topFloor, int bottomFloor);
double getExpFromUniform(double probability, double lambda);
double getNormalRV(double mu, double sigma);
double getUniformRV();
int* uniformTest(int topFloor, int bottomFloor, int numRounds, int (*searchToUse)(int balloonPop, int topFloor, int bottomFloor));
int* expTest(int topFloor, int bottomFloor, int numRounds, double lambda, int (*searchToUse)(int balloonPop, int topFloor, int bottomFloor));
int* normTest(int topFloor, int bottomFloor, int numRounds, double mu, double sigma, int (*searchToUse)(int balloonPop, int topFloor, int bottomFloor));
int getDistrubution(int argc, char **argv);
int getSearchFunction(int argc, char **argv);
void printUsage();

int main(int argc, char *argv[])
{
	int distro = getDistrubution(argc, argv);
	int searchFunction = getSearchFunction(argc, argv);
	int startingArg = 3;
	int userTopFloor = atol(argv[3]);
	int userBottomFloor = atol(argv[4]);
	int userNumRounds = atol(argv[5]);
	//printf("%d - %d - %d - %d \n",distro,userTopFloor,userBottomFloor,userNumRounds);
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

	switch(distro)
	{
	case 0:
		break;
	case 1:
		printf("Uniform test\n");
		uniformTest(userTopFloor,userBottomFloor,userNumRounds,searchToUse);
		break;
	case 2:
		printf("Exponential test\n");
		double userLambda = atof(argv[startingArg+3]);
		expTest(userTopFloor,userBottomFloor,userNumRounds,userLambda,searchToUse);
		break;
	case 3:
		printf("Normal test\n");
		double userMu = atof(argv[startingArg+3]);
		double userSigma = atof(argv[startingArg+4]);
		normTest(userTopFloor,userBottomFloor,userNumRounds,userMu,userSigma,searchToUse);
	}
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
	int floorGuess=(topFloor-bottomFloor)/2+bottomFloor;//+1 to correct for the error of a negative floor
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


int* uniformTest(int topFloor, int bottomFloor, int numRounds, int (*searchToUse)(int balloonPop, int topFloor, int bottomFloor))
{
	int array[2];//First Item is count
	int *arrayPointer;
	int passed;
	int totalRounds;
	int balloonPop;
	arrayPointer=&array[0];
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
		totalRounds+=counter;
		passed++;
	}
	printf("number passed : %d\nAverage number of rounds %f\n",passed,(double)totalRounds/(double)numRounds);

	return arrayPointer;
}

int* expTest(int topFloor, int bottomFloor, int numRounds, double lambda, int (*searchToUse)(int balloonPop, int topFloor, int bottomFloor))
{
	int array[2];//First Item is count
	int *arrayPointer;
	arrayPointer=&array[0];
	double probability;
	int balloonPop;
	int passed =0;
	int totalRounds=0;
	int i;
	for(i=0; i<numRounds; i++){
		probability = getUniformRV();
		balloonPop = (int) getExpFromUniform(probability,lambda)+bottomFloor;
		//printf("%f, %f\n",probability,expRV);
		//balloonPop=bottomFloor;
		//balloonPop=topFloor;
		int counter;
		counter = searchToUse(balloonPop,topFloor,bottomFloor);
		totalRounds+=counter;
		passed++;
	}
	printf("lambda = %f\n",lambda);
	printf("number passed : %d\nAverage number of rounds %f\n",passed,(double)totalRounds/(double)numRounds);


	return arrayPointer;

}

int* normTest(int topFloor, int bottomFloor, int numRounds, double mu, double sigma, int (*searchToUse)(int balloonPop, int topFloor, int bottomFloor))
{
	int array[2];//First Item is count
	int *arrayPointer;
	arrayPointer=&array[0];
	int balloonPop;
	int passed=0;
	int totalRounds=0;
	//double meanTest=0;
	int i;
	for(i=0;i<numRounds;i++)
	{
		balloonPop = (int) getNormalRV(mu,sigma);
		int counter;
		//printf("%d from \n",balloonPop);
		counter = searchToUse(balloonPop,topFloor,bottomFloor);
		if (counter!=-1){
			passed++;
			totalRounds+=counter;
		}
		//meanTest+=normRV;
	}
	printf("mu: %f sigma: %f \n",mu,sigma);
	printf("number passed : %d\nAverage number of rounds %f\n",passed,(double)totalRounds/(double)numRounds);
	//printf("%f\n%f\n",meanTest,meanTest / numRounds);//Used on
	//mu=0;sigma=1; to test that it was centered on what was expected
	return arrayPointer;

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

