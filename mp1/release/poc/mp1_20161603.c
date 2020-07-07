#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MIN_INT -2147483648

struct _position {
   int row;
   int col;
};
typedef struct _position* position;
int mss_pow6(int** matrix, int m, int n) {
   int mss = MIN_INT, sum;
   position from, to;

   from = malloc(sizeof(struct _position));
   to = malloc(sizeof(struct _position));

   from->row = from->col = 0;
   to->row = to->col = 0;

   for(from->row = 0 ; from->row < m ; from->row++) {
      for(from->col = 0 ; from->col < n ; from->col++) {
         for(to->row = from->row ; to->row < m ; to->row++) {
            for(to->col = from->col ; to->col < n ; to->col++) {
               sum = 0;
               for(int i=from->row ; i<=to->row ; i++) {
                  for(int j=from->col ; j<=to->col ; j++) {
                     sum += matrix[i][j];
                  }
               }
               if(sum > mss) mss = sum;
            }
         }
      }
   }

   free(from);
   free(to);
   return mss;
}
int mss_pow4(int** matrix, int m, int n) {
   int mss = MIN_INT, partial_mss = MIN_INT;
   int* sub_arr = malloc(sizeof(int)*m);

   for(int left = 0 ; left < n ; left++) {
      memset(sub_arr, 0, m*sizeof(int));
      for(int right = left ; right < n ; right++) {
         for(int i=0 ; i<m ; i++)
            sub_arr[i] += matrix[i][right];

         int arr_sum;
         for(int i=0 ; i<m ; i++) {
            arr_sum = 0;
            for(int j=i ; j<m ; j++) {
               arr_sum += sub_arr[j];
               if(arr_sum > mss)
                  mss = arr_sum;
            }
         }
      }
   }
   free(sub_arr);
   return mss;
}
int mss_subseq(int* arr, int length) {
   int sum, mss = MIN_INT;
   sum = 0;
   for(int i=0 ; i<length ; i++) {
      sum += arr[i];
      if(sum > mss) mss = sum;
      if(sum < 0) sum = 0;
   }
   return mss;
}
int mss_pow3(int** matrix, int m, int n) {
   int mss = MIN_INT, partial_mss = MIN_INT;
   int* sub_arr = malloc(sizeof(int)*m);

   for(int left = 0 ; left < n ; left++) {
      memset(sub_arr, 0, m*sizeof(int));
      for(int right = left ; right < n ; right++) {
         for(int i=0 ; i<m ; i++)
            sub_arr[i] += matrix[i][right];
         partial_mss = mss_subseq(sub_arr, m);
         if(partial_mss > mss)
            mss = partial_mss;
      }
   }
   free(sub_arr);
   return mss;
}

int main(int argc, const char* argv[]) {
   FILE* fp;
   int algorithm_index;
   int mss, m, n, **mat;
   if(argc != 3) {
      fprintf(stderr, "Usage: %s [filename] [algorithm index]\n", argv[0]);
      exit(1);
   }
   if(!(fp = fopen(argv[1], "r"))) {
      fprintf(stderr, "File not found!\n");
      exit(1);
   }

   fscanf(fp, " %d %d", &m, &n);
   mat = malloc(sizeof(int*)*m);
   for(int i=0 ; i<m ; i++)
      mat[i] = malloc(sizeof(int)*n);
   for(int i=0 ; i<m ; i++) {
      for(int j=0 ; j<n ; j++) {
         fscanf(fp, "%d", &mat[i][j]);
      }
   }


   algorithm_index = atoi(argv[2]);

   clock_t start_time = clock();

   switch(algorithm_index) {
      case 1:
         mss = mss_pow6(mat, m, n);
         break;
      case 2:
         mss = mss_pow4(mat, m, n);
         break;
      case 3:
         mss = mss_pow3(mat, m, n);
         break;
      default:
         fprintf(stderr, "Invalid algorithm index!\n");
         fclose(fp);
         exit(1);
   }
   clock_t termination_time = clock();
   fclose(fp);

   for(int i=0 ; i<m ; i++)
      free(mat[i]);
   free(mat);

   printf("%s\n", argv[1]);
   printf("%d\n%d\n%d\n%d\n%.6f\n", algorithm_index, m, n, mss,
         ((double)(termination_time-start_time)/CLOCKS_PER_SEC)*1000);
}
