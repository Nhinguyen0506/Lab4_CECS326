/**
 * SJF scheduling
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "schedulers.h"
#include "task.h"
#include "cpu.h"
#include "list.h"

struct node *taskList = NULL;  // Head of the task list

// Add a task to the list
void add(char *name, int priority, int burst) {
    // Allocate memory for a new Task structure
    Task *t = malloc(sizeof(Task));
    if (t == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for task.\n");
        exit(1);
    }

    // Allocate memory for the task name and copy it
    t->name = malloc(strlen(name) + 1);
    if (t->name == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for task name.\n");
        free(t);  // Free task structure if name allocation fails
        exit(1);
    }
    strcpy(t->name, name);

    // Set task priority and burst time
    t->priority = priority;
    t->burst = burst;

    // Insert the task into taskList
    insert(&taskList, t);
}

/* pickNextTask: pick the next task to execute with the shortest burst
 * taskList must not be empty!
 */
Task *pickNextTask() {
    if (taskList == NULL) {
        fprintf(stderr, "Error: taskList is empty! pickNextTask called with no tasks to execute.\n");
        exit(1);  // Exit to avoid undefined behavior
    }

    Task *shortest_task = taskList->task;
    struct node *current = taskList;

    // Traverse the list to find the task with the shortest burst
    while (current != NULL) {
        if (current->task->burst < shortest_task->burst) {
            shortest_task = current->task;
        }
        current = current->next;
    }
    return shortest_task;
}

// Schedule the tasks using Shortest Job First Scheduling
void schedule() {
    while (taskList != NULL) {
        Task *t = pickNextTask(); // Get the task with the shortest burst time
        run(t, t->burst);         // Run the task for its full burst time

        // Remove the completed task from the list
        delete(&taskList, t);

        // Free memory allocated for the task name and the task itself
        free(t->name);
        free(t);
    }
}
