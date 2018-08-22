#include <vector>

/* ODESolver_Method enumeration */
enum ODESolver_Method
{
	Euler_Method=1,
	Heun_Method,
	Runge_Kutta_N4_Method,
};

/* Pure Abstract Class ODESolver */
class ODESolver
{
	/* Protected Members */
	public:	
		/* Old Result */
		std::vector<float> m_OldResult;
		/* New Result */
		std::vector<float> m_Result;
		/* Current Time */
		float m_T;
		/* TimeStep */
		float m_DT;
		/* Ptr to void */
		void *VoidPtr;
		/* Derivative Function */
		void (*Derivative_Function) (float t, const std::vector<float> &CurrentState, std::vector<float>& Derivatives, void *Void_Ptr);

	
		/* ODESolver constructor */
		ODESolver(void (*Derivative_Func) (float , const std::vector<float> &, std::vector<float>&, void *), const std::vector<float> &initialState, float t, float dt, void *Void_Ptr=NULL) : m_OldResult(initialState),m_Result(initialState),Derivative_Function(Derivative_Func),m_T(t),m_DT(dt),VoidPtr(Void_Ptr) {}
		/* ODESolver Destructor */
		virtual ~ODESolver ()=0 {}
		/* Function to set the time t */
		void SetT(float t) { m_T=t; }
		/* Function to get the time t */
		float GetT() const { return m_T; }
		/* Function to set the stepsize */
		void SetStepSize (float dt) { m_DT=dt; }
		/* Function to get the stepsize */
		float GetStepSize (void) const { return m_DT; }
		/* Function to set the Void_Ptr */
		void SetVoidPtr (void *Void_Ptr) { VoidPtr=Void_Ptr; }
		/* Function to get the void ptr */
		void *GetVoidPtr (void) { return VoidPtr; }
		/* Function to set the derivative */
		void SetDerivative (void (*Derivative_Func) (float , const std::vector<float> &, std::vector<float>&, void *)) { Derivative_Function=Derivative_Func; }
		/* Function to reset all the members */
		void Reset (void (*Derivative_Func) (float , const std::vector<float> &, std::vector<float>&, void *), const std::vector<float> &initialState, float t, float dt, void *Void_Ptr=NULL)
		{
			m_OldResult=initialState;
			m_Result=initialState;
			Derivative_Function=Derivative_Func;
			m_T=t;
			m_DT=dt;
			VoidPtr=Void_Ptr;
		}
		/* Function to get the result */
		std::vector<float>& GetResult (void) { return m_Result; }
		/* Function to get the current state */
		std::vector<float>& GetCurrentState (void) { return m_OldResult; }
		///* Function to reset the old solver state according to a desired value */
		//void SetCurrentState (Vector3D Velocity, Vector3D Angular_Velocity) 
		//{ 
		//	m_OldResult[0]=Velocity.X;			m_OldResult[1]=Velocity.Y;			m_OldResult[2]=Velocity.Z;
		//	m_OldResult[3]=Angular_Velocity.X;	m_OldResult[4]=Angular_Velocity.Y;	m_OldResult[5]=Angular_Velocity.Z;
		//}
		/* Function to solve */
		virtual void Solve () =0;
};

/* Euler class derived from ODESolver class */
class Euler : public ODESolver
{
	public:
		/* Constructor to Set the Initial Variables and the number of variables and the derivatives functions */
		Euler(void (*Derivative_Func) (float, const std::vector<float> &, std::vector<float>&, void *), const std::vector<float> &initialState, float t, float dt, void *Void_Ptr=NULL) : ODESolver(Derivative_Func, initialState, t, dt, Void_Ptr) {}
		/* Function to solve all the CurrentStates with the derivatives */
		virtual void Solve (void);
};

/* Heun class derived from ODESolver class */
class Heun : public ODESolver
{
	public:
		/* Constructor to Set the Initial Variables and the number of variables and the derivatives functions */
		Heun(void (*Derivative_Func) (float , const std::vector<float> &, std::vector<float>& , void *), const std::vector<float> &initialState, float t, float dt, void *Void_Ptr=NULL) : ODESolver(Derivative_Func, initialState, t, dt, Void_Ptr) {}
		/* Function to solve all the CurrentStates with the derivatives */
		virtual void Solve (void);
};

/* Heun class derived from ODESolver class */
class Runge_Kutta_N4 : public ODESolver
{
	public:
		/* Constructor to Set the Initial Variables and the number of variables and the derivatives functions */
		Runge_Kutta_N4 (void (*Derivative_Func) (float , const std::vector<float> &, std::vector<float>& , void *), const std::vector<float> &initialState, float t, float dt, void *Void_Ptr=NULL) : ODESolver(Derivative_Func, initialState, t, dt, Void_Ptr) {}
		/* Function to solve all the CurrentStates with the derivatives */
		virtual void Solve (void);
};