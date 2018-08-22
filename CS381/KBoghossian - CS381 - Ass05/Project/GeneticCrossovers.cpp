#include "GeneticAlgorithm.h"

/* Function to crossover 2 parents at a certain position and
   creating 2 children (binary-nonorder) */
void Crossover_Binary_SinglePoint (Chromosome Dad, Chromosome Mom, Chromosome &Child1, Chromosome &Child2)
{
	/* Take a random bit position in the chromosome can't be the first bit nor the last one 
	   because we'll have children similar to their parents */
	int bitPartition = 1+rand()%(Dad.Bit_Count-2);

	/* Create the 2 children like their parents */
	Child1.InheritFrom(Dad);
	Child2.InheritFrom(Mom);
	
	/* Change the bits from the random position till the end */
	for(int n=bitPartition; n<Dad.Bit_Count; n++)
	{
		Child1.Genes[n]=Mom.Genes[n];
		Child2.Genes[n]=Dad.Genes[n];
	}
}

/* Function to work on continous values, it swaps any gene from MOM to DAD to create Child1
										then the same gene from DAD to MOM to create Child2 */
void Crossover_Continuous_Discrete (Chromosome Dad, Chromosome Mom, Chromosome &Child1, Chromosome &Child2)
{
	/* Take a random gene position */
	int GenePos = rand()%Dad.Gene_Count;

	/* Create the 2 children like their parents */
	Child1.InheritFrom(Dad);
	Child2.InheritFrom(Mom);

	/* The bit values in the genes array */
	GenePos*=Dad.Gene_Length;
	/* Swap the genes using the starting gene bit position of 1 to 2 */
	Child1.SwapGenes(GenePos,Child2,GenePos);
}

/* Function to work on Ordered specific values, it swaps any number of positions from mom and put them
   in dad while checking for duplications */
void Crossover_Order_Partially (Chromosome Dad, Chromosome Mom, Chromosome &Child1, Chromosome &Child2)
{
	/* Get a random number of swapping positions */
	int SwapsCount = 1+rand()%Dad.Gene_Count;

	/* Create an array of random positions */
	int *SwappingPositions = new int [SwapsCount];
	/* Current randPos */
	int curr_randPos=0;
	
	/* Get all the swapping positions randomly */
	for(int p=0; p<SwapsCount; p++)
	{
		/* Take a random position */
repeat:	curr_randPos=rand()%Dad.Gene_Count;

		/* Check for duplications in the array */
		for(int i=0; i<p; i++)
			/* if there is a duplication --> take another number */
			if(SwappingPositions[i]==curr_randPos)
				goto repeat;

		/* Save the position in the array of swapping positions */
		SwappingPositions[p]=curr_randPos;
	}

	/* Create the 2 children like their parents */
	Child1.InheritFrom(Dad);
	Child2.InheritFrom(Mom);

	Chromosome TmpDad, TmpMom;
	/* Perform the swapping process */
	int SimilarValuePos=0;
	/* Loop through all the required swaps */
	for(int p=0; p<SwapsCount; p++)
	{
		/* Create temporary copy from the current children */
		TmpDad.InheritFrom(Child1);
		TmpMom.InheritFrom(Child2);

		/* Get the decimal value from Mom at index the current swappingposition
		   Search for the index of this decimal value but in Dad
		   Swap the index found with the swappingposition index */
		SimilarValuePos = TmpDad.Get_GenePosition( TmpMom.DecodeGene_FromGenesArray(SwappingPositions[p]*TmpMom.Gene_Length) );
		Child1.SwapGenes(SimilarValuePos,Child1,SwappingPositions[p]*TmpMom.Gene_Length);
		/* Get the decimal value from Dad at index the current swappingposition
		   Search for the index of this decimal value but in Mom
		   Swap the index found with the swappingposition index */
		SimilarValuePos = TmpMom.Get_GenePosition( TmpDad.DecodeGene_FromGenesArray(SwappingPositions[p]*TmpDad.Gene_Length) );
		Child2.SwapGenes(SimilarValuePos,Child2,SwappingPositions[p]*TmpDad.Gene_Length);
		
		/* Delete the temporary copy */
		TmpDad.Destroy();
		TmpMom.Destroy();
	}
	/* Delete the array of swapping positions */
	delete [] SwappingPositions;
}

/* Function to work on Ordered specific values, it swaps any number of positions from mom and put them
   in dad then filling the blanks with the remaining unused values */
void Crossover_Order_PositionBased (Chromosome Dad, Chromosome Mom, Chromosome &Child1, Chromosome &Child2)
{
	/* Get a random number of swapping positions */
	int SwapsCount = 1+rand()%Dad.Gene_Count;

	/* Create an array of random positions */
	int *SwappingPositions = new int [SwapsCount];
	/* Create an array of the values that will replace other values */
	int *ReplacingValues = new int [SwapsCount];
	/* Current randPos */
	int curr_randPos=0;
	
	/* Get all the swapping positions randomly */
	for(int p=0; p<SwapsCount; p++)
	{
		/* Take a random position */
repeat:	curr_randPos=rand()%Dad.Gene_Count;

		/* Check for duplications in the array */
		for(int i=0; i<p; i++)
			/* if there is a duplication --> take another number */
			if(SwappingPositions[i]==curr_randPos)
				goto repeat;

		/* Save the position in the array of swapping positions */
		SwappingPositions[p]=curr_randPos;
	}

	/* Create the 2 children like their parents */
	Child1.InheritFrom(Dad);
	Child2.InheritFrom(Mom);

	/* Setting the initial pointers */
	Chromosome *CurrentChild = &Child1;
	Chromosome *CurrentParent1 = &Dad;
	Chromosome *CurrentParent2 = &Mom;
	bool Child2_Created=false;

create_child:
	/* -----------Child creation----------- */
	/* Copy the replacing values to their respective positions in the current child */
	/* Get the replacing values from Parent2 */
	for(int p=0; p<SwapsCount; p++)
	{
		CurrentChild->Set_GeneValue(SwappingPositions[p],CurrentParent2->Genes,SwappingPositions[p]);
		ReplacingValues[p] = CurrentParent2->DecodeGene_FromGenesArray(SwappingPositions[p]*CurrentParent2->Gene_Length);
	}
	int ReplacingValuePos=0;
	/* Loop from 0 --> number of genes */
	for(int blankPos=0; blankPos<Child1.Gene_Count; blankPos++)
	{
		/* If this is a blank position (it isn't found in the randompositions) */
		if(!CurrentParent1->ValueFoundInArray(SwappingPositions,SwapsCount,blankPos))
		{
			/* Loop untill a nonused value is found */
			while(CurrentParent1->ValueFoundInArray(ReplacingValues,SwapsCount,CurrentParent1->DecodeGene_FromGenesArray(ReplacingValuePos*CurrentParent1->Gene_Length)))
				ReplacingValuePos++;
			/* Copy the gene from the parent to the child */
			CurrentChild->Set_GeneValue(blankPos,CurrentParent1->Genes,ReplacingValuePos);
			/* After copying the nonused gene --> update the replacingValuePos to take another value */
			ReplacingValuePos++;
		}
	}
	/* If the 2nd child hasn't been created yet */
	if(!Child2_Created)
	{
		/* force the creation process to create only a second child */
		Child2_Created=true;
		/* Setting the pointers for the second child */
		CurrentChild = &Child2;
		CurrentParent1 = &Mom;
		CurrentParent2 = &Dad;
		/* Create the child */
		goto create_child;
	}

	/* Delete the array of swapping positions */
	delete [] SwappingPositions;
	delete [] ReplacingValues;
}

/* Function to work on ranged values, it swaps any gene from MOM to DAD to create Child1
										then the same gene from DAD to MOM to create Child2 */
void Crossover_RealValue (Chromosome Dad, Chromosome Mom, Chromosome &Child1, Chromosome &Child2)
{
	/* Take a random gene position */
	int GenePos = rand()%Dad.Gene_Count;

	/* Create the 2 children like their parents */
	Child1.InheritFrom(Dad);
	Child2.InheritFrom(Mom);

	/* The bit values in the genes array */
	GenePos*=Dad.Gene_Length;
	
	/* Convert the gene to decimal */
	int DecimalValue2 = Child1.DecodeGene_FromGenesArray(0);
	/* Convert the gene of the second chromosome to decimal */
	int DecimalValue = Child2.DecodeGene_FromGenesArray(0);
	int result=9;
	/* if the result is out of range and the decimalValue is less than 9 --> Take the first decimal and compute the 
	   new decimal value */
	while(result>=9 && DecimalValue<9)
		result = DecimalValue + rand()%(abs(DecimalValue2-DecimalValue)+1);
	/* Set the decimal value to the child */
	Child1.Set_GeneValue(0,Child1.Decode(result),0);
	result=9;
	/* if the result is out of range and the decimalValue is less than 9 --> Take the second decimal and compute the 
	   new decimal value */
	while(result>=9 && DecimalValue2<9)
		result = DecimalValue2 + rand()%(abs(DecimalValue2-DecimalValue)+1);
	/* Set the decimal value to the child */
	Child2.Set_GeneValue(0,Child2.Decode(result),0);
}