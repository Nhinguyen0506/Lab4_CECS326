/**
 * Round-robin scheduling
 */
#include <stdlib.h>
#include <stdio.h>

#include "schedulers.h"
#include "task.h"
#include "cpu.h"
#include "list.h"

#define QUANTUM 10  // Define the time quantum for round-robin

struct node *taskList = NULL;  // Head of the task list
struct node *next_node = NULL; // Pointer for round-robin traversal

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
        free(t);  // Free task structure if name allocation fails
        exit(1);
    }
    strcpy(t->name, name);

    // Set task priority and initial burst time
    t->priority = priority;
    t->burst = burst;

    // Insert into taskList
    insert(&taskList, t);
}

/* pick the next task to execute in a round-robin fashion
 * make sure taskList must not be empty!
 */
Task *pickNextTask() {
    if (next_node == NULL) 
    {
        exit(1);
    }

    Task *ret = next_node->task;
    // Move to the next task, or wrap around to the beginning if at the end
    next_node = (next_node->next) ? next_node->next : taskList;
    return ret;
}

// Invoke the scheduler
void schedule() {
    next_node = taskList; // Initialize next_node to the head of the list
    while (taskList != NULL) 
    {
        Task *t = pickNextTask();  // Get the next task in round-robin order

        // Determine time slice (minimum of QUANTUM or remaining burst time)
        int slice = (QUANTUM < t->burst) ? QUANTUM : t->burst;
        run(t, slice);  // Run the task for the computed time slice
        t->burst -= slice;

        // If the task is complete, remove it from the list and free memory
        if (t->burst <= 0) 
        {
            delete(&taskList, t);
            
            // Free memory for the completed task
            free(t->name);
            free(t);
        }
    }
}
