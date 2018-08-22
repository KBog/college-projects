#include <math.h>
#include "Standard Header.h"

#ifndef _GENERAL_H_
#define _GENERAL_H_

template<int T>
struct Matrix
{
	float M[3][T];
};

/* Function to return the absolute value of a number */
inline double Abs (double Value)
{return ((Value>0)?Value:-Value);}

/* Function to return the magnitude of a vector */
inline float operator* (VECTOR V)
{return ((float)sqrt(V.x*V.x + V.y*V.y));}

/* Function to compute the dot product of 2 vectors */
inline float operator% (VECTOR V1, VECTOR V2)
{return (V1.x*V2.x + V1.y*V2.y);}

/* This function transforms a vector to its unit vector */
inline void operator~ (VECTOR &V)
{float Magnitude=(float)sqrt(V.x*V.x + V.y*V.y);
V.x/=Magnitude; V.y/=Magnitude;}

/* This function transforms a vector to its unit vector */
inline VECTOR GetUnitVector (VECTOR V)
{
	VECTOR result;
	float Magnitude=(float)sqrt(V.x*V.x + V.y*V.y);
	if(Magnitude!=0)
	{
		result.x=V.x/Magnitude; result.y=V.y/Magnitude;
	}
	else
	{
		result.x=result.y=0;
	}
	return result;
}

/* This function transforms a vector to its opposite */
inline void operator- (VECTOR &V)
{V.x=-V.x; V.y=-V.y;}

/* This function checks if 2 vectors are equal */
inline bool operator==(VECTOR U, VECTOR V)
{return ((U.x==V.x) && (U.y==V.y));}

/* Function to check if all the components of a vector are equal to 0 */
inline bool operator==(VECTOR V, float Value)
{return (((V.x<0.0001)&&(V.x>-0.0001))&&((V.y<0.0001)&&(V.y>-0.0001)));}

/* Initialize Matrix */
template<int T>
inline void operator!(Matrix<T>& M)
{
	for(int i=0; i<T; i++)
	{
		M.M[0][i]=0;
		M.M[1][i]=0;
		M.M[2][i]=0;
	}
}

/* Function to multiply matrices */
template<int T>
inline Matrix<T> operator*(Matrix<T> M, Matrix<T> N)
{
	Matrix<T> result;
	/* Initializing the matrix */
	!result;
	int I, J, i;
	for(I=0; I<T; I++)
	{
		i=0;
		while(i<T)
		{
			for(J=0; J<T; J++)
				result.M[I][i] += M.M[I][J]*N.M[J][i];
			i++;
		}
	}
	return result;
}

#endif