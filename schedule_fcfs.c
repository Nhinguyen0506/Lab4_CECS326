/**
 * FCFS scheduling
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "task.h"
#include "list.h"
#include "cpu.h"
#include "schedulers.h"

struct node *taskList = NULL;
// Add a task
void add(char *name, int priority, int burst) {
    // Allocate memory
    Task *t = malloc(sizeof(Task));
    if (t == NULL) 
    {
        return;
    }
    // Allocate memory for the task name and copy it
    t->name = malloc(strlen(name) + 1);
    if (t->name == NULL) 
    {
        free(t); // Free if fails
        return;
    }
    strcpy(t->name, name);
    t->priority = priority;
    t->burst = burst;
    // Create a new node for the task and add it to the end of the list
    struct node *newNode = malloc(sizeof(struct node));
    if (newNode == NULL) {
        free(t->name);
        free(t);
        return;
    }
    newNode->task = t;
    newNode->next = NULL;
    if (taskList == NULL) {
        taskList = newNode; // If the list is empty, set the new node as the head
    } 
    else 
    {
        struct node *current = taskList;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}
// Pick the next task
Task *pickNextTask() {
    if (taskList == NULL) {
        return NULL;
    }
    return taskList->task; // Return the first task in the list
}


// Schedule the tasks using FCFS
void schedule() {
    if (taskList == NULL) 
    {
        return;  // Exit
    }
    while (taskList != NULL) 
    {
        Task *t = pickNextTask(); 
        run(t, t->burst);         
        delete(&taskList, t);
        // Free memory allocated 
        free(t->name);
        free(t);
    }
}
