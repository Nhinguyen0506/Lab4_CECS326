/*
 * Priority scheduling
 */
#include <stdlib.h>
#include <stdio.h>

#include "schedulers.h"
#include "task.h"
#include "cpu.h"
#include "list.h"

struct node *taskList = NULL;

// Add a task to the list
void add(char *name, int priority, int burst) {
    // Allocate memory for a new Task structure
    Task *t = malloc(sizeof(Task));
    if (t == NULL) 
    {
        exit(1);
    }

    // Allocate memory for the task name and copy it
    t->name = malloc(strlen(name) + 1);
    if (t->name == NULL) 
    {
        free(t); // Free task structure if name allocation fails
        exit(1);
    }
    strcpy(t->name, name);

    // Set task priority and burst
    t->priority = priority;
    t->burst = burst;

    // Insert the task into taskList
    insert(&taskList, t);
}

/* pick the next task to execute with the Highest Priority
 *  make sure taskList must not be empty!
 */
Task *pickNextTask() {
    if (taskList == NULL) 
    {
        exit(1);  // Exit to avoid undefined behavior
    }

    Task *highest_priority_job = taskList->task;
    struct node *n = taskList;

    // Iterate through the list to find the highest priority task
    while (n) 
    {
        if (n->task->priority > highest_priority_job->priority) {
            highest_priority_job = n->task;
        }
        n = n->next;
    }
    return highest_priority_job;
}

// Schedule the tasks using Priority Scheduling
void schedule() {
    while (taskList) 
    {
        Task *t = pickNextTask(); // Get the highest priority task
        run(t, t->burst);         // Run the task

        delete(&taskList, t);     // Remove the completed task from the list

        // Free memory allocated for the task name and the task itself
        free(t->name);
        free(t);
    }
}
