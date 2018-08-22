#include "Viewing.h"
#include "Standard Header.h"
#include "Support_Functions.h"
#include "FuzzyLogic.h"
#include <vector>
#include <list>

using namespace std;
#define INFINITY 1000

#define TRAILSIZE 100
extern ViewPort *ViewPort_List;
extern bool StartFollow;

enum FuzzyRulesType { AngleRule_Type, DamageRule_Type, FuelRule_Type, AttackRule_Type };

class Missile_Fuzzy : public FuzzyLogic
{
	public:
			float AttackPercentage;

			Missile_Fuzzy(int fuzzyrules_count) :
			FuzzyLogic(fuzzyrules_count)
			{
				AttackPercentage=0;

				/* Defining the angle fuzzy sets */
				FuzzyRules[AngleRule_Type].Create_FuzzyRule(7);
				/* TooFar CW */
				FuzzyRules[AngleRule_Type].Sets[0].Create_FuzzySet(FuzzySet::LeftShoulder_Type,-10,INFINITY,INFINITY,-100,-90);
				/* Far CW */
				FuzzyRules[AngleRule_Type].Sets[1].Create_FuzzySet(FuzzySet::Trapezoid_Type,-6,-100,-90,-25,-15);
				/* CW */
				FuzzyRules[AngleRule_Type].Sets[2].Create_FuzzySet(FuzzySet::Trapezoid_Type,-4,-25,-15,-10,0);
				/* locked */
				FuzzyRules[AngleRule_Type].Sets[3].Create_FuzzySet(FuzzySet::Trapezoid_Type,0,-10,-1,1,10);
				/* CCW */
				FuzzyRules[AngleRule_Type].Sets[4].Create_FuzzySet(FuzzySet::Trapezoid_Type,4,0,10,15,25);
				/* Far CCW */
				FuzzyRules[AngleRule_Type].Sets[5].Create_FuzzySet(FuzzySet::Trapezoid_Type,6,15,25,90,100);
				/* TooFar CCW */
				FuzzyRules[AngleRule_Type].Sets[6].Create_FuzzySet(FuzzySet::RightShoulder_Type,10,90,100,INFINITY,INFINITY);

				/* Defining the Damage fuzzy sets */
				FuzzyRules[DamageRule_Type].Create_FuzzyRule(2);
				/* Damage Low */
				FuzzyRules[DamageRule_Type].Sets[0].Create_FuzzySet(FuzzySet::LeftShoulder_Type,0,0,0,3.5f,6);
				/* Damage High */
				FuzzyRules[DamageRule_Type].Sets[1].Create_FuzzySet(FuzzySet::RightShoulder_Type,0,4,6.5f,10,10);

				/* Defining the Fuel fuzzy sets */
				FuzzyRules[FuelRule_Type].Create_FuzzyRule(3);
				/* Fuel Low */
				FuzzyRules[FuelRule_Type].Sets[0].Create_FuzzySet(FuzzySet::LeftShoulder_Type,0,0,0,3,4);
				/* Fuel Medium */
				FuzzyRules[FuelRule_Type].Sets[1].Create_FuzzySet(FuzzySet::Trapezoid_Type,0,3,4.5f,5.5f,7);
				/* Fuel High */
				FuzzyRules[FuelRule_Type].Sets[2].Create_FuzzySet(FuzzySet::RightShoulder_Type,0,6,7.5f,10,10);

				/* Defining the Attack fuzzy sets */
				FuzzyRules[AttackRule_Type].Create_FuzzyRule(3);
				/* Attack Low */
				FuzzyRules[AttackRule_Type].Sets[0].Create_FuzzySet(FuzzySet::LeftShoulder_Type,0,0,0,3.5f,5);
				/* Attack Medium */
				FuzzyRules[AttackRule_Type].Sets[1].Create_FuzzySet(FuzzySet::Trapezoid_Type,0,4,5.5f,6.5f,8);
				/* Attack High */
				FuzzyRules[AttackRule_Type].Sets[2].Create_FuzzySet(FuzzySet::RightShoulder_Type,0,7,8.5f,10,10);
			}

			/* Function to get the angle between the follower direction and the vector(follower->target) */
			float Get_ObjectAngle (VERTEX SourcePos, VECTOR SourceDirection, VERTEX TargetPos)
			{
				/* Compute the vector from the source to the destination */
				VECTOR V;	
				V.x=TargetPos.x-SourcePos.x;
				V.y=TargetPos.y-SourcePos.y;
				
				float dp=0, cp=0, mag1=0, mag2=0;
				/* Compute the dot product between the 2 vectors */
				Vector_DotProduct(dp,V,SourceDirection);
				/* Compute the cross product between the 2 vectors */
				Vector_CrossProduct(cp,SourceDirection,V);
				/* Compute the magnitude of the 2 vectors */
				Vector_Magnitude(mag1,V);
				Vector_Magnitude(mag2,SourceDirection);

				if(dp>1)
					dp=1;
				else if(dp<0)
					dp=0;

				/* Compute the cosine alpha of the 2 vectors */
				float Cosine = dp/(mag1*mag2);

				float angle = acosf(Cosine)*180/3.1415f;
				if(cp<0)
					angle=-angle;

				return angle;
			}

			/* Function to return the angle that must adjust the follower to align it to the direction of the target */
			float GetAdjustmentAngle (VERTEX SourcePos, VECTOR SourceDirection, VERTEX TargetPos, float TargetDamage, int MissileFuel)
			{
				int CteIdx=0;
				float FuzzyValue=0;

				/* Setting the value between 0 & 10 */
				TargetDamage/=10;
				float tmpMissileFuel=((float)MissileFuel)/10;

				/* Get the angle between the source and the target */
				float currentAngle = Get_ObjectAngle(SourcePos,SourceDirection,TargetPos);

				/* Check the degree of membership of all the rules */
				FuzzyRules[AngleRule_Type].AllSets_DegreeOfMembership(currentAngle);
				FuzzyRules[DamageRule_Type].AllSets_DegreeOfMembership(TargetDamage);
				FuzzyRules[FuelRule_Type].AllSets_DegreeOfMembership(tmpMissileFuel);

				/* Get the defuzzified value for the angle */
				float AdjustmentAngle = Get_FuzzyFinalDegreeOfMembership(AngleRule_Type,OR_Type);
				
				/* Get the attack high masked value --> Max(FuelLow,DamageLow) */
				FuzzyRules[AttackRule_Type].Sets[2].MaskedValue = OR(FuzzyRules[FuelRule_Type].Sets[0].DegOfMem,FuzzyRules[DamageRule_Type].Sets[0].DegOfMem);
				/* Get the attack medium masked value --> Min(FuelMed,DamageHigh) */
				FuzzyRules[AttackRule_Type].Sets[1].MaskedValue = AND(FuzzyRules[FuelRule_Type].Sets[1].DegOfMem,FuzzyRules[DamageRule_Type].Sets[1].DegOfMem);
				/* Get the attack low masked value --> Min(FuelHigh,DamageHigh) */
				FuzzyRules[AttackRule_Type].Sets[0].MaskedValue = AND(FuzzyRules[FuelRule_Type].Sets[2].DegOfMem,FuzzyRules[DamageRule_Type].Sets[1].DegOfMem);

				/* Get the centroid and return the percentage*angleadjutment */
				AttackPercentage = FuzzyRules[AttackRule_Type].Compute_Centroid();
				return AdjustmentAngle*AttackPercentage;
			}
};

class Missile
{
	public:
		Character *Shape;
		vector<Point_Float> Trail;
		int Fuel;

		VERTEX InitialPosition;
		VECTOR InitialDirection;
		
		Missile (Character *shape)
		{
			Fuel=100;
			Shape=shape;
			InitialPosition=Shape->GetSettings().Position;
			InitialDirection.x=Shape->GetSettings().Vector_Direction.x=1;
			InitialDirection.y=Shape->GetSettings().Vector_Direction.y=0;
			Shape->GetSettings().Angle=0;
			Trail.clear();
		}

		void Reset (void)
		{
			Fuel=100;
			Shape->GetSettings().Position=InitialPosition;
			InitialDirection.x=Shape->GetSettings().Vector_Direction.x=1;
			InitialDirection.y=Shape->GetSettings().Vector_Direction.y=0;
			Shape->GetSettings().Angle=0;
			Trail.clear();
		}

		void InitMissile (int key)
		{
			if(key==1)
				Shape->GetSettings().Angle+=2;
			if(key==2)
				Shape->GetSettings().Angle-=2;
			
			float Cosine=cosf(Shape->GetSettings().Angle*3.1415f/180),Sine=sinf(Shape->GetSettings().Angle*3.1415f/180);

			VECTOR Tmp;	Tmp.x=Shape->GetSettings().Vector_Direction.x;	Tmp.y=Shape->GetSettings().Vector_Direction.y;
			Shape->GetSettings().Vector_Direction.x = Tmp.x*Cosine - Tmp.y*Sine;
			Shape->GetSettings().Vector_Direction.y = Tmp.x*Sine + Tmp.y*Cosine;

			Vector_Unit(Shape->GetSettings().Vector_Direction,Shape->GetSettings().Vector_Direction);
		}

		void Update (float angle_adjustment)
		{
			static int loops=60;
			Shape->GetSettings().Angle+=angle_adjustment;
			angle_adjustment*=3.1415f/180;
			float Cosine=cosf(Shape->GetSettings().Angle*3.1415f/180),Sine=sinf(Shape->GetSettings().Angle*3.1415f/180);
			
			Shape->GetSettings().Vector_Direction.x = InitialDirection.x*Cosine - InitialDirection.y*Sine;
			Shape->GetSettings().Vector_Direction.y = InitialDirection.x*Sine + InitialDirection.y*Cosine;
			Vector_Unit(Shape->GetSettings().Vector_Direction,Shape->GetSettings().Vector_Direction);

			Shape->GetSettings().Position.x+=Shape->GetSettings().Vector_Direction.x*Shape->GetSettings().Speed;
			Shape->GetSettings().Position.y+=Shape->GetSettings().Vector_Direction.y*Shape->GetSettings().Speed;

			loops--;
			if(loops<=0)
			{
				loops=60;
				Fuel--;
				if(Fuel<0)
					Fuel=0;
			}
			
		}

		void Draw (void)
		{
			Matrix<3> Object_Matrix, Mapping_Matrix, Final;
			/* Building the viewing matrix */
			Viewing_Transformation(ViewPort_List, Mapping_Matrix);
			
			/* Transforming the objects from their model coodinates to the world coord */
			Shape->BuildCharacterMatrix(Object_Matrix.M);
			/* Concatinating the final matrix */
			Final=Mapping_Matrix*Object_Matrix;

			if(StartFollow)
			{
				Point_Float V;
				V.x=Shape->GetPrimitives()[0].V0.x;
				V.y=Shape->GetPrimitives()[0].V0.y;				
				/*Get Transformed Vertices*/				
				V=Final.M*V;
				if(V.x>0 && V.x<WINDOW_WIDTH-1 && V.y>0 && V.y<WINDOW_HEIGHT-1)
					Trail.push_back(V);

				for(unsigned int i=0; i<Trail.size(); i++)
					Graphics_WritePixel((int)Trail[i].x,(int)Trail[i].y,200,200,200);
			}
			/* Drawing the object */
			Shape->Draw(Final.M);	
		}
};