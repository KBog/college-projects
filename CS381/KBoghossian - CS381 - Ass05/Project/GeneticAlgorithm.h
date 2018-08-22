#ifndef _GENETICALGORITHM_H_
#define _GENETICALGORITHM_H_

#include <vector>
#include <iostream>
#include <time.h>

using namespace std;

/* The chromosome */
struct Chromosome
{
	/* The array of bits that constitute the chromosome
	   each set of bits is a gene */
	int *Genes;
	/* The number of genes in the chromosome */
	int Gene_Count;
	/* The number of bits per gene */
	int Gene_Length;
	/* Chromosome length (in bits) */
	int Bit_Count;
	/* The fitness (will be evaluated) */
	float Fitness;

	/* Default constructor */
	Chromosome ()
	{
		Fitness=0;
		Genes=NULL;
		Gene_Count=Gene_Length=0;
	}
	/* Function to create a chromosome using the random method */
	void Create_Chromosome (int gene_count, int gene_length, bool AllowDuplication)
	{
		/* Setting the genes variables */
		Gene_Count=gene_count;
		Gene_Length=gene_length;
		/* Getting the number of bits in the chromosome */
		Bit_Count=Gene_Count*Gene_Length;
		/* Allocating space for the genes */
		Genes = new int [Bit_Count];

		/* If we allow duplication of values */
		if(AllowDuplication)
		{
			/* Set random (0-1) genes in the chromosome */
			for(int b=0; b<Bit_Count; b++)
				Genes[b] = rand()%2;
		}
		/* No duplication is allowed */
		else
		{
			
			/* Allocating space for the decoded array just to be used for 
			   duplication check */
			int *Decimals = new int [Gene_Count];
			int ChosenValuesIdx=0;
			/* Initializing the array to 0 */
			memset(Decimals,-1,Gene_Count*sizeof(int));
			/* Create the array of bits for the current value */
			int *CurrentCode = new int [Gene_Length];
			bool ValueAdded=false;
			
			int b=0;
			int DecimalValue=0;
			/* Loop to fill all the values in the choromosome */
			for(int g=0; g<Gene_Count; g++)
			{
				ValueAdded=false;
				
				/* loop until we added a non duplicate value */
				while(!ValueAdded)
				{
					/* Add random 0's & 1's */
					for(int i=0; i<Gene_Length; i++)
						CurrentCode[i]=rand()%2;
					/* Get the decoded to decimal value */
					DecimalValue=Decode(CurrentCode);
					/* if the decoded array isn't in the chosen decoded values --> add the bits of the value to the genes */
					if(!ValueFoundInArray(Decimals,Gene_Count,DecimalValue))
					{
						Decimals[ChosenValuesIdx++]=DecimalValue;
						ValueAdded=true;
						/* Copy the current set of bits to the genes */
						for(int i=0; i<Gene_Length; i++)
							Genes[b++]=CurrentCode[i];
					}
				}
			}
			delete [] Decimals;
		}
	}

	/* Function to decode a set of bits to the corresponding decimal value */
	int Decode (int *Code)
	{
		char Result=0;
		int bitPos=0;
		char byte=0;

		/* Loop from the end of the array of int till the start
		   because we are working with binary (bit 7,6,5,4,3,2,1,0) */
		for(int n=Gene_Length-1; n>-1; n--,bitPos++)
		{
			/* if the current value contains a 1 */
			if(Code[n])
			{
				/* Shift the 1 bitPos times */
				byte = 1<<bitPos;
				/* Add a 1 to the resulting byte */
				Result=Result | byte;
			}
		}
		/* Return the number represented by the set of bits */
		return (int)Result;
	}

	/* Function to decode a decimal value to an array of bits (binary array) */
	int * Decode (char DecimalValue)
	{
		int *Result = new int [Gene_Length];
		int bitPos=0;
		char byte=0;

		for(int n=Gene_Length-1; n>-1; n--,bitPos++)
		{
			/* Shift the 1 bitPos times */
			byte = 1<<bitPos;
			/* If the current one is available --> Insert a 1 to array of bits */
			if(DecimalValue & byte)
				Result[n]=1;
			else
				Result[n]=0;
		}
		/* Return the array of bits */
		return Result;
	}

	/* Function that takes the GenePosition in the array of Genes
	   then returns its value */
	int DecodeGene_FromGenesArray (int GenePos)
	{
		/* Create the array of bits that make the gene */
		int *Gene = new int [Gene_Length];
		/* Take the gene's bits from the GenePos and
		   counting till Gene_Length number of bits */
		for(int i=0; i<Gene_Length; i++)
			Gene[i]=Genes[GenePos+i];
		/* Decode the array and return the decimal value of the gene */
		int decimal = Decode(Gene);

		/* Delete the memory allocated to store the gene's bits */
		delete [] Gene;
		/* return the decimal value */
		return decimal;
	}

	/* Function to decode the genes in the chromosome into decimals */
	int *DecodeChromosome (void)
	{
		int *Decimals = new int [Gene_Count];
		for(int g=0; g<Gene_Count; g++)
			Decimals[g]=DecodeGene_FromGenesArray(g*Gene_Length);
		return Decimals;
	}

	/* Function to get the gene position having the decimal value equal
	   to the parameter */
	int Get_GenePosition (int DecimalGene)
	{
		/* Loop through all the genes */
		for(int g=0; g<Gene_Count; g++)
			/* Convert the gene to decimal and check if it's equal to the DecimalGene */
			if(DecodeGene_FromGenesArray(g*Gene_Length)==DecimalGene)
				/* Return it's Position */
				return g*Gene_Length;
		/* if the gene is not found in the genes array */
		return -1;
	}

	/* Function to copy a gene from SourceGenesArray to the local genesarray */
	void Set_GeneValue (int Destination_GenePos, int *SourceGenes, int Source_GenePos)
	{
		/* Get the bit positions according to the genepos */
		int realDestinationBitPos = (Destination_GenePos*Gene_Length);
		int realSourceBitPos = (Source_GenePos*Gene_Length);
		/* Copy all the bits of the SourceGene to the DestinationGene */
		for(int b=0; b<Gene_Length; b++)
			Genes[realDestinationBitPos+b]=SourceGenes[realSourceBitPos+b];
	}

	/* Search for a value if it is found in an array of values */
	bool ValueFoundInArray (int *Array, int Size, int Value)
	{
		/* Loop through the array of values */
		for(int i=0; i<Size; i++)
			if(Array[i]==Value)
				break;
		/* if we looped through all the values --> not found */
		return !(i==Size);
	}

	/* Function to swap 2 genes by swapping all their bits */
	void SwapGenes (int Gene1_StartBitPos, Chromosome &With, int Gene2_StartBitPos)
	{
		for(int n=0; n<Gene_Length; n++)
			swap(Genes[Gene1_StartBitPos+n],With.Genes[Gene2_StartBitPos+n]);
	}

	/* Function to copy the chromosome properties from the source to the destination */
	void InheritFrom (Chromosome Source)
	{
		/* Copy the counters */
		Gene_Count=Source.Gene_Count;
		Gene_Length=Source.Gene_Length;
		Bit_Count=Source.Bit_Count;
		/* Allocate space for deep copy */
		Genes = new int [Bit_Count];
		memcpy(Genes,Source.Genes,Bit_Count*sizeof(int));
	}

	/* Function to insert a gene from a specific position to another position 
	   The ToGenePos will be pushed to the left */
	void MoveGene (int From, int To)
	{
		/* Allocate space for the gene to be moved */
		int *GeneToMove = new int [Gene_Length];

		int n=0;
		/* Store the gene to be moved */
		for(; n<Gene_Length; n++)
			GeneToMove[n]=Genes[From+n];
		
		/* if we are moving a gene from a position less 
		   than the destination, shift the remaining elements
		   to the left */
		if(From<To)
		{
			/* Take the starting position of the Source copy idx */
			int startingPos=From+n;
			/* while the destination copy idx is less than the target Pos */
			while(From<To)
				/* copy from the source idx to the destination from the most
				   significant to the least */
				Genes[From++]=Genes[startingPos++];
			
		}
		/* We are moving a gene from a position greater than
		   the destination --> shift the remaining elements to the right */
		else
		{
			/* Take the starting position of the Source copy idx */
			int startingPos=From-1;
			/* Compute the destination idx */
			From=(From+n)-1;
			/* Take the last bit in the gene */
			int GeneLastBit = To+(Gene_Length-1);
			/* while we didn't reach the last bit --> copy the gene from the least
			   significant bit to the most */
			while(From>GeneLastBit)
				Genes[From--]=Genes[startingPos--];
		}

		/* Insert the gene to its new position */
		for(n=0; n<Gene_Length; n++)
			Genes[To+n]=GeneToMove[n];

		delete [] GeneToMove;
	}

	/* To manually deallocate the genes */
	void Destroy (void)
	{
		delete [] Genes;
		Genes=NULL;
	}
};

/* //////////////////////////////////////////////////////// */
/****************** CROSSOVER FUNCTIONS *********************/
/* //////////////////////////////////////////////////////// */
/* Function to crossover 2 parents at a certain position and
   creating 2 children (binary-nonorder) */
void Crossover_Binary_SinglePoint (Chromosome Dad, Chromosome Mom, Chromosome &Child1, Chromosome &Child2);
/* Function to work on continous values, it swaps any gene from MOM to DAD to create Child1
										then the same gene from DAD to MOM to create Child2 */
void Crossover_Continuous_Discrete (Chromosome Dad, Chromosome Mom, Chromosome &Child1, Chromosome &Child2);
/* Function to work on Ordered specific values, it swaps any number of positions from mom and put them
   in dad while checking for duplications */
void Crossover_Order_Partially (Chromosome Dad, Chromosome Mom, Chromosome &Child1, Chromosome &Child2);
/* Function to work on Ordered specific values, it swaps any number of positions from mom and put them
   in dad then filling the blanks with the remaining unused values */
void Crossover_Order_PositionBased (Chromosome Dad, Chromosome Mom, Chromosome &Child1, Chromosome &Child2);
/* Function to work on ranged values, it swaps any gene from MOM to DAD to create Child1
										then the same gene from DAD to MOM to create Child2 */
void Crossover_RealValue (Chromosome Dad, Chromosome Mom, Chromosome &Child1, Chromosome &Child2);

/* //////////////////////////////////////////////////////// */
/****************** MUTATIONS FUNCTIONS *********************/
/* //////////////////////////////////////////////////////// */
/* Function to mutate a parent to create a child */
void Mutate_Order_Exchange (Chromosome Parent, Chromosome &Child);
/* A function that moves a random gene to a random position */
void Mutate_Order_Insertion (Chromosome Parent, Chromosome &Child);
/* A function that picks a random gene pos to invert it */
void Mutate_NonOrder_Binary (Chromosome Dad, Chromosome &Child1);
/* A function that picks a random gene pos and adds a delta value to the gene */
void Mutate_NonOrder_RealValue (Chromosome Dad, Chromosome &Child1);

/* ///////////////////////////////////////////////////////// */
/****************** SELECTIONS FUNCTIONS *********************/
/* ///////////////////////////////////////////////////////// */
/* Function that will select the winner chromosome between a number of 
   random chosen chromosomes */
int Selection_Tournament (Chromosome *population, int population_size, int Value);
/* Function that will select the chromosome using the roulette wheel selection method */
int Selection_RouletteWheel (Chromosome *population, int population_size, int Value);

/* /////////////////////////////////////////////////////////// */
/****************** REPLACEMENTS FUNCTIONS *********************/
/* /////////////////////////////////////////////////////////// */
/* Function that keeps the best (elite) [n] old chromosomes with the new
   population */
void Replacement_Ellitism (Chromosome *old_population, int old_population_size, Chromosome *new_population, int &new_population_currentsize, int bestfitChromosome);

/* Genetic class */
class Genetic
{
	public:
			/* Array of chromosomes */
			Chromosome *Population;
			/* Number of chromosome in the population */
			int Population_Size;
			/* The number of genes per chromosome */
			int Chromosome_GeneCount;
			/* The number of bits per gene */
			int Gene_Length;
			/* The number of generations */
			int Generation_Count;
			/* The chromosome that has the highest fitness score */
			int BestFitChromosome;
			/* The best fitness score */
			float BestFitnessScore;
			/* The worst fitness score (to compute the average) */
			float WorstFitnessScore;
			/* The total fitness score */
			float TotalFitnessScore;
			/* The mutation probability */
			int Mutation_Probability;

			/* The pointers to the functions that will be used while the program is running. They are pointer to functions
			   because when we loop through the Generation function we will call the pointers that will be initialized to 
			   the desired methods of each function */
			void (*Mutate) (Chromosome Parent, Chromosome &Child);
			void (*Crossover) (Chromosome Dad, Chromosome Mom, Chromosome &Child1, Chromosome &Child2);
			void (*Replacement) (Chromosome *old_population, int old_population_size, Chromosome *new_population, int &new_population_currentsize, int AdditionalValue);
			int (*Selection) (Chromosome *population, int population_size, int Value);

			/* Genetic constructor */
			Genetic(int population_size, int generation_count, int chromosome_genecount, int gene_length, bool AllowDuplication, int mutation_probability):
			Population_Size(population_size),Generation_Count(generation_count),Chromosome_GeneCount(chromosome_genecount),Gene_Length(gene_length),Mutation_Probability(mutation_probability),
			BestFitChromosome(0)
			{
				/* Default values for the variables */
				BestFitChromosome=-1;
				BestFitnessScore=5000;
				WorstFitnessScore=-1;
				TotalFitnessScore=0;

				srand((unsigned int)time(NULL));
	
				/* Default operators */
				Mutate=Mutate_Order_Insertion;
				Crossover=Crossover_Order_PositionBased;
				Replacement=Replacement_Ellitism;
				Selection=Selection_Tournament;
			}

			/* The Fitness function that computes the best and worst fit along with the total */
			void FitnessFunction (void)
			{
				/* Loop through the population */
				for(int ch=0; ch<Population_Size; ch++)
				{
					/* Compute the best fitness score */
					if(Population[ch].Fitness<BestFitnessScore)
					{
						BestFitnessScore=Population[ch].Fitness;
						BestFitChromosome=ch;
					}
					/* Compute the worst fitness score */
					if(Population[ch].Fitness>WorstFitnessScore)
						WorstFitnessScore=Population[ch].Fitness;
				}
				/* Compute the real fitness score */
				for(ch=0; ch<Population_Size; ch++)
				{
					TotalFitnessScore+=Population[ch].Fitness;
					Population[ch].Fitness = WorstFitnessScore - Population[ch].Fitness;
					
				}
			}

			/* The main algorithm loop */
			virtual void Generation (void)
			{
				BestFitChromosome=-1;
				BestFitnessScore=5000;
				WorstFitnessScore=-1;
				TotalFitnessScore=0;

				/* Update the fitness values of all the chromosomes */
				FitnessFunction();

				int Value = 0;
				if(Selection==Selection_RouletteWheel)
					Value=(int)TotalFitnessScore;
				else
					Value=20;

				/* Set the new number of children to 0 */
				int NewChildren=0;
				/* Allocate space for all the children */
				Chromosome *Children = new Chromosome [Population_Size];

				/* Executing the replacement method. If the pointer is null 
				   total replacement */
				if(Replacement)
					Replacement(Population,Population_Size,Children,NewChildren,BestFitChromosome);

				/* Loop untill a new population of PopSize number of children
				   have been created */
				for(; NewChildren<Population_Size; NewChildren+=2)
				{
					/* Selecting the 1st parent */
					int Dad = Selection(Population,Population_Size,Value);
					/* Selecting the 2nd parent */
					int Mom = Selection(Population,Population_Size,Value);

					Chromosome Child1,Child2;
					/* Doing the crossover to create 2 new children */
					Crossover(Population[Dad],Population[Mom],Child1,Child2);

					/* Doing a mutation on Child1 and Child2 */
					if((rand()%100)<Mutation_Probability)
					{
						Mutate(Child1,Child1);
						Mutate(Child2,Child2);
					}

					/* Add the 2 children to the array of new children */
					Children[NewChildren]=Child1;
					Children[NewChildren+1]=Child2;
				}

				/* Destroying the old population */
				DestroyPopulation();
				/* Switching the global pointer to the new population */
				Population=Children;

				/* One generation has passed */
				Generation_Count--;
			}

			/* Function to destroy the main population */
			void DestroyPopulation (void)
			{
				/* Loop through all the chromosomes */
				for(int ch=0; ch<Population_Size; ch++)
					Population[ch].Destroy();
				delete [] Population;
				Population=NULL;
			}

			/* Genetic destructor */
			~Genetic()
			{
				if(Population)
					DestroyPopulation();
			}
};

#endif