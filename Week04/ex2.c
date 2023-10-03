#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>
#include <time.h>

const int SIZE = 120;

int main(void)
{

  int status;
  srand(time(0));
  FILE *file = fopen("temp.txt", "w");
  int u[SIZE], v[SIZE];
  for (int i = 0; i < SIZE; i++)
  {
    u[i] = rand() % 100;
    v[i] = rand() % 100;
  }

  int n;
  scanf("%d", &n);
  int processes[n];
  assert(SIZE % n == 0);
  int start = 0, end = SIZE / n;
  for (int i = 0; i < n; i++)
  {
    pid_t pid = fork();
    if (pid == 0)
    {
      int result = 0;
      for (int i = start; i < start + end; i++)
      {
        result += u[i] * v[i];
      }
      fprintf(file, "%d\n", result);
      exit(EXIT_SUCCESS);
    }
    else
    {
      processes[i] = pid;
    }
    start += end;
  }
  for (int i = 0; i < n; i++)
    waitpid(processes[i], &status, 0);
  fclose(file);

  long long res = 0;
  file = fopen("temp.txt", "r");
  for (int i = 0; i < n; i++)
  {
    int values;
    fscanf(file, "%d", &values);
    res += values;
  }
  printf("dot product of v and u is: %lld\n", res);

  fclose(file);
  return EXIT_SUCCESS;
}
