#include "todo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/************** Lifecycle **************/
TaskList *todo_init(void)
{
    TaskList *tl = malloc(sizeof(TaskList));

    if (tl == NULL)
    {
        return NULL;
    }

    tl->count = 0;
    tl->capacity = 0;
    tl->next_id = 1;
    tl->items = NULL;

    return tl;
};

void todo_free(TaskList *tl)
{
    if (tl == 0)
    {
        return;
    }

    free(tl->items);

    tl->items = NULL;

    tl->count = tl->capacity = 0;

    tl->next_id = 1;

    return;
}

/************** Operations on TaskList ************************/

bool todo_add(TaskList *tl, char *text)
{
    // check for capacity maybe add a function that allocates more mem

    if (tl == NULL)
    {
        return false;
    }
    
    tl->count = tl->capacity = +1;

    // Ensure the text is null-terminated
    text[TODO_MAX_DESC - 1] = '\0';
    strncpy(tl->items->text, text, TODO_MAX_DESC - 1);

    tl->next_id++;
    tl->items->done = false;
    tl->items->id = +1;

    return true;
}

// bool delete_task(TaskList *tl, int id);

const Task *todo_find(const TaskList *tl, int id)
{
    if (tl == NULL)
    {
        return NULL;
    }

    for (size_t i = 0; i < tl->count; i++)
    {
        if (tl->items[i].id == id)
        {
            return &tl->items[i];
        }
    }

    return NULL;
}

bool todo_toggle_done(TaskList *tl, int id, bool done)
{
    if (tl == NULL)
    {
        return false;
    }

    for (size_t i = 0; i < tl->count; i++)
    {
        if (tl->items[i].id == id)
        {
            if (done == true)
            {
                tl->items[i].done = true;
                return true;
            }
        }
    }

    return false;
}

size_t todo_size(const TaskList *tl)
{
    return tl->count;
}

/************** UI / Output **************/
// void todo_list_print(const TaskList *tl);
