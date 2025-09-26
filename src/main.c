#include <stdio.h>
#include "todo.h"

int main()
{
    printf("Todo CLI Application\n");

    // Initialize todo list
    TaskList *todo_list = todo_init();

    if (todo_list == NULL)
    {
        printf("Failed to initialize todo list\n");
        return 1;
    }

    // Your main program logic goes here
    printf("Todo list initialized successfully!\n");

    // Clean up
    todo_free(todo_list);

    return 0;
}
