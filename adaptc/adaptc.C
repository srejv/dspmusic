//adaptc.c - non real-time adaptation demonstration
#include <stdio.h>
#include <math.h>
#define beta 0.01                      //convergence rate
#define N  21                          //order of filter 
#define NS  60                         //number of samples
#define Fs  8000                       //sampling frequency
#define pi  3.1415926                     
#define DESIRED 2*cos(2*pi*T*1000/Fs)  //desired signal
#define NOISE sin(2*pi*T*1000/Fs)      //noise signal

float desired[NS], Y_out[NS], error[NS];

void main()
{
  long I, T;
  float D, Y, E; 
  float W[N+1] = {0.0};
  float X[N+1] = {0.0};

  for (T = 0; T < NS; T++)            //start adaptive algorithm
  {    
    X[0] = NOISE;                     //new noise sample
    D = DESIRED;                      //desired signal
    Y = 0;                            //filter'output set to zero
    for (I = 0; I <= N; I++)
      Y += (W[I] * X[I]);              //calculate filter output
    E = D - Y;                        //calculate error signal
    for (I = N; I >= 0; I--)         
    {
       W[I] = W[I] + (beta*E*X[I]);    //update filter coefficients
       if (I != 0) X[I] = X[I-1];     //update data sample 
    }   
    desired[T] = D; 
    Y_out[T] = Y;
    error[T] = E;
  }   
  printf("done!\n");
}


