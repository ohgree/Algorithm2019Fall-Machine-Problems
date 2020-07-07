#define MIN_INT -2147483648
#define SWAP(a, b, type) { \
   type temp;    \
   temp = a;     \
   a = b;        \
   b = temp;     \
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
void bubble_sort(double* list, int length) {
   for(int i=0 ; i<length ; i++) {
      for(int j=0 ; j<length-i-1 ; j++) {
         if(list[j] > list[j+1]) {
            SWAP(list[j], list[j+1], double);
         }
      }
   }
}
void insertion_sort(double* list, int n) {
   int key, j;
   for(int i=1 ; i<n ; i++) {
      key = list[i];
      for(j=i-1 ; j>=0 && list[j] > key; j--) {
         list[j+1] = list[j];
      }
      list[j+1] = key;
   }
}
int partition(double* list, int left, int right) {
   int pivot;
   pivot = left;
   for(int i=left ; i<right ; i++) {
      if(list[right] > list[i]) {
         SWAP(list[i], list[pivot], double);
         pivot++;
      }
   }
   SWAP(list[right], list[pivot], double);
   return pivot;
}
void quick_sort(double* list, int left, int right) {
   int pivot;
   if(left >= right)
      return;
   pivot = partition(list, left, right); 
   quick_sort(list, left, pivot-1);
   quick_sort(list, pivot+1, right);
}
void adjust(double* list, int root, int n) {
   int child;
   double rkey;
   rkey = list[root];
   child = root*2;
   while(child <= n) {
      if(child < n && list[child] < list[child+1])
         child++;

      if(rkey > list[child])
         break;

      list[child/2] = list[child];
      child *= 2;
   }
   list[child/2] = rkey;
}
void heap_sort(double* list, int n) {
   //save last element
   double last = list[n];
   //shift list right by one, to use heap
   for(int i=n ; i>0 ; i--)
      list[i] = list[i-1];
   for(int i=n/2 ; i>0 ; i--) 
      adjust(list, i, n);
   for(int i=n-1 ; i>0 ; i--) {
      SWAP(list[1], list[i+1], double);
      adjust(list, 1, i);
   }
   for(int i=0 ; i<n ; i++)
      list[i] = list[i+1];
   //restore last element
   list[n] = last;
}
double* median3(double* a, double* b, double* c) {
   if(*a <= *b && *b <= *c)
      return b;
   if(*a <= *c && *c <= *b)
      return c;
   if(*b <= *a && *a <= *c)
      return a;
   if(*b <= *c && *c <= *a)
      return c;
   if(*c <= *a && *a <= *b)
      return a;
   if(*c <= *b && *b <= *a)
      return b;
   return NULL;
}
void intro_sort(double* data, int from, int to, int depth) {
   int pivot;
   if(to-from+1 <= 16) {
      //TODO: use insertion sort when (to-from+1) <= 16 elements
      insertion_sort(data+from, to-from+1);
   }
   if(from >= to)
      return;
   if(!depth) {
      //using heapsort when depth reached its limit
      heap_sort(&data[from], to-from+1);
      return;
   }
   depth--;
   double* m = median3(&data[from], &data[(from+to)/2], &data[to]);
   SWAP(*m, data[to], double);
   pivot = partition(data, from, to);
   intro_sort(data, from, pivot-1, depth);
   intro_sort(data, pivot+1, to, depth);
}
int main(int argc, const char* argv[]) {
   int n;
   double* data;
   FILE* fp;
   if(argc != 3) {
      fprintf(stderr, "Usage: %s filename index\n", argv[0]);
      exit(1);
   }
   if(!(fp = fopen(argv[1], "r"))) {
      fprintf(stderr, "Cannot open file %s\n", argv[1]);
      exit(1);
   }
   fscanf(fp, "%d", &n);
   data = malloc(sizeof(double)*(n+1));
   for(int i=0 ; i<n ; i++)
      fscanf(fp, "%lf", &data[i]);
   fclose(fp);

   clock_t start_time = clock();
   switch(atoi(argv[2])) {
      case 1: //bubble_sort(data, n); break;
         insertion_sort(data, n); break;
      case 2: quick_sort(data, 0, n-1); break;
      case 3: heap_sort(data, n); break;
      case 4: intro_sort(data, 0, n-1, 2*(int)log2(n)); break;
      default:
              fprintf(stderr, "Algorithm index must be between 1-4\n");
              exit(1);
   }
   clock_t term_time = clock();


   printf("[DEBUG]\n");
   printf("%s\n%s\n%d\n%.6f\n", argv[1], argv[2], n,
         ((double)(term_time-start_time)/CLOCKS_PER_SEC));
   for(int i=0 ; i<n; i++) {
      printf("%lg ", data[i]);
   }

   return 0;
}
