#include <stdio.h>
#include <stdlib.h>
int main(int argc, const char* argv[]) {
   FILE* fp;
   if(argc < 3) {
      fprintf(stderr, "Usage: %s filename num bound [seed]\n", argv[0]);
      exit(1);
   }
   if(!(fp = fopen(argv[1], "w"))) {
      fprintf(stderr, "Error while opening file %s\n", argv[0]);
      exit(1);
   }
   int num = atoi(argv[2]);
   int bound = atoi(argv[3]);
   srand(atoi(argv[4]));
   fprintf(fp, "%d ", num);
   for(int i=0 ; i<num ; i++) {
      fprintf(fp, "%lf ", (double)rand()/(double)(RAND_MAX/bound));
   }
   fclose(fp);
}
