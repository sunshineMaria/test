#include <stdio.h>

#include <stdlib.h>

#include <direct.h>

char old_dir[MAXDIR];

char new_dir[MAXDIR];

int main(void)

{

   if (getcurdir(0, old_dir))

   {

      perror("getcurdir()");

      exit(1);

   }

   printf("Current directory is: \\%s\n", old_dir);

   if (chdir("\\"))

   {

      perror("chdir()");

      exit(1);

   }

   if (getcurdir(0, new_dir))

   {

      perror("getcurdir()");

      exit(1);

   }

   printf("Current directory is now: \\%s\n", new_dir);

   printf("\nChanging back to orignal directory: \\%s\n", old_dir);

   if (chdir(old_dir))

   {

      perror("chdir()");

      exit(1);

   }

   return 0;

}
