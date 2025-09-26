#ifndef TODO_H
#define TODO_H

#include <stdbool.h>
#include <stddef.h>
#define TODO_MAX_DESC 256

typedef struct todoTask
{
    int id;
    bool done;
    char text[TODO_MAX_DESC];

} Task;

typedef struct
{
    size_t count;
    Task *items;
    int next_id;
    size_t capacity;

} TaskList;

/************** Lifecycle **************/
TaskList *todo_init(void);

void todo_free(TaskList *tl);

/************** Operations on TaskList ************************/

TaskList add_task(TaskList *tl, const char *text);

//bool delete_task(TaskList *tl, int id);

const Task *todo_find(const TaskList *tl, int id);

bool todo_done(TaskList *tl, int id, bool done);

size_t todo_size(const TaskList *tl);

/************** UI / Output **************/
//void task_print(const TaskList *tl);

#endif
