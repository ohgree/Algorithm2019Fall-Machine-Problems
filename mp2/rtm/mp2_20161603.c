#define MIN_INT -2147483648
#define SWAP(a, b, type) { \
   type temp;    \
   temp = a;     \
   a = b;        \
   b = temp;     \
}
#define logB(x, base) log(x)/log(base)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
/**
 * Performs insertion sort on given list.
 * @param list Integer array pointer to sort.
 * @param n    Number of elements in the array list.
 */
void insertion_sort(int* list, int n) {
   int j;
   int key;
   for(int i=1 ; i<n ; i++) {
      key = list[i];
      for(j=i-1 ; j>=0 && list[j] > key; j--) {
         list[j+1] = list[j];
      }
      list[j+1] = key;
   }
}
/**
 * Partition function for quicksort.
 * The pivot is the rightmost element in the given list.
 * @param  list  List to partition.
 * @param  left  Left index of the list. Inclusive
 * @param  right Right index of the list. Inclusive
 * @return       Pivot index in the list.
 */
int partition(int* list, int left, int right) {
   int pivot;
   pivot = left;
   for(int i=left ; i<right ; i++) {
      if(list[right] > list[i]) {
         SWAP(list[i], list[pivot], int);
         pivot++;
      }
   }
   SWAP(list[right], list[pivot], int);
   return pivot;
}
/**
 * Standard quicksort algorithm
 * @param list  Integer array list to perform quicksort
 * @param left  Left index of the array. Inclusive.
 * @param right Right index of the array. Inclusive.
 */
void quick_sort(int* list, int left, int right) {
   int pivot;
   if(left >= right)
      return;
   pivot = partition(list, left, right);
   quick_sort(list, left, pivot-1);
   quick_sort(list, pivot+1, right);
}
/**
 * Adjust function for heapsort.
 * Two subtrees of given root is max heap. And this function
 * correctly places the value in root node into the heap.
 * @param list Whole heap array
 * @param root Root node number
 * @param n    Total number of nodes in the entire heap array list.
 */
void adjust(int* list, int root, int n) {
   int child;
   int rkey;
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
/**
 * Performs heapsort by creating a new heap, and removes elements from it
 * one by one.
 * @param list Integer array list to sort.
 * @param n    Total number of elements in the array list.
 */
void heap_sort(int* list, int n) {
   //save last element
   int last = list[n];

   //shift list right by one, to use heap
   for(int i=n ; i>0 ; i--)
      list[i] = list[i-1];

   //creating a max heap
   for(int i=n/2 ; i>0 ; i--)
      adjust(list, i, n);

   //removing root values from heap
   for(int i=n-1 ; i>0 ; i--) {
      SWAP(list[1], list[i+1], int);
      adjust(list, 1, i);
   }

   //shift list left by one
   for(int i=0 ; i<n ; i++)
      list[i] = list[i+1];

   //restore last element
   list[n] = last;
}
/**
 * Return median of three integers given.
 * @param  a Pointer to an integer.
 * @param  b Pointer to an integer.
 * @param  c Pointer to an integer.
 * @return   Address pointing to the median value.
 */
int* median3(int* a, int* b, int* c) {
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
/**
 * Algorithm 4 Implementation.
 * Basically a quicksort algorithm with tail recursion optimising.
 * Uses insertion sort if current list size is less than certain number.
 * Performs heapsort if total depth of quicksort tree is below a certain
 * level, to improve execution time.
 * @param data  Integer list to sort.
 * @param from  Left index of specified list. Inclusive.
 * @param to    Right index of specified list. Inclusive.
 * @param depth Maximum depth for this function to recursively call itself.
 */
void intro_sort(int* data, int from, int to, int depth) {
   int pivot;
   int from2, to2;
   int* m;
   from2 = from;
   to2 = to;
   // improving performance using tail recursion optimisation
   while(to2 > from2) {
      if(to2-from2+1 <= 16) {
         // use insertion sort when size <= 16 elements
         insertion_sort(data+from2, to2-from2+1);
         return;
      }
      if(!depth) {
         // using heapsort when depth reached its limit
         heap_sort(&data[from2], to2-from2+1);
         return;
      }
      depth--;
      // use median-of-three method to get the value to use as pivot.
      m = median3(&data[from2], &data[(from2+to2)/2], &data[to2]);
      SWAP(*m, data[to2], int);
      pivot = partition(data, from2, to2);

      // only the smallest of the two divided list is recursively called.
      if(pivot < (from2+to2)/2) {
         intro_sort(data, from2, pivot-1, depth);
         from2 = pivot+1;
      } else {
         intro_sort(data, pivot+1, to2, depth);
         to2 = pivot-1;
      }
   }
}
int main(int argc, const char* argv[]) {
   int n;
   int* data;
   FILE* fp;
   // error check
   if(argc != 3) {
      fprintf(stderr, "Usage: %s filename index\n", argv[0]);
      exit(1);
   }
   if(!(fp = fopen(argv[1], "r"))) {
      fprintf(stderr, "Cannot open file %s\n", argv[1]);
      exit(-1);
   }
   // read file
   fscanf(fp, "%d", &n);
   data = malloc(sizeof(int)*(n+1));
   for(int i=0 ; i<n ; i++)
      fscanf(fp, "%d", &data[i]);
   fclose(fp);

   // timer start
   clock_t start_time = clock();
   switch(atoi(argv[2])) {
      case 1: insertion_sort(data, n); break;
      case 2: quick_sort(data, 0, n-1); break;
      case 3: heap_sort(data, n); break;
      case 4: intro_sort(data, 0, n-1, 2*(int)logB(n, 2)); break;
      default:
              fprintf(stderr, "Algorithm index must be between 1-4\n");
              exit(1);
   }
   // timer finish
   clock_t term_time = clock();

   //use strchr() to correctly get filename from path
   char* path = (char*)argv[1];

   int fnameLen = 10;

   char* fname;
   fname = path;
   while((path = strchr(path, '/'))) {
      fname = ++path;
   }
   fnameLen += strlen(fname);
   
   char* outfile = malloc(sizeof(char)*fnameLen+1);
   strcpy(outfile, "result_");
   strcat(outfile, argv[2]);
   strcat(outfile,  "_");
   strcat(outfile, fname);

   // writing to file
   if(!(fp = fopen(outfile, "w"))) {
      fprintf(stderr, "Writing to file %s failed\n", fname);
      exit(-1);
   }

   free(outfile);

   fprintf(fp, "%s\n%s\n%d\n%.6f\n", argv[1], argv[2], n,
         ((double)(term_time-start_time)/CLOCKS_PER_SEC));
   for(int i=0 ; i<n-1; i++) {
      fprintf(fp, "%d ", data[i]);
   }
   fprintf(fp, "%d", data[n-1]);
   fclose(fp);
   free(data);

   return 0;
}
