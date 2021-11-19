#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <omp.h>

// Size of the world
int row = 4098;
int col = 4098;

// Number of generations simulated
int N = 100;

//Returns the count of alive neighbours if a cell
int alive_neighbours(int *matrix, int m, int n)
{
  int i, j, count = 0;
  for (i = m - 1; i <= m + 1; i += 2)
  {
    for (j = n - 1; j <= n + 1; j++)
    {
      if (matrix[i * col + j] == 1)
      {
        count++;
      }
    }
  }
  i = m;
  for (j = n - 1; j <= n + 1; j += 2)
  {
    if (matrix[i * col + j] == 1)
    {
      count++;
    }
  }
  return count;
}
void print_matrix(int *matrix, const char myString[35])
{
  int i, j;
  printf("%s\n", myString);
  for (i = 0; i < row; i++)
  {
    for (j = 0; j < col; j++)
    {
      printf("%d ", matrix[i * col + j]);
    }
    printf("\n");
  }
  printf("-----------------------\n");
}

int main(int argc, char const *argv[])
{
  // Uncomment to start measuring time of the whole simulation together with rows 139->143
  // struct timespec begin, end;
  // clock_gettime(CLOCK_REALTIME, &begin);

  // Initialize variables
  int i, j;
  const char string_init[35] = "Initial world", string_cell_neigh[35] = "Alive neighgbours of each cell", string_next[35] = "Updated world";
  int count_alive_neighbours;
  int *world = (int *)malloc(row * col * sizeof(int *));
  int *worldNextGen = (int *)malloc(row * col * sizeof(int *));

  // Uncomment line 67, 126 and 135 to print a matrix showing the number of adjacent neighbours alive for every cell
  //int *cell_nr_of_neighbours = (int *)malloc(row * col * sizeof(int *));

  // Initialize the first generation of cells in the world with a boarder of zeros that stays zero during the entire simulation
  for (i = 0; i < row; i++)
  {
    for (j = 0; j < col; j++)
    {
      if (i == 0 || i == row - 1 || j == 0 || j == col - 1)
      {
        *(world + i * col + j) = 0;
      }
      else
      {
        *(world + i * col + j) = rand() % 2;
      }
    }
  }

  // Simulates N generations
  for (int n = 1; n <= N; n++)
  {
    // printf("%d generation\n", n);

    //Counting nr of alive neigbours of every element
    #pragma omp parallel num_threads(4)
    {
      #pragma omp for schedule(static, 1024)
      for (i = 1; i < row - 1; i++)
      {
        for (j = 1; j < col - 1; j++)
        {
          count_alive_neighbours = alive_neighbours(world, i, j);
    
          // Uncomment if printing nr of neighbours matrix
          // cell_nr_of_neighbours[i * col + j] = count_alive_neighbours;

          //   Game rules
          // Any dead cell with three live neighbours becomes a live cell.
          // Any live cell with two or three live neighbours survives.
          // All other live cells die in the next generation. Similarly, all other dead cells stay dead.

          if (world[i * col + j] == 0 && count_alive_neighbours == 3)
          {
            worldNextGen[i * col + j] = 1;
          }
          else if (world[i * col + j] == 1 && count_alive_neighbours == 2 || count_alive_neighbours == 3)
          {
            worldNextGen[i * col + j] = 1;
          }
          else
          {
            worldNextGen[i * col + j] = 0;
          }
        }
      }
    }

    // Uncomment line 125 and 127 in order to print the initial world and updated world for every time step
    // print_matrix(world, "Initial world");
    // print_matrix(cell_nr_of_neighbours, "Alive neighgbours of each cell");
    // print_matrix(worldNextGen, "Updated world");

    // Copy the updated matrix into the initial matrix
    memcpy(world, worldNextGen, ((row + 2) * (col + 2) * sizeof(int)));
  }

  free(world);
  free(worldNextGen);
  //free(cell_nr_of_neighbours);

  // Uncomment to measure time for the whole simulation in main
  // Stop measuring time, calculate the elapsed time and print
  // clock_gettime(CLOCK_REALTIME, &end);
  // long seconds = end.tv_sec - begin.tv_sec;
  // long nanoseconds = end.tv_nsec - begin.tv_nsec;
  // double elapsed = seconds + nanoseconds * 1e-9;
  // printf("Time measured: %.9f seconds.\n", elapsed);

  return 0;
}
