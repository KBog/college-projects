#include "GeneticAlgorithm.h"
#include "Standard Header.h"

#define POOLSIZE 16
#define ROWS 10
#define COLUMNS 10

extern Character *Square;
extern Character *BlackS;
extern ViewPort *ViewPort_List;

extern bool STARTRACE;

enum CellColor 
{ 
Red,
Green,
Blue,
Yellow,
Dark_Red,
Cyan,
Orange,
White,
Maroon,
Olive_Green,
Dark_Blue,
Pink,
Dark_Yellow,
Light_Grey,
Dark_Grey,
Dark_Purple
};

struct Cell
{
	Color_RGB color;
	float Delay;
	CellColor ColorName;
};

struct RacerPos
{
	int r,c;
};

class Racer
{
	public:
			RacerPos Position;
			Chromosome *chromosome_color;
			Chromosome *chromosome_position;
			bool ExecutingMove;
			float delay;
			bool BoardVisiting [ROWS][COLUMNS];
};

/* Race class that is derived from the genetic class */
class Race : public Genetic
{
	public:
			/* The board that contains pointer to cells */
			Cell * Board[ROWS][COLUMNS];
			/* The pool of 16 color */
			Cell Pool[POOLSIZE];
			/* The array of racers */
			Racer *Racers;
			/* To know if we found the best solution */
			bool BestSolution;
			/* To know the number of finished racers */
			int NumberOfFinishedRacers;
			/* The 2 arrays of chromosomes for the color
			   priority and for the position priority */
			Chromosome *Population_color;
			Chromosome *Population_position;

			/* Race constructor */
			Race (int population_size, int generation_count, int chromosome_genecount, int gene_length, bool AllowDuplication, int mutation_probability):
			Genetic(population_size,generation_count,chromosome_genecount,gene_length,AllowDuplication,mutation_probability)
			{
				NumberOfFinishedRacers=0;
				BestSolution=false;
				Racers = new Racer [population_size];

				/* Allocate storage for the initial population */
				Population_color = new Chromosome[Population_Size];
				Population_position = new Chromosome[Population_Size];

				int r=0;
				/* Creating the initial population */
				for(int p=0; p<Population_Size; p++)
				{
					/* Add the chromosome to the array of chromosomes of color priority */
					Population_color[p].Create_Chromosome(Chromosome_GeneCount,Gene_Length,AllowDuplication);

					/* Add the chromosome to tha array of chromosomes of the position */
					Population_position[p].Create_Chromosome(1,4,true);
					Population_position[p].Genes = Population_position[p].Decode(r);
					r++;
					if(r>=ROWS)
						r=0;
				}
					

				/* Red */
				Pool[0].color.r=255;
				Pool[0].color.g=0;
				Pool[0].color.b=0;
				Pool[0].Delay=5;
				Pool[0].ColorName=Red;
				/* Green */
				Pool[1].color.r=0;
				Pool[1].color.g=255;
				Pool[1].color.b=0;
				Pool[1].Delay=10;
				Pool[1].ColorName=Green;
				/* Blue */
				Pool[2].color.r=0;
				Pool[2].color.g=0;
				Pool[2].color.b=255;
				Pool[2].Delay=15;
				Pool[2].ColorName=Blue;
				/* Yellow */
				Pool[3].color.r=255;
				Pool[3].color.g=255;
				Pool[3].color.b=0;
				Pool[3].Delay=20;
				Pool[3].ColorName=Yellow;
				/* Dark Red */
				Pool[4].color.r=100;
				Pool[4].color.g=0;
				Pool[4].color.b=0;
				Pool[4].Delay=25;
				Pool[4].ColorName=Dark_Red;
				/* Cyan */
				Pool[5].color.r=0;
				Pool[5].color.g=255;
				Pool[5].color.b=255;
				Pool[5].Delay=30;
				Pool[5].ColorName=Cyan;
				/* Orange */
				Pool[6].color.r=255;
				Pool[6].color.g=128;
				Pool[6].color.b=0;
				Pool[6].Delay=35;
				Pool[6].ColorName=Orange;
				/* White */
				Pool[7].color.r=255;
				Pool[7].color.g=255;
				Pool[7].color.b=255;
				Pool[7].Delay=40;
				Pool[7].ColorName=White;
				/* Maroon */
				Pool[8].color.r=100;
				Pool[8].color.g=50;
				Pool[8].color.b=0;
				Pool[8].Delay=45;
				Pool[8].ColorName=Maroon;
				/* Olive Green */
				Pool[9].color.r=128;
				Pool[9].color.g=128;
				Pool[9].color.b=64;
				Pool[9].Delay=50;
				Pool[9].ColorName=Olive_Green;
				/* Dark Blue */
				Pool[10].color.r=0;
				Pool[10].color.g=0;
				Pool[10].color.b=100;
				Pool[10].Delay=55;
				Pool[10].ColorName=Dark_Blue;
				/* Pink */
				Pool[11].color.r=255;
				Pool[11].color.g=128;
				Pool[11].color.b=255;
				Pool[11].Delay=60;
				Pool[11].ColorName=Pink;
				/* Dark Yellow */
				Pool[12].color.r=128;
				Pool[12].color.g=128;
				Pool[12].color.b=0;
				Pool[12].Delay=65;
				Pool[12].ColorName=Dark_Yellow;
				/* Light Grey */
				Pool[13].color.r=180;
				Pool[13].color.g=180;
				Pool[13].color.b=180;
				Pool[13].Delay=70;
				Pool[13].ColorName=Light_Grey;
				/* Dark Grey */
				Pool[14].color.r=100;
				Pool[14].color.g=100;
				Pool[14].color.b=100;
				Pool[14].Delay=75;
				Pool[14].ColorName=Dark_Grey;
				/* Dark Purple */
				Pool[15].color.r=100;
				Pool[15].color.g=0;
				Pool[15].color.b=100;
				Pool[15].Delay=80;
				Pool[15].ColorName=Dark_Purple;

				/* Setting the random board initially */
				for(int r=0; r<ROWS; r++)
					for(int c=0; c<COLUMNS; c++)
						Board[r][c]=&Pool[rand()%POOLSIZE];

				ResetRacers();
			}

			/* Function to execute the move of the racer */
			void ExecuteBestRacerMove (int racerIdx)
			{
				/* If the racer is not executing a move */
				if(!Racers[racerIdx].ExecutingMove)
				{
					Racers[racerIdx].ExecutingMove=true;
					/* Taking the r & c of the racer */
					int r = Racers[racerIdx].Position.r;
					int c = Racers[racerIdx].Position.c;
					RacerPos BestPosition;
					/* Default value is a big number */
					int HighestPriorityColorIdx=Chromosome_GeneCount+1;
					/* Decoding the bit array into decimal */
					int *Decimals = Population_color[racerIdx].DecodeChromosome();
					
					/* Top move */
					/* if the upper cell is valid and he's not on the starting line and the upper cell is not visited yet */
					if((r-1)>=0 && c!=-1 && !Racers[racerIdx].BoardVisiting[r-1][c])
					{
						/* Setting the cell to visited */
						Racers[racerIdx].BoardVisiting[r-1][c]=true;
						int g=0;
						/* Loop through the genes to get the priority of the current cell color */
						for(; g<Chromosome_GeneCount; g++)
						{
							if(Decimals[g]==(int)Board[r-1][c]->ColorName)
								break;
						}
						if(g<HighestPriorityColorIdx)
						{
							HighestPriorityColorIdx=g;
							BestPosition.r=r-1;
							BestPosition.c=c;
						}
					}
					/* Right move */
					/* if the right cell is valid and he's not on the ending line and the right cell is not visited yet */
					if((c+1)<COLUMNS)
					{
						/* Setting the cell to visited */
						Racers[racerIdx].BoardVisiting[r][c+1]=true;
						int g=0;
						/* Loop through the genes to get the priority of the current cell color */
						for(; g<Chromosome_GeneCount; g++)
						{
							if(Decimals[g]==(int)Board[r][c+1]->ColorName)
								break;
						}
						/* get the highest priority */
						if(g<HighestPriorityColorIdx)
						{
							HighestPriorityColorIdx=g;
							BestPosition.r=r;
							BestPosition.c=c+1;
						}
					}
					/* Bottom move */
					/* if the lower cell is valid and he's not on the starting line and the lower cell is not visited yet */
					if((r+1)<ROWS && c!=-1 && !Racers[racerIdx].BoardVisiting[r+1][c])
					{
						/* Setting the cell to visited */
						Racers[racerIdx].BoardVisiting[r+1][c]=true;
						int g=0;
						/* Loop through the genes to get the priority of the current cell color */
						for(; g<Chromosome_GeneCount; g++)
						{
							if(Decimals[g]==(int)Board[r+1][c]->ColorName)
								break;
						}
						/* get the highest priority */
						if(g<HighestPriorityColorIdx)
						{
							HighestPriorityColorIdx=g;
							BestPosition.r=r+1;
							BestPosition.c=c;
						}
					}

					/* Execute the move */
					Racers[racerIdx].Position.r=BestPosition.r;
					Racers[racerIdx].Position.c=BestPosition.c;
					/* Update the delay */
					Racers[racerIdx].delay=Pool[Decimals[HighestPriorityColorIdx]].Delay;
					Racers[racerIdx].chromosome_color->Fitness+=Racers[racerIdx].delay;

					Racers[racerIdx].delay/=2;

					delete [] Decimals;
				}
				else
				{
					/* Just update the delay variable (racer remains on the cell) */
					Racers[racerIdx].delay--;
					if(Racers[racerIdx].delay<=0)
						Racers[racerIdx].ExecutingMove=false;
				}
			}

			/* Function to update the racers' movement and fitness */
			void UpdateRacers (void)
			{
				
				/* Loop through all the racers to move them */
				for(int ch=0; ch<Population_Size; ch++)
				{
					/* If the racers hasn't reached the finish line */
					if(Racers[ch].Position.c==COLUMNS-1)
					{
						Racers[ch].Position.c=COLUMNS;
						NumberOfFinishedRacers++;
					}
					else if(Racers[ch].Position.c!=COLUMNS)
						ExecuteBestRacerMove(ch);
						
				}
				/* If all the racers have finished racing */
				if(NumberOfFinishedRacers==Population_Size)
				{
					/* Do a generation */
					Generation();
					STARTRACE=false;
					/* Reset the racers to their position */
					ResetRacers();
					/* If we finished all the generations --> we found the
					   best solution */
					if(Generation_Count<=0)
						BestSolution=true;
				}
			}

			void ResetRacers (void)
			{
				NumberOfFinishedRacers=0;
				/* Loop through the population */
				for(int i=0; i<Population_Size; i++)
				{
					/* Resetting the variables */
					Racers[i].ExecutingMove=false;
					Racers[i].delay=0;
					Racers[i].chromosome_color=&Population_color[i];
					Racers[i].chromosome_position=&Population_position[i];
					Racers[i].Position.c=-1;
					Racers[i].Position.r=Racers[i].chromosome_position->DecodeGene_FromGenesArray(0);
					memset(Racers[i].BoardVisiting,0,ROWS*COLUMNS*sizeof(bool));
				}
			}

			void Generation (void)
			{
				
				BestFitChromosome=-1;
				BestFitnessScore=5000;
				WorstFitnessScore=-1;
				TotalFitnessScore=0;

				/* Update the fitness values of all the chromosomes */
				Population=Population_color;
				FitnessFunction();

				int Value = 0;
				if(Selection==Selection_RouletteWheel)
					Value=(int)TotalFitnessScore;
				else
					Value=20;

				/* Set the new number of children to 0 */
				int NewChildren=0;
				/* Allocate space for all the children */
				Chromosome *Children_color = new Chromosome [Population_Size];
				Chromosome *Children_position = new Chromosome [Population_Size];

				/* Executing the replacement method. If the pointer is null 
				   total replacement */
				if(Replacement)
				{
					Replacement(Population_color,Population_Size,Children_color,NewChildren,BestFitChromosome);
					NewChildren=0;
					Replacement(Population_position,Population_Size,Children_position,NewChildren,BestFitChromosome);
				}

				/* Loop untill a new population of PopSize number of children
				   have been created */
				for(; NewChildren<Population_Size; NewChildren+=2)
				{
					/* Selecting the 1st parent */
					int Dad = Selection(Population_color,Population_Size,Value);
					/* Selecting the 2nd parent */
					int Mom = Selection(Population_color,Population_Size,Value);

					Chromosome Child1_color,Child2_color,Child1_position,Child2_position;
					/* Doing the crossover to create 2 new children for the color population */
					Crossover(Population_color[Dad],Population_color[Mom],Child1_color,Child2_color);
					/* Doing the crossover to create 2 new children for the position population */
					Crossover_Continuous_Discrete(Population_position[Dad],Population_position[Mom],Child1_position,Child2_position);
					//Crossover_RealValue(Population_position[Dad],Population_position[Mom],Child1_position,Child2_position);

					/* Doing a mutation on Child1 and Child2 of color if the random value is less than the probability to mutate */
					if((rand()%100)<Mutation_Probability)
					{
						Mutate(Child1_color,Child1_color);
						Mutate(Child2_color,Child2_color);
					}
					/* Doing a mutation on Child1 and Child2 of position if the random value is less than the probability to mutate */
					if((rand()%100)<Mutation_Probability)
					{
						Mutate_NonOrder_RealValue(Child1_position,Child1_position);
						Mutate_NonOrder_RealValue(Child2_position,Child2_position);
					}

					/* Add the 2 children to the array of new children */
					Children_color[NewChildren]=Child1_color;
					Children_color[NewChildren+1]=Child2_color;
					/* Add the 2 children to the array of new children */
					Children_position[NewChildren]=Child1_position;
					Children_position[NewChildren+1]=Child2_position;
				}

				/* Destroying the old population */
				Population=Population_color;
				DestroyPopulation();
				/* Destroying the old population */
				Population=Population_position;
				DestroyPopulation();
				/* Switching the global pointer to the new population */
				Population_color=Children_color;
				Population_position=Children_position;

				/* One generation has passed */
				Generation_Count--;
			}

			void Draw (void)
			{
				Matrix<3> Object_Matrix, Mapping_Matrix, Final;
				/* Building the viewing matrix */
				Viewing_Transformation(ViewPort_List, Mapping_Matrix);

				Vertex Initial_Pos;
				Initial_Pos.x=-200;
				Initial_Pos.y=180;


				for(int r=0; r<ROWS; r++)
					for(int c=0; c<COLUMNS; c++)
					{
						Square->GetSettings().Position.x = Initial_Pos.x + 40*c;
						Square->GetSettings().Position.y = Initial_Pos.y - 40*r;

						Square->GetPrimitives()[0].Color=Square->GetPrimitives()[1].Color=Board[r][c]->color;

						/* Transforming the objects from their model coodinates to the world coord */
						Square->BuildCharacterMatrix(Object_Matrix.M);
						/* Concatinating the final matrix */
						Final=Mapping_Matrix*Object_Matrix;
						/* Drawing the object */
						Square->Draw(Final.M);
					}

				for(int i=0; i<Population_Size; i++)
				{
					BlackS->GetSettings().Position.x = Initial_Pos.x + Racers[i].Position.c*40;
					BlackS->GetSettings().Position.y = Initial_Pos.y - Racers[i].Position.r*40;

					/* Transforming the objects from their model coodinates to the world coord */
					BlackS->BuildCharacterMatrix(Object_Matrix.M);
					/* Concatinating the final matrix */
					Final=Mapping_Matrix*Object_Matrix;
					/* Drawing the object */
					BlackS->Draw(Final.M);
				}
			}

			~Race()
			{
				Population=Population_color;
				DestroyPopulation();
				Population=Population_position;
				DestroyPopulation();
			}
};