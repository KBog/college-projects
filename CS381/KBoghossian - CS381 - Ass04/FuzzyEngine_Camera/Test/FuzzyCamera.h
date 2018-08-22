#include "FuzzyLogic.h"
#include "TestRenderApp.h"

using namespace std;
#define INFINITY 1000

enum FuzzyRulesType { AngleRule_Type, DistanceRule_Type, FollowRule_Type };
float AngleRange=0;
float DistanceRange=0;

class Camera_Fuzzy : public FuzzyLogic
{
	public:
			float AttackPercentage;

			Camera_Fuzzy(int fuzzyrules_count) :
			FuzzyLogic(fuzzyrules_count)
			{
				AttackPercentage=0;

				/* Defining the angle fuzzy sets */
				FuzzyRules[AngleRule_Type].Create_FuzzyRule(3);
				/* Far CW */
				FuzzyRules[AngleRule_Type].Sets[0].Create_FuzzySet(FuzzySet::LeftShoulder_Type,-6,-INFINITY,-INFINITY,-55,-1);
				/* locked */
				FuzzyRules[AngleRule_Type].Sets[1].Create_FuzzySet(FuzzySet::Triangle_Type,0.001f,-1,-0,0,1);
				/* Far CCW */
				FuzzyRules[AngleRule_Type].Sets[2].Create_FuzzySet(FuzzySet::RightShoulder_Type,6,1,55,INFINITY,INFINITY);

				/* Defining the Distance fuzzy sets */
				FuzzyRules[DistanceRule_Type].Create_FuzzyRule(2);
				/* Locked */
				FuzzyRules[DistanceRule_Type].Sets[0].Create_FuzzySet(FuzzySet::LeftShoulder_Type,0,0,0,0,0.1f);
				/* Distance from Close to far */
				FuzzyRules[DistanceRule_Type].Sets[1].Create_FuzzySet(FuzzySet::RightShoulder_Type,1,0.1f,10.5f,4.75f,4.5f);

				/* Defining the Follow fuzzy sets */
				FuzzyRules[FollowRule_Type].Create_FuzzyRule(2);
				/* Follow Low */
				FuzzyRules[FollowRule_Type].Sets[0].Create_FuzzySet(FuzzySet::LeftShoulder_Type,0,0,0,3.5f,5);
				/* Follow High */
				FuzzyRules[FollowRule_Type].Sets[1].Create_FuzzySet(FuzzySet::RightShoulder_Type,0,4,5.5f,6.5f,8);
			}

			/* Function to get the angle between the follower direction and the vector(follower->target) */
			void Get_ObjectAngle (Point3D SourcePos, Vector3D SourceDirection, Point3D TargetPos, Vector3D TargetDirection, float &CurrentAngle, float &CurrentDistance)
			{	
				SourceDirection.SetNormalize();
				
				float dp=0;
				Vector3D cp;
				/* Compute the dot product between the 2 vectors */
				dp = TargetDirection.GetDotProduct(SourceDirection);
				/* Compute the cross product between the 2 vectors */
				cp = SourceDirection.GetCrossProduct(TargetDirection);

				/* Compute the cosine alpha of the 2 vectors */
				float Cosine = dp;
				if(Cosine>1)
					Cosine=1;
				else if(Cosine<0)
					Cosine=0;

				CurrentAngle = acosf(Cosine)*180/3.1415f;
				/* if the cross product is negative --> negate the angle */
				if(cp.Y<0)
					CurrentAngle=-CurrentAngle;
				CurrentDistance=SourcePos.Distance(TargetPos);
			}

			/* Function to return the angle that must adjust the follower to align it to the direction of the target */
			void GetAdjustment_AngleSpeed (Point3D SourcePos, Vector3D SourceDirection, Point3D TargetPos, Vector3D TargetDirection, float &NewAngleAdjustment, float &NewSpeed)
			{
				int CteIdx=0;
				float FuzzyValue=0;

				/* Get the angle between the source and the target */
				float currentAngle;
				float currentDistance;
				Get_ObjectAngle(SourcePos,SourceDirection,TargetPos,TargetDirection,currentAngle,currentDistance);
				AngleRange=currentAngle;
				DistanceRange=currentDistance;

				/* Check the degree of membership of all the rules */
				FuzzyRules[AngleRule_Type].AllSets_DegreeOfMembership(currentAngle);
				FuzzyRules[DistanceRule_Type].AllSets_DegreeOfMembership(currentDistance);

				/* Get the defuzzified value for the angle */
				float AngleAdjustment = Get_FuzzyFinalDegreeOfMembership(AngleRule_Type,OR_Type);
				float SpeedAdjustment = Get_FuzzyFinalDegreeOfMembership(DistanceRule_Type,OR_Type);
				
				/* Get the follow high masked value --> Max(Angle Far CCW,Distance Far) */
				FuzzyRules[FollowRule_Type].Sets[1].MaskedValue = OR(FuzzyRules[AngleRule_Type].Sets[0].DegOfMem,FuzzyRules[DistanceRule_Type].Sets[1].DegOfMem);
				/* Get the follow high masked value --> Max between the old prev max with this Max(Angle Far CW,Distance Far) */
				FuzzyRules[FollowRule_Type].Sets[1].MaskedValue = OR(FuzzyRules[FollowRule_Type].Sets[1].MaskedValue,OR(FuzzyRules[AngleRule_Type].Sets[2].DegOfMem,FuzzyRules[DistanceRule_Type].Sets[1].DegOfMem));
				/* Get the attack medium masked value --> Min(FuelMed,DamageHigh) */
				FuzzyRules[FollowRule_Type].Sets[0].MaskedValue = OR(FuzzyRules[AngleRule_Type].Sets[1].DegOfMem,FuzzyRules[DistanceRule_Type].Sets[1].DegOfMem);

				/* Get the centroid and return the percentage*angleadjutment */
				AttackPercentage = FuzzyRules[FollowRule_Type].Compute_Centroid();
				/* Take the percentage of the angle adjustment and speed adjustment */
				NewAngleAdjustment = AngleAdjustment*AttackPercentage;
				NewSpeed = SpeedAdjustment*AttackPercentage;
			}
};