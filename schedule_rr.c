/**
 * Round-robin scheduling
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "schedulers.h"
#include "task.h"
#include "cpu.h" // Lenhth of a time quantum is 10 milliseconds (Professor required)
#include "list.h"



struct node *taskList = NULL;  // Head of the task list
struct node *next_node = NULL; // Pointer for round-robin traversal

void log_error(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
}
// Add a task to the list
void add(char *name, int priority, int burst) {
    // Allocate memory for a new Task structure
    Task *t = malloc(sizeof(Task));
    if (t == NULL) 
    {
        log_error("Unable to allocate memory.");
        return;
    }
    // Allocate memory for the task name and copy it
    t->name = malloc(strlen(name) + 1);
    if (t->name == NULL) 
    {
        log_error("Unable to allocate memory for task name.");
        free(t); // Free task structure if fails
        return;
    }
    strcpy(t->name, name);
    t->priority = priority;
    t->burst = burst;
    // Insert the task into the list
    insert(&taskList, t);
}

// pick the next task to execute
Task *pickNextTask() {
    if (next_node == NULL) 
    {
        return NULL;  // Exit to prevent running an empty task list
    }
    Task *ret = next_node->task;
    if (next_node->next != NULL) {
        next_node = next_node->next; // Move to the next task
    } else {
        next_node = taskList; // Wrap around to the beginning of the list
    }
    return ret;
}
// Run the task
void schedule() {
    next_node = taskList; // Initialize the next_node pointer
    while (taskList != NULL) 
    {
        Task *t = pickNextTask();  // Get the next task in round-robin order

        // Determine time slice (minimum of QUANTUM or remaining burst time)
        int slice;
        if (QUANTUM < t->burst) {
            slice = QUANTUM; // cpu.h defines QUANTUM as 10 milliseconds
        } else {
            slice = t->burst; // Otherwise, use the remaining burst time
        }
        run(t, slice);  // Run the task
        t->burst -= slice;

        // Remove it from the list and free memory
        if (t->burst <= 0) 
        {
            delete(&taskList, t);
            // Free memory for the completed task
            free(t->name);
            free(t);
        }
    }
}
