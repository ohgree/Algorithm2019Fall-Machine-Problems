#include <stdio.h>
int max3(int a, int b, int c) {
   if(a < b)
      a = b;
   if(a < c)
      a = c;
   return a;
}
int algo3(int a[], int left, int right) {
   int centre;

   if(left == right) 
      return a[left] > 0 ? a[left] : 0;

   centre = (left+right)/2;
   int leftMax, rightMax;
   leftMax = algo3(a, left, centre);
   rightMax = algo3(a, centre+1, right);

   int leftPartSum, rightPartSum, maxLPart, maxRPart;
   leftPartSum = rightPartSum = maxLPart = maxRPart = 0;
   for(int i=centre ; i>=left ; i--) {
      leftPartSum += a[i];
      if(leftPartSum > maxLPart)
         maxLPart = leftPartSum;
   }
   for(int i=centre+1 ; i<=right ; i++) {
      rightPartSum += a[i];
      if(rightPartSum > maxRPart)
         maxRPart = rightPartSum;
   }
   return max3(leftMax, rightMax, maxLPart+maxRPart);
}
int kadane(int a[], int length) {
   int thisSum, maxSum;
   thisSum = maxSum = 0;
   for(int i=0 ; i<length ; i++) {
      thisSum += a[i];
      if(thisSum > maxSum)
         maxSum = thisSum;
      else if(thisSum < 0)
         thisSum = 0;
   }
   return maxSum;
}
int main(int argc, const char* argv[]) {
   int a[6] = {-1, 11, -4, 13, -5, -2};
   printf("%d %d", algo3(a, 0, 5), kadane(a, 6));
   return 0;
}
