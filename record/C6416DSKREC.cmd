/*C6416dsk.cmd Linker command file*/

MEMORY
{
  IVECS:     org =          0h,  len =      0x220
  IRAM:      org =  0x00000220,  len = 0x0002FDE0
  SRAM_EXT:  org =  0x80000000,  len = 0x01000000
  FLASH:     org =  0x90000000,  len = 0x00020000                     
}

SECTIONS
{
  .EXT_RAM  :> SRAM_EXT
  .vectors :> IVECS		/*Created in vectors file*/
  .text    :> IRAM		/*Created by C Compiler*/
  .bss     :> IRAM
  .cinit   :> IRAM
  .stack   :> IRAM
  .sysmem  :> IRAM
  .const   :> IRAM
  .switch  :> IRAM
  .far     :> IRAM
  .cio     :> IRAM
  .csldata :> IRAM
}