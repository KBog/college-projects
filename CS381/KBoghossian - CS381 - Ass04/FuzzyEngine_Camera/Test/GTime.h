#ifndef _GTIME_H_
#define _GTIME_H_

#include <string>
using namespace std;

/* Class Time that saves the time of the Game and each level or mission timer */
class GameTime
{
	public:
		/* Time Spent on playing the Game */
		int TimeLoop;
		/* Time Component which be maximum calculated in Hours and minum in Seconds */
		unsigned int Seconds, Minutes, Hours;
		/* Time displayed as string */
		string Time_Str;
		/* Time_Case represent which Time component we are dealing with */
		int Time_Case;
		//Timer is on!
		bool Timer_IsOn;
		/* Variable for the Timer given in seconds */
		unsigned int Timer_Sec;
		/* Variable created to check if the given mission is accomplished or not */
		bool Mission_Accomplished;

		/* Default GTime Constructor */
		GameTime()
		{
			TimeLoop= 0;
			Seconds= Minutes= Hours= 0;
			Time_Case= 0;
			Timer_IsOn= false;
			Timer_Sec= 0;
			Mission_Accomplished= false;
		}
		/* Defined Time into Constructor to set the Time Components */
		GameTime(string str)
		{
			Convert_StrToTime(str);
		}
		/* Setting the GTime Seconds member */
		void SetSeconds(unsigned int sec);

		/* Setting the GTime Minutes member */
		void SetMinutes(unsigned int min);

		/* Setting the GTime Hours member */
		void SetHours(unsigned int hr);

		/* Converting string to time component */
		void Convert_StrToTime(string str);

		/* Getting the values of the Time component into an Str to Display */
		string Get_TimeStr(void);

		/* Setting a certain timer for a given job or mission */
		void Set_Timer(unsigned int sec);

		/* Getting the timer_Str */
		string Get_TimerStr(void);

		/* Updating time according the number of loops */
		void Update_Time(void);

};

#endif