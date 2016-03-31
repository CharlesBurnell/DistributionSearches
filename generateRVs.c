#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "generateRVs.h"
//#include "testStructure.h"


/**
 * This returns a uniform rv on (0,1)
 * Helper function used so we can make other RVs
 * Using quantile fucntions
 */
double generateUniformRV()
{
	double uniformRV;
	// consider adding 1 to rand and 2 to maxrand to make it
	// guarenteed (0,1).
	double randomNumber = (double)rand();
	uniformRV = (double)randomNumber/(double)RAND_MAX;
	//printf("%d : %d in generateURV\n",(int)randomNumber,(int)uniformRV);
	return uniformRV;
}


double getUniformRV(struct test *thisTest)
{
	int topFloor = thisTest->topFloor;
	int bottomFloor = thisTest->bottomFloor;
	double numFloors = (double) topFloor - (double) bottomFloor;
	double probability = generateUniformRV();
	double UniformRV = numFloors*probability+(double) bottomFloor;
	return UniformRV;
}

/*
 * This uses 2 variables in U(0,1) to generate a RV ~ N(mu,sigma)
 */
//double getNormalRV(double mu, double sigma)
double getNormalRV(struct test *thisTest)
{
	double mu = thisTest->firstParameter;
	double sigma = thisTest->secondParameter;
	double Unif1 = generateUniformRV();
	double Unif2 = generateUniformRV();
	double Norm1 = sqrt(-2*log(Unif1))*cos(2*M_PI*Unif2);
	Norm1 = Norm1*sigma+mu;
	return Norm1;
}
/**
 * This uses the quantile passed in as the probability to generate
 * a value in an expontial RV with lamba as the scale vector
 */
//double getExponentialRV(double probability, double lambda)
double getExponentialRV(struct test *thisTest)
{
	//printf("in exp RV\n");
	double expRV;
	double probability = generateUniformRV();
	double lambda = thisTest->firstParameter;
	expRV=-log(1.0-probability)/lambda;
//	printf("%f : %f : %f\n",lambda,probability,expRV);
	return expRV;
}


/**
 * This generates a Cauchy RV with parameters
 * xnaught being the location parameter and
 * gamma being the scale RV.
 * This is similar to a Normal RV except that
 * there is no variance in it.
 */
//double getCauchyRV(double xnaught, double gamma)
double getCauchyRV(struct test *thisTest)
{
	double probability = generateUniformRV();
	double xnaught = thisTest->firstParameter;
	double gamma = thisTest->secondParameter;
	//printf("%f : %f \n",xnaught,gamma);
	// Quantile function is from wikipedia and checked
	// against a stats textbook not derived by myself
	double caucheyRV = xnaught + gamma * tan(M_PI *(probability-1/2));
	return caucheyRV;
}
