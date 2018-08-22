#ifndef QUATERNION_H
#define QUATERNION_H

#include "TestRenderApp.h"

class Quaternion
{
public:
	float constant;
	Vector3D imaginary;

	Quaternion();
	Quaternion(float q, Vector3D v);
	Quaternion(float q,float X,float Y,float Z);
	~Quaternion();

	void SetParameters(float q,float X,float Y,float Z) {constant = q; imaginary.X = X; imaginary.Y = Y; imaginary.Z = Z;};
	void SetParameters(float q, Vector3D v){constant = q; imaginary.X = v.X; imaginary.Y = v.Y; imaginary.Z = v.Z;};


	Quaternion & operator= (const Quaternion & q);
	Quaternion operator+ (const Quaternion & q);
	Quaternion operator- (const Quaternion & q); 
	Quaternion operator* (const Quaternion & q);
	Quaternion operator* (float f);
	Quaternion operator/ (float f);
	Quaternion & operator+= (const Quaternion & q);
	Quaternion & operator-= (const Quaternion & q);
	Quaternion & operator*= (const Quaternion & q);
	Quaternion & operator*= (float f);
	Quaternion & operator/= (float f);
	bool operator== (const Quaternion & q);
	bool operator!= (const Quaternion & q);

	//friends
	friend bool operator==(const Quaternion & q1,const Quaternion & q2);
    friend bool operator!=(const Quaternion & q1,const Quaternion & q2);
    friend Quaternion operator+(const Quaternion & q1,const Quaternion & q2);
	friend Quaternion operator-(const Quaternion & q1,const Quaternion & q2);
	friend Quaternion operator*(const Quaternion & q1,const Quaternion & q2);
    friend Quaternion operator*(const Quaternion & q,float f);
    friend Quaternion operator*(float f,const Quaternion & q);

	
	const Quaternion& Exp();//exponential
	const Quaternion& Log();//log

	void SetIdentity();
	void Clear();
	Vector3D GetDirection(void);
	void SetConjugate();
	float GetLength();

	void SetNormalize();
	void SetAdditiveInverse();
	void SetMultiplicativeInverse();
	Quaternion GetMultiplicativeInverse();
	float GetDotProduct(Quaternion & q);
	void Set_Vector_Angle_To_Quaternion(Vector3D & v, float & angle);
	void Set_Quaternion_To_Vector_Angle(Vector3D & v, float & angle);
	void Set_Quaternion(Vector3D & v1, Vector3D & v2);//quaternion of rotation between 2 unit vectors orientations
	Vector3D GetRotationAxis();//return vector of rotation
	float GetRotationAngle();//IN DEGREE
	Quaternion Slerp(const Quaternion &From, const Quaternion &To, float Interpolation);
	//still rotate using euler angles
	Vector3D SetRotate(Vector3D v);//rotate arround the current quaternion
	void Set_EulerZYX_To_Quaternion(float ztheta, float ytheta, float xtheta);//Yaw,Pitch,Roll, angles are in degree
	Vector3D Set_Quaternion_To_EulerZYX();
	void Set_Quaternion_To_Matrix(Matrix3D & m);
	void Set_Matrix_To_Quaternion(Matrix3D & m);

	
	static void SetIdentity(Quaternion & q);
	static void Clear(Quaternion & q);

	static const Quaternion Exp(Quaternion & q);//exponential
	static const Quaternion Log(Quaternion & q);//log
	static Vector3D Set_Quaternion_To_EulerZYX(Quaternion &q);

	static Quaternion GetConjugate(Quaternion & q);
	static float GetLength(Quaternion & q);
	static Quaternion GetNormalize(Quaternion & q);
	static Quaternion GetAdditiveInverse(Quaternion & q);
	static Quaternion GetMultiplicativeInverse(Quaternion & q);
	static float GetDotProduct(Quaternion & q1, Quaternion & q2);
	static Vector3D GetDirection(Quaternion &q);

	static Vector3D GetRotationAxis(Quaternion &q);//return vector of rotation
	static float GetRotationAngle(Quaternion &q);//IN DEGREE

	static void Vector_Angle_To_Quaternion(Quaternion & q, Vector3D & v, float angle);
	static Quaternion Vector_Angle_To_Quaternion(Vector3D & v, float angle);
	static void Quaternion_To_Vector_Angle(Quaternion & q, Vector3D & v, float & angle);
	static void Get_Quaternion(Quaternion & q, Vector3D & v1, Vector3D & v2);//quaternion of rotation between 2 unit vectors orientations
	static Quaternion Get_Quaternion(Vector3D & v1, Vector3D & v2);//quaternion of rotation between 2 unit vectors orientations

	
	static Vector3D Rotate(Vector3D & v, Quaternion & q);
	static Vector3D Rotate(Vector3D & v, float angleOfRotation, Vector3D & axisOfRotation);
	static void EulerZYX_To_Quaternion(Quaternion & q, float ztheta, float ytheta, float xtheta);
	static Quaternion EulerZYX_To_Quaternion(float ztheta, float ytheta, float xtheta);
	static void Quaternion_To_Matrix(Quaternion & q, Matrix3D & m);
	static void Matrix_To_Quaternion(Quaternion & q, Matrix3D & m);
	static Quaternion Matrix_To_Quaternion(Matrix3D & m);

protected:
private:
};

#endif