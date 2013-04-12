#include <stdio.h>
#include <c6x.h>
#include "c6xdsk.h"
#include "codec_edma.h"
#define  DATA_SIZE 2
#define  BLOCK_SIZE 80
#define  NUM_OF_BLOCKS 18
#define  DELAY 5

/* change to TI code */
#include "filter.h" 


int pcm_out,pcm_in,dma_index;
int src,dst;
short *in_ptr,*out_ptr;
short in[NUM_OF_BLOCKS*BLOCK_SIZE], out[NUM_OF_BLOCKS*BLOCK_SIZE];
int main()
{
 
  int i;
	/* dsp and periphiral initialization */    
	CSR=0x100;			         /* disable all interrupts            */
  IER=1;                   /* disable all interrupts except NMI */
  ICR=0xffff;              /* clear all pending interrupts      */
  *(unsigned volatile int *)EMIF_GCR = 0x3300; 	     /* EMIF global control               */
  *(unsigned volatile int *)EMIF_CE0 = 0x30;         /* EMIF CE0control                   */
  *(unsigned volatile int *)EMIF_CE1 = 0xffffff03;	 /* EMIF CE1 control, 8bit async      */
  *(unsigned volatile int *)EMIF_SDCTRL = 0x07117000; /* EMIF SDRAM control                */
  *(unsigned volatile int *)EMIF_SDRP = 0x61a;       /* EMIF SDRM refresh period          */
  *(unsigned volatile int *)EMIF_SDEXT = 0x54519; /* EMIF SDRAM extension    */

  for (i=0; i<NUM_OF_BLOCKS*BLOCK_SIZE; i++)     /* clear all buffers  */
  {
    in[i]=0;
    out[i]=0;
  }

  out[0] = 0xaa;
  out[1] = 0;
  out[2] = 0x1;
  out[3] = 0x0386;         /* data for software reset */
  out[4] = 0;

  
  out[5] = 0;
  out[6] = 0x1;
  out[7] = 0x0306;         /* set reg 3 for mic input */
  out[8] = 0;

  out[9] = 0;
  out[10] = 0x1;
  out[11] = 0x0400;         /* data for software reset */
  out[12] = 0;

  out[13] = 0;
  out[14] = 0x1;
  out[15] = 0x0502;         /* data for software reset */
  out[16] = 0;

  ICR = 0x100;
  IER |= 0x102;             /* enable int 8 (EDMA) */
  CSR |= 1;
  
   
  edma_init(); 
  mcbsp0_init();

  while (1) {}

 
}

void mcbsp0_init()
{
    /* set up McBSP0                                                                   */
    *(unsigned volatile int *)McBSP0_SPCR = 0;			        /*  reset serial port                         */
    *(unsigned volatile int *)McBSP0_PCR = 0;              /*  set pin control reg.;                     */
    *(unsigned volatile int *)McBSP0_RCR = 0x10040;        /*  set rx control reg. one 16 bit data/frame */
    *(unsigned volatile int *)McBSP0_XCR = 0x10040;        /*  set tx control reg. one 16 bit data/frame */
    *(unsigned volatile int *)McBSP0_DXR = 0;
    *(unsigned volatile int *)McBSP0_SPCR = 0x12001;       /*  setup SP control reg.;                    */
}

void edma_init()
{ 
  *(unsigned volatile int *)ECR = 0xffff;    /* clear all pending edma event */
  *(unsigned volatile int *)EER = 0x3000;    /* enable event 12 & 13 */

  *(unsigned volatile int *)CIPR = 0xffff;   /* clear all pending edma interrupts */
  *(unsigned volatile int *)CIER = 0x100;    /* enable CCE8 */
  pcm_in = (int)&in;
  pcm_out = (int)&out;
  in_ptr = (short *)pcm_in;
  out_ptr = (short *)( pcm_out + DELAY*BLOCK_SIZE*DATA_SIZE);

  dma_index = 1;
  /* set up edma for mcbsp0 transmitter */
  *(unsigned volatile int *)(EVENTC_PARAMS+OPT) = 0x49000002;
  *(unsigned volatile int *)(EVENTC_PARAMS+SRC) = pcm_out;
  *(unsigned volatile int *)(EVENTC_PARAMS+CNT) = BLOCK_SIZE;
  *(unsigned volatile int *)(EVENTC_PARAMS+DST) = McBSP0_DXR;
  *(unsigned volatile int *)(EVENTC_PARAMS+IDX) = 0;
  *(unsigned volatile int *)(EVENTC_PARAMS+LNK) = EVENTN_PARAMS;
  
  /* set up edma for mcbsp0 receiver  */
  *(unsigned volatile int *)(EVENTD_PARAMS+OPT) = 0x48380002;
  *(unsigned volatile int *)(EVENTD_PARAMS+SRC) = McBSP0_DRR;
  *(unsigned volatile int *)(EVENTD_PARAMS+CNT) = BLOCK_SIZE;
  *(unsigned volatile int *)(EVENTD_PARAMS+DST) = pcm_in;
  *(unsigned volatile int *)(EVENTD_PARAMS+IDX) = 0;
  *(unsigned volatile int *)(EVENTD_PARAMS+LNK) = EVENTO_PARAMS;

  src = pcm_out+BLOCK_SIZE*DATA_SIZE;
  dst = pcm_in+BLOCK_SIZE*DATA_SIZE;
  
  /* set up edma reload params for mcbsp0 transmitter  */
  *(unsigned volatile int *)(EVENTN_PARAMS+OPT) = 0x49000002;
  *(unsigned volatile int *)(EVENTN_PARAMS+SRC) = src;
  *(unsigned volatile int *)(EVENTN_PARAMS+CNT) = BLOCK_SIZE;
  *(unsigned volatile int *)(EVENTN_PARAMS+DST) = McBSP0_DXR;
  *(unsigned volatile int *)(EVENTN_PARAMS+IDX) = 0;
  *(unsigned volatile int *)(EVENTN_PARAMS+LNK) = EVENTN_PARAMS;

  /* set up edma reload params for mcbsp0 receiver  */
  *(unsigned volatile int *)(EVENTO_PARAMS+OPT) = 0x48380002;
  *(unsigned volatile int *)(EVENTO_PARAMS+SRC) = McBSP0_DRR;
  *(unsigned volatile int *)(EVENTO_PARAMS+CNT) = BLOCK_SIZE;
  *(unsigned volatile int *)(EVENTO_PARAMS+DST) = dst;
  *(unsigned volatile int *)(EVENTO_PARAMS+IDX) = 0;
  *(unsigned volatile int *)(EVENTO_PARAMS+LNK) = EVENTO_PARAMS;
  
}

interrupt void edma_isr()
{
    int i,temp;
    if ( dma_index  == NUM_OF_BLOCKS -1) 
    {
		dma_index =0;
		dst = pcm_in;	 
		src =  pcm_out;
    }
    else 
    {
		dma_index++;		
		src += BLOCK_SIZE*DATA_SIZE;
		dst += BLOCK_SIZE*DATA_SIZE;
    }

    *(unsigned volatile int *)CIPR |= 0x100;  /* clear CCE8 bit  */
    *(unsigned volatile int *)(EVENTN_PARAMS+SRC) = src;
    *(unsigned volatile int *)(EVENTO_PARAMS+DST) = dst;

    temp = (int)in_ptr - pcm_in - NUM_OF_BLOCKS*BLOCK_SIZE*DATA_SIZE;
    if (temp >= 0) 
    {
      in_ptr = (short *)pcm_in;  
    }
      
    temp = (int)out_ptr - pcm_out - NUM_OF_BLOCKS*BLOCK_SIZE*DATA_SIZE;
    if (temp >= 0) 
    {
      out_ptr = (short *)pcm_out;  
    }
/*	This is the original code to copy the input buffer to the output buffer
    It is replaced with the code to process the block below

    for (i=0;i<BLOCK_SIZE;i++) *out_ptr++ = *in_ptr++ & 0xfffe;
*/

 ProcBlock(in_ptr,out_ptr, BLOCK_SIZE);
 in_ptr  += BLOCK_SIZE;
 out_ptr += BLOCK_SIZE;
 
}
