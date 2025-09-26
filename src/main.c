#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "todo.h"

/* --- helpers --- */
static void flush_stdin(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    { /* discard */
    }
}

static void read_line(char *buf, size_t sz)
{
    if (!fgets(buf, (int)sz, stdin))
    {
        if (sz)
            buf[0] = '\0';
        return;
    }
    char *p = strchr(buf, '\n');
    if (p)
        *p = '\0';
}

static void print_menu(void)
{
    puts("\n=== TODO CLI ===");
    puts("1) List tasks");
    puts("2) Add task");
    puts("3) Mark DONE");
    puts("4) Mark UNDONE");
    puts("5) Delete task");
    puts("6) Count");
    puts("7) Load from file (simple)");
    puts("8) Load from CSV file");
    puts("9) Save to file");
    puts("0) Exit\n");
    printf("Choose: ");
}

int main(void)
{
    TaskList *tl = todo_init();
    if (!tl)
    {
        fprintf(stderr, "init failed\n");
        return 1;
    }

    for (;;)
    {
        print_menu();

        int choice = -1;
        if (scanf("%d", &choice) != 1)
        {
            flush_stdin();
            continue;
        }
        flush_stdin();

        if (choice == 0)
        {
            break; // auto-clean on exit
        }

        switch (choice)
        {
        case 1:
        { // list
            todo_list_print(tl);
        }
        break;

        case 2:
        { // add
            char buf[TODO_MAX_DESC];
            printf("Description: ");
            read_line(buf, sizeof(buf));
            if (buf[0] == '\0')
            {
                puts("Empty description ignored.");
                break;
            }
            if (todo_add(tl, buf))
            {
                puts("Added.");
            }
            else
            {
                puts("Add failed.");
            }
        }
        break;

        case 3:
        { // done
            int id;
            printf("ID to mark DONE: ");
            if (scanf("%d", &id) != 1)
            {
                flush_stdin();
                puts("Invalid ID.");
                break;
            }
            flush_stdin();
            if (todo_toggle_done(tl, id, true))
                puts("Updated.");
            else
                puts("ID not found.");
        }
        break;

        case 4:
        { // undone
            int id;
            printf("ID to mark UNDONE: ");
            if (scanf("%d", &id) != 1)
            {
                flush_stdin();
                puts("Invalid ID.");
                break;
            }
            flush_stdin();
            if (todo_toggle_done(tl, id, false))
                puts("Updated.");
            else
                puts("ID not found.");
        }
        break;

        case 5:
        { // delete
            int id;
            printf("ID to delete: ");
            if (scanf("%d", &id) != 1)
            {
                flush_stdin();
                puts("Invalid ID.");
                break;
            }
            flush_stdin();
            if (todo_delete(tl, id))
                puts("Deleted.");
            else
                puts("ID not found.");
        }
        break;

        case 6:
        { // count
            printf("Total tasks: %zu\n", todo_size(tl));
        }
        break;

        case 7:
        { // Load from simple text file
            char filename[256];
            printf("Enter filename (e.g., todos.txt): ");
            read_line(filename, sizeof(filename));

            if (todo_load_from_file(tl, filename))
            {
                printf("Successfully loaded tasks from '%s'\n", filename);
            }
            else
            {
                printf("Failed to load from '%s' (file not found or empty)\n", filename);
            }
        }
        break;

        case 8:
        { // Load from CSV file
            char filename[256];
            printf("Enter CSV filename (e.g., todos.csv): ");
            read_line(filename, sizeof(filename));

            if (todo_load_csv_file(tl, filename))
            {
                printf("Successfully loaded CSV tasks from '%s'\n", filename);
            }
            else
            {
                printf("Failed to load CSV from '%s'\n", filename);
            }
        }
        break;

        case 9:
        { // Save to file
            char filename[256];
            printf("Enter filename to save (e.g., my_todos.csv): ");
            read_line(filename, sizeof(filename));

            if (todo_save_to_file(tl, filename))
            {
                printf("Successfully saved tasks to '%s'\n", filename);
            }
            else
            {
                printf("Failed to save to '%s'\n", filename);
            }
        }
        break;

        default:
            puts("Unknown choice.");
        }
    }

    todo_free(tl);
    puts("Goodbye!");
    return 0;
}
