/* @Author
Student Name: <Zafer Yıldız>
Student ID : <150170054>
Date: <13/11/2019> */

/*
PLEASE, DO NOT CHANGE void display(bool verbose, bool testing), int getUsableDay() and int getUsableTime() FUNCTIONS.
YOU HAVE TO WRITE THE REQUIRED  FUNCTIONS THAT IS MENTIONED ABOVE. YOU CAN ADD NEW FUNCTIONS IF YOU NEED.
*/

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <iomanip>

#include "task_management_tool.h"

using namespace std;


void WorkPlan::display(bool verbose,bool testing)
{
    string inone="***";
    if (head!=NULL)
    {
        Task *pivot =new Task;
        Task *compeer =new Task;

        pivot=head;
        do
        {
            if(testing)
                inone+=" ";
            else
                cout << pivot->day <<". DAY"<< endl;
            compeer=pivot;
            while(compeer!=NULL)
            {
                string PREV= compeer->previous!=NULL?compeer->previous->name:"NULL";
                string NEXT=compeer->next!=NULL?compeer->next->name:"NULL";
                string CONT=compeer->counterpart!=NULL?compeer->counterpart->name:"NULL";
                if (testing)
                    inone+=compeer->name;
                else
                if(verbose)
                    cout<<"\t"<<setw(2)<< compeer->time <<":00\t"<< PREV<<"\t<- "<< compeer->name <<"("<< compeer->priority <<")->\t"<<NEXT <<"\t |_"<<CONT<<endl;
                else
                    cout<<"\t"<<setw(2)<< compeer->time <<":00\t"<< compeer->name <<"("<< compeer->priority <<")"<<endl;
                compeer=compeer->counterpart;
            }
            pivot=pivot->next;
        }while(pivot!=head);
        if(testing)
        {
            cout<<inone<<endl;
            cout<<"(checking cycled list:";
            if (checkCycledList())
                cout<<" PASS)"<<endl;
            else
                cout<<" FAIL)"<<endl;
        }
    }
    else
        cout<<"There is no task yet!"<<endl;
}

int WorkPlan::getUsableDay()
{
	return usable_day;
}

int WorkPlan::getUsableTime()
{
	return usable_time;
}


void WorkPlan::create()
{
	head = NULL;
	usable_day = 0;
	usable_time = 0;
	day_count = 0;
}

void WorkPlan::close()
{
    Task* temp;
    Task* appointment;
    while(this->day_count > 0){
	    temp = head;
	    head = head->next;
	    appointment = temp->counterpart;
	    while(appointment){
	        temp->counterpart = temp->counterpart->counterpart;
	        delete appointment;
	        appointment = temp->counterpart;
	    }
	    delete temp;
	    this->day_count--;
	}
}

void WorkPlan::add(Task *oldtask)
{
    Task *task = new Task;                                      //I create a new node every time because the adress of the new node should be different
    task->next = NULL;
    task->previous = NULL;
    task->counterpart = NULL;
    task->name = new char[100];                                 //I've allocated new space for the char array.

    strcpy(task->name,oldtask->name);                           //I've copied the content of the oldtask and task.
    task->day=oldtask->day;
    task->time=oldtask->time;
    task->priority=oldtask->priority;

    if(day_count == 0){                                         //If day count is 0,that means there is no node in the linked list.
        task->next = task;                                      //So all pointers except counterpart should be point this node.
        task->previous = task;
        task->counterpart = NULL;
        head = task;                                            //Head should be changed from NULL to address of new task.
        day_count++;                                            //I've incremented day number.
        return;
    }else if(day_count == 1){                                   //If there is one day in the linked list;

        if(task->day < head->day){                              //And if the new node's day is smaller than head's day.
            task->next = head;                                  //New task should be replaced on left of the head
            task->previous = head;
            task->counterpart = NULL;                           //There is no counterpart.
            head->next = task;                                  //Head should be updated because the first node is changed.
            head->previous = task;
            head = task;
            day_count++;                                        //I've incremented day number because it is a different day.
            return;
        }else if(task->day > head->day){                        //If new node's day is greater than head's day.
            task->next = head;                                  //New task should be replaced on right of the hand.
            task->previous = head;
            task->counterpart = NULL;                           //There is no counterpart.
            head->next = task;                                  //Because it is a circular linked list and there is 2 node in the list head's next and previous should be updated when a new node is added to the beginning or end.
            head->previous = task;
            day_count++;                                        //I've incremented day number.
            return;
        }else{                                                  //If task->day is equal to head->day
            Task* temp = head;
            Task* tail = NULL;
                                                                //We should search for a suitable position for the newcoming.
            if(head->counterpart != NULL){
                int level = 0;                                  //I've created a variable to hold depth level in the linked list to decide position of the newcoming node.
                while(task->time > temp->time && temp->counterpart != NULL){
                    tail = temp;
                    temp = temp->counterpart;                    //while task's time is greater than temp's time and temp's counterpart is not NULL, I go to under node of the temp
                    level++;                                     //As a result, I've found the suitable position for the newcoming node.
                }
                if(level == 0){
                    task->counterpart = head;                    //If level is equal to zero that means we are still in the top node.
                    head = task;                                 //So we should change head to task
                }else{
                    tail->counterpart = task;                    //If level is not equal to zero, that means we are not on the top and we will add the newcoming node between two nodes(tail and temp)
                    task->counterpart = temp;
                }
            }else if(head->counterpart == NULL){                 //If there is nothing under the head
                if(task->time < temp->time){                     //and task'time is smaller than temp
                    task->counterpart = temp;                    //the newcoming node will be new head
                    head = task;
                }else{
                    temp->counterpart = task;                    //if task's time is not smaller than temp, the newcoming node should be replaced under the head.
                }
            }
        }

    }else{
        bool allocatedday = false;                               //I've created a boolean to hold whether there is a previously allocated day or not.
        int daycounter = 1;                                      //I've created an integer to hold order of the day I am looking
        Task* temp = head;

        while(temp->next != head){                               //I am searching for the task whose day is the same as newcoming node's day

            if(temp->day == task->day){
                allocatedday = true;                             //If it is founded allocatedday becomes true.
                break;
            }else{                                               //If it is not founded I've slided temp and I've incremented daycounter
                temp = temp->next;
                daycounter++;
            }
        }
        if(daycounter != 1 && temp->next == head){               //I've made an extra check for the last node in the linked list
            if(temp->day == task->day){
                allocatedday = true;
            }
        }
        if(allocatedday){                                        //If the new node's day is previously allocated

            if(daycounter == 1){                                 //and if daycounter is equal to 1 that means I will add the new node in the first day.
                if(task->time < temp->time){
                    task->next = head->next;
                    task->previous = head->previous;
                    (head->next)->previous = task;               //If task's time is smaller than temp's time I've made necessary operations to link the new node.
                    (head->previous)->next = task;
                    task->counterpart = temp;
                    temp->next = NULL;
                    temp->previous = NULL;
                    head = task;
                }else{                                          //If temp->time is greater than or equal to task's time(This else statement is only for the first task of current day)
                    if(temp->time == task->time){               //If the temp's time is equal to task's time I am comparing priorities.
                        if(temp->priority > task->priority){    //If temp's priority is greater than task's priority that means I won't touch temp its position will be same but I need to search for a appropriate day and hour for the task
                            checkAvailableNextTimesFor(temp);
                            task->time = this->usable_time;     //I've founded correct day and time
                            task->day = this->usable_day;
                            add(task);                          //And I've added the task to the linked list.
                        }else{
                            checkAvailableNextTimesFor(temp);   //If temp's priority is smaller than task's priority that means I should first remove temp and then add task
                            Task* newtask = new Task;           //I've used a new task for this operation whose day and time is usable day and usable time
                            newtask->next = NULL;
                            newtask->previous = NULL;
                            newtask->counterpart = NULL;
                            newtask->name = new char[100];
                            strcpy(newtask->name,temp->name);
                            newtask->day = this->usable_day;
                            newtask->time = this->usable_time;
                            newtask->priority = temp->priority;
                            remove(temp);                       //Firstly,I've removed temp and then
                            add(newtask);                       //added newtask
                            add(task);                          //added task
                            return;
                        }
                    }else{                                                  //If task's time is not equal to temp's time
                        Task *tail = NULL;
                        while (temp != NULL && (task->time > temp->time)) {
                            tail = temp;                                    //I am looking for suitable place for the newcoming node.
                            temp = temp->counterpart;
                        }
                        if(temp != NULL && temp->time == task->time){       //If temp's time is equal to task's time and temp is not NULL,I am comparing priorities of them.
                            if(temp->priority > task->priority){            //If temp's priority is greater than task's priority
                                checkAvailableNextTimesFor(temp);           //I am searching for available day and time for the temp
                                task->time = this->usable_time;             //And I've assigned this values to task's time and day
                                task->day = this->usable_day;
                                add(task);                                  //Finally,I've added task to the linked list.
                                return;
                            }else{                                          //If temp's priority is smaller than task's priority that means I should first remove temp and then add newtask and task to the linked with usable day and time.
                                checkAvailableNextTimesFor(temp);           //I've used again a new node to adding newcoming node as I've mentioned above.
                                Task* newtask = new Task;
                                newtask->next = NULL;
                                newtask->previous = NULL;
                                newtask->counterpart = NULL;
                                newtask->name = new char[100];
                                strcpy(newtask->name,temp->name);
                                newtask->day = this->usable_day;
                                newtask->time = this->usable_time;
                                newtask->priority = temp->priority;
                                remove(temp);
                                add(newtask);
                                add(task);
                                return;
                            }


                        }else{                                              //If temp'time and task's time are different.
                            if(temp == NULL) {                              //And it is last task of the day
                                tail->counterpart = NULL;                   //I've made necessary operations to link newcoming node to the linked list
                                task->counterpart = NULL;
                            }else{                                          //If it is not the last task of the day
                                tail->counterpart = task;                   //I've made necessary operations to link newcoming node to the linked list
                                task->counterpart = temp;
                            }
                        }
                    }
                }
            }else if(daycounter == day_count){                             //If I am adding to last day of the linked list.
                if(task->time < temp->time){                               //If task's time is smaller than temp's time
                    head->previous = task;
                    task->next = head;
                    task->previous = temp->previous;                        //I've made necessary operations to link the newcoming node
                    task->counterpart = temp;
                    (temp->previous)->next = task;
                    temp->next = NULL;
                    temp->previous = NULL;
                    return;
                }else{                                                      //If task's time is greater than or equal to temp's time(This else statement is only for the first task of the current day)

                    if(temp->time == task->time){                           //If task's time is equal to temp's time
                        if(temp->priority > task->priority){                //I am comparing first task's priority and temp's priority.
                            checkAvailableNextTimesFor(temp);               //If temp's priority is greater than task's priority
                            task->time = this->usable_time;                 //I am searching for usable day and time.
                            task->day = this->usable_day;
                            add(task);                                      //And I am adding task to the linked list with usable day and time.
                            return;
                        }else{                                              //If temp's priority is smaller than task's priority that means I should remove temp and then add task to the temp's old position then I need to add temp again to the linked list again with usable day and time
                            checkAvailableNextTimesFor(temp);               //I've used again newnode to make necessary add and remove operaitons.
                            Task* newtask = new Task;
                            newtask->next = NULL;
                            newtask->previous = NULL;
                            newtask->counterpart = NULL;
                            newtask->name = new char[100];
                            strcpy(newtask->name,temp->name);
                            newtask->day = this->usable_day;
                            newtask->time = this->usable_time;
                            newtask->priority = temp->priority;
                            remove(temp);
                            add(newtask);
                            add(task);
                            return;
                        }

                    }else{                                                      //If task's time is not equal to temp's time
                        Task* tail = NULL;
                        while(temp != NULL && (task->time > temp->time) ){      //I've slided temp to correct position
                            tail = temp;
                            temp = temp->counterpart;
                        }
                        if(temp != NULL && temp->time == task->time){           //Then, if temp's time is equal to task's time
                            if(temp->priority > task->priority){                //I am comparing theri priorities
                                checkAvailableNextTimesFor(temp);               //If temp's priority is greater than task's priority that means I don't need to remove temp I should only add task with usable day and time
                                task->time = this->usable_time;                 //I've assigned task's time and day with usable day and usable time.
                                task->day = this->usable_day;
                                add(task);                                      //Then, I've added task to the linked list
                            }else{                                              //If temp's priority is smaller than task's priority that means I should first remove temp then add task by newtask
                                checkAvailableNextTimesFor(temp);
                                Task* newtask = new Task;                       //I've used again a new node to add this node to the linked list
                                newtask->next = NULL;
                                newtask->previous = NULL;
                                newtask->counterpart = NULL;
                                newtask->name = new char[100];
                                strcpy(newtask->name,temp->name);
                                newtask->day = this->usable_day;
                                newtask->time = this->usable_time;
                                newtask->priority = temp->priority;
                                remove(temp);
                                add(newtask);
                                add(task);
                                return;
                            }
                        }else{                                                  //Temp's time is not equal to task's time
                            if(temp == NULL){                                   //If temp is NULL
                                tail->counterpart = task;                       //That means I will add newcoming node as last task of this day
                                task->counterpart = NULL;
                            }else{                                              //If temp is not NULL that means I will add newcoming node between first task and the last node
                                tail->counterpart = task;
                                task->counterpart = temp;
                            }
                        }

                    }
                }
            }else{                                                              //If I will add newcoming node between the first day and last day
                if(task->time < temp->time){                                    //If task's time is smaller than temp's time
                    task->next = temp->next;                                    //That means we will add newcoming node as the first task of the current day
                    task->previous = temp->previous;
                    task->counterpart = temp;
                    (temp->previous)->next = task;
                    (temp->next)->previous = task;
                    temp->next = NULL;
                    temp->previous = NULL;
                }else{                                                          //If task's time is greater than or equal to task's time(This else statement is only for first of the current day)
                    if(temp->time == task->time){                               //If temp's time is equal to task's time
                        if(temp->priority > task->priority){                    //If temp's priority is greater than task's priority
                            checkAvailableNextTimesFor(temp);                   //that means I will not remove temp.I will just add task with usable day and time
                            task->time = this->usable_time;
                            task->day = this->usable_day;
                            add(task);
                        }else{                                                  //If temp's priority is smaller than task's priority
                            checkAvailableNextTimesFor(temp);                   //That means I should remove temp and then I should add newcoming task with usable day and time
                            Task* newtask = new Task;
                            newtask->next = NULL;
                            newtask->previous = NULL;
                            newtask->counterpart = NULL;
                            newtask->name = new char[100];
                            strcpy(newtask->name,temp->name);
                            newtask->day = this->usable_day;
                            newtask->time = this->usable_time;
                            newtask->priority = temp->priority;
                            remove(temp);
                            add(newtask);
                            add(task);
                        }
                        return;
                    }else{                                                      //If temp's time is not equal to task's time
                        Task* tail = NULL;
                        while(temp != NULL && (task->time > temp->time) ){      //I've slided temp to correct place
                            tail = temp;
                            temp = temp->counterpart;
                        }

                        if(temp != NULL && temp->time == task->time){           //If temp is not NULL and temp's time is equal to task's time
                            if(temp->priority > task->priority){                //And temp's priority is greater than task's priority
                                checkAvailableNextTimesFor(temp);               //that means I will not remove temp.I will just add task with usable day and time
                                task->time = this->usable_time;
                                task->day = this->usable_day;
                                add(task);
                            }else{                                              //If temp's priority is smaller than task's priority
                                checkAvailableNextTimesFor(temp);               //That means I should remove temp and then I should add newcoming task with usable day and time
                                Task* newtask = new Task;
                                newtask->next = NULL;
                                newtask->previous = NULL;
                                newtask->counterpart = NULL;
                                newtask->name = new char[100];
                                strcpy(newtask->name,temp->name);
                                newtask->day = this->usable_day;
                                newtask->time = this->usable_time;
                                newtask->priority = temp->priority;
                                remove(temp);
                                add(newtask);
                                add(task);
                            }
                            return;
                        }
                        else{                                                   //Temp's time is not equal to task's time
                            if(temp == NULL){                                   //If temp is NULL
                                tail->counterpart = task;                       //That means I will add newcoming node as last task of this day
                                task->counterpart = NULL;
                            }else{                                              //If temp is not NULL that means I will add newcoming node between first task and the last node
                                tail->counterpart = task;
                                task->counterpart = temp;
                            }
                        }
                    }
                }
            }
        }else{                                                                   //If it is not allocated day
            Task* temp = head;
            Task* tail = NULL;
            int daycounter = 1;
            while(temp->next != head && (task->day > temp->day) ){
                tail = temp;
                temp = temp->next;
                daycounter++;
            }
            if(daycounter == 1){
                (head->previous)->next = task;
                task->previous = head->previous;
                head->previous = task;
                task->next = head;
                head = task;
                task->counterpart = NULL;
                day_count++;
            }else if(task->day > temp->day){
                temp->next = task;
                task->previous = temp;
                task->next = head;
                head->previous = task;
                task->counterpart = NULL;
                day_count++;
            }else{
                tail->next = task;
                task->previous = tail;
                task->next = temp;
                temp->previous = task;
                task->counterpart = NULL;
                day_count++;
            }
        }
    }
} 

Task * WorkPlan::getTask(int day, int time)
{
	Task* temp = head;

	if(temp->day == day){                               //If temp's day is equal to day
	    if(temp->time == time){                         //and if temp's time is also equal to time
	        return temp;                                //then it is the node we are looking for
	    }else{                                          //if temp's time is not equal to time
	        while(temp->time != time){                  //I've change temp with temp->counterpart
	            temp = temp->counterpart;
	        }
	        return temp;
	    }
	}else{                                              //If temp's day is not equal to day
        while(temp->next != head){                      //If temp's day is equal to day
            if(temp->day == day){                       //and I am checking whether temp->day is equal to day or not.
                while(temp->time != time){              //I go down until temp->time is equal to time
                    temp = temp->counterpart;
                }if(temp->time == time){                //If temp's time is equal to time that is the node we are looking for
                    return temp;
                }
            }else{
                temp = temp->next;                      //else slide temp
            }
        }
	}
    return temp;
}

void WorkPlan::checkAvailableNextTimesFor(Task *delayed)
{
    bool sameday = false;                               //whether the suitable day is the same with node's day or not
    bool available = false;                             //whether an available time for the node
    Task* temp = head;
    Task* tail = NULL;
    Task* currentday = NULL;

    while(temp->day != delayed->day){                   //It equalizes temp and delayed
        temp = temp->next;
    }
    currentday = temp;
    while(temp && delayed->time != temp->time){
        temp = temp->counterpart;                       //It equalizes temp's time and delayed's time
    }
    while(delayed->counterpart && !sameday){            //while delayed->counterpart is not NULL and there is no available time in day of delayed.
        tail = temp;
        temp = temp->counterpart;
        if(temp == NULL){
            break;
        }else{
            if( (temp->time - tail->time) > 1){         //It checks whether there is a suitable gap for the delayed node or not
                this->usable_time = tail->time + 1;
                this->usable_day = delayed->day;
                sameday = true;
            }
        }
    }
    if(!sameday){                                                  //If there is no suitable position in the delayed->day
        temp = currentday;
        while(temp->next != head){                                 //I am traversing whole linked list
            temp = currentday->next;
            if(temp->counterpart == NULL && temp->time > 8){       //If temp's time is greater than 8, that means 8 is empty and it is suitable for delayed node
                this->usable_day = temp->day;
                this->usable_time = 8;
                available = true;
                return;
            }
            int count = 0;
            while(temp->counterpart && !available ){                //while temp->counterpart and there is no available time

                if(temp->time > 8 && count ==  0){                  //I've controlled this if statement for only first time
                    this->usable_day = temp->day;                   //It controls whether the top node's time is greater than 8 or not
                    this->usable_time = 8;
                    available = true;
                    return;
                }else{
                    tail = temp;
                    temp = temp->counterpart;
                    if( (temp->time - tail->time) > 1){             //I've controlled sub nodes to find a suitable hour
                        this->usable_time = tail->time + 1;
                        this->usable_day = temp->day;
                        available = true;
                        return;
                    }
                }
                count++;
            }
            if(!available){
                currentday = currentday->next;                      //If there is no available time in this day I've slided currentday

            }
        }
    }
}

void WorkPlan::delayAllTasksOfDay(int day){

    Task* temp = head;
    int daycounter = 1;

    while(temp->day != day){                                        //I've slided temp to day
        temp = temp->next;
        daycounter++;
    }
    if(daycounter == this->day_count){                              //If temp points the last node in the linked list
        if(temp->counterpart){                                      //And if temp is not the only task in this day
            int hour = 8;
            while(temp){                                            //I've slided temp to last task of current day
                Task* newnode = new Task;                           //And I've first removed current node and add newnode with usable day and time
                Task* temp2 = temp->counterpart;
                newnode->name = new char[100];
                strcpy(newnode->name,temp->name);
                newnode->day = (temp->day)+1;
                newnode->time = hour;
                newnode->priority = temp->priority;
                remove(temp);
                add(newnode);
                temp = temp2;
                hour++;
            }
            return;
        }else{                                                      //If temp is the only node of this day
            Task* newnode = new Task;                               //I've removed node and then added as newnode with suitable day and time
            newnode->name = new char[100];
            strcpy(newnode->name,temp->name);
            newnode->day = (temp->day)+1;
            newnode->time = 8;
            newnode->priority = temp->priority;
            remove(temp);
            add(newnode);
            return;
        }
    }else{                                                          //If it is not the last day of the linked list
        if(temp->counterpart){
            while(temp){                                            //I've slided temp to last task of current day
                Task* newnode = new Task;                            //And I've first removed current node and add newnode with usable day and time
                Task* temp2 = temp->counterpart;
                newnode->name = new char[100];
                strcpy(newnode->name,temp->name);
                newnode->day = (temp->next)->day;
                newnode->time = 7;
                newnode->priority = temp->priority;
                remove(temp);
                add(newnode);
                checkAvailableNextTimesFor(newnode);
                remove(newnode);
                newnode->day = this->usable_day;
                newnode->time =this->usable_time;
                add(newnode);
                temp = temp2;
            }
            return;
        }else{                                                      //If temp is the only node of this day
            Task* newnode = new Task;                               //I've removed node and then added as newnode with suitable day and time
            newnode->name = new char[100];
            strcpy(newnode->name,temp->name);
            newnode->day = (temp->next)->day;
            newnode->time = 8;
            newnode->priority = temp->priority;
            remove(temp);
            checkAvailableNextTimesFor(newnode);
            newnode->day = this->usable_day;
            newnode->time =this->usable_time;
            add(newnode);
            return;
        }
    }
}

void WorkPlan::remove(Task *target)
{
	Task* temp = head;
	Task* tail = NULL;
	int daycounter = 1;

	while(temp->day != target->day){                                    //I've slided temp to target's day.
        tail = temp;
	    temp = temp->next;
	    daycounter++;
	}
	if(daycounter == 1){                                                //If it is the first day of the linked list
        if(temp->counterpart != NULL){                                  //If temp's counterpart is not NULL
            if(temp->time == head->time){                               //And if temp's time is equal to head's time
                (temp->counterpart)->next = temp->next;                 //That means I should update head after I've broken necessary links to remove target node
                (temp->counterpart)->previous = temp->previous;
                (head->previous)->next = temp->counterpart;
                (head->next)->previous = temp->counterpart;
                head = temp->counterpart;
                delete temp;
                return;
            }else{                                                      //If temp's time is not equal to head's time
                tail = NULL;
                while(temp->time != target->time){                      //I've slided temp to correct position.
                    tail = temp;
                    temp = temp->counterpart;
                }
                if(temp->counterpart == NULL){                          //If there is no task under temp
                    tail->counterpart = NULL;                           //tail's counterpart should ne NULL
                    delete temp;
                }else{
                    tail->counterpart = temp->counterpart;              //If there is at least one under temp that means I want to remove a node that is between the first and last task
                    delete temp;
                }
            }
        }else{                                                          //If temp's counterpart is NULL
            (temp->next)->previous = head->previous;                    //That means this is the only task of first day so I should update head
            (head->previous)->next = temp->next;
            head = temp->next;
            temp->next = NULL;
            temp->previous = NULL;
            this->day_count--;
            delete temp;
        }
	}else if(temp->next == head){                                       //If it is last day
	    if((temp->counterpart) != NULL){                                //And this is not the only task of this day
            if(temp->time == (head->previous)->time){                   //If temp's time is equal to first task of the last day
                (temp->counterpart)->next = head;
                (temp->counterpart)->previous = temp->previous;
                (temp->previous)->next = temp->counterpart;
                head->previous = temp->counterpart;
                delete temp;
            }else{                                                      //If temp's is not the first task of the last day
                Task* tail = NULL;
                while(temp->time != target->time){                      //I've slided temp to correct position
                    tail = temp;
                    temp = temp->counterpart;
                }
                if(temp->counterpart == NULL){                          //Then, if it is tha last task of the day
                    tail->counterpart = NULL;
                    delete temp;
                }else{                                                  //If it is not the last task of the day
                    tail->counterpart = temp->counterpart;
                    delete temp;
                }
            }
        }else{                                                          //If this is the only task of the last day
            (temp->previous)->next = head;                              //Head's previous sholud be changed
            head->previous = temp->previous;
            temp->next = NULL;
            temp->previous = NULL;
            this->day_count--;
            delete temp;
	    }
	}else{                                                              //If it is between the first day and the last node
        if((temp->counterpart) != NULL){                                //If this is not the only task of this day
            if(temp->time == target->time) {                            //If the first task of this is the target
                (temp->previous)->next = temp->counterpart;
                (temp->next)->previous = temp->counterpart;
                (temp->counterpart)->next = temp->next;
                (temp->counterpart)->previous = temp->previous;
                temp->next = NULL;
                temp->previous = NULL;
                delete temp;
            }else{                                                      //If target is not the first task of this day
                Task* tail = NULL;
                while(temp->time != target->time){                      //I've slided temp to correct position
                    tail = temp;
                    temp = temp->counterpart;
                }
                if(temp->counterpart == NULL){                          //If there is no task under temp
                    tail->counterpart = NULL;
                    delete temp;
                }else{                                                  //If temp is not last task of this day
                    tail->counterpart = temp->counterpart;
                    delete temp;
                }
            }
        }else{                                                          //If temp is the only task of this day.
            (temp->previous)->next = temp->next;
            (temp->next)->previous = temp->previous;
            temp->next = NULL;
            temp->previous = NULL;
            this->day_count--;
            delete temp;
            return;
        }
	}
}

bool WorkPlan::checkCycledList()
{				
	Task *pivot=new Task();
	pivot=head;
	int patient=100;
	bool r=false;
	while (pivot!=NULL&&patient>0)
	{		
		patient--;
		pivot=pivot->previous;
		if(pivot==head)
		{
			r=true;
			break;
		}
	}
	cout<<"("<<100-patient<<")";
	patient=100;
	bool l=false;
	while (pivot!=NULL&&patient>0)
	{		
		patient--;
		pivot=pivot->next;
		if(pivot==head)
		{
			l=true;
			break;
		}
	}
	return r&l;
}
