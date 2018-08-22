#ifndef _FUZZYLOGIC_H_
#define _FUZZYLOGIC_H_

/* struct to store the fuzzy set properties */
struct FuzzySet
{
	/* Setting all the available types for one set */
	enum FuzzySetType 
	{ 
		Triangle_Type, 
		Trapezoid_Type, 
		LeftShoulder_Type, 
		RightShoulder_Type 
	};
	/* The Fuzzyset type */
	FuzzySetType Type;
	/* The 2 lower bounds of the set */
	float LowerBound_1, LowerBound_2;
	/* The 2 upper bounds of the set */
	float UpperBound_1, UpperBound_2;
	/* The constant used for the fuzzy control */
	float Constant;
	
	/* The Degree of membership of the value in this set */
	float DegOfMem;
	/* The Masked value */
	float MaskedValue;

	/* Function to create the fuzzy set */
	void Create_FuzzySet (FuzzySetType type, float constant, float lowerbound_1, float upperbound_1, float upperbound_2, float lowerbound_2)
	{
		Type=type;
		LowerBound_1=lowerbound_1;
		UpperBound_1=upperbound_1;
		UpperBound_2=upperbound_2;
		LowerBound_2=lowerbound_2;
		Constant=constant;
		MaskedValue=1;
		DegOfMem=0;
	}

	/* Function that returns the degree of membership of the current angle by checking in the 
		   current fuzzy set - it returns true if it belongs else if it doesn't */
	bool DegreeOfMembership (float &currentValue, float &degofmem)
	{
		degofmem=0;
		/* Check the type of the fuzzy set */
		switch(Type)
		{
			case LeftShoulder_Type:
				/* if between the lowerbound1 & upperbound1 */
				if(currentValue<UpperBound_2)
					degofmem=1;
				/* if between the 2 upperbounds */
				else if(currentValue>LowerBound_2)
					degofmem=0;
				/* if between the upperbound2 & lowerbound2 */
				else
					degofmem=(currentValue-LowerBound_2)/(UpperBound_2-LowerBound_2);
				/* else greater than the upper bound */
				break;

			case RightShoulder_Type:
				/* if between the lowerbound1 & upperbound1 */
				if(currentValue>UpperBound_1)
					degofmem=1;
				/* if between the 2 upperbounds */
				else if(currentValue<LowerBound_1)
					degofmem=0;
				/* if between the upperbound2 & lowerbound2 */
				else
					degofmem=(currentValue-LowerBound_1)/(UpperBound_1-LowerBound_1);
				/* else greater than the upper bound */
				break;

			case Trapezoid_Type:
				/* Checking if less than the lowerbound */
				if(currentValue<=LowerBound_1)
					degofmem=0;
				/* if between the lowerbound1 & upperbound1 */
				else if(currentValue>LowerBound_1 && currentValue<=UpperBound_1)
					degofmem=(currentValue-LowerBound_1)/(UpperBound_1-LowerBound_1);
				/* if between the 2 upperbounds */
				else if(currentValue>UpperBound_1 && currentValue<=UpperBound_2)
					degofmem=1;
				/* if between the upperbound2 & lowerbound2 */
				else if(currentValue>UpperBound_2 && currentValue<=LowerBound_2)
					degofmem=(currentValue-LowerBound_2)/(UpperBound_2-LowerBound_2);
				/* else greater than the upper bound */
				break;

			case Triangle_Type:
				/* Checking if less than the lowerbound */
				if(currentValue<=LowerBound_1)
					degofmem=0;
				/* if between the lowerbound1 & upperbound1 */
				else if(currentValue>LowerBound_1 && currentValue<=UpperBound_1)
					degofmem=(currentValue-LowerBound_1)/(UpperBound_1-LowerBound_1);
				/* if between the upperbound2 & lowerbound2 */
				else if(currentValue>UpperBound_2 && currentValue<=LowerBound_2)
					degofmem=(currentValue-LowerBound_2)/(UpperBound_2-LowerBound_2);
				/* else greater than the upper bound */
				break;
		}
		/* Setting the degree of mem of this set to be the found one */
		DegOfMem=degofmem;
		/* if we have a degree of membership --> return true */
		return (degofmem)?true:false;
	}
};

/* The Fuzzy rule struct */
struct FuzzyRule
{
	/* All the sets in the rule */
	FuzzySet *Sets;
	/* The number of sets we have in the array */
	int Sets_Count;
	
	/* The 2 indices that will be created when getting the fuzzy value */
	int SetIdx_1, SetIdx_2;
	/* The 2 degree of membership that will be created if the current value belongs
	   to 1 or 2 sets */
	float DegOfMem_1, DegOfMem_2;

	/* FuzzyRule construtor */
	void Create_FuzzyRule (int sets_count)
	{
		Sets_Count=sets_count;
		Sets = new FuzzySet [sets_count];
		SetIdx_1=SetIdx_2=-1;
		DegOfMem_1=DegOfMem_2=0;
	}

	/* Fuzzy rule destructor */
	~FuzzyRule()
	{
		delete [] Sets;
	}

	/* Function that returns the final degree of membership */
	void AllSets_DegreeOfMembership (float currentValue)
	{
		int counter=0;
		float none=0;

		/* Loop through all the fuzzy sets */
		for(int fs=0; fs<Sets_Count; fs++)
		{
			/* For the first degree of membership */
			if(!counter)
			{
				/* If we have a degree of membership other than 0 --> fill the degree
		           of membership in the fuzzy rule variable */
				if(Sets[fs].DegreeOfMembership(currentValue,DegOfMem_1))
				{
					/* Save the set's ID */
					SetIdx_1=fs;
					/* Update the number of chosen degofmem */
					counter++;
				}
			}
			/* Check the DegOfMem for the second degree of membership */
			else if(counter==1)
			{
				/* If we have a degree of membership other than 0 --> fill the degree
		           of membership in the fuzzy rule variable */
				if(Sets[fs].DegreeOfMembership(currentValue,DegOfMem_2))
				{
					/* Save the set's ID */
					SetIdx_2=fs;
					/* Update the number of chosen degofmem */
					counter++;
				}
			}
			/* Reset the DegOfMem of the current set */
			else
				Sets[fs].DegreeOfMembership(currentValue,none);
		}
	}

	/* Function to compute the centroid */
	float Compute_Centroid (void)
	{
		float cur_low1=0, next_low1=0;
		float num=0, deno=0;

		/* Loop through all the sets */
		for(int fs=0; fs<Sets_Count; fs++)
		{
			/* Getting the current range */
			/* If it is the last set --> take last_low2-last_low1 */
			if(fs==Sets_Count-1)
			{
				cur_low1=Sets[fs].LowerBound_1;
				next_low1=Sets[fs].LowerBound_2;
			}
			else
			{
				cur_low1=Sets[fs].LowerBound_1;
				next_low1=Sets[fs+1].LowerBound_1;
			}

			/* Update the current denominator */
			deno += (next_low1-cur_low1)*Sets[fs].MaskedValue;

			/* Update the current numerator */
			while(next_low1>cur_low1)
			{
				num += next_low1*Sets[fs].MaskedValue;
				next_low1--;
			}
		}
		/* Retuning the normalized centroid */
		return (num/deno)/Sets[Sets_Count-1].UpperBound_2;
	}
};

/* Fuzzy Logic Class */
class FuzzyLogic
{
	public:
		/* The Constants defined at initial */
		FuzzyRule *FuzzyRules;
		/* Number of fuzzy rules we have in this fuzzy logic */
		int FuzzyRules_Count;

		enum LogicOperator { AND_Type, OR_Type, NOT_Type };

		/* Fuzzy logic default constructor */
		FuzzyLogic (int fuzzyrules_count) : FuzzyRules_Count(fuzzyrules_count)
		{
			FuzzyRules = new FuzzyRule [FuzzyRules_Count];
		}

		/* Fuzzy logic destructor */
		~FuzzyLogic ()
		{
			delete [] FuzzyRules;
		}

		/* Function to return the angle that the follower must rotate according to the Constant specified
		   in the constant array and the degree of membership found */
		float FuzzyControl (int RuleIdx, float &DegOfMem, int &CteIdx)
		{
			return DegOfMem*FuzzyRules[RuleIdx].Sets[CteIdx].Constant;
		}

		/* Function to get the final degree of membership using any of the fuzzy logic operators */
		float Get_FuzzyFinalDegreeOfMembership (int RuleIdx, LogicOperator Operator)
		{
			int CteIdx=0;
			float FuzzyValue=0;

			/* if we have 2 degree of membership --> call the logic operation */
			if(FuzzyRules[RuleIdx].DegOfMem_1 && FuzzyRules[RuleIdx].DegOfMem_2)
			{
				/* Performing the logic operator */
				if(Operator==AND_Type)	//AND
					FuzzyValue=AND(FuzzyRules[RuleIdx].DegOfMem_1,FuzzyRules[RuleIdx].DegOfMem_2);
				else if(Operator==OR_Type)	//OR
					FuzzyValue=OR(FuzzyRules[RuleIdx].DegOfMem_1,FuzzyRules[RuleIdx].DegOfMem_2);				

				/* If the chosen fuzzyvalue is the same as the degofmem_1 --> save the chosen index to be the 1 */
				if(FuzzyValue==FuzzyRules[RuleIdx].DegOfMem_1)
					CteIdx=FuzzyRules[RuleIdx].SetIdx_1;
				/* else save the 2 */
				else
					CteIdx=FuzzyRules[RuleIdx].SetIdx_2;
			}
			/* if only we have the first degree of membership --> return */
			/* we have the second degree of membership */
			else if(FuzzyRules[RuleIdx].DegOfMem_1)
			{
				/* Perform the Not operator */
				if(Operator==NOT_Type)	//NOT
					FuzzyValue=NOT(FuzzyRules[RuleIdx].DegOfMem_1);
				else
					FuzzyValue=FuzzyRules[RuleIdx].DegOfMem_1;

				CteIdx=FuzzyRules[RuleIdx].SetIdx_1;
			}
			else
			{
				if(Operator==NOT_Type)	//NOT
					FuzzyValue=NOT(FuzzyRules[RuleIdx].DegOfMem_2);
				else
					FuzzyValue=FuzzyRules[RuleIdx].DegOfMem_2;

				CteIdx=FuzzyRules[RuleIdx].SetIdx_2;
			}

			/* Get the defuzzified value for the angle */
			return FuzzyControl(RuleIdx,FuzzyValue,CteIdx);
		}

		/* Compute the minimum between the 2 degofmembership values */
		float AND (float V1, float V2)
		{
			/* Returning the min */
			return min(V1,V2);
		}

		/* Compute the maximum between the 2 degofmembership values */
		float OR (float V1, float V2)
		{
			/* Returning the max */
			return max(V1,V2);
		}

		/* Compute the 1-V value */
		float NOT (float V)
		{
			return (1-V);
		}
};

#endif