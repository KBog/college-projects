#include "Quaternion.h"

#define PIOVER180 0.0174532925199f
#define PIOVER360 0.0087266462599f
#define THREESIXTYOVERPI 114.59155902616464f
#define ONEEIGHTYOVERPI 57.295779f

Quaternion::Quaternion()
{
	SetIdentity();
}


Quaternion::Quaternion(float q, Vector3D v)
{
	constant = q;
	imaginary.X = v.X;
	imaginary.Y = v.Y;
	imaginary.Z = v.Z;
}

Quaternion::Quaternion(float q,float X,float Y,float Z)
{
	constant = q;
	imaginary.X = X;
	imaginary.Y = Y;
	imaginary.Z = Z;
}

Quaternion::~Quaternion()
{}

Quaternion & Quaternion::operator= (const Quaternion & q)
{
	if(this != &q)
	{
		constant = q.constant;
		imaginary.X = q.imaginary.X;
		imaginary.Y = q.imaginary.Y;
		imaginary.Z = q.imaginary.Z;
	}
	return (*this);
}

Quaternion Quaternion::operator+ (const Quaternion & q)
{
	return Quaternion(constant + q.constant,imaginary.X + q.imaginary.X,imaginary.Y + q.imaginary.Y,imaginary.Z + q.imaginary.Z);
}


Quaternion Quaternion::operator- (const Quaternion & q)
{
	return Quaternion(constant - q.constant,imaginary.X - q.imaginary.X,imaginary.Y - q.imaginary.Y,imaginary.Z - q.imaginary.Z);
}


Quaternion Quaternion::operator* (const Quaternion & q)
{
   return Quaternion
   (
		 constant * q.constant - imaginary.X * q.imaginary.X - imaginary.Y * q.imaginary.Y - imaginary.Z * q.imaginary.Z,
		 constant * q.imaginary.X + imaginary.X * q.constant + imaginary.Y * q.imaginary.Z - imaginary.Z * q.imaginary.Y,
		 constant * q.imaginary.Y + imaginary.Y * q.constant + imaginary.Z * q.imaginary.X - imaginary.X * q.imaginary.Z,
		 constant * q.imaginary.Z + imaginary.Z * q.constant + imaginary.X * q.imaginary.Y - imaginary.Y * q.imaginary.X
  );
}


Quaternion Quaternion::operator* (float f)
{
	return Quaternion(constant*f, imaginary.X*f, imaginary.Y*f, imaginary.Z*f);
}

Quaternion Quaternion::operator/ (float f)
{
	return Quaternion(constant/f, imaginary.X/f, imaginary.Y/f, imaginary.Z/f);
}

Quaternion &Quaternion::operator+= (const Quaternion & q)
{
	constant += q.constant;
	imaginary.X += q.imaginary.X;
	imaginary.Y += q.imaginary.Y;
	imaginary.Z += q.imaginary.Z;
	return (*this);
}

Quaternion &Quaternion::operator-= (const Quaternion & q)
{
	constant -= q.constant;
	imaginary.X -= q.imaginary.X;
	imaginary.Y -= q.imaginary.Y;
	imaginary.Z -= q.imaginary.Z;
	return (*this);
}

Quaternion &Quaternion::operator*= (const Quaternion & q)
{
	constant =	constant * q.constant - imaginary.X * q.imaginary.X - imaginary.Y * q.imaginary.Y - imaginary.Z * q.imaginary.Z;
	imaginary.X = constant * q.imaginary.X + imaginary.X * q.constant + imaginary.Y * q.imaginary.Z - imaginary.Z * q.imaginary.Y;
	imaginary.Y = constant * q.imaginary.Y + imaginary.Y * q.constant + imaginary.Z * q.imaginary.X - imaginary.X * q.imaginary.Z;
	imaginary.Z = constant * q.imaginary.Z + imaginary.Z * q.constant + imaginary.X * q.imaginary.Y - imaginary.Y * q.imaginary.X;
	
	return (*this);
}

Quaternion & Quaternion::operator*= (float f)
{
	constant = constant * f;
	imaginary.X = imaginary.X * f;
	imaginary.Y = imaginary.Y * f;
	imaginary.Z = imaginary.Z * f;

	return (*this);
}


Quaternion & Quaternion::operator/= (float f)
{
	constant = constant / f;
	imaginary.X = imaginary.X / f;
	imaginary.Y = imaginary.Y / f;
	imaginary.Z = imaginary.Z / f;

	return (*this);
}


bool Quaternion::operator== (const Quaternion & q)
{
	return((imaginary.X==q.imaginary.X) && (imaginary.Y==q.imaginary.Y) && (imaginary.Z==q.imaginary.Z) && (constant == q.constant));
}

bool Quaternion::operator!= (const Quaternion & q)
{
	return((imaginary.X!=q.imaginary.X) || (imaginary.Y!=q.imaginary.Y) || (imaginary.Z!=q.imaginary.Z) || (constant != q.constant));
}

//friends
bool operator==(const Quaternion & q1,const Quaternion & q2)
{
	return((q1.imaginary.X==q2.imaginary.X) && (q1.imaginary.Y==q2.imaginary.Y) && (q1.imaginary.Z==q2.imaginary.Z) && (q1.constant == q2.constant));
}

bool operator!=(const Quaternion & q1,const Quaternion & q2)
{
	return((q1.imaginary.X!=q2.imaginary.X) || (q1.imaginary.Y!=q2.imaginary.Y) || (q1.imaginary.Z!=q2.imaginary.Z) || (q1.constant != q2.constant));
}


Quaternion operator+(const Quaternion & q1,const Quaternion & q2)
{
	return Quaternion(q1.constant + q2.constant, q1.imaginary.X + q2.imaginary.X, q1.imaginary.Y + q2.imaginary.Y, q1.imaginary.Z + q2.imaginary.Z);
}

Quaternion operator-(const Quaternion & q1,const Quaternion & q2)
{
	return Quaternion(q1.constant - q2.constant, q1.imaginary.X - q2.imaginary.X, q1.imaginary.Y - q2.imaginary.Y, q1.imaginary.Z - q2.imaginary.Z);
}

Quaternion operator*(const Quaternion & q1,const Quaternion & q2)
{
	return Quaternion
	(
		 q1.constant * q2.constant - q1.imaginary.X * q2.imaginary.X - q1.imaginary.Y * q2.imaginary.Y - q1.imaginary.Z * q2.imaginary.Z,
		 q1.constant * q2.imaginary.X + q1.imaginary.X * q2.constant + q1.imaginary.Y * q2.imaginary.Z - q1.imaginary.Z * q2.imaginary.Y,
		 q1.constant * q2.imaginary.Y + q1.imaginary.Y * q2.constant + q1.imaginary.Z * q2.imaginary.X - q1.imaginary.X * q2.imaginary.Z,
		 q1.constant * q2.imaginary.Z + q1.imaginary.Z * q2.constant + q1.imaginary.X * q2.imaginary.Y - q1.imaginary.Y * q2.imaginary.X
	);
}
Quaternion operator*(const Quaternion & q,float f)
{
	return Quaternion(q.constant*f, q.imaginary.X*f, q.imaginary.Y*f, q.imaginary.Z*f);
}

Quaternion operator*(float f,const Quaternion & q)
{
	return Quaternion(f * q.constant, f*q.imaginary.X, f*q.imaginary.Y, f*q.imaginary.Z);
}

//Functions
void Quaternion::SetIdentity()
{
	//set the constant to 1
	imaginary.X = 0.0f;
	imaginary.Y = 0.0f;
	imaginary.Z = 0.0f;
	constant= 1.0f;
}

void Quaternion::Clear()
{
	//set everything to 0
	imaginary.X = 0.0f;
	imaginary.Y = 0.0f;
	imaginary.Z = 0.0f;
	constant= 0.0f;
}

void Quaternion::SetConjugate()
{
	  imaginary.X *= -1.0f;
      imaginary.Y *= -1.0f;
      imaginary.Z *= -1.0f;
}

float Quaternion::GetLength()
{
	return sqrtf(constant*constant + imaginary.X*imaginary.X + imaginary.Y*imaginary.Y + imaginary.Z*imaginary.Z);
}


void Quaternion::SetNormalize()
{
	float length= sqrtf(constant*constant + imaginary.X*imaginary.X + imaginary.Y*imaginary.Y + imaginary.Z*imaginary.Z);
	if(length==0.0f)
		return;
	else 
	{
		float oneOverLength = 1.0f/length;
		imaginary.X *= oneOverLength;
		imaginary.Y *= oneOverLength;
		imaginary.Z *= oneOverLength;
		constant *= oneOverLength;
	}
}


void Quaternion::SetAdditiveInverse()
{
	constant *= -1.0f;
	imaginary.X *= -1.0f;
	imaginary.Y *= -1.0f;
	imaginary.Z *= -1.0f;
}


void Quaternion::SetMultiplicativeInverse()
{
	float lengthSquare = constant*constant + imaginary.X*imaginary.X + imaginary.Y*imaginary.Y + imaginary.Z*imaginary.Z;
	if(lengthSquare == 0.0f)
		return;
	else 
	{
		float oneOverLength = 1.0f/lengthSquare;
		imaginary.X *= (-1.0f*oneOverLength);
		imaginary.Y *= (-1.0f*oneOverLength);
		imaginary.Z *= (-1.0f*oneOverLength);
		constant *= oneOverLength;
	}
}


float Quaternion::GetDotProduct(Quaternion & q)
{
	return (imaginary.X * q.imaginary.X + imaginary.Y * q.imaginary.Y + imaginary.Z * q.imaginary.Z + constant * q.constant);
}



void Quaternion::Set_Vector_Angle_To_Quaternion(Vector3D & v, float & angle) //angle in degrees
{
	float angleOver2 = angle*PIOVER360;//convert the angle to radian and divide it by 2
	float SIN = sinf(angleOver2); //getting the sin of the angle
	//the direction vector should be normalized
	v.SetNormalize();
	imaginary.X = SIN * v.X; //the imaginary parts are multiplied by the sin and the vector
	imaginary.Y = SIN * v.Y;
	imaginary.Z = SIN * v.Z;

	constant = cosf(angleOver2);//the real part is multiplied by the cosine
	SetNormalize();
	
}



void Quaternion::Set_Quaternion_To_Vector_Angle(Vector3D & v, float & angle)
{
	//get the constant
	angle = acosf(constant);

	//get the inverse of the sin of the angle;
	float SININV = 1.0f/sinf(angle);

	//find the vector
	v.X = imaginary.X * SININV;
	v.Y = imaginary.Y * SININV;
	v.Z = imaginary.Z * SININV;

	angle *= THREESIXTYOVERPI;//now we have the angle in degree 
	//first we multiply the angle *2 then convert it to degree, this is why 360/Pi
}


void Quaternion::Set_Quaternion(Vector3D & v1, Vector3D & v2)
{
	//make sure the vectors are normalized
	v1.SetNormalize();
	v2.SetNormalize();

	Vector3D vec = v1.GetCrossProduct(v2);//get the normal of v1 and v2
	float d = v1.GetDotProduct(v2);
	float s = sqrtf((1+d)*2); //get the 1/sinf(angle /2)

	if(s!=0.0f)
	{
		imaginary.X = vec.X / s;
		imaginary.Y = vec.Y / s;
		imaginary.Z = vec.Z / s;
		constant = s/2.0f;
	}
	else return;
}


void Quaternion::Set_EulerZYX_To_Quaternion(float ztheta, float ytheta, float xtheta)//theta are in degree
{
	float coszover2 = cosf(ztheta*PIOVER360);
	float cosyover2 = cosf(ytheta*PIOVER360);
	float cosxover2 = cosf(xtheta*PIOVER360);
	

	float sinzover2 = sinf(ztheta*PIOVER360);
	float sinyover2 = sinf(ytheta*PIOVER360);
	float sinxover2 = sinf(xtheta*PIOVER360);

	constant =    coszover2*cosyover2*cosxover2 + sinzover2*sinyover2*sinxover2;
	imaginary.X = coszover2*cosyover2*sinxover2 - sinzover2*sinyover2*cosxover2;
	imaginary.Y = coszover2*sinyover2*cosxover2 + sinzover2*cosyover2*sinxover2;
	imaginary.Z = sinzover2*cosyover2*cosxover2 - coszover2*sinyover2*sinxover2;
}



//this formula is found in all the books,
//there are 11 cases, but all the books take the euler equation ZYX
//so the matrix corresponds to the ZYX
void Quaternion::Set_Quaternion_To_Matrix(Matrix3D &m)
{
	m.Ma00 = 1.0f - 2.0f*imaginary.Y*imaginary.Y - 2.0f*imaginary.Z*imaginary.Z;
	m.Ma01 = 2.0f*imaginary.X*imaginary.Y - 2.0f*constant*imaginary.Z;
	m.Ma02 = 2.0f*imaginary.X*imaginary.Z + 2.0f*constant*imaginary.Y;
	m.Ma03 = 0.0f;


	m.Ma10 = 2.0f*imaginary.X*imaginary.Y + 2.0f*constant*imaginary.Z;
	m.Ma11 = 1.0f - 2.0f*imaginary.X*imaginary.X - 2.0f*imaginary.Z*imaginary.Z;
	m.Ma12 = 2.0f*imaginary.Y*imaginary.Z - 2.0f*constant*imaginary.X;
	m.Ma13 = 0.0f;


	m.Ma20 = 2.0f*imaginary.X*imaginary.Z - 2.0f*constant*imaginary.Y;
	m.Ma21 = 2.0f*imaginary.Y*imaginary.Z + 2.0f*constant*imaginary.X;
	m.Ma22 = 1.0f - 2.0f*imaginary.X*imaginary.X - 2.0f*imaginary.Y*imaginary.Y;
	m.Ma23 = 0.0f;
	
	
	m.Ma30 = 0.0f;
	m.Ma31 = 0.0f;
	m.Ma32 = 0.0f;
	m.Ma33 = 1.0f;
}


//since we used the ZYX matrix, it is not quranteed to convert a random quaternion to a matrix, take this matrix and convert it back to quaternion
//the value will be the same but the sign will change
//the quaternion should be chosen carefully
void Quaternion::Set_Matrix_To_Quaternion(Matrix3D &m)
{
	float trace = m.Ma00+m.Ma11+m.Ma22;
	//if the trace is positive calculate directly
	if(trace >= 0.0f)
	{
		//by finding the square root
		//it can be found because it is a positive number
		constant = 0.5f*sqrtf(trace+ 1.0f);
		float scalar = 0.25f/constant;
		imaginary.X = (m.Ma12 - m.Ma21)* scalar;
		imaginary.Y = (m.Ma20 - m.Ma02)* scalar;
		imaginary.Z = (m.Ma01 - m.Ma10)* scalar;
	}
	else
    {
		float Ma[4][4];
		Ma[0][0] = m.Ma00;	Ma[0][1] = m.Ma01;	Ma[0][2] = m.Ma02;	Ma[0][3] = m.Ma03;
		Ma[1][0] = m.Ma10;	Ma[1][1] = m.Ma11;	Ma[1][2] = m.Ma12;	Ma[1][3] = m.Ma13;
		Ma[2][0] = m.Ma20;	Ma[2][1] = m.Ma21;	Ma[2][2] = m.Ma22;	Ma[2][3] = m.Ma23;
		Ma[3][0] = m.Ma30;	Ma[3][1] = m.Ma31;	Ma[3][2] = m.Ma32;	Ma[3][3] = m.Ma33;

		//if the trace was negative
		float* q[3] = {&imaginary.X, &imaginary.Y, &imaginary.Z};
        static int componets[3] = {1,2,0};
        int i = 0;
		//we are finding here the largest diagonal value
        if(m.Ma11>m.Ma00)
            i = 1;
        if(m.Ma22>Ma[i][i])
            i = 2;
		//Set j and k to point to the next two components
        int j = componets[i];
        int k = componets[j];

        float scalar = sqrtf(Ma[i][i]-Ma[j][j]-Ma[k][k] + 1.0f);
        *q[i] = 0.5f*scalar;
        scalar = 0.5f/scalar;//scalar now is the largest component
        *q[j] = (Ma[j][i]+Ma[i][j])*scalar;
        *q[k] = (Ma[k][i]+Ma[i][k])*scalar;
		constant = (Ma[k][j]-Ma[j][k])*scalar;

		m.Ma00 = Ma[0][0];	m.Ma01 = Ma[0][1];	m.Ma02 = Ma[0][2];	m.Ma03 = Ma[0][3];
		m.Ma10 = Ma[1][0];	m.Ma11 = Ma[1][1];	m.Ma12 = Ma[1][2];	m.Ma13 = Ma[1][3];
		m.Ma20 = Ma[2][0];	m.Ma21 = Ma[2][1];	m.Ma22 = Ma[2][2];	m.Ma23 = Ma[2][3];
		m.Ma30 = Ma[3][0];	m.Ma31 = Ma[3][1];	m.Ma32 = Ma[3][2];	m.Ma33 = Ma[3][3];
	}	
}

Quaternion Quaternion::GetMultiplicativeInverse()
{
	Quaternion q(constant,imaginary.X,imaginary.Y,imaginary.Z);
	float lengthSquare = constant*constant + imaginary.X*imaginary.X + imaginary.Y*imaginary.Y + imaginary.Z*imaginary.Z;
	if(lengthSquare == 0.0f)
		return q;
	else 
	{
		float oneOverLength = 1.0f/lengthSquare;
		q.imaginary.X *= (-1.0f*oneOverLength);
		q.imaginary.Y *= (-1.0f*oneOverLength);
		q.imaginary.Z *= (-1.0f*oneOverLength);
		q.constant *= oneOverLength;
		return q;
	}
}
Vector3D Quaternion::SetRotate(Vector3D v)//rotate arround the current quaternion
{
	Quaternion temp(0,v),res;

	Quaternion mul = temp * GetMultiplicativeInverse();
	res = mul * (*this);
		return res.imaginary;	
}
Quaternion Quaternion::Slerp(const Quaternion &From, const Quaternion &To, float Interpolation)
{
	Quaternion temp;
	//Get the dot product between the 2 quaternions
	float dotproduct = From.imaginary.X * To.imaginary.X + From.imaginary.Y * To.imaginary.Y + From.imaginary.Z * To.imaginary.Z + From.constant * To.constant;
	
	//if the angle between the 2 quaternions is more than 90degrees
	//we invert one of the quaternions
	if(dotproduct < 0.0f)
	{
		dotproduct = -dotproduct;
		temp = -1.0f * To;
	}
	else temp = To;

	//do the Slerp
	if(dotproduct < 0.95f)
	{
		float angle = acosf(dotproduct);
		float SIN,SININTERPOLATION,SINOMEGA;
		SIN = sinf(angle);
		SININTERPOLATION = sinf(angle*Interpolation);
		SINOMEGA = sinf(angle*(1.0f-Interpolation));
		return (From*SINOMEGA+ temp*SININTERPOLATION)/SIN;
	}
	else//if the angle is too small do it as linear interpolation
		return GetNormalize(From + Interpolation*(temp-From));

}

Vector3D Quaternion::GetDirection(void)
{
	return Vector3D(2.0f * (imaginary.X * imaginary.Z - constant * imaginary.Y),2.0f * (imaginary.Y * imaginary.Z + constant * imaginary.X),1.0f - 2.0f * (imaginary.X * imaginary.X + imaginary.Y * imaginary.Y));
}


Vector3D Quaternion::GetRotationAxis()
{
	float SINOS = 1.0f - constant*constant;

	if (SINOS <= 0.0f)
		return Vector3D(1.0f, 0.0f, 0.0f);

   float SINOVER2 = (1.0f / sqrtf(SINOS));
	
   // Return vector of rotation
   return Vector3D(imaginary.X * SINOVER2, imaginary.Y * SINOVER2, imaginary.Z * SINOVER2);
 }

float Quaternion::GetRotationAngle()
{
   float length = imaginary.X*imaginary.X + imaginary.Y*imaginary.Y + imaginary.Z*imaginary.Z;
 
   if(length < 1e-12)
     return 0.0f;
   else
     return (float)(ONEEIGHTYOVERPI*2.0f*acos(constant));
 }

const Quaternion& Quaternion::Exp()
{                               
  float length = sqrtf(imaginary.X*imaginary.X + imaginary.Y*imaginary.Y + imaginary.Z*imaginary.Z);
  float mul;

  if (length > 1.0e-4)
    mul = sinf(length)/length;
  else
    mul = 1.0f;

  constant = cosf(length);

  imaginary.X *= mul;
  imaginary.Y *= mul;
  imaginary.Z *= mul; 

  return *this;
}

const Quaternion& Quaternion::Log()
{
  float length = sqrtf(imaginary.X*imaginary.X + imaginary.Y*imaginary.Y + imaginary.Z*imaginary.Z);
  length = atanf(length/constant);

  constant = 0.0f;

  imaginary.X *= length;
  imaginary.Y *= length;
  imaginary.Z *= length;

  return *this;
}





//statics
Quaternion Quaternion::GetConjugate(Quaternion &q)
{	
	return Quaternion(q.constant, q.imaginary.X *(-1.0f), q.imaginary.Y *(-1.0f), q.imaginary.Z *(-1.0f));	
}

Vector3D Quaternion::Rotate(Vector3D &v,float angleOfRotation, Vector3D & axisOfRotation)
{

	Quaternion temp(0,v),res;
	Quaternion q(angleOfRotation, v);

	Quaternion mul = temp * q.GetMultiplicativeInverse();
	res = mul * q;
		return res.imaginary;
}

Vector3D Quaternion::Rotate(Vector3D &v, Quaternion &q)
{
	Quaternion temp(0,v),res;

	Quaternion mul = temp * q.GetMultiplicativeInverse();
	res = mul * q;
		return res.imaginary;
	
}
/*
Vector3D Quaternion::SetRotate(Vector3D v)//rotate arround the current quaternion
{
	Quaternion temp(0,v),res;

	Quaternion mul = temp * GetMultiplicativeInverse();
	res = mul * (*this);
		return res.imaginary;	
}
*/

Vector3D Quaternion::GetRotationAxis(Quaternion &q)//return vector of rotation
{
   float SINOS = 1.0f - q.constant*q.constant;
   if(SINOS <= 0.0f)
		return Vector3D(1.0f, 0.0f, 0.0f);

   float SINOVER2 = (1.0f / sqrtf(SINOS));
	
   // Return vector of rotation
   return Vector3D(q.imaginary.X * SINOVER2, q.imaginary.Y * SINOVER2, q.imaginary.Z * SINOVER2);
}

const Quaternion Quaternion::Exp(Quaternion &q)
{   
  float length = sqrtf(q.imaginary.X*q.imaginary.X + q.imaginary.Y*q.imaginary.Y + q.imaginary.Z*q.imaginary.Z);
  float mul;
  if(length > 1.0e-4)
    mul = sinf(length)/length;
  else
    mul = 1.0f;

  return Quaternion(cosf(length),mul*q.imaginary.X,mul *q.imaginary.Y,mul * q.imaginary.Z) ;
}

const Quaternion Quaternion::Log(Quaternion &q)
{
  float length = sqrtf(q.imaginary.X*q.imaginary.X + q.imaginary.Y*q.imaginary.Y + q.imaginary.Z*q.imaginary.Z);
  length = atanf(length/q.constant);

  return Quaternion(0.0f, length*q.imaginary.X,length *q.imaginary.Y,length * q.imaginary.Z);
}

float Quaternion::GetRotationAngle(Quaternion &q)//IN DEGREE
{
   float length = q.imaginary.X*q.imaginary.X + q.imaginary.Y*q.imaginary.Y + q.imaginary.Z*q.imaginary.Z;
 //if the length is so small
   if(length < 1e-12)
     return 0.0f;
   else
     return (float)(ONEEIGHTYOVERPI*2.0f*acos(q.constant));
}

Vector3D Quaternion::GetDirection(Quaternion &q)
{
	return Vector3D(2.0f * (q.imaginary.X * q.imaginary.Z - q.constant * q.imaginary.Y),2.0f * (q.imaginary.Y * q.imaginary.Z + q.constant * q.imaginary.X),1.0f - 2.0f * (q.imaginary.X * q.imaginary.X + q.imaginary.Y * q.imaginary.Y));
}


float Quaternion::GetLength(Quaternion & q)
{
	return sqrtf(q.constant*q.constant + q.imaginary.X*q.imaginary.X + q.imaginary.Y*q.imaginary.Y + q.imaginary.Z*q.imaginary.Z);
}


Quaternion Quaternion::GetNormalize(Quaternion & q)
{
	float length= sqrtf(q.constant*q.constant + q.imaginary.X*q.imaginary.X + q.imaginary.Y*q.imaginary.Y + q.imaginary.Z*q.imaginary.Z);
	if(length==0.0f)
		return q;
	else 
	{
		float oneOverLength = 1.0f/length;
		q.imaginary.X *= oneOverLength;
		q.imaginary.Y *= oneOverLength;
		q.imaginary.Z *= oneOverLength;
		q.constant *= oneOverLength;
		return q;
	}
}

Quaternion Quaternion::GetAdditiveInverse(Quaternion & q)
{
	return Quaternion(q.constant*(-1.0f), q.imaginary.X *(-1.0f), q.imaginary.Y *(-1.0f), q.imaginary.Z *(-1.0f));
}


Quaternion Quaternion::GetMultiplicativeInverse(Quaternion & q)
{
	float lengthSquare = q.constant*q.constant + q.imaginary.X*q.imaginary.X + q.imaginary.Y*q.imaginary.Y + q.imaginary.Z*q.imaginary.Z;
	if(lengthSquare == 0.0f)
		return q;
	else 
	{
		float oneOverLength = 1.0f/lengthSquare;
		q.imaginary.X *= (-1.0f*oneOverLength);
		q.imaginary.Y *= (-1.0f*oneOverLength);
		q.imaginary.Z *= (-1.0f*oneOverLength);
		q.constant *= oneOverLength;
		return q;
	}
}

float Quaternion::GetDotProduct(Quaternion & q1, Quaternion & q2)
{
	return (q1.imaginary.X * q2.imaginary.X + q1.imaginary.Y * q2.imaginary.Y + q1.imaginary.Z * q2.imaginary.Z + q1.constant * q2.constant);
}

	
void Quaternion::Vector_Angle_To_Quaternion(Quaternion & q, Vector3D & v, float angle)
{
	float angleOver2 = angle*PIOVER360;//convert the angle to radian and divide it by 2
	float SIN = sinf(angleOver2); //getting the sin of the angle
	//the direction vector should be normalized
	v.SetNormalize();
	q.imaginary.X = SIN * v.X; //the imaginary parts are multiplied by the sin and the vector
	q.imaginary.Y = SIN * v.Y;
	q.imaginary.Z = SIN * v.Z;

	q.constant = cosf(angleOver2);//the real part is multiplied by the cosine
	q.SetNormalize();
}


Quaternion Quaternion::Vector_Angle_To_Quaternion(Vector3D & v, float angle)
{
	Quaternion q;
	float angleOver2 = angle*PIOVER360;//convert the angle to radian and divide it by 2
	float SIN = sinf(angleOver2); //getting the sin of the angle
	//the direction vector should be normalized
	v.SetNormalize();
	q.imaginary.X = SIN * v.X; //the imaginary parts are multiplied by the sin and the vector
	q.imaginary.Y = SIN * v.Y;
	q.imaginary.Z = SIN * v.Z;

	q.constant = cosf(angleOver2);//the real part is multiplied by the cosine
	q.SetNormalize();
	return q;
}


void Quaternion::Quaternion_To_Vector_Angle(Quaternion & q, Vector3D & v, float & angle)
{
	//get the constant
	angle = acosf(q.constant);

	//get the inverse of the sin of the angle;
	float SININV = 1.0f/sinf(angle);

	//find the vector
	v.X = q.imaginary.X * SININV;
	v.Y = q.imaginary.Y * SININV;
	v.Z = q.imaginary.Z * SININV;

	angle *= THREESIXTYOVERPI;//now we have the angle in degree 
	//first we multiply the angle *2 then convert it to degree, this is why 360/Pi
}


void Quaternion::Get_Quaternion(Quaternion & q, Vector3D & v1, Vector3D & v2)//quaternion of rotation between 2 unit vectors orientations
{
	//make sure the vectors are normalized
	v1.SetNormalize();
	v2.SetNormalize();

	Vector3D vec = v1.GetCrossProduct(v2);//get the normal of v1 and v2
	float d = v1.GetDotProduct(v2);
	float s = sqrtf((1+d)*2); //get the 1/sinf(angle /2)

	if(s!=0.0f)
	{
		q.imaginary.X = vec.X / s;
		q.imaginary.Y = vec.Y / s;
		q.imaginary.Z = vec.Z / s;
		q.constant = s/2.0f;
	}
	else return;
}

Quaternion Quaternion::Get_Quaternion(Vector3D & v1, Vector3D & v2)
{
	Quaternion q(1.0f,0.0f,0.0f,0.0f);
	//make sure the vectors are normalized
	v1.SetNormalize();
	v2.SetNormalize();

	Vector3D vec = v1.GetCrossProduct(v2);//get the normal of v1 and v2
	float d = v1.GetDotProduct(v2);
	float s = sqrtf((1+d)*2); //get the 1/sinf(angle /2)

	if(s!=0.0f)
	{
		q.imaginary.X = vec.X / s;
		q.imaginary.Y = vec.Y / s;
		q.imaginary.Z = vec.Z / s;
		q.constant = s/2.0f;
		return q;
	}
	else return q;

}


void Quaternion::EulerZYX_To_Quaternion(Quaternion & q, float ztheta, float ytheta, float xtheta)
{
	float coszover2 = cosf(ztheta*PIOVER360);
	float cosyover2 = cosf(ytheta*PIOVER360);
	float cosxover2 = cosf(xtheta*PIOVER360);
	

	float sinzover2 = sinf(ztheta*PIOVER360);
	float sinyover2 = sinf(ytheta*PIOVER360);
	float sinxover2 = sinf(xtheta*PIOVER360);

	q.constant =    coszover2*cosyover2*cosxover2 + sinzover2*sinyover2*sinxover2;
	q.imaginary.X = coszover2*cosyover2*sinxover2 - sinzover2*sinyover2*cosxover2;
	q.imaginary.Y = coszover2*sinyover2*cosxover2 + sinzover2*cosyover2*sinxover2;
	q.imaginary.Z = sinzover2*cosyover2*cosxover2 - coszover2*sinyover2*sinxover2;
}

Quaternion Quaternion::EulerZYX_To_Quaternion(float ztheta, float ytheta, float xtheta)
{
	float coszover2 = cosf(ztheta*PIOVER360);
	float cosyover2 = cosf(ytheta*PIOVER360);
	float cosxover2 = cosf(xtheta*PIOVER360);
	

	float sinzover2 = sinf(ztheta*PIOVER360);
	float sinyover2 = sinf(ytheta*PIOVER360);
	float sinxover2 = sinf(xtheta*PIOVER360);

	return Quaternion(coszover2*cosyover2*cosxover2 + sinzover2*sinyover2*sinxover2,
	coszover2*cosyover2*sinxover2 - sinzover2*sinyover2*cosxover2,
	coszover2*sinyover2*cosxover2 + sinzover2*cosyover2*sinxover2,
	sinzover2*cosyover2*cosxover2 - coszover2*sinyover2*sinxover2);
}


void Quaternion::Quaternion_To_Matrix(Quaternion & q, Matrix3D & m)
{
	m.Ma00 = 1.0f - 2.0f*q.imaginary.Y*q.imaginary.Y - 2.0f*q.imaginary.Z*q.imaginary.Z;
	m.Ma01 = 2.0f*q.imaginary.X*q.imaginary.Y - 2.0f*q.constant*q.imaginary.Z;
	m.Ma02 = 2.0f*q.imaginary.X*q.imaginary.Z + 2.0f*q.constant*q.imaginary.Y;
	m.Ma03 = 0.0f;


	m.Ma10 = 2.0f*q.imaginary.X*q.imaginary.Y + 2.0f*q.constant*q.imaginary.Z;
	m.Ma11 = 1.0f - 2.0f*q.imaginary.X*q.imaginary.X - 2.0f*q.imaginary.Z*q.imaginary.Z;
	m.Ma12 = 2.0f*q.imaginary.Y*q.imaginary.Z - 2.0f*q.constant*q.imaginary.X;
	m.Ma13 = 0.0f;


	m.Ma20 = 2.0f*q.imaginary.X*q.imaginary.Z - 2.0f*q.constant*q.imaginary.Y;
	m.Ma21 = 2.0f*q.imaginary.Y*q.imaginary.Z + 2.0f*q.constant*q.imaginary.X;
	m.Ma22 = 1.0f - 2.0f*q.imaginary.X*q.imaginary.X - 2.0f*q.imaginary.Y*q.imaginary.Y;
	m.Ma23 = 0.0f;
	
	
	m.Ma30 = 0.0f;
	m.Ma31 = 0.0f;
	m.Ma32 = 0.0f;
	m.Ma33 = 1.0f;
}



void Quaternion::Matrix_To_Quaternion(Quaternion & q, Matrix3D & m)
{
	float trace = m.Ma00+m.Ma11+m.Ma22;
	//if the trace is positive calculate directly
	if(trace >= 0.0f)
	{
		//by finding the square root
		//it can be found because it is a positive number
		q.constant = 0.5f*sqrtf(trace+ 1.0f);
		float scalar = 0.25f/q.constant;
		q.imaginary.X = (m.Ma12 - m.Ma21)* scalar;
		q.imaginary.Y = (m.Ma20 - m.Ma02)* scalar;
		q.imaginary.Z = (m.Ma01 - m.Ma10)* scalar;
	}
	else
    {
		float Ma[4][4];
		Ma[0][0] = m.Ma00;	Ma[0][1] = m.Ma01;	Ma[0][2] = m.Ma02;	Ma[0][3] = m.Ma03;
		Ma[1][0] = m.Ma10;	Ma[1][1] = m.Ma11;	Ma[1][2] = m.Ma12;	Ma[1][3] = m.Ma13;
		Ma[2][0] = m.Ma20;	Ma[2][1] = m.Ma21;	Ma[2][2] = m.Ma22;	Ma[2][3] = m.Ma23;
		Ma[3][0] = m.Ma30;	Ma[3][1] = m.Ma31;	Ma[3][2] = m.Ma32;	Ma[3][3] = m.Ma33;

		//if the trace was negative
		float* qu[3] = {&q.imaginary.X, &q.imaginary.Y, &q.imaginary.Z};
        static int componets[3] = {1,2,0};
        int i = 0;
		//we are finding here the largest diagonal value
        if(m.Ma11>m.Ma00)
            i = 1;
        if(m.Ma22>Ma[i][i])
            i = 2;
		//Set j and k to point to the next two components
        int j = componets[i];
        int k = componets[j];

        float scalar = sqrtf(Ma[i][i]-Ma[j][j]-Ma[k][k] + 1.0f);
        *qu[i] = 0.5f*scalar;
        scalar = 0.5f/scalar;//scalar now is the largest component
        *qu[j] = (Ma[j][i]+Ma[i][j])*scalar;
        *qu[k] = (Ma[k][i]+Ma[i][k])*scalar;
		q.constant = (Ma[k][j]-Ma[j][k])*scalar;

		m.Ma00 = Ma[0][0];	m.Ma01 = Ma[0][1];	m.Ma02 = Ma[0][2];	m.Ma03 = Ma[0][3];
		m.Ma10 = Ma[1][0];	m.Ma11 = Ma[1][1];	m.Ma12 = Ma[1][2];	m.Ma13 = Ma[1][3];
		m.Ma20 = Ma[2][0];	m.Ma21 = Ma[2][1];	m.Ma22 = Ma[2][2];	m.Ma23 = Ma[2][3];
		m.Ma30 = Ma[3][0];	m.Ma31 = Ma[3][1];	m.Ma32 = Ma[3][2];	m.Ma33 = Ma[3][3];
	}
}


Quaternion Quaternion::Matrix_To_Quaternion(Matrix3D & m)
{
	Quaternion q;
	float trace = m.Ma00+m.Ma11+m.Ma22;
	//if the trace is positive calculate directly
	if(trace >= 0.0f)
	{
		//by finding the square root
		//it can be found because it is a positive number
		q.constant = 0.5f*sqrtf(trace+ 1.0f);
		float scalar = 0.25f/q.constant;
		q.imaginary.X = (m.Ma12 - m.Ma21)* scalar;
		q.imaginary.Y = (m.Ma20 - m.Ma02)* scalar;
		q.imaginary.Z = (m.Ma01 - m.Ma10)* scalar;
		return q;
	}
	else
    {
		float Ma[4][4];
		Ma[0][0] = m.Ma00;	Ma[0][1] = m.Ma01;	Ma[0][2] = m.Ma02;	Ma[0][3] = m.Ma03;
		Ma[1][0] = m.Ma10;	Ma[1][1] = m.Ma11;	Ma[1][2] = m.Ma12;	Ma[1][3] = m.Ma13;
		Ma[2][0] = m.Ma20;	Ma[2][1] = m.Ma21;	Ma[2][2] = m.Ma22;	Ma[2][3] = m.Ma23;
		Ma[3][0] = m.Ma30;	Ma[3][1] = m.Ma31;	Ma[3][2] = m.Ma32;	Ma[3][3] = m.Ma33;

		//if the trace was negative
		float* qu[3] = {&q.imaginary.X, &q.imaginary.Y, &q.imaginary.Z};
        static int componets[3] = {1,2,0};
        int i = 0;
		//we are finding here the largest diagonal value
        if(m.Ma11>m.Ma00)
            i = 1;
        if(m.Ma22>Ma[i][i])
            i = 2;
		//Set j and k to point to the next two components
        int j = componets[i];
        int k = componets[j];

        float scalar = sqrtf(Ma[i][i]-Ma[j][j]-Ma[k][k] + 1.0f);
        *qu[i] = 0.5f*scalar;
        scalar = 0.5f/scalar;//scalar now is the largest component
        *qu[j] = (Ma[j][i]+Ma[i][j])*scalar;
        *qu[k] = (Ma[k][i]+Ma[i][k])*scalar;
		q.constant = (Ma[k][j]-Ma[j][k])*scalar;
		
		m.Ma00 = Ma[0][0];	m.Ma01 = Ma[0][1];	m.Ma02 = Ma[0][2];	m.Ma03 = Ma[0][3];
		m.Ma10 = Ma[1][0];	m.Ma11 = Ma[1][1];	m.Ma12 = Ma[1][2];	m.Ma13 = Ma[1][3];
		m.Ma20 = Ma[2][0];	m.Ma21 = Ma[2][1];	m.Ma22 = Ma[2][2];	m.Ma23 = Ma[2][3];
		m.Ma30 = Ma[3][0];	m.Ma31 = Ma[3][1];	m.Ma32 = Ma[3][2];	m.Ma33 = Ma[3][3];

		return q;
	}
}


Vector3D Quaternion::Set_Quaternion_To_EulerZYX()
{
	float r11, r21, r31,r32,r33,r12,r13,q11,q22,q33,q00;
	float tmp;
	Vector3D u;

	q00 = constant*constant;
	q11 = imaginary.X*imaginary.X;
	q22 = imaginary.Y*imaginary.Y;
	q33 = imaginary.Z*imaginary.Z;

	r11 = q00 + q11 - q22 - q33;
	r21 = 2.0f * (imaginary.X*imaginary.Y + constant*imaginary.Z);
	r31 = 2.0f * (imaginary.X*imaginary.Z - constant*imaginary.Y);
	r32 = 2.0f * (imaginary.Y*imaginary.Z + constant*imaginary.X);

	r33 = q00 - q11 - q22 + q33;

	tmp = (float)(fabs(r31));
	if(tmp > 0.99999999f)
	{
		r12 = 2.0f * (imaginary.X*imaginary.Y - constant*imaginary.Z);
		r13 = 2.0f * (imaginary.X*imaginary.Z + constant*imaginary.Y);

		u.X = 0.0f;//roll
		u.Y = (90.0f*r31)/tmp;//pitch
		u.Z = atan2f(-r12, -r31*r13)*ONEEIGHTYOVERPI;//yaw
		return u;
	}
	u.X = atan2f(r32,r33)* ONEEIGHTYOVERPI;//roll
	u.Y = asinf(-r31)* ONEEIGHTYOVERPI;//pitch
	u.Z = atan2f(r21, r11) * ONEEIGHTYOVERPI;//yaw
	return u;
}
Vector3D Quaternion::Set_Quaternion_To_EulerZYX(Quaternion &q)
{
	float r11, r21, r31,r32,r33,r12,r13,q11,q22,q33,q00;
	float tmp;
	Vector3D u;

	q00 = q.constant*q.constant;
	q11 = q.imaginary.X*q.imaginary.X;
	q22 = q.imaginary.Y*q.imaginary.Y;
	q33 = q.imaginary.Z*q.imaginary.Z;

	r11 = q00 + q11 - q22 - q33;
	r21 = 2.0f * (q.imaginary.X*q.imaginary.Y + q.constant*q.imaginary.Z);
	r31 = 2.0f * (q.imaginary.X*q.imaginary.Z - q.constant*q.imaginary.Y);
	r32 = 2.0f * (q.imaginary.Y*q.imaginary.Z + q.constant*q.imaginary.X);

	r33 = q00 - q11 - q22 + q33;

	tmp = (float)(fabs(r31));
	if(tmp > 0.99999999f)
	{
		r12 = 2.0f * (q.imaginary.X*q.imaginary.Y - q.constant*q.imaginary.Z);
		r13 = 2.0f * (q.imaginary.X*q.imaginary.Z + q.constant*q.imaginary.Y);

		u.X = 0.0f;//roll
		u.Y = (90.0f*r31)/tmp;//pitch
		u.Z = atan2f(-r12, -r31*r13)*ONEEIGHTYOVERPI;//yaw
		return u;
	}
	u.X = atan2f(r32,r33)* ONEEIGHTYOVERPI;//roll
	u.Y = asinf(-r31)* ONEEIGHTYOVERPI;//pitch
	u.Z = atan2f(r21, r11) * ONEEIGHTYOVERPI;//yaw
	return u;
}

void Quaternion::SetIdentity(Quaternion & q)
{
	q.imaginary.X = 0.0f;
	q.imaginary.Y = 0.0f;
	q.imaginary.Z = 0.0f;
	q.constant = 1.0f;
}

void Quaternion::Clear(Quaternion & q)
{
	q.imaginary.X = 0.0f;
	q.imaginary.Y = 0.0f;
	q.imaginary.Z = 0.0f;
	q.constant = 0.0f;
}
