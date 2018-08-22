#include "ODESolver.h"
#include <iostream>

using namespace std;

/* Euler Method Solver */
void Euler::Solve (void)
{
	/* Evaluating the Derivative function */
	Derivative_Function(m_T, m_OldResult, m_Result, VoidPtr);
	/* Euler Method */
	for(vector<float>::size_type i=0; i<m_Result.size(); ++i)
		m_Result[i] = m_OldResult[i] + m_DT * m_Result[i];
	/* Update the old result by the new one */
	m_OldResult=m_Result;
	/* Update the current time by the timestep */
	m_T+=m_DT;
}

/* Heun Method Solver */
void Heun::Solve (void)
{
	vector<float> Temp_Result(m_OldResult.size(), 0);
	
	/* Evaluating the Derivative function py */
	Derivative_Function(m_T, m_OldResult, m_Result, VoidPtr);
	/* Filling the Temp_Result */
	for(vector<float>::size_type i=0; i<m_OldResult.size(); ++i)
		Temp_Result[i]=m_OldResult[i] + m_DT*m_Result[i];
	/* Evaluating the Derivative function with the new values */
	Derivative_Function(m_T + m_DT, Temp_Result, Temp_Result, VoidPtr);

	/* Heun Method */
	for(vector<float>::size_type i=0; i<m_Result.size(); ++i)
		m_Result[i] = m_OldResult[i] + (m_DT*0.5f)*(m_Result[i] + Temp_Result[i]);
	/* Update the old result by the new one */
	m_OldResult=m_Result;
	/* Update the current time by the timestep */
	m_T+=m_DT;
}

/* Runge_Kutta_N4 Solver */
void Runge_Kutta_N4::Solve (void)
{
	/*Variables to make computation faster*/
	float mid_step=m_DT*0.5f;
	vector<float> f1(m_OldResult.size(),0);
	vector<float> f2(m_OldResult.size(),0);
	vector<float> f3(m_OldResult.size(),0);
	vector<float> f4(m_OldResult.size(),0);
	
	/* Evaluating f1 */
	Derivative_Function(m_T, m_OldResult, f1, VoidPtr);
	/* Filling f2 */
	for(vector<float>::size_type i=0; i<m_OldResult.size(); ++i)
		f2[i]=m_OldResult[i] + mid_step*f1[i];
	/* Evaluating f2 */
	Derivative_Function(m_T + mid_step, f2, f2, VoidPtr);
	/* Filling f3 */
	for(vector<float>::size_type i=0; i<m_OldResult.size(); ++i)
		f3[i]=m_OldResult[i] + mid_step*f2[i];
	/* Evaluating f3 */
	Derivative_Function(m_T + mid_step, f3, f3, VoidPtr);
	/* Filling f4 */
	for(vector<float>::size_type i=0; i<m_OldResult.size(); ++i)
		f4[i]=m_OldResult[i] + m_DT*f3[i];
	/* Evaluating f4 */
	Derivative_Function(m_T + m_DT, f4, f4, VoidPtr);
	
	/* Runge_Kutta_N4 Method */
	for(vector<float>::size_type i=0; i<m_Result.size(); ++i)
		m_Result[i] = m_OldResult[i] + (m_DT*0.166667f*(f1[i] + 2*f2[i] + 2*f3[i] + f4[i]));
	/* Update the old result by the new one */
	m_OldResult=m_Result;
	/* Update the current time by the timestep */
	m_T+=m_DT;
}