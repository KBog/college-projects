#ifndef _FUZZYMINIMAX_H_
#define _FUZZYMINIMAX_H_

#include "FuzzyLogic.h"

using namespace std;

enum FuzzyRulesType { SoldierNbrRule_Type, TimeRule_Type, AttackRule_Type };

class MiniMax_Fuzzy : public FuzzyLogic
{
	public:
			float AttackPercentage;

			MiniMax_Fuzzy(int fuzzyrules_count) :
			FuzzyLogic(fuzzyrules_count)
			{
				AttackPercentage=0;

				/* Defining the Lose or Win fuzzy sets */
				FuzzyRules[SoldierNbrRule_Type].Create_FuzzyRule(4);
				/* Lose High */
				FuzzyRules[SoldierNbrRule_Type].Sets[0].Create_FuzzySet(FuzzySet::LeftShoulder_Type,4,-7,-7,-6,-4);
				/* Lose Medium */
				FuzzyRules[SoldierNbrRule_Type].Sets[1].Create_FuzzySet(FuzzySet::Trapezoid_Type,3,-6,-4,-3,-1);
				/* Lose Low */
				FuzzyRules[SoldierNbrRule_Type].Sets[2].Create_FuzzySet(FuzzySet::Triangle_Type,2,-3,-1,-1,0);
				/* Win High */
				FuzzyRules[SoldierNbrRule_Type].Sets[3].Create_FuzzySet(FuzzySet::RightShoulder_Type,1,-1,0,1000,1000);

				/* Defining the Time fuzzy sets */
				FuzzyRules[TimeRule_Type].Create_FuzzyRule(3);
				/* Time Low */
				FuzzyRules[TimeRule_Type].Sets[0].Create_FuzzySet(FuzzySet::LeftShoulder_Type,0,0,0,2,4);
				/* Time Medium */
				FuzzyRules[TimeRule_Type].Sets[1].Create_FuzzySet(FuzzySet::Trapezoid_Type,0,2,4,6,8);
				/* Time High */
				FuzzyRules[TimeRule_Type].Sets[2].Create_FuzzySet(FuzzySet::RightShoulder_Type,0,6,8,10,10);

				/* Defining the Attack fuzzy sets */
				FuzzyRules[AttackRule_Type].Create_FuzzyRule(3);
				/* Attack Low */
				FuzzyRules[AttackRule_Type].Sets[0].Create_FuzzySet(FuzzySet::LeftShoulder_Type,0,0,0,3.5f,5);
				/* Attack Medium */
				FuzzyRules[AttackRule_Type].Sets[1].Create_FuzzySet(FuzzySet::Trapezoid_Type,0,4,5.5f,6.5f,8);
				/* Attack High */
				FuzzyRules[AttackRule_Type].Sets[2].Create_FuzzySet(FuzzySet::RightShoulder_Type,0,7,8.5f,10,10);
			}

			/* Function to return the angle that must adjust the follower to align it to the direction of the target */
			int Get_Depth (int MaxDepth, int WhiteNbr, int BlackNbr, float TimeToThink)
			{
				int CteIdx=0;
				float FuzzyValue=0;

				/* Get the angle between the source and the target */
				int CurrentDifference = BlackNbr-WhiteNbr;

				/* Check the degree of membership of all the rules */
				FuzzyRules[SoldierNbrRule_Type].AllSets_DegreeOfMembership((float)CurrentDifference);
				FuzzyRules[TimeRule_Type].AllSets_DegreeOfMembership(TimeToThink);
				
				/* Get the attack high value from the max of TimeLow | LoseHigh,LoseMed,LoseLow consecutively */
				float AttackHigh = OR(FuzzyRules[TimeRule_Type].Sets[2].DegOfMem,FuzzyRules[SoldierNbrRule_Type].Sets[0].DegOfMem);
				AttackHigh = OR(AttackHigh, OR(FuzzyRules[TimeRule_Type].Sets[2].DegOfMem,FuzzyRules[SoldierNbrRule_Type].Sets[1].DegOfMem));

				/* Get the attack high value from the max of TimeLow | LoseHigh,LoseMed,LoseLow consecutively */
				float AttackMed = AND(FuzzyRules[TimeRule_Type].Sets[1].DegOfMem,FuzzyRules[SoldierNbrRule_Type].Sets[2].DegOfMem);

				/* Get the attack high value from the max of TimeLow | LoseHigh,LoseMed,LoseLow consecutively */
				float AttackLow = OR(FuzzyRules[TimeRule_Type].Sets[0].DegOfMem,FuzzyRules[SoldierNbrRule_Type].Sets[3].DegOfMem);
				
				FuzzyRules[AttackRule_Type].Sets[2].MaskedValue = AttackHigh;
				FuzzyRules[AttackRule_Type].Sets[1].MaskedValue = AttackMed;
				FuzzyRules[AttackRule_Type].Sets[0].MaskedValue = AttackLow;

				/* Get the centroid and return the percentage*angleadjutment */
				AttackPercentage = FuzzyRules[AttackRule_Type].Compute_Centroid();
				return int(MaxDepth*AttackPercentage);
			}
};

#endif