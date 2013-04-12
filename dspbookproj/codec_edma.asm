;******************************************************************************
;* TMS320C6x ANSI C Codegen                                      Version 4.10 *
;* Date/Time created: Thu Oct 25 20:21:37 2001                                *
;******************************************************************************

;******************************************************************************
;* GLOBAL FILE PARAMETERS                                                     *
;*                                                                            *
;*   Architecture      : TMS320C620x                                          *
;*   Optimization      : Enabled at level 3                                   *
;*   Optimizing for    : Speed                                                *
;*                       Based on options: -o3, no -ms                        *
;*   Endian            : Little                                               *
;*   Interrupt Thrshld : Disabled                                             *
;*   Memory Model      : Small                                                *
;*   Calls to RTS      : Near                                                 *
;*   Pipelining        : Enabled                                              *
;*   Speculative Load  : Disabled                                             *
;*   Memory Aliases    : Presume are aliases (pessimistic)                    *
;*   Debug Info        : Debug                                                *
;*                                                                            *
;******************************************************************************

	.asg	A15, FP
	.asg	B14, DP
	.asg	B15, SP
	.global	$bss

	.file	"codec_edma.c"
	.global	_pcm_out
	.bss	_pcm_out,4,4
	.sym	_pcm_out,_pcm_out, 4, 2, 32
	.global	_pcm_in
	.bss	_pcm_in,4,4
	.sym	_pcm_in,_pcm_in, 4, 2, 32
	.global	_dma_index
	.bss	_dma_index,4,4
	.sym	_dma_index,_dma_index, 4, 2, 32
	.global	_src
	.bss	_src,4,4
	.sym	_src,_src, 4, 2, 32
	.global	_dst
	.bss	_dst,4,4
	.sym	_dst,_dst, 4, 2, 32
	.global	_in_ptr
	.bss	_in_ptr,4,4
	.sym	_in_ptr,_in_ptr, 19, 2, 32
	.global	_out_ptr
	.bss	_out_ptr,4,4
	.sym	_out_ptr,_out_ptr, 19, 2, 32
	.global	_in
	.bss	_in,2880,4
	.sym	_in,_in, 51, 2, 23040,, 1440
	.global	_out
	.bss	_out,2880,4
	.sym	_out,_out, 51, 2, 23040,, 1440
;	C:\TI\C6000\CGTOOLS\BIN\opt6x.exe -s -O3 c:\windows\TEMP\TI461897_2 c:\windows\TEMP\TI461897_4 -w C:/ti/myprojects/scram8k_dma/ 
	.sect	".text"
	.global	_mcbsp0_init
	.sym	_mcbsp0_init,_mcbsp0_init, 32, 2, 0
	.func	74

;******************************************************************************
;* FUNCTION NAME: _mcbsp0_init                                                *
;*                                                                            *
;*   Regs Modified     : A0,A3,B4,B5                                          *
;*   Regs Used         : A0,A3,B3,B4,B5                                       *
;*   Local Frame Size  : 0 Args + 0 Auto + 0 Save = 0 byte                    *
;******************************************************************************
_mcbsp0_init:
;** --------------------------------------------------------------------------*
	.line	2
	.sym	C$1,20, 14, 4, 32
	.sym	C$2,0, 14, 4, 32
;** 77	-----------------------    *(volatile unsigned *)0x18c0008u = C$1 = 0u;
;** 78	-----------------------    *(volatile unsigned *)0x18c0024u = C$1;
;** 79	-----------------------    *(volatile unsigned *)0x18c000cu = C$2 = 65600u;
;** 80	-----------------------    *(volatile unsigned *)0x18c0010u = C$2;
;** 81	-----------------------    *(volatile unsigned *)0x18c0004u = C$1;
;** 82	-----------------------    *(volatile unsigned *)0x18c0008u = 73729u;
;** 82	-----------------------    return;
	.line	4
           MVKL    .S2     0x18c0008,B5      ; |77| 

           MVKH    .S2     0x18c0008,B5      ; |77| 
||         ZERO    .D2     B4                ; |77| 

           STW     .D2T2   B4,*B5            ; |77| 
	.line	5
           MVKL    .S1     0x18c0024,A0      ; |78| 
           MVKH    .S1     0x18c0024,A0      ; |78| 
           STW     .D1T2   B4,*A0            ; |78| 
	.line	6
           MVKL    .S1     0x18c000c,A3      ; |79| 
           MVKH    .S1     0x18c000c,A3      ; |79| 
           MVKL    .S1     0x10040,A0        ; |79| 
           MVKH    .S1     0x10040,A0        ; |79| 
           STW     .D1T1   A0,*A3            ; |79| 
	.line	7
           MVKL    .S2     0x18c0010,B5      ; |80| 
           MVKH    .S2     0x18c0010,B5      ; |80| 
           STW     .D2T1   A0,*B5            ; |80| 
	.line	8
           MVKL    .S2     0x18c0004,B5      ; |81| 
           MVKH    .S2     0x18c0004,B5      ; |81| 
           STW     .D2T2   B4,*B5            ; |81| 
	.line	9

           MVKL    .S2     0x12001,B4        ; |82| 
||         MVKL    .S1     0x18c0008,A0      ; |82| 

           MVKH    .S2     0x12001,B4        ; |82| 
||         MVKH    .S1     0x18c0008,A0      ; |82| 

           STW     .D1T2   B4,*A0            ; |82| 
	.line	10
           B       .S2     B3                ; |83| 
           NOP             5
           ; BRANCH OCCURS                   ; |83| 
	.endfunc	83,000000000h,0


	.sect	".text"
	.global	_edma_init
	.sym	_edma_init,_edma_init, 32, 2, 0
	.func	85

;******************************************************************************
;* FUNCTION NAME: _edma_init                                                  *
;*                                                                            *
;*   Regs Modified     : A0,A3,A4,A5,B0,B1,B2,B4,B5,B6,B7,B8,B9               *
;*   Regs Used         : A0,A3,A4,A5,B0,B1,B2,B3,B4,B5,B6,B7,B8,B9,DP         *
;*   Local Frame Size  : 0 Args + 0 Auto + 0 Save = 0 byte                    *
;******************************************************************************
_edma_init:
;** --------------------------------------------------------------------------*
	.line	2
	.sym	C$1,0, 14, 4, 32
	.sym	C$2,4, 14, 4, 32
	.sym	C$3,3, 4, 4, 32
	.sym	C$4,24, 14, 4, 32
	.sym	C$5,20, 14, 4, 32
	.sym	C$6,22, 14, 4, 32
	.sym	C$7,17, 14, 4, 32
	.sym	C$8,16, 14, 4, 32
	.sym	C$9,21, 4, 4, 32
	.sym	C$10,23, 14, 4, 32
	.sym	C$11,25, 4, 4, 32
	.sym	C$12,18, 4, 4, 32
	.sym	C$13,20, 211, 4, 32,, 1440
	.sym	C$14,0, 211, 4, 32,, 1440
	.sym	C$15,21, 14, 4, 32
;** 87	-----------------------    *(volatile unsigned *)0x1a0fff8u = C$15 = 65535u;
;** 88	-----------------------    *(volatile unsigned *)0x1a0fff4u = 12288u;
;** 90	-----------------------    *(volatile unsigned *)0x1a0ffe4u = C$15;
;** 91	-----------------------    *(volatile unsigned *)0x1a0ffe8u = 256u;
;** 92	-----------------------    C$14 = &in;
;** 92	-----------------------    pcm_in = C$11 = (int)C$14;
;** 93	-----------------------    C$13 = &out;
;** 93	-----------------------    pcm_out = C$12 = (int)C$13;
;** 94	-----------------------    in_ptr = (short *)(int)C$14;
;** 95	-----------------------    out_ptr = (short *)((int)C$13+800);
;** 97	-----------------------    dma_index = 1;
;** 99	-----------------------    *(volatile unsigned *)0x1a00120u = C$10 = 0x49000002u;
;** 100	-----------------------    *(volatile unsigned *)0x1a00124u = C$12;
;** 101	-----------------------    *(volatile unsigned *)0x1a00128u = C$4 = 80u;
;** 102	-----------------------    *(volatile unsigned *)0x1a0012cu = C$8 = 0x18c0004u;
;** 103	-----------------------    *(volatile unsigned *)0x1a00130u = C$2 = 0u;
;** 104	-----------------------    *(volatile unsigned *)0x1a00134u = C$7 = 0x1a00180u;
;** 107	-----------------------    *(volatile unsigned *)0x1a00138u = C$6 = 0x48380002u;
;** 108	-----------------------    *(volatile unsigned *)0x1a0013cu = C$5 = 0x18c0000u;
;** 109	-----------------------    *(volatile unsigned *)0x1a00140u = C$4;
;** 110	-----------------------    *(volatile unsigned *)0x1a00144u = C$11;
;** 111	-----------------------    *(volatile unsigned *)0x1a00148u = C$2;
;** 112	-----------------------    *(volatile unsigned *)0x1a0014cu = C$1 = 0x1a00198u;
;** 114	-----------------------    src = C$9 = C$12+160;
;** 115	-----------------------    dst = C$3 = C$11+160;
;** 118	-----------------------    *(volatile unsigned *)0x1a00180u = C$10;
;** 119	-----------------------    *(volatile unsigned *)0x1a00184u = C$9;
;** 120	-----------------------    *(volatile unsigned *)0x1a00188u = C$4;
;** 121	-----------------------    *(volatile unsigned *)0x1a0018cu = C$8;
;** 122	-----------------------    *(volatile unsigned *)0x1a00190u = C$2;
;** 123	-----------------------    *(volatile unsigned *)0x1a00194u = C$7;
;** 126	-----------------------    *(volatile unsigned *)0x1a00198u = C$6;
;** 127	-----------------------    *(volatile unsigned *)0x1a0019cu = C$5;
;** 128	-----------------------    *(volatile unsigned *)0x1a001a0u = C$4;
;** 129	-----------------------    *(volatile unsigned *)0x1a001a4u = C$3;
;** 130	-----------------------    *(volatile unsigned *)0x1a001a8u = C$2;
;** 131	-----------------------    *(volatile unsigned *)0x1a001acu = C$1;
;** 131	-----------------------    return;
	.line	3
           MVKL    .S2     0x1a0fff8,B4      ; |87| 

           MVKH    .S2     0x1a0fff8,B4      ; |87| 
||         ZERO    .D2     B5                ; |87| 

           SET     .S2     B5,0x0,0xf,B5     ; |87| 
           STW     .D2T2   B5,*B4            ; |87| 
	.line	4
           MVKL    .S1     0x1a0fff4,A0      ; |88| 

           MVK     .S2     12288,B4          ; |88| 
||         MVKH    .S1     0x1a0fff4,A0      ; |88| 

           STW     .D1T2   B4,*A0            ; |88| 
	.line	6
           MVKL    .S1     0x1a0ffe4,A0      ; |90| 
           MVKH    .S1     0x1a0ffe4,A0      ; |90| 
           STW     .D1T2   B5,*A0            ; |90| 
	.line	7
           MVKL    .S1     0x1a0ffe8,A0      ; |91| 

           MVKH    .S1     0x1a0ffe8,A0      ; |91| 
||         MVK     .S2     256,B4            ; |91| 

           STW     .D1T2   B4,*A0            ; |91| 
	.line	8
           MVK     .S2     (_in-$bss),B4     ; |92| 

           ADD     .D2     DP,B4,B4
||         ADD     .S2     DP,B4,B9

           STW     .D2T2   B9,*+DP(_pcm_in)  ; |92| 
||         MV      .S1X    B4,A0

	.line	9
           MVK     .S2     (_out-$bss),B4    ; |93| 
           ADD     .D2     DP,B4,B2

           STW     .D2T2   B2,*+DP(_pcm_out) ; |93| 
||         ADD     .S2     DP,B4,B4

	.line	10
           STW     .D2T1   A0,*+DP(_in_ptr)  ; |94| 
	.line	11
           MVK     .S2     800,B5            ; |95| 
           ADD     .D2     B5,B4,B4          ; |95| 
           STW     .D2T2   B4,*+DP(_out_ptr) ; |95| 
	.line	13
           MVK     .S2     1,B4              ; |97| 
           STW     .D2T2   B4,*+DP(_dma_index) ; |97| 
	.line	15

           MVKL    .S1     0x1a00120,A0      ; |99| 
||         MVKL    .S2     0x49000002,B7     ; |99| 

           MVKH    .S1     0x1a00120,A0      ; |99| 
||         MVKH    .S2     0x49000002,B7     ; |99| 

           STW     .D1T2   B7,*A0            ; |99| 
	.line	16
           MVKL    .S2     0x1a00124,B4      ; |100| 
           MVKH    .S2     0x1a00124,B4      ; |100| 
           STW     .D2T2   B2,*B4            ; |100| 
	.line	17
           MVK     .S2     0x50,B8           ; |101| 
           MVKL    .S2     0x1a00128,B4      ; |101| 
           MVKH    .S2     0x1a00128,B4      ; |101| 
           STW     .D2T2   B8,*B4            ; |101| 
	.line	18
           MVKL    .S2     0x18c0004,B0      ; |102| 
           MVKH    .S2     0x18c0004,B0      ; |102| 
           MVKL    .S2     0x1a0012c,B4      ; |102| 
           MVKH    .S2     0x1a0012c,B4      ; |102| 
           STW     .D2T2   B0,*B4            ; |102| 
	.line	19
           MVKL    .S1     0x1a00130,A0      ; |103| 

           MVKH    .S1     0x1a00130,A0      ; |103| 
||         ZERO    .D1     A4                ; |103| 

           STW     .D1T1   A4,*A0            ; |103| 
	.line	20
           MVKL    .S2     0x1a00180,B1      ; |104| 
           MVKH    .S2     0x1a00180,B1      ; |104| 
           MVKL    .S2     0x1a00134,B4      ; |104| 
           MVKH    .S2     0x1a00134,B4      ; |104| 
           STW     .D2T2   B1,*B4            ; |104| 
	.line	23
           MVKL    .S2     0x48380002,B6     ; |107| 
           MVKH    .S2     0x48380002,B6     ; |107| 
           MVKL    .S2     0x1a00138,B4      ; |107| 
           MVKH    .S2     0x1a00138,B4      ; |107| 
           STW     .D2T2   B6,*B4            ; |107| 
	.line	24
           MVKL    .S2     0x1a0013c,B5      ; |108| 

           ZERO    .D2     B4                ; |108| 
||         MVKH    .S2     0x1a0013c,B5      ; |108| 

           MVKH    .S2     0x18c0000,B4      ; |108| 
           STW     .D2T2   B4,*B5            ; |108| 
	.line	25
           MVKL    .S2     0x1a00140,B5      ; |109| 
           MVKH    .S2     0x1a00140,B5      ; |109| 
           STW     .D2T2   B8,*B5            ; |109| 
	.line	26
           MVKL    .S1     0x1a00144,A0      ; |110| 
           MVKH    .S1     0x1a00144,A0      ; |110| 
           STW     .D1T2   B9,*A0            ; |110| 
	.line	27
           MVKL    .S1     0x1a00148,A0      ; |111| 
           MVKH    .S1     0x1a00148,A0      ; |111| 
           STW     .D1T1   A4,*A0            ; |111| 
	.line	28

           MVKL    .S2     0x1a0014c,B5      ; |112| 
||         MVKL    .S1     0x1a00198,A0      ; |112| 

           MVKH    .S2     0x1a0014c,B5      ; |112| 
||         MVKH    .S1     0x1a00198,A0      ; |112| 

           STW     .D2T1   A0,*B5            ; |112| 
	.line	30
           MVK     .S2     160,B5            ; |114| 
           ADD     .D2     B5,B2,B5          ; |114| 
           STW     .D2T2   B5,*+DP(_src)     ; |114| 
	.line	31
           MVK     .S1     160,A3            ; |115| 
           ADD     .S1X    A3,B9,A3          ; |115| 
           STW     .D2T1   A3,*+DP(_dst)     ; |115| 
	.line	34
           MVKL    .S1     0x1a00180,A5      ; |118| 
           MVKH    .S1     0x1a00180,A5      ; |118| 
           STW     .D1T2   B7,*A5            ; |118| 
	.line	35
           MVKL    .S1     0x1a00184,A5      ; |119| 
           MVKH    .S1     0x1a00184,A5      ; |119| 
           STW     .D1T2   B5,*A5            ; |119| 
	.line	36
           MVKL    .S2     0x1a00188,B5      ; |120| 
           MVKH    .S2     0x1a00188,B5      ; |120| 
           STW     .D2T2   B8,*B5            ; |120| 
	.line	37
           MVKL    .S1     0x1a0018c,A5      ; |121| 
           MVKH    .S1     0x1a0018c,A5      ; |121| 
           STW     .D1T2   B0,*A5            ; |121| 
	.line	38
           MVKL    .S1     0x1a00190,A5      ; |122| 
           MVKH    .S1     0x1a00190,A5      ; |122| 
           STW     .D1T1   A4,*A5            ; |122| 
	.line	39
           MVKL    .S1     0x1a00194,A5      ; |123| 
           MVKH    .S1     0x1a00194,A5      ; |123| 
           STW     .D1T2   B1,*A5            ; |123| 
	.line	42
           MVKL    .S1     0x1a00198,A5      ; |126| 
           MVKH    .S1     0x1a00198,A5      ; |126| 
           STW     .D1T2   B6,*A5            ; |126| 
	.line	43
           MVKL    .S1     0x1a0019c,A5      ; |127| 
           MVKH    .S1     0x1a0019c,A5      ; |127| 
           STW     .D1T2   B4,*A5            ; |127| 
	.line	44
           MVKL    .S1     0x1a001a0,A5      ; |128| 
           MVKH    .S1     0x1a001a0,A5      ; |128| 
           STW     .D1T2   B8,*A5            ; |128| 
	.line	45
           MVKL    .S1     0x1a001a4,A5      ; |129| 
           MVKH    .S1     0x1a001a4,A5      ; |129| 
           STW     .D1T1   A3,*A5            ; |129| 
	.line	46
           MVKL    .S1     0x1a001a8,A3      ; |130| 
           MVKH    .S1     0x1a001a8,A3      ; |130| 
           STW     .D1T1   A4,*A3            ; |130| 
	.line	47
           MVKL    .S1     0x1a001ac,A3      ; |131| 
           MVKH    .S1     0x1a001ac,A3      ; |131| 
           STW     .D1T1   A0,*A3            ; |131| 
	.line	49
           B       .S2     B3                ; |133| 
           NOP             5
           ; BRANCH OCCURS                   ; |133| 
	.endfunc	133,000000000h,0


	.sect	".text"
	.global	_main
	.sym	_main,_main, 36, 2, 0
	.func	18

;******************************************************************************
;* FUNCTION NAME: _main                                                       *
;*                                                                            *
;*   Regs Modified     : A0,A3,A4,A5,B0,B1,B2,B3,B4,B5,B6,B7,B8,B9,SP         *
;*   Regs Used         : A0,A3,A4,A5,B0,B1,B2,B3,B4,B5,B6,B7,B8,B9,DP,SP      *
;*   Local Frame Size  : 0 Args + 0 Auto + 4 Save = 4 byte                    *
;******************************************************************************
_main:
;** --------------------------------------------------------------------------*
	.line	2
	.sym	C$1,20, 14, 4, 32
	.sym	C$2,0, 14, 4, 32
	.sym	C$3,20, 20, 4, 32
	.sym	L$1,20, 4, 4, 32
	.sym	K$0,0, 14, 4, 32
	.sym	U$38,0, 20, 4, 32
	.sym	U$34,4, 20, 4, 32
	.sym	K$29,3, 4, 4, 32
;** 23	-----------------------    CSR = 256u;
;** 24	-----------------------    IER = 1u;
;** 25	-----------------------    ICR = 65535u;
;** 26	-----------------------    *(volatile unsigned *)0x1800000u = 13056u;
;** 27	-----------------------    *(volatile unsigned *)0x1800008u = 48u;
;** 28	-----------------------    *(volatile unsigned *)0x1800004u = 0xffffff03u;
;** 29	-----------------------    *(volatile unsigned *)0x1800018u = 0x7117000u;
;** 30	-----------------------    *(volatile unsigned *)0x180001cu = 1562u;
;** 31	-----------------------    *(volatile unsigned *)0x1800020u = 345369u;
;** 33	-----------------------    // LOOP BELOW UNROLLED BY FACTOR(2)
;**  	-----------------------    K$29 = 0;
;** 33	-----------------------    L$1 = 720;
;**  	-----------------------    U$38 = &((int *)out)[0];
;**  	-----------------------    U$34 = &((int *)in)[0];
;**  	-----------------------    #pragma MUST_ITERATE(720, 720, 720)
;**	-----------------------g2:
;** 35	-----------------------    *U$34++ = K$29;
;** 36	-----------------------    *U$38++ = K$29;
;** 37	-----------------------    if ( --L$1 ) goto g2;
           STW     .D2T2   B3,*SP--(8)       ; |19| 
	.line	6
           MVK     .S1     256,A0            ; |23| 
           MVC     .S2X    A0,CSR            ; |23| 
	.line	7
           MVK     .S1     1,A0              ; |24| 
           MVC     .S2X    A0,IER            ; |24| 
	.line	8
           ZERO    .D1     A0                ; |25| 
           SET     .S1     A0,0x0,0xf,A0     ; |25| 
           MVC     .S2X    A0,ICR            ; |25| 
	.line	9
           ZERO    .D2     B4                ; |26| 

           MVK     .S1     13056,A0          ; |26| 
||         MVKH    .S2     0x1800000,B4      ; |26| 

           STW     .D2T1   A0,*B4            ; |26| 
	.line	10
           MVKL    .S2     0x1800008,B4      ; |27| 

           MVK     .S1     48,A0             ; |27| 
||         MVKH    .S2     0x1800008,B4      ; |27| 

           STW     .D2T1   A0,*B4            ; |27| 
	.line	11
           MVKL    .S2     0x1800004,B4      ; |28| 

           MVK     .S1     -253,A0           ; |28| 
||         MVKH    .S2     0x1800004,B4      ; |28| 

           STW     .D2T1   A0,*B4            ; |28| 
	.line	12

           MVKL    .S2     0x1800018,B4      ; |29| 
||         MVKL    .S1     0x7117000,A0      ; |29| 

           MVKH    .S1     0x7117000,A0      ; |29| 
||         MVKH    .S2     0x1800018,B4      ; |29| 

           STW     .D2T1   A0,*B4            ; |29| 
	.line	13
           MVKL    .S2     0x180001c,B4      ; |30| 

           MVK     .S1     1562,A0           ; |30| 
||         MVKH    .S2     0x180001c,B4      ; |30| 

           STW     .D2T1   A0,*B4            ; |30| 
	.line	14

           MVKL    .S2     0x54519,B4        ; |31| 
||         MVKL    .S1     0x1800020,A0      ; |31| 

           MVKH    .S1     0x1800020,A0      ; |31| 
||         MVKH    .S2     0x54519,B4        ; |31| 

           ZERO    .S1     A3
||         STW     .D1T2   B4,*A0            ; |31| 

	.line	16
           MVK     .S1     (_out-$bss),A0
           MVK     .S1     (_in-$bss),A4
;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*
;*      Loop source line               : 33
;*      Loop opening brace source line : 34
;*      Loop closing brace source line : 37
;*      Loop Unroll Multiple             : 2x
;*      Known Minimum Trip Count         : 720
;*      Known Maximum Trip Count         : 720
;*      Known Max Trip Count Factor      : 720
;*      Loop Carried Dependency Bound(^) : 0
;*      Unpartitioned Resource Bound     : 1
;*      Partitioned Resource Bound(*)    : 1
;*      Resource Partition:
;*                                A-side   B-side
;*      .L units                     0        0     
;*      .S units                     1*       0     
;*      .D units                     1*       1*    
;*      .M units                     0        0     
;*      .X cross paths               0        0     
;*      .T address paths             1*       1*    
;*      Long read paths              1*       1*    
;*      Long write paths             0        0     
;*      Logical  ops (.LS)           0        0     (.L or .S unit)
;*      Addition ops (.LSD)          0        1     (.L or .S or .D unit)
;*      Bound(.L .S .LS)             1*       0     
;*      Bound(.L .S .D .LS .LSD)     1*       1*    
;*
;*      Searching for software pipeline schedule at ...
;*         ii = 1  Schedule found with 7 iterations in parallel
;*      done
;*
;*      Epilog not entirely removed
;*      Collapsed epilog stages     : 1
;*      Collapsed prolog stages     : 0
;*      Minimum required memory pad : 0 bytes
;*
;*      Minimum safe trip count     : 6 (after unrolling)
;*----------------------------------------------------------------------------*
L1:    ; PIPED LOOP PROLOG
           B       .S1     L2                ; (P) |37| 
           B       .S1     L2                ; (P) @|37| 
           B       .S1     L2                ; (P) @@|37| 

           MV      .L2X    A3,B5
||         MVK     .S2     0x2d0,B4          ; |33| 
||         B       .S1     L2                ; (P) @@@|37| 

           SUB     .D2     B4,11,B0
||         ADD     .L1X    DP,A0,A0
||         ADD     .S2X    DP,A4,B4
||         B       .S1     L2                ; (P) @@@@|37| 

;** --------------------------------------------------------------------------*
L2:    ; PIPED LOOP KERNEL

           STW     .D2T2   B5,*B4++          ; |35| 
||         STW     .D1T1   A3,*A0++          ; |36| 
|| [ B0]   B       .S1     L2                ; @@@@@|37| 
|| [ B0]   SUB     .S2     B0,1,B0           ; @@@@@@|37| 

;** --------------------------------------------------------------------------*
L3:    ; PIPED LOOP EPILOG
;** --------------------------------------------------------------------------*
;** 39	-----------------------    C$3 = &((int *)out)[0];
;** 39	-----------------------    *C$3 = 170;
;** 41	-----------------------    C$3[1] = 59113473;
;** 43	-----------------------    C$3[2] = K$29;
;** 47	-----------------------    C$3[3] = 50724865;
;** 49	-----------------------    C$3[4] = K$29;
;** 52	-----------------------    C$3[5] = 67108865;
;** 54	-----------------------    C$3[6] = K$29;
;** 57	-----------------------    C$3[7] = 84017153;
;** 59	-----------------------    *((short *)C$3+32) = K$29;
;** 61	-----------------------    ICR = K$0 = 256u;
;** 62	-----------------------    IER = IER|0x102u;
;** 63	-----------------------    CSR = CSR|1u;
;** 66	-----------------------    edma_init();
;** 77	-----------------------    *(volatile unsigned *)0x18c0008u = C$1 = 0u;  // [2]
;** 78	-----------------------    *(volatile unsigned *)0x18c0024u = C$1;  // [2]
;** 79	-----------------------    *(volatile unsigned *)0x18c000cu = C$2 = 65600u;  // [2]
;** 80	-----------------------    *(volatile unsigned *)0x18c0010u = C$2;  // [2]
;** 81	-----------------------    *(volatile unsigned *)0x18c0004u = C$1;  // [2]
;** 82	-----------------------    *(volatile unsigned *)0x18c0008u = 73729u;  // [2]

           STW     .D1T1   A3,*A0++          ; (E) @@|36| 
||         STW     .D2T2   B5,*B4++          ; (E) @@|35| 

           STW     .D1T1   A3,*A0++          ; (E) @@@|36| 
||         STW     .D2T2   B5,*B4++          ; (E) @@@|35| 

           STW     .D2T2   B5,*B4++          ; (E) @@@@|35| 
||         STW     .D1T1   A3,*A0++          ; (E) @@@@|36| 

           STW     .D2T2   B5,*B4++          ; (E) @@@@@|35| 
||         STW     .D1T1   A3,*A0++          ; (E) @@@@@|36| 

           STW     .D1T1   A3,*A0++          ; (E) @@@@@@|36| 
||         STW     .D2T2   B5,*B4++          ; (E) @@@@@@|35| 

	.line	22

           MVK     .S2     170,B5            ; |39| 
||         MVK     .S1     (_out-$bss),A0    ; |39| 

           ADD     .S2X    DP,A0,B4
           STW     .D2T2   B5,*B4            ; |39| 
	.line	24
           MVKL    .S1     0x3860001,A0      ; |41| 
           MVKH    .S1     0x3860001,A0      ; |41| 
           STW     .D2T1   A0,*+B4(4)        ; |41| 
	.line	26
           STW     .D2T1   A3,*+B4(8)        ; |43| 
	.line	30
           MVKL    .S2     0x3060001,B5      ; |47| 
           MVKH    .S2     0x3060001,B5      ; |47| 
           STW     .D2T2   B5,*+B4(12)       ; |47| 
	.line	32
           STW     .D2T1   A3,*+B4(16)       ; |49| 
	.line	35
           MVKL    .S1     0x4000001,A0      ; |52| 
           MVKH    .S1     0x4000001,A0      ; |52| 
           STW     .D2T1   A0,*+B4(20)       ; |52| 
	.line	37
           STW     .D2T1   A3,*+B4(24)       ; |54| 
	.line	40
           MVKL    .S1     0x5020001,A0      ; |57| 
           MVKH    .S1     0x5020001,A0      ; |57| 
           STW     .D2T1   A0,*+B4(28)       ; |57| 
	.line	42
           STH     .D2T1   A3,*+B4(32)       ; |59| 
	.line	44
           MVK     .S1     0x100,A0          ; |61| 
           MVC     .S2X    A0,ICR            ; |61| 
	.line	45
           MVC     .S2     IER,B4            ; |62| 
           MVK     .S1     258,A0            ; |62| 
           OR      .S2X    A0,B4,B4          ; |62| 
           MVC     .S2     B4,IER            ; |62| 
	.line	46
           MVC     .S2     CSR,B4            ; |63| 
           OR      .S2     1,B4,B4           ; |63| 
           MVC     .S2     B4,CSR            ; |63| 
	.line	49
           B       .S2     _edma_init        ; |66| 
           MVKL    .S2     RL0,B3            ; |66| 
           MVKH    .S2     RL0,B3            ; |66| 
           NOP             3
RL0:       ; CALL OCCURS                     ; |66| 

           ZERO    .D2     B4                ; |77| 
||         MVKL    .S2     0x18c0008,B5      ; |77| 

           MVKH    .S2     0x18c0008,B5      ; |77| 
           STW     .D2T2   B4,*B5            ; |77| 
           MVKL    .S1     0x18c0024,A0      ; |78| 
           MVKH    .S1     0x18c0024,A0      ; |78| 
           STW     .D1T2   B4,*A0            ; |78| 
           MVKL    .S1     0x18c000c,A3      ; |79| 
           MVKL    .S1     0x10040,A0        ; |79| 
           MVKH    .S1     0x10040,A0        ; |79| 
           MVKH    .S1     0x18c000c,A3      ; |79| 
           STW     .D1T1   A0,*A3            ; |79| 
           MVKL    .S2     0x18c0010,B5      ; |80| 
           MVKH    .S2     0x18c0010,B5      ; |80| 
           STW     .D2T1   A0,*B5            ; |80| 
           MVKL    .S2     0x18c0004,B5      ; |81| 
           MVKH    .S2     0x18c0004,B5      ; |81| 
           STW     .D2T2   B4,*B5            ; |81| 

           MVKL    .S2     0x12001,B4        ; |82| 
||         MVKL    .S1     0x18c0008,A0      ; |82| 

           MVKH    .S2     0x12001,B4        ; |82| 
||         MVKH    .S1     0x18c0008,A0      ; |82| 

           STW     .D1T2   B4,*A0            ; |82| 
;*----------------------------------------------------------------------------*
;*   SOFTWARE PIPELINE INFORMATION
;*      Disqualified loop: bad loop structure
;*----------------------------------------------------------------------------*
L4:    
;**	-----------------------g5:
;** 69	-----------------------    goto g5;
	.line	52
           B       .S1     L4                ; |69| 
           NOP             5
           ; BRANCH OCCURS                   ; |69| 
;** --------------------------------------------------------------------------*
	.endfunc	72,000080000h,8


	.sect	".text"
	.global	_edma_isr
	.sym	_edma_isr,_edma_isr, 32, 2, 0
	.func	135

;******************************************************************************
;* FUNCTION NAME: _edma_isr                                                   *
;*                                                                            *
;*   Regs Modified     : A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,B0,B1,B2,B3,B4,B5,B6,  *
;*                           B7,B8,B9,SP                                      *
;*   Regs Used         : A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,B0,B1,B2,B3,B4,B5,B6,  *
;*                           B7,B8,B9,DP,SP                                   *
;*   Local Frame Size  : 0 Args + 0 Auto + 84 Save = 84 byte                  *
;******************************************************************************
_edma_isr:
;** --------------------------------------------------------------------------*
	.line	2
	.sym	C$1,20, 4, 4, 32
	.sym	U$21,21, 19, 4, 32
	.sym	U$29,20, 19, 4, 32
	.sym	U$13,22, 4, 4, 32
	.sym	K$24,23, 4, 4, 32
	.sym	U$12,20, 4, 4, 32
	.sym	U$9,21, 4, 4, 32
	.sym	U$6,0, 4, 4, 32
;** 138	-----------------------    if ( (C$1 = dma_index) == 17 ) goto g2;
           STW     .D2T2   B9,*SP--(88)      ; |136| 
           STW     .D2T1   A0,*+SP(12)       ; |136| 
           STW     .D2T1   A1,*+SP(16)       ; |136| 
           STW     .D2T1   A2,*+SP(20)       ; |136| 
           STW     .D2T1   A3,*+SP(24)       ; |136| 
           STW     .D2T1   A4,*+SP(28)       ; |136| 
           STW     .D2T1   A5,*+SP(32)       ; |136| 
           STW     .D2T1   A6,*+SP(36)       ; |136| 
           STW     .D2T1   A7,*+SP(40)       ; |136| 
           STW     .D2T1   A8,*+SP(44)       ; |136| 
           STW     .D2T1   A9,*+SP(48)       ; |136| 
           STW     .D2T2   B0,*+SP(52)       ; |136| 
           STW     .D2T2   B1,*+SP(56)       ; |136| 
           STW     .D2T2   B2,*+SP(60)       ; |136| 
           STW     .D2T2   B3,*+SP(64)       ; |136| 
           STW     .D2T2   B4,*+SP(68)       ; |136| 
           STW     .D2T2   B5,*+SP(72)       ; |136| 
           STW     .D2T2   B6,*+SP(76)       ; |136| 
           STW     .D2T2   B7,*+SP(80)       ; |136| 
           STW     .D2T2   B8,*+SP(84)       ; |136| 
	.line	4
           LDW     .D2T2   *+DP(_dma_index),B4 ; |138| 
           MVK     .S2     17,B5             ; |138| 
           NOP             3
           CMPEQ   .L2     B4,B5,B0          ; |138| 
   [ B0]   B       .S1     L5                ; |138| 
           NOP             5
           ; BRANCH OCCURS                   ; |138| 
;** --------------------------------------------------------------------------*
;** 146	-----------------------    dma_index = C$1+1;
;** 147	-----------------------    src = U$6 = src+160;
;** 148	-----------------------    dst = U$9 = dst+160;
;**  	-----------------------    U$12 = pcm_in;
;**  	-----------------------    U$13 = pcm_out;
;** 148	-----------------------    goto g3;
	.line	12
           ADD     .D2     1,B4,B4           ; |146| 
           STW     .D2T2   B4,*+DP(_dma_index) ; |146| 
	.line	13
           LDW     .D2T2   *+DP(_src),B4     ; |147| 
           MVK     .S1     160,A0            ; |147| 
           NOP             3
           ADD     .S1X    A0,B4,A0          ; |147| 
           STW     .D2T1   A0,*+DP(_src)     ; |147| 
	.line	14
           LDW     .D2T2   *+DP(_dst),B5     ; |148| 
           MVK     .S2     160,B6            ; |148| 
           B       .S1     L6                ; |148| 
           LDW     .D2T2   *+DP(_pcm_in),B4
           NOP             2

           LDW     .D2T2   *+DP(_pcm_out),B6
||         ADD     .S2     B6,B5,B5          ; |148| 

           STW     .D2T2   B5,*+DP(_dst)     ; |148| 
           ; BRANCH OCCURS                   ; |148| 
;** --------------------------------------------------------------------------*
L5:    
;**	-----------------------g2:
;** 140	-----------------------    dma_index = 0;
;** 141	-----------------------    dst = U$9 = U$12 = pcm_in;
;** 142	-----------------------    src = U$6 = U$13 = pcm_out;
	.line	6
           ZERO    .D2     B4                ; |140| 
           STW     .D2T2   B4,*+DP(_dma_index) ; |140| 
	.line	7
           LDW     .D2T2   *+DP(_pcm_in),B4  ; |141| 
           NOP             4
           MV      .D2     B4,B5             ; |141| 
           STW     .D2T2   B5,*+DP(_dst)     ; |141| 
	.line	8
           LDW     .D2T2   *+DP(_pcm_out),B6 ; |142| 
           NOP             4
           MV      .S1X    B6,A0             ; |142| 
           STW     .D2T1   A0,*+DP(_src)     ; |142| 
;** --------------------------------------------------------------------------*
L6:    
;**	-----------------------g3:
;** 151	-----------------------    *(volatile unsigned *)0x1a0ffe4u |= 0x100u;
;** 152	-----------------------    *(volatile unsigned *)0x1a00184u = U$6;
;** 153	-----------------------    *(volatile unsigned *)0x1a001a4u = U$9;
;** 156	-----------------------    U$21 = in_ptr;
;** 156	-----------------------    if ( (int)U$21-U$12 < 2880 ) goto g5;
;** 158	-----------------------    in_ptr = U$21 = (short *)U$12;
;**	-----------------------g5:
;** 162	-----------------------    U$29 = out_ptr;
;** 162	-----------------------    if ( (int)U$29-U$13+(K$24 = (-2880)) < 0 ) goto g7;
;** 164	-----------------------    out_ptr = U$29 = (short *)U$13;
;**	-----------------------g7:
;** 172	-----------------------    ProcBlock(U$21, U$29, 80);
;** 173	-----------------------    in_ptr = in_ptr+160;
;** 174	-----------------------    out_ptr = out_ptr+160;
;** 174	-----------------------    return;
	.line	17
           MVKL    .S1     0x1a0ffe4,A3      ; |151| 
           MVKH    .S1     0x1a0ffe4,A3      ; |151| 
           LDW     .D1T1   *A3,A4            ; |151| 
           NOP             4
           SET     .S1     A4,8,8,A4         ; |151| 
           STW     .D1T1   A4,*A3            ; |151| 
	.line	18
           MVKL    .S1     0x1a00184,A3      ; |152| 
           MVKH    .S1     0x1a00184,A3      ; |152| 
           STW     .D1T1   A0,*A3            ; |152| 
	.line	19
           MVKL    .S2     0x1a001a4,B7      ; |153| 
           MVKH    .S2     0x1a001a4,B7      ; |153| 
           STW     .D2T2   B5,*B7            ; |153| 
	.line	22
           LDW     .D2T2   *+DP(_in_ptr),B5  ; |156| 
           MVK     .S1     2880,A0           ; |156| 
           NOP             3
           SUB     .D2     B5,B4,B7          ; |156| 
           CMPLT   .L1X    B7,A0,A1          ; |156| 
	.line	24
   [!A1]   MV      .D2     B4,B5             ; |158| 
   [!A1]   STW     .D2T2   B5,*+DP(_in_ptr)  ; |158| 
	.line	28
           LDW     .D2T2   *+DP(_out_ptr),B4 ; |162| 
           MVK     .S2     0xfffff4c0,B7     ; |162| 
           NOP             3
           SUB     .D2     B4,B6,B8          ; |162| 
           ADD     .D2     B7,B8,B7          ; |162| 
           CMPLT   .L2     B7,0,B0           ; |162| 
	.line	30
   [!B0]   MV      .D2     B6,B4             ; |164| 
   [!B0]   STW     .D2T2   B4,*+DP(_out_ptr) ; |164| 
	.line	38
           B       .S1     _ProcBlock        ; |172| 
           MVKL    .S2     RL2,B3            ; |172| 
           MV      .L1X    B5,A4             ; |172| 
           MVKH    .S2     RL2,B3            ; |172| 
           MVK     .S1     0x50,A6           ; |172| 
           NOP             1
RL2:       ; CALL OCCURS                     ; |172| 
	.line	39
           LDW     .D2T2   *+DP(_in_ptr),B4  ; |173| 
           NOP             4
           ADDK    .S2     160,B4            ; |173| 
           STW     .D2T2   B4,*+DP(_in_ptr)  ; |173| 
	.line	40
           LDW     .D2T2   *+DP(_out_ptr),B4 ; |174| 
           NOP             4
           ADDK    .S2     160,B4            ; |174| 
           STW     .D2T2   B4,*+DP(_out_ptr) ; |174| 
	.line	42
           LDW     .D2T2   *+SP(64),B3       ; |176| 
           LDW     .D2T1   *+SP(36),A6       ; |176| 
           LDW     .D2T1   *+SP(28),A4       ; |176| 
           LDW     .D2T2   *+SP(60),B2       ; |176| 
           LDW     .D2T2   *+SP(56),B1       ; |176| 
           LDW     .D2T1   *+SP(48),A9       ; |176| 
           LDW     .D2T1   *+SP(44),A8       ; |176| 
           LDW     .D2T1   *+SP(40),A7       ; |176| 
           LDW     .D2T1   *+SP(32),A5       ; |176| 
           LDW     .D2T1   *+SP(20),A2       ; |176| 
           LDW     .D2T1   *+SP(24),A3       ; |176| 
           LDW     .D2T1   *+SP(12),A0       ; |176| 
           LDW     .D2T2   *+SP(84),B8       ; |176| 
           LDW     .D2T2   *+SP(80),B7       ; |176| 
           LDW     .D2T1   *+SP(16),A1       ; |176| 
           LDW     .D2T2   *+SP(72),B5       ; |176| 
           LDW     .D2T2   *+SP(76),B6       ; |176| 
           LDW     .D2T2   *+SP(52),B0       ; |176| 

           B       .S2     IRP               ; |176| 
||         LDW     .D2T2   *+SP(68),B4       ; |176| 

           LDW     .D2T2   *++SP(88),B9      ; |176| 
           NOP             4
           ; BRANCH OCCURS                   ; |176| 
	.endfunc	176,003ff03ffh,88


;; Inlined function references:
;; [  2] mcbsp0_init
;******************************************************************************
;* UNDEFINED EXTERNAL REFERENCES                                              *
;******************************************************************************
	.global	_ProcBlock

;******************************************************************************
;* TYPE INFORMATION                                                           *
;******************************************************************************
