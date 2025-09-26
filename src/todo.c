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
    if (tl == NULL)
    {
        return;
    }

    // Free all text allocations first
    for (size_t i = 0; i < tl->count; i++)
    {
        free(tl->items[i].text);
    }

    // Free the items array
    free(tl->items);
    tl->items = NULL;

    tl->count = tl->capacity = 0;
    tl->next_id = 1;

    // Free the TaskList itself
    free(tl);

    return;
}

/************** Health check ************************/

TaskList *ensure_capacity(TaskList *tl, size_t needSize)
{
    if (tl->count >= needSize)
    {
        size_t new_cap;

        if (tl->capacity)
        {
            new_cap = tl->capacity * 2; // Double current capacity
        }
        else
        {
            new_cap = 8; // Initial capacity
        }

        Task *p = (Task *)realloc(tl->items, new_cap * sizeof(Task));

        if (!p)
            return NULL;

        tl->items = p;

        tl->capacity = new_cap;
    }

    return tl;
};

/************** Operations on TaskList ************************/

bool todo_add(TaskList *tl, const char *text)
{
    if (tl == NULL)
    {
        return false;
    }

    // Ensure we have capacity for at least one more item
    if (tl->count >= tl->capacity)
    {
        size_t new_cap = tl->capacity ? tl->capacity * 2 : 8;
        Task *new_items = realloc(tl->items, new_cap * sizeof(Task));
        if (!new_items)
        {
            return false; // Allocation failed
        }
        tl->items = new_items;
        tl->capacity = new_cap;
    }

    Task *t = &tl->items[tl->count++];
    t->id = tl->next_id++;
    t->done = false;

    // Allocate memory for text since it's a char* pointer
    t->text = malloc(strlen(text) + 1);
    if (!t->text)
    {
        tl->count--; // Rollback count increment
        return false;
    }

    strcpy(t->text, text);
    return true;
}

bool todo_delete(TaskList *tl, int id)
{
    if (!tl)
        return false;
    for (size_t i = 0; i < tl->count; i++)
    {
        if (tl->items[i].id == id)
        {
            // Free the text of the item being deleted (correct index!)
            free(tl->items[i].text);

            // Move the last item to this position (if not the last item)
            if (i < tl->count - 1)
            {
                tl->items[i] = tl->items[tl->count - 1];
            }

            tl->count--;
            return true;
        }
    }
    return false;
}

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
            tl->items[i].done = done;
            return true;
        }
    }

    return false;
}

size_t todo_size(const TaskList *tl)
{
    if (tl == NULL)
    {
        return 0;
    }

    return tl->count;
}

/************** UI / Output **************/
void todo_list_print(const TaskList *tl)
{
    if (!tl || tl->count == 0)
    {
        puts("(no tasks)");
        return;
    }
    printf("ID   Status  Description\n");
    printf("--   ------  -----------\n");

    for (size_t i = 0; i < tl->count; i++)
    {
        const Task *t = &tl->items[i];
        printf("%-4d %-6s  %s\n", t->id, t->done ? "✔" : "✗", t->text ? t->text : "");
    }
}

/************** File I/O **************/

bool todo_load_from_file(TaskList *tl, const char *filename)
{
    if (!tl || !filename)
    {
        return false;
    }

    FILE *file = fopen(filename, "r");
    if (!file)
    {
        return false; // File doesn't exist or can't be opened
    }

    char line[TODO_MAX_DESC];
    while (fgets(line, sizeof(line), file))
    {
        // Remove newline character
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
        {
            line[len - 1] = '\0';
        }

        // Skip empty lines
        if (strlen(line) == 0)
        {
            continue;
        }

        // Add the task (simple format: each line is a task)
        if (!todo_add(tl, line))
        {
            fclose(file);
            return false; // Failed to add task
        }
    }

    fclose(file);
    return true;
}

bool todo_load_csv_file(TaskList *tl, const char *filename)
{
    if (!tl || !filename)
    {
        return false;
    }

    FILE *file = fopen(filename, "r");
    if (!file)
    {
        return false;
    }

    char line[TODO_MAX_DESC];
    while (fgets(line, sizeof(line), file))
    {
        // Remove newline
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
        {
            line[len - 1] = '\0';
        }

        if (strlen(line) == 0)
            continue;

        // Parse CSV: id,done,text
        char *id_str = strtok(line, ",");
        char *done_str = strtok(NULL, ",");
        char *text = strtok(NULL, ""); // Rest of line

        if (id_str && done_str && text)
        {
            // Add the task with the text
            if (todo_add(tl, text))
            {
                // Update the last added task with correct ID and done status
                Task *last_task = &tl->items[tl->count - 1];
                last_task->id = atoi(id_str);
                last_task->done = (strcmp(done_str, "true") == 0);

                // Update next_id to avoid conflicts
                if (last_task->id >= tl->next_id)
                {
                    tl->next_id = last_task->id + 1;
                }
            }
        }
    }

    fclose(file);
    return true;
}

bool todo_save_to_file(const TaskList *tl, const char *filename)
{
    if (!tl || !filename)
    {
        return false;
    }

    FILE *file = fopen(filename, "w");
    if (!file)
    {
        return false; // Can't create/write to file
    }

    for (size_t i = 0; i < tl->count; i++)
    {
        const Task *t = &tl->items[i];
        // Save in CSV format: id,done,text
        fprintf(file, "%d,%s,%s\n",
                t->id,
                t->done ? "true" : "false",
                t->text ? t->text : "");
    }

    fclose(file);
    return true;
}
