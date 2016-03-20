#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "generateRVs.h"


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
