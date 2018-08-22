#include "GTime.h"

/* Setting the GTime Seconds member */
void GameTime::SetSeconds(unsigned int sec)
{
	Seconds= sec;
}
/* Setting the GTime Minutes member */
void GameTime::SetMinutes(unsigned int min)
{
	Minutes= min;
}
/* Setting the GTime Hours member */
void GameTime::SetHours(unsigned int hr)
{
	Hours= hr;
}
/* Converting string to time component */
void GameTime::Convert_StrToTime(string str)
{
	/* Time_Case variable represents which case we are dealing with,
	considering the cases are: Seconds=3, Minutes=2, Hours=1 */
	Time_Case=0;

	string tmp_str;

	for(unsigned int i=0; i<str.size(); i++)
	{
	if(str[i]!= ':')
		{
			tmp_str += str[i];
			continue;
		}
		goto Swit;
		Cont:;
	}
	Swit:
	Time_Case++;
	switch(Time_Case)
	{
		case 1:
			Hours= (unsigned int)atoi(tmp_str.c_str());
			tmp_str.clear();
			goto Cont;
			break;
		case 2:
			Minutes= (unsigned int)atoi(tmp_str.c_str());
			tmp_str.clear();
			goto Cont;
			break;
		case 3:
			Seconds= (unsigned int)atoi(tmp_str.c_str());
			tmp_str.clear();
			break;
	}
}
/* Getting the values of the Time component into an Str to Display */
string GameTime::Get_TimeStr(void)
{
	string time_str;
	char *arr= new char[3];
	time_str+= itoa(Hours,arr,10);
	time_str+= ':';
	time_str+= itoa(Minutes,arr,10);
	time_str+= ':';
	time_str+= itoa(Seconds,arr,10);
	return time_str;
}
/* Setting a certain timer for a given job or mission */
void GameTime::Set_Timer(unsigned int sec)
{
	Mission_Accomplished= false;
	Timer_IsOn= true;
	Timer_Sec= sec;
}
/* Getting the timer_Str */
string GameTime::Get_TimerStr(void)
{
	string timer_str;
	char *arr= new char[5];
	timer_str += itoa(Timer_Sec, arr, 10);
	return timer_str;
}
/* Updating time according the number of loops */
void GameTime::Update_Time(void)
{
	TimeLoop++;
	if(TimeLoop >= 59)
	{
		Seconds++;
		TimeLoop= 0;
		if(Seconds >= 59)
		{
			Minutes++;
			Seconds= 0;
			if(Minutes >= 59)
			{
				Hours++;
				Minutes= 0;
			}
		}
		/* In case there was a timer */
		if(Timer_IsOn && !Mission_Accomplished)
		{
			if(Timer_Sec > 0)
				Timer_Sec--;
			else
			{
				Timer_IsOn= false;
				Mission_Accomplished= true;
				Timer_IsOn= false;
			}
		}
	}
}
