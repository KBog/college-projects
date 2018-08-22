#include "GeneticAlgorithm.h"

/* Function that will select the chromosome using the roulette wheel selection method */
int Selection_RouletteWheel (Chromosome *population, int population_size, int Value)
{
	int chromosomeIdx=0;
	/* Turn the wheel by selecting the slice that the ball will reach */
	float Slice = (float)(rand()%Value);
	float CurrentTotal = 0;

	/* Loop through the population */
	for(int ch=0; ch<population_size; ch++)
	{
		/* Sum the fitness score of the current chromosome to the total 
		   (the higher fitness the higher pobability to be selected) */
		CurrentTotal+=population[ch].Fitness;
		/* If the current fitness has depassed the slice --> this is the 
		   chromosome that has been selected */
		if(CurrentTotal>Slice)
		{
			chromosomeIdx=ch;
			break;
		}
	}
	/* Return the selected chromosome */
	return chromosomeIdx;
}

/* Function that will select the winner chromosome between a number of 
   random chosen chromosomes */
int Selection_Tournament (Chromosome *population, int population_size, int Value)
{
	int NumberOfParticipants=0;
	int randomPicked=0;
	int winner=0;
	float CurrentBestFitness=-1;
	int Percentage = (int)Value;
	/* if we have a population more than 20 --> select 20% from the total population */
	if(population_size>=20)
		NumberOfParticipants=(population_size*Percentage)/100;

	/* Loop to get random picked participants */
	for(int i=0; i<NumberOfParticipants; i++)
	{
		/* Pick a random participant */
		randomPicked=rand()%population_size;

		if(population[randomPicked].Fitness>CurrentBestFitness)
		{
			CurrentBestFitness=population[randomPicked].Fitness;
			winner=randomPicked;
		}
	}
	/* Return the tournament winner */
	return winner;
}