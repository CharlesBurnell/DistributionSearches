#include <stdlib.h>
#include "testStructure.h"


struct test* createTest()
{
	struct test* thisTest = (struct test*) malloc(sizeof(struct test));
	return thisTest;
}
