-c
-heap  0x400
-stack 0x400 /* very large stack for DSP programs. */
-lrts6701.lib
  
MEMORY
{
    vecs:   o = 00000000h   l = 200h
    IRAM:   o = 00000200h   l = 0000ffe0h
    CE0:    o = 80000000h   l = 01000000h 
}

SECTIONS
{
    "vectors"   >       vecs
    .cinit      >       IRAM
    .text       >       IRAM
    .stack      >       IRAM
    .bss        >       IRAM
    .cinit      >       IRAM 
    .const      >       IRAM
    .data       >       IRAM
    .far        >       IRAM
    .switch     >       IRAM
    .sysmem     >       IRAM
    .tables     >       IRAM
    .cio        >       IRAM
}                             

 
