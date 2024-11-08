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
// Add a task
void add(char *name, int priority, int burst) {
    // Allocate memory
    Task *t = malloc(sizeof(Task));
    if (t == NULL) {
        return;
    }
    t->name = malloc(strlen(name) + 1);
    if (t->name == NULL) {
        free(t); // Free task structure if name allocation fails
        return;
    }
    strcpy(t->name, name);
    t->priority = priority;
    t->burst = burst;
    // Create a new node for the task
    struct node *newNode = malloc(sizeof(struct node));
    if (newNode == NULL) {
        free(t->name);
        free(t);
        return;
    }
    newNode->task = t;
    newNode->next = NULL;

    // If the list is empty, set the new node as the head
    if (taskList == NULL) {
        taskList = newNode;
    } 
    else 
    {
        // Traverse to the end of the list
        struct node *current = taskList;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// pick the next task
Task *pickNextTask() {
    if (next_node == NULL) 
    {
        return NULL;  // Exit
    }
    Task *ret = next_node->task;
    if (next_node->next != NULL) {
        next_node = next_node->next; // Move to the next task
    } else {
        next_node = taskList; 
    }
    return ret;
}
// Run the task
void schedule() {
    next_node = taskList; // Initialize the next_node pointer
    if (taskList == NULL) 
    {
        return NULL;  // Exit to prevent running an empty task list
    }
    while (taskList != NULL) 
    {
        Task *t = pickNextTask();  // Get the next task in round-robin order
        int slice; // Determine time slice (minimum of QUANTUM or remaining burst time)
        if (QUANTUM < t->burst) {
            slice = QUANTUM; // cpu.h defines QUANTUM as 10 milliseconds
        } else {
            slice = t->burst;
        }
        run(t, slice);  // Run the task
        t->burst -= slice; // Update the remaining burst time
        if (t->burst <= 0) 
        {
            delete(&taskList, t);
            // Free memory for the completed task
            free(t->name);
            free(t);
        }
    }
}
