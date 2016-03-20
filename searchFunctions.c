#include "searchFunctions.h"
#include <stdio.h>
#include <stdlib.h>
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
//		return -1;
	}
	//printf("%d\n",counter);
	return counter;
}

/*
 * This is a search that is like a binary search but randomly selects point
 */

int randomSearchForFloor(int balloonPop, int topFloor, int bottomFloor)
{
	int count = 1;
	int floorGuess = rand()%(topFloor-bottomFloor)+bottomFloor;
	while (floorGuess!=balloonPop){
		floorGuess = rand()%(topFloor-bottomFloor)+bottomFloor;
		count++;
	}
	return count;
}
