//dotp4.c dot product of two vectors 

int dotp(short *a, short *b, int ncount); //function prototype
#include <stdio.h>              //for printf
#define count 4                 //# of data in each array
short x[count] = {1,2,3,4};	    //declaration of 1st array
short y[count] = {0,2,4,6};     //declaration of 2nd array

main()
{
  int result = 0;    			//result sum of products

  result = dotp(x, y, count);	//call dotp function
  printf("result = %d (decimal) \n", result); //print result
}

int dotp(short *a, short *b, int ncount) //dot product function
{                        		 
  int i; 
  int sum = 0;  
  for (i = 0; i < ncount; i++) 
     sum += a[i] * b[i];       	//sum of products
  return(sum);                  //return sum as result
}
