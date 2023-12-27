#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

typedef struct 
{
  char root[256];
  int leaf;
} node;

int
isDirectory (const char *path)
{
  struct stat path_stat;
  stat(path, &path_stat);
  return S_ISREG(path_stat.st_mode);
}

int 
listDirectory (const char *path)
{
  printf("!!! %s\n", path);
  node *buff_node;
  DIR *dir;
  struct dirent *entry;
  char buff_path[256];
  dir = opendir(path);

  if (!dir)
  {
    fprintf(stderr, "[ERROR] Couldn't open a directory ");
    return EXIT_FAILURE;
  }
  

  while ((entry = readdir(dir)) != NULL)
  {
    if (strcmp("..", entry->d_name) == 0) { continue; }
    else {
    printf("%s\n", entry->d_name);

    if ((isDirectory(entry->d_name)) == 0) 
    { 
      sprintf(buff_path, "%s/%s/", path, entry->d_name);
      printf("%s\n", buff_path);
      listDirectory(entry->d_name); 
    }
  }
  }
  closedir(dir);
  
  return EXIT_SUCCESS;
}

int
main (int argc, char *argv[])
{
  listDirectory(argv[1]);
  return EXIT_SUCCESS;
}
