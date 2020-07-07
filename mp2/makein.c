#define SWAP(a, b, type) { \
   type temp;    \
   temp = a;     \
   a = b;        \
   b = temp;     \
}
#include <stdio.h>
#include <stdlib.h>
int partition(int* list, int left, int right) {
   int pivot;
   pivot = left;
   for(int i=left ; i<right ; i++) {
      if(list[right] < list[i]) {
         SWAP(list[i], list[pivot], int);
         pivot++;
      }
   }
   SWAP(list[right], list[pivot], int);
   return pivot;
}
void quick_sort(int* list, int left, int right) {
   int pivot;
   if(left >= right)
      return;
   pivot = partition(list, left, right); 
   quick_sort(list, left, pivot-1);
   quick_sort(list, pivot+1, right);
}
int main(int argc, const char* argv[]) {
   FILE* fp;
   if(argc < 6) {
      fprintf(stderr, "Usage: %s filename num min max sorted [seed]\n", argv[0]);
      exit(1);
   }
   if(!(fp = fopen(argv[1], "w"))) {
      fprintf(stderr, "Error while opening file %s\n", argv[0]);
      exit(1);
   }
   int num = atoi(argv[2]);
   int min = atoi(argv[3]);
   int max = atoi(argv[4]);
   int* data;
   data = malloc(sizeof(int)*num);
   srand(atoi(argv[6]));
   for(int i=0 ; i<num ; i++) {
      data[i] = rand()%(max-min)+min;
   }

   if(atoi(argv[5])) {
      quick_sort(data, 0, num-1);
   }

   fprintf(fp, "%d ", num);
   for(int i=0 ; i<num-1 ; i++) {
      fprintf(fp, "%d ", data[i]);
   }
   fprintf(fp, "%d", data[num-1]);
   fclose(fp);

   return 0;
}
