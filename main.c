#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

typedef struct summary
{
  size_t dirs;
  size_t files;
} summary_t;

typedef struct node
{
  char *name;
  int is_dir;
  struct node *next;
} node_t;

int
listDirectory (const char *path, const char *cur_prefix, summary_t *summary)
{
    node_t *head = NULL, *current, *iter; // init
    char *full_path, *segment, *pointer, *next_prefix;
    size_t size = 0;
    size_t index;
  
    DIR *d = opendir(path); // directory variable
    struct dirent *dir; // struct to store data about directory/file

    if (!d)
    {
      fprintf(stderr, "[ERROR] Can't open directory {%s}\n", path);
    }
        summary->dirs++;
        while ((dir = readdir(d)) != NULL)
        {
            if (strcmp("..", dir->d_name) == 0 || (strcmp(".", dir->d_name)) == 0)
            {
                continue;
            }

            current = malloc(sizeof(node_t));
            current->name = strcpy(malloc(strlen(dir->d_name)), dir->d_name);
            current->is_dir = (dir->d_type == DT_DIR);
            current->next = NULL;
            
            if (head == NULL)
            {
              head = current;
            }
            else if (strcmp(current->name, head->name) < 0)
            {
              current->next = head;
              head = current;
            }
            else 
            {
              for (iter = head; iter->next && strcmp(current->name, iter->next->name) > 0; iter = iter->next);
              current->next = iter->next;
              iter->next = current;
            }
        ++size;
        }
        closedir(d);
        if (!head) {return 0;}

        for (index = 0; index < size; ++index)
        {
          if (index == size - 1)
          {
            pointer = "└── ";
            segment = "    ";
          }  
          else
          {
            pointer = "├── ";
            segment = "│   ";
          }

        printf("%s%s%s\n", cur_prefix, pointer, head->name);
        
        if (head->is_dir)
        {
          full_path = malloc(strlen(path) + strlen(head->name) + 2);
          sprintf(full_path, "%s/%s", path, head->name);

          next_prefix = malloc(strlen(cur_prefix) + strlen(head->name) + 1);
          sprintf(next_prefix, "%s%s", cur_prefix, segment);

          listDirectory(full_path, next_prefix, summary);
          free(full_path);
          free(next_prefix);
        }
        else
        {
          summary->files++;
        }
        current = head;
        head = head->next;

        free(current->name);
        free(current);
        }
    return EXIT_SUCCESS;
}

int
main (int argc, char *argv[])
{
  char *directory;

  if (argc != 2)
  {
    printf("[ERROR] Usage: %s [PATH]\n", argv[0]);
    return EXIT_FAILURE;
  }

  directory = argv[1];

  summary_t summary = {0, 0};

  listDirectory(directory, "", &summary);
  printf("\n%zu directories, %zu files\n", summary.dirs - 1, summary.files);
  return EXIT_SUCCESS;
}
