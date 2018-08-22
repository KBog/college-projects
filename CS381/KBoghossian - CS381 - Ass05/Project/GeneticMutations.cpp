#include "GeneticAlgorithm.h"

/* Function to mutate a parent to create a child using
   SWAP any 2 genes */
void Mutate_Order_Exchange (Chromosome Parent, Chromosome &Child)
{
	int Pos1=0, Pos2=0;

	/* Get a random gene position */
	Pos1=rand()%(Parent.Gene_Count);
	/* Get another random gene position */
	Pos2=rand()%(Parent.Gene_Count);
	/* if the pos2=pos1 --> take another pos2 */
	while(Pos2==Pos1)
		Pos2=rand()%(Parent.Gene_Count);

	/* Craete the child that is similar to the parent but with swapped positions */
	Child.InheritFrom(Parent);
	/* The bit values in the genes array */
	Pos1*=Parent.Gene_Length;
	Pos2*=Parent.Gene_Length;
	/* Swap the genes using the starting gene bit position of 1 to 2 */
	Child.SwapGenes(Pos1,Child,Pos2);
}

/* A function that moves a random gene to a random position */
void Mutate_Order_Insertion (Chromosome Dad, Chromosome &Child1)
{
	/* Take a random gene to be moved */
	int GenePos=rand()%Dad.Gene_Count;
	/* Take a random new position for this gene */
	int InsertionPos=rand()%Dad.Gene_Count;

	/* if the insertion position is equal to the gene pos --> take another random */
	while(InsertionPos==GenePos)
		InsertionPos=rand()%Dad.Gene_Count;

	/* Compute the real gene position in the genes array */
	GenePos*=Dad.Gene_Length;
	InsertionPos*=Dad.Gene_Length;

	/* Craete the child that is similar to the parent but with swapped positions */
	Child1.InheritFrom(Dad);

	/* Move the gene from the GenePos to the InsertionPos */
	Child1.MoveGene(GenePos,InsertionPos);
}

/* A function that picks a random gene pos to invert it */
void Mutate_NonOrder_Binary (Chromosome Dad, Chromosome &Child1)
{
	/* Take a random gene to be moved */
	int GenePos=rand()%Dad.Gene_Count;
	/* Compute the real gene position in the genes array */
	GenePos*=Dad.Gene_Length;

	/* Craete the child that is similar to the parent but with swapped positions */
	Child1.InheritFrom(Dad);

	/* Flip a randomly selected gene (flip all the bits that form the gene) */
	for(int n=0; n<Child1.Gene_Length; n++)
		Child1.Genes[GenePos+n]=!Child1.Genes[GenePos+n];
}

/* A function that picks a random gene pos and adds a delta value to the gene */
void Mutate_NonOrder_RealValue (Chromosome Dad, Chromosome &Child1)
{
	/* Take a random gene to be moved */
	int GenePos=rand()%Dad.Gene_Count;
	/* Compute the real gene position in the genes array */
	GenePos*=Dad.Gene_Length;

	/* Craete the child that is similar to the parent but with swapped positions */
	Child1.InheritFrom(Dad);

	/* Take a random value between 0-->2 */
	int AddSub = rand()%3;
	/* Take the converted to decimal value */
	int DecimalValue = Child1.DecodeGene_FromGenesArray(0);
	/* If the value is less than 9 & it must be augmented */
	if(DecimalValue<9 && AddSub==1)
	{
		/* Add one to the value */
		DecimalValue++;
		/* Set the new value */
		Child1.Set_GeneValue(0,Child1.Decode(DecimalValue),0);
	}
	/* If the value is greater than 0 and must be decreased */
	else if(DecimalValue>0 && !AddSub)
	{
		/* Decrease the value by 1 */
		DecimalValue--;
		/* Set the new value */
		Child1.Set_GeneValue(0,Child1.Decode(DecimalValue),0);
	}
}