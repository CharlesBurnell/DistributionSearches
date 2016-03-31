
struct test{
	int topFloor;
	int bottomFloor;
	int numRounds;
	struct typeOfRVstruct{
		int distro;
		int numberOfParameters;
	}typeOfRV;
	//Update here when new distribution added
	double firstParameter;
	//Update here when new distribution added
	double secondParameter;
	//This is where the search function is stored
	//in the test structure
	int (*searchToUse)(int balloonPop, int topFloor, int bottomFloor);

	double (*generateRVFunc)(struct test *thisTest);

	//Rethink if this is really needed.
	//Could be used to circumevent the parameters
	//and the other RV struct.
	int argc;
	char **argv;
};

struct test* createTest();

