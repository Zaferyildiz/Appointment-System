/* @Author
Student Name: <Zafer Yıldız>
Student ID : <150170054>
Date: <13/11/2019> */


#ifndef TASK_MANAGEMENT_TOOL
#define TASK_MANAGEMENT_TOOL

#include <stdio.h>

#define strnicmp strncasecmp

#include "task.h"

struct WorkPlan{
	void create();
	void close();
	void add(Task *task);
	void checkAvailableNextTimesFor(Task *delayed);
	int  getUsableDay();
	int getUsableTime();
	void remove(Task *target);
	void display(bool verbose, bool testing);
	void delayAllTasksOfDay(int day);
	Task * getTask(int day, int time);
	bool checkCycledList();

	
	Task *head;
	int usable_day;
	int usable_time;
	int day_count;
};
#endif 

