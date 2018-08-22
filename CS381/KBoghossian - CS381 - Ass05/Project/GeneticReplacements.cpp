#include "GeneticAlgorithm.h"

/* Function that keeps the best (elite) [n] old chromosomes with the new
   population */
void Replacement_Ellitism (Chromosome *old_population, int old_population_size, Chromosome *new_population, int &new_population_currentsize, int bestfitChromosome)
{
	/* Copy the elite chromosome 10% of the popsize number of times */
	int NumberOfCopies=(old_population_size*10)/100;

	/* Inherit from the best fit chromosome NumberOfCopies times */
	for(new_population_currentsize=0; new_population_currentsize<NumberOfCopies; new_population_currentsize++)
		new_population[new_population_currentsize].InheritFrom(old_population[bestfitChromosome]);
}