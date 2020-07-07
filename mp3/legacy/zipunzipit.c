#define EXTENSION_CHARLEN 3
#define ASCII_MAXVALUE 127

#define SWAP(a, b, type) { \
   type temp;              \
   temp = a;               \
   a = b;                  \
   b = temp;               \
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct _node {
   char symbol;
   int freq;
   struct _node* left;
   struct _node* right;
};
typedef struct _node* node; 

char* dectobin(int dec) {
   int msb;
   char* binary;
   for(msb = 0 ; dec >= pow(2.0, msb+1.0) ; msb++);
   binary = malloc(sizeof(char)*(msb+1+1));
   for(int i=msb ; i>=0 ; i--) {}
   binary[msb+1] = '\0';
   return binary;
}
void PQinsert(node* pq, int* n, node value) {
   pq[(*n)++] = value;
}
node PQdelete(node* pq, int* n) {
   int min = 0;
   for(int i=0 ; i<*n ; i++) {
      if(pq[min]->freq > pq[i]->freq)
         min = i;
   }
   SWAP(pq[min], pq[*n-1], node);
   return pq[--(*n)];
}
char* compress(char* str, FILE* outfile) {
   int freq[ASCII_MAXVALUE+1] = {0};
   node pq[ASCII_MAXVALUE+1];
   node data, huffmantree;
   int pqsize = 0;
   for(unsigned long i=0 ; i<strlen(str) ; i++) {
      freq[(unsigned int)str[i]]++;
   }
   for(int i=0 ; i<=ASCII_MAXVALUE ; i++) {
      if(!freq[i]) continue;
      // char i occurs freq[i] times
      data = malloc(sizeof(struct _node));
      data->symbol = (char)i;
      data->freq = freq[i];
      data->left = data->right = NULL;
      PQinsert(pq, &pqsize, data);
   }

   while(pqsize != 1) {
      node v = PQdelete(pq, &pqsize);
      node w = PQdelete(pq, &pqsize);

      node new = malloc(sizeof(struct _node));
      new->left = v;
      new->right = w;
      new->freq = v->freq + w->freq;
      new->symbol = -1;

      PQinsert(pq, &pqsize, new);
   }
   huffmantree = pq[0];




}
char* decompress(char* str) {

}
int main(int argc, const char* argv[]) {
   FILE* fp;
   char *str, *result;
   long filelen;
   char* out_fname;
   
   if(argc != 3) {
      printf("invalid number of arguments\n");
      printf("usage: %s [-c|-d] filename\n", argv[0]);
      exit(1);
   }
   const char* filename = argv[2];

   // read raw bytes from file
   if(!(fp = fopen(filename, "rb"))) {
      printf("cannot open file: %s\n", filename);
      exit(1);
   }
   fseek(fp, 0, SEEK_END);
   filelen = ftell(fp);
   rewind(fp);

   str = malloc(sizeof(char)*(filelen+1));
   fread(str, filelen, 1, fp);
   fclose(fp);
   str[filelen] = '\0';

   // output file name
   out_fname = malloc(sizeof(char)*(strlen(filename)+EXTENSION_CHARLEN+1));
   strncpy(out_fname, filename, strlen(filename));

   if(!strcmp(argv[1], "-c")) {
      // compress
      result = compress(str, fopen(out_fname, "wb"));
      strncat(out_fname, ".zz", 3);
   } else if(!strcmp(argv[1], "-d")) {
      // decompress
      result = decompress(str);
      strncat(out_fname, ".yy", 3);
   } else {
      printf("invalid option: %s\n", argv[1]);
      exit(1);
   }

   fp = fopen(out_fname, "wb");
   fwrite(result, strlen(result), 1, fp);

   return 0;
}
