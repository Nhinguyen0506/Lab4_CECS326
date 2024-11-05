/**
 * FCFS scheduling
 */
 
#include <stdlib.h>
#include <stdio.h>

#include "task.h"
#include "list.h"
#include "cpu.h"
#include "schedulers.h"

struct node *taskList = NULL; // Head of the task list

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

    // Set the task's priority and burst
    t->priority = priority;
    t->burst = burst;

    // Insert into the task list
    insert(&taskList, t);
}

// Pick the next task to execute with FCFS and make sure taskList must not be empty!
Task *pickNextTask() {
    if (taskList == NULL) 
    {
        exit(1);  // Exit to avoid undefined behavior
    }
    return taskList->task; // Return the first task in the list
}

// Schedule the tasks using FCFS
void schedule() {
    if (taskList == NULL) 
    {
        exit(1);  // Exit to prevent running with an empty task list
    }

    while (taskList != NULL) 
    {
        Task *t = pickNextTask(); // Get the next task
        run(t, t->burst);         // Run the task for its full burst time

        // Remove the completed task from the list
        delete(&taskList, t);

        // Free memory allocated for the task name and the task itself
        free(t->name);
        free(t);
    }
}
