#ifndef _CONSTRAINT_H_
#define _CONSTRAINT_H_

#include <Application.h>

class Constraint
{
	public:
			/* The 6 degrees of freedom */
			Vector3D DOF_T, DOF_R, Angles_Limit_Pos, Angles_Limit_Neg;

			Constraint(Vector3D DegreeOfFreedom_Translation, Vector3D DegreeOfFreedom_Rotation, Vector3D Angles_Limitation_Pos=Vector3D(0,0,0), Vector3D Angles_Limitation_Neg=Vector3D(0,0,0)):
			DOF_T(DegreeOfFreedom_Translation), DOF_R(DegreeOfFreedom_Rotation), Angles_Limit_Pos(Angles_Limitation_Pos), Angles_Limit_Neg(Angles_Limitation_Neg)
			{
			}

			/* Function to get the forcesSum & torquesSum and return the result by reference */
			void Apply_Constraint (Vector3D &ForcesSum, Vector3D &TorquesSum)
			{
				float X=fabsf(ForcesSum.X), Y=fabsf(ForcesSum.Y), Z=fabsf(ForcesSum.Z);
				float Magnitude = ForcesSum.GetLength();
				if(Magnitude)
				{
					ForcesSum.X/=Magnitude;
					ForcesSum.Y/=Magnitude;
					ForcesSum.Z/=Magnitude;
				}
				/* Apply constraint for the translation */
				X-=DOF_T.X;
				Y-=DOF_T.Y;
				Z-=DOF_T.Z;
				if(X<0)
					X=0;
				if(Y<0)
					Y=0;
				if(Z<0)
					Z=0;
				ForcesSum.SetXYZ(ForcesSum.X*X,ForcesSum.Y*Y,ForcesSum.Z*Z);
				/* Apply constraint for the rotation */
				X=fabsf(TorquesSum.X); Y=fabsf(TorquesSum.Y); Z=fabsf(TorquesSum.Z);
				Magnitude = TorquesSum.GetLength();
				if(Magnitude)
				{
					TorquesSum.X/=Magnitude;
					TorquesSum.Y/=Magnitude;
					TorquesSum.Z/=Magnitude;
				}
				X-=DOF_R.X;
				Y-=DOF_R.Y;
				Z-=DOF_R.Z;
				if(X<0)
					X=0;
				if(Y<0)
					Y=0;
				if(Z<0)
					Z=0;

				TorquesSum.SetXYZ(TorquesSum.X*X,TorquesSum.Y*Y,TorquesSum.Z*Z);
			}


			Vector3D Apply_AnglesConstraint (float Angle_X , float Angle_Y , float Angle_Z , Vector3D AxisOfRotation_X , Vector3D AxisOfRotation_Y , Vector3D AxisOfRotation_Z , Vector3D Angular_Velocity , float Inv_Inertia_X , float Inv_Inertia_Y , float Inv_Inertia_Z , Vector3D TorquesSum)
			{
				Vector3D Lamda;
				float One_Over_TimeStep = 60;
				float ErrorTerm_X=(Angle_X-Angles_Limit_Pos.X);
				float ErrorTerm_Y=(Angle_Y-Angles_Limit_Pos.Y);
				float ErrorTerm_Z=(Angle_Z-Angles_Limit_Pos.Z);
	
				if((Angle_X > Angles_Limit_Pos.X && Angles_Limit_Pos.X) || (Angle_X < Angles_Limit_Neg.X && Angles_Limit_Neg.X))
				{
					Lamda.X = (One_Over_TimeStep*ErrorTerm_X - AxisOfRotation_X.X*(Inv_Inertia_X*TorquesSum.X + One_Over_TimeStep*Angular_Velocity.X)) / (AxisOfRotation_X.X*AxisOfRotation_X.X*Inv_Inertia_X);
					Lamda.Y = (One_Over_TimeStep*ErrorTerm_X - AxisOfRotation_X.Y*(Inv_Inertia_Y*TorquesSum.Y + One_Over_TimeStep*Angular_Velocity.Y)) / (AxisOfRotation_X.Y*AxisOfRotation_X.Y*Inv_Inertia_Y);
					Lamda.Z = (One_Over_TimeStep*ErrorTerm_X - AxisOfRotation_X.Z*(Inv_Inertia_Z*TorquesSum.Z + One_Over_TimeStep*Angular_Velocity.Z)) / (AxisOfRotation_X.Z*AxisOfRotation_X.Z*Inv_Inertia_Z);
					
					//Lamda.X = (One_Over_TimeStep*ErrorTerm_X)/(AxisOfRotation_X.X*AxisOfRotation_X.X*Inv_Inertia_X) - TorquesSum.X/AxisOfRotation_X.X - (One_Over_TimeStep*Angular_Velocity.X)/(AxisOfRotation_X.X*Inv_Inertia_X);
					//Lamda.Y = (One_Over_TimeStep*ErrorTerm_X)/(AxisOfRotation_X.Y*AxisOfRotation_X.Y*Inv_Inertia_Y) - TorquesSum.Y/AxisOfRotation_X.Y - (One_Over_TimeStep*Angular_Velocity.Y)/(AxisOfRotation_X.Y*Inv_Inertia_Y);
					//Lamda.Z = (One_Over_TimeStep*ErrorTerm_X)/(AxisOfRotation_X.Z*AxisOfRotation_X.Z*Inv_Inertia_Z) - TorquesSum.Z/AxisOfRotation_X.Z - (One_Over_TimeStep*Angular_Velocity.Z)/(AxisOfRotation_X.Z*Inv_Inertia_Z);
					
					//Lamda.X = ((-One_Over_TimeStep)*Angular_Velocity.X)/(Inv_Inertia_Tensor.Ma00*AxisOfRotation.X);
					//Lamda.Y = ((-One_Over_TimeStep)*Angular_Velocity.Y)/(Inv_Inertia_Tensor.Ma11*AxisOfRotation.Y);
					//Lamda.Z = ((-One_Over_TimeStep)*Angular_Velocity.Z)/(Inv_Inertia_Tensor.Ma22*AxisOfRotation.Z);

					Lamda.X *= AxisOfRotation_X.X;
					Lamda.Y *= AxisOfRotation_X.Y;
					Lamda.Z *= AxisOfRotation_X.Z;
					return Lamda;
				}

				if((Angle_Y > Angles_Limit_Pos.Y && Angles_Limit_Pos.Y) || (Angle_Y < Angles_Limit_Neg.Y && Angles_Limit_Neg.Y))
				{
					
					Lamda.X = (One_Over_TimeStep*ErrorTerm_Y)/(AxisOfRotation_Y.X*AxisOfRotation_Y.X*Inv_Inertia_X) - TorquesSum.X/AxisOfRotation_Y.X - (One_Over_TimeStep*Angular_Velocity.X)/(AxisOfRotation_Y.X*Inv_Inertia_X);
					Lamda.Y = (One_Over_TimeStep*ErrorTerm_Y)/(AxisOfRotation_Y.Y*AxisOfRotation_Y.Y*Inv_Inertia_Y) - TorquesSum.Y/AxisOfRotation_Y.Y - (One_Over_TimeStep*Angular_Velocity.Y)/(AxisOfRotation_Y.Y*Inv_Inertia_Y);
					Lamda.Z = (One_Over_TimeStep*ErrorTerm_Y)/(AxisOfRotation_Y.Z*AxisOfRotation_Y.Z*Inv_Inertia_Z) - TorquesSum.Z/AxisOfRotation_Y.Z - (One_Over_TimeStep*Angular_Velocity.Z)/(AxisOfRotation_Y.Z*Inv_Inertia_Z);
					
					//Lamda.X = ((-One_Over_TimeStep)*Angular_Velocity.X)/(Inv_Inertia_Tensor.Ma00*AxisOfRotation.X);
					//Lamda.Y = ((-One_Over_TimeStep)*Angular_Velocity.Y)/(Inv_Inertia_Tensor.Ma11*AxisOfRotation.Y);
					//Lamda.Z = ((-One_Over_TimeStep)*Angular_Velocity.Z)/(Inv_Inertia_Tensor.Ma22*AxisOfRotation.Z);

					Lamda.X *= AxisOfRotation_Y.X;
					Lamda.Y *= AxisOfRotation_Y.Y;
					Lamda.Z *= AxisOfRotation_Y.Z;
					return Lamda;
				}
				
				if((Angle_Z > Angles_Limit_Pos.Z && Angles_Limit_Pos.Z) || (Angle_Z < Angles_Limit_Neg.Z && Angles_Limit_Neg.Z))
				{
					
					Lamda.X = (One_Over_TimeStep*ErrorTerm_Z)/(AxisOfRotation_Z.X*AxisOfRotation_Z.X*Inv_Inertia_X) - TorquesSum.X/AxisOfRotation_Z.X - (One_Over_TimeStep*Angular_Velocity.X)/(AxisOfRotation_Z.X*Inv_Inertia_X);
					Lamda.Y = (One_Over_TimeStep*ErrorTerm_Z)/(AxisOfRotation_Z.Y*AxisOfRotation_Z.Y*Inv_Inertia_Y) - TorquesSum.Y/AxisOfRotation_Z.Y - (One_Over_TimeStep*Angular_Velocity.Y)/(AxisOfRotation_Z.Y*Inv_Inertia_Y);
					Lamda.Z = (One_Over_TimeStep*ErrorTerm_Z)/(AxisOfRotation_Z.Z*AxisOfRotation_Z.Z*Inv_Inertia_Z) - TorquesSum.Z/AxisOfRotation_Z.Z - (One_Over_TimeStep*Angular_Velocity.Z)/(AxisOfRotation_Z.Z*Inv_Inertia_Z);
					
					//Lamda.X = ((-One_Over_TimeStep)*Angular_Velocity.X)/(Inv_Inertia_Tensor.Ma00*AxisOfRotation.X);
					//Lamda.Y = ((-One_Over_TimeStep)*Angular_Velocity.Y)/(Inv_Inertia_Tensor.Ma11*AxisOfRotation.Y);
					//Lamda.Z = ((-One_Over_TimeStep)*Angular_Velocity.Z)/(Inv_Inertia_Tensor.Ma22*AxisOfRotation.Z);

					Lamda.X *= AxisOfRotation_Z.X;
					Lamda.Y *= AxisOfRotation_Z.Y;
					Lamda.Z *= AxisOfRotation_Z.Z;
					return Lamda;
				}
				return Vector3D(0,0,0);
			}
};

#endif