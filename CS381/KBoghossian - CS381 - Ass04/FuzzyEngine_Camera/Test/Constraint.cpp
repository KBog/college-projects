//#include "Constraint.h"
//
//Constraint::Constraint(Vector3D DegreeOfFreedom_Translation, Vector3D DegreeOfFreedom_Rotation, Vector3D Angles_Limitation_Pos=Vector3D(0,0,0), Vector3D Angles_Limitation_Neg=Vector3D(0,0,0)):
//DOF_T(DegreeOfFreedom_Translation), DOF_R(DegreeOfFreedom_Rotation), Angles_Limit_Pos(Angles_Limitation_Pos), Angles_Limit_Neg(Angles_Limitation_Neg)
//{
//}
//
///* Function to get the forcesSum & torquesSum and return the result by reference */
//void Constraint::Apply_Constraint (Vector3D &ForcesSum, Vector3D &TorquesSum)
//{
//	float X=fabsf(ForcesSum.X), Y=fabsf(ForcesSum.Y), Z=fabsf(ForcesSum.Z);
//	float Magnitude = ForcesSum.GetLength();
//	if(Magnitude)
//	{
//		ForcesSum.X/=Magnitude;
//		ForcesSum.Y/=Magnitude;
//		ForcesSum.Z/=Magnitude;
//	}
//	/* Apply constraint for the translation */
//	X-=DOF_T.X;
//	Y-=DOF_T.Y;
//	Z-=DOF_T.Z;
//	if(X<0)
//		X=0;
//	if(Y<0)
//		Y=0;
//	if(Z<0)
//		Z=0;
//	ForcesSum.SetXYZ(ForcesSum.X*X,ForcesSum.Y*Y,ForcesSum.Z*Z);
//	/* Apply constraint for the rotation */
//	X=fabsf(TorquesSum.X); Y=fabsf(TorquesSum.Y); Z=fabsf(TorquesSum.Z);
//	Magnitude = TorquesSum.GetLength();
//	if(Magnitude)
//	{
//		TorquesSum.X/=Magnitude;
//		TorquesSum.Y/=Magnitude;
//		TorquesSum.Z/=Magnitude;
//	}
//	X-=DOF_R.X;
//	Y-=DOF_R.Y;
//	Z-=DOF_R.Z;
//	if(X<0)
//		X=0;
//	if(Y<0)
//		Y=0;
//	if(Z<0)
//		Z=0;
//
//	TorquesSum.SetXYZ(TorquesSum.X*X,TorquesSum.Y*Y,TorquesSum.Z*Z);
//}
//
//void Constraint::Apply_AnglesConstraint (Quaternion &Orientation_Quat, vector<float> &FirstOrderResult, vector<float> &SecondOrderResult)
//{
//	Vector3D Angles = Orientation_Quat.Set_Quaternion_To_EulerZYX(Orientation_Quat);
//	if(Angles.X+FirstOrderResult[3]>Angles_Limit_Pos.X)
//	{
//		FirstOrderResult[3]=0;
//		Orientation_Quat.Set_EulerZYX_To_Quaternion(Angles.Z,Angles.Y,Angles_Limit_Pos.X);
//	}
//	else if(Angles.X-FirstOrderResult[3]<Angles_Limit_Neg.X)
//	{
//		FirstOrderResult[3]=0;
//		Orientation_Quat.Set_EulerZYX_To_Quaternion(Angles.Z,Angles.Y,Angles_Limit_Neg.X);
//	}
//
//	if(Angles.Y+FirstOrderResult[4]>Angles_Limit_Pos.Y)
//	{
//		FirstOrderResult[4]=0;
//		Orientation_Quat.Set_EulerZYX_To_Quaternion(Angles.Z,Angles_Limit_Pos.Y,Angles.X);
//		
//		SecondOrderResult[3]=Orientation_Quat.constant;
//		SecondOrderResult[4]=Orientation_Quat.imaginary.X;
//		SecondOrderResult[5]=Orientation_Quat.imaginary.Y;
//		SecondOrderResult[6]=Orientation_Quat.imaginary.Z;
//	}
//	/*else if(Angles.Y-FirstOrderResult[4]<Angles_Limit_Neg.Y)
//	{
//		FirstOrderResult[4]=0;
//		Orientation_Quat.Set_EulerZYX_To_Quaternion(Angles.Z,Angles_Limit_Neg.Y,Angles.X);
//	}*/
//
//	if(Angles.Z+FirstOrderResult[5]>Angles_Limit_Pos.Z || Angles.Z-FirstOrderResult[5]<Angles_Limit_Neg.Z)
//	{
//		FirstOrderResult[5]=0;
//		Orientation_Quat.Set_EulerZYX_To_Quaternion(0,Angles.Y,Angles.X);
//	}
//}