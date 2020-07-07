//============================================================================//
// make_input.cc                                                              //
// create input files for mp1                                                 //
//                                                                            //
// version: 0.1, 9/26/2019                                                    //
// Jungmin So                                                                 //
//                                                                            //
// usage: make_input num_rows num_columns min max seed                        //
//============================================================================//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {

  if(argc != 7) {
    printf("usage: %s filename num_rows num_cols min max seed\n", argv[0]);
    return 0;
  }

  // record program start time
  clock_t start_time = clock();

  // store user arguments in variables
  char filename[200];
  strcpy(filename, argv[1]);
  int num_rows = atoi(argv[2]);
  int num_cols = atoi(argv[3]);
  int min = atoi(argv[4]);
  int max = atoi(argv[5]);
  int seed = atoi(argv[6]);

  // set random number generator seed
  srand(seed);

  // open output file
  FILE *outfile;
  outfile = fopen(filename, "w");
  if(outfile == NULL) {
    printf("Error: output file cannot be opened.\n");
    return 0;
  }

  fprintf(outfile, "%d %d\n", num_rows, num_cols);
  for(int i=0; i<num_rows; i++) {
    for(int j=0; j<num_cols; j++) {
      int rv = ( rand() % (max-min) ) + min;
      fprintf(outfile, "%d ", rv);
    }
    fprintf(outfile, "\n");
  }

  fclose(outfile);

  // record program end time
  clock_t end_time = clock();

  printf("A 2D array with %d rows and %d columns generated and saved to %s.\n", num_rows, num_cols, filename);
  printf("Running time: %.6f seconds\n", (double)(end_time - start_time)/CLOCKS_PER_SEC);

  return 0;
}
