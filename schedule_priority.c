/**
 * Priority scheduling
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "schedulers.h"
#include "task.h"
#include "cpu.h"
#include "list.h"

struct node *taskList = NULL;

void log_error(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
}
// Add a task
void add(char *name, int priority, int burst) {
    // Allocate memory for a new Task structure
    Task *t = malloc(sizeof(Task));
    if (t == NULL) {
        log_error("Unable to allocate memory for task.");
        return;
    }

    // Allocate memory for the task name and copy it
    t->name = malloc(strlen(name) + 1);
    if (t->name == NULL) {
        log_error("Unable to allocate memory for task name.");
        free(t); // Free task structure if name allocation fails
        return;
    }
    strcpy(t->name, name);
    t->priority = priority;
    t->burst = burst;

    // Create a new node for the task
    struct node *newNode = malloc(sizeof(struct node));
    if (newNode == NULL) {
        log_error("Unable to allocate memory for list node.");
        free(t->name);
        free(t);
        return;
    }
    newNode->task = t;
    newNode->next = NULL;

    if (taskList == NULL) {
        taskList = newNode; // If the list is empty, set the new node as the head
    } else
    {
        struct node *current = taskList;
        struct node *previous = NULL;

        // Find the correct position to insert the new node (highest to lowest priority)
        while (current != NULL && current->task->priority <= t->priority) {
            previous = current;
            current = current->next;
        }

        if (previous == NULL) {
            // Insert at the beginning if the new task has the highest priority
            newNode->next = taskList;
            taskList = newNode;
        } else {
            // Insert in the correct position
            previous->next = newNode;
            newNode->next = current;
        }
    }
}

// Pick the next task with the highest priority
Task *pickNextTask() {
    if (taskList == NULL) {
        return NULL;  // Return NULL if the task list is empty
    }
    return taskList->task; // Return the first task in the list (highest priority)
}
// Schedule using Priority Scheduling
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

