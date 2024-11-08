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

void log_error(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
}
// Add a task
void add(char *name, int priority, int burst) {
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
    // Create a new node for the task and add it to the end of the list
    struct node *newNode = malloc(sizeof(struct node));
    if (newNode == NULL) {
        log_error("Unable to allocate memory for list node.");
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
        return NULL;  // Exit to prevent running an empty task list
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
