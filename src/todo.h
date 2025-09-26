#ifndef TODO_H
#define TODO_H

#include <stdbool.h>
#include <stddef.h>
#define TODO_MAX_DESC 256

typedef struct
{
    int id;
    bool done;
    char *text;
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

bool todo_add(TaskList *tl, const char *text);

bool todo_delete(TaskList *tl, int id);

const Task *todo_find(const TaskList *tl, int id);

bool todo_toggle_done(TaskList *tl, int id, bool done);

size_t todo_size(const TaskList *tl);

/************** UI / Output **************/
void todo_list_print(const TaskList *tl);

/************** File I/O **************/
bool todo_load_from_file(TaskList *tl, const char *filename);
bool todo_load_csv_file(TaskList *tl, const char *filename);
bool todo_save_to_file(const TaskList *tl, const char *filename);

#endif
