//Noise_gen.h header file for pseudo-random noise sequence

typedef struct BITVAL    //register bits to be packed as integer
{
 unsigned int b0:1, b1:1, b2:1, b3:1, b4:1, b5:1, b6:1;
 unsigned int b7:1, b8:1, b9:1, b10:1, b11:1, b12:1,b13:1;
 unsigned int dweebie:2; //Fills the 2 bit hole - bits 14-15
} bitval;

typedef union SHIFT_REG
{
 unsigned int regval;
 bitval bt;
} shift_reg;

