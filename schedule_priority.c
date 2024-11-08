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

// Pick the next task with the Highest Priority
Task *pickNextTask() {
    if (taskList == NULL) 
    {
        return;  // Exit to prevent running an empty task list
    }
    Task *priority_job = taskList->task;
    struct node *n = taskList;
    // Iterate through the list
    while (n) 
    {
        if (n->task->priority > priority_job->priority) 
        {
            priority_job = n->task;
        }
        n = n->next;
    }
    return priority_job;
}
// Schedule using Priority Scheduling
void schedule() {
    if (taskList == NULL) 
    {
        return;  // Exit to prevent running an empty task list
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

