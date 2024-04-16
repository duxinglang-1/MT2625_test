/********************************************************  
memory map 
  Start Address	Size	CM4 view Devices
    0x0000_0000	64MB	EMI	
    0x0400_0000	64KB	CM4 TCM	
	0x0401_0000	32KB	CM4 TCM / cache	
	0x0401_8000	32KB	Reserved	
	0x0402_0000	896KB	Reserved	
	0x0410_0000	1MB   	Boot ROM CM4
	0x0420_0000	32KB	SYSRAM
	0x0430_0000	8KB	    RTC Retention SRAM
	0x0440_0000	1MB	    Reserved
	0x0800_0000	128MB	SFC0
 	0x1000_0000	128MB	SW virtual memory management
	0x1800_0000	128MB	SW virtual memory management
*********************************************************/
 
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
 
/* Memory Spaces Definitions:4M FLASH,4M PSRAM,32K SYSRAM,64K TCM, 8K retention SRAM */
MEMORY
{
    ROM_HEAD_1(rx)        : ORIGIN = 0x08000000, LENGTH = 4K
    ROM_HEAD_2(rx)        : ORIGIN = 0x08001000, LENGTH = 4K
    ROM_BL(rx)            : ORIGIN = 0x08002000, LENGTH = 64K
    ROM_RTOS(rx)          : ORIGIN = 0x08012000, LENGTH = 3524K
    ROM_NVDM_RESERVED(rx) : ORIGIN = 0x08383000, LENGTH = 500K
    RAM(rwx)              : ORIGIN = 0x00000000, LENGTH = 4096K 
    RAM_MD (rwx)          : ORIGIN = 0x00400000, LENGTH = 0K
    VRAM (rwx)            : ORIGIN = 0x10000000, LENGTH = 4096K
    VRAM_MD (rwx)         : ORIGIN = 0x10400000, LENGTH = 0K    
    TCM (rwx)             : ORIGIN = 0x04000000, LENGTH = 64K
    TCM_MD (rwx)          : ORIGIN = 0x04010000, LENGTH = 0K
    SYSRAM (rwx)          : ORIGIN = 0x04200000, LENGTH = 32K
    RETSRAM (rwx)         : ORIGIN = 0x04300000, LENGTH = 8K
    RETSRAM_MD (rwx)      : ORIGIN = 0x04302000, LENGTH = 0K
}
 
 /* Highest address of the stack */
_stack_end = ORIGIN(TCM) + LENGTH(TCM);    /* end of TCM */

/* Generate a link error if stack don't fit into TCM */
_stack_size = 0x1000; /* required amount of stack 4KB*/
 
/* stack start */
_stack_start = _stack_end - _stack_size;
 


/* Entry Point */
ENTRY(Reset_Handler)

SECTIONS
{
    . = ORIGIN(RAM);
    
    /*AP PART*/
   . = ALIGN(4);
   .text :ALIGN(4)
    {
        _text_start = .;
        Image$$TEXT$$Base = .;
	Image$$VECTOR$$TABLE$$Base = .;
        KEEP(*(.isr_vector))
        KEEP(*(.reset_handler))
        KEEP(*(.init))
        KEEP(*(.fini))


        Image$$TEXT$$Limit = .;
        _text_end = .;
        
    } > RAM  AT> RAM
       . = ALIGN(4);
     _ram_noncached_rw_load = LOADADDR(.noncached_ram_data);

    .noncached_ram_data . : ALIGN(4)
    {
    . = ALIGN(4);
        Image$$NONCACHED_DATA$$Base = .;
        _ram_noncached_rw_start = .;
        
        *(.noncached_rwdata)
        *(.md_noncached_ram_rwdata)     
        Image$$NONCACHED_DATA$$Limit = .;
        _ram_noncached_rw_end = .; 
    } > RAM  AT> RAM   
    . = ALIGN(4);
    .noncached_ram_bss . (NOLOAD) :
    {                
        Image$$NONCACHED_ZI$$Base = .;
        _ram_noncached_zi_start = .;
        
        *(.noncached_zidata)
        *(.md_noncached_ram_zidata)
        Image$$NONCACHED_ZI$$Limit = .;
        _ram_noncached_zi_end = .;
       
    } > RAM  AT> RAM

       . = . + ORIGIN(VRAM);
    . = ALIGN(32);
    _ram_code_load = LOADADDR(.cached_ram_text);
    .cached_ram_text . : ALIGN(32)
    {
        Image$$RAM_TEXT$$Base = .;
        _ram_code_start = .;

        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
        *(.ram_code)
        *(.ram_rodata)
        *(.md_text)
        *(.md_rodata)
        *(.md_ram_code)
        *(.md_ram_rodata)
        Image$$RAM_TEXT$$Limit = .;
        _ram_code_end = .;
    } > VRAM   AT> RAM

    . = ALIGN(4);
    _data_load = LOADADDR(.cached_ram_data);

     .cached_ram_data . :ALIGN(4)
    {       
        _data_start = .;
        Image$$CACHED_DATA$$RW$$Base = .;
        
        
        *(.data)
        *(.data*)
        *(.md_ram_rwdata)
        . = ALIGN(4);
        /* preinit data */
        PROVIDE (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE (__preinit_array_end = .);

        . = ALIGN(4);
        /* init data */
        PROVIDE (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE (__init_array_end = .);
        
        . = ALIGN(4);
        /* finit data */
        PROVIDE (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE (__fini_array_end = .);
        
        _data_end = .;       
       Image$$CACHED_DATA$$RW$$Limit = .;
    
        
    } > VRAM  AT> RAM
    
    
    . = ALIGN(4);
    .bss (NOLOAD) : 
    {
          _bss_start = .;
        Image$$CACHED_DATA$$ZI$$Base = .;
        
        *(.bss)
        *(.bss*)
        *(COMMON)
        *(.md_ram_zidata)
        . = ALIGN(4);
        _bss_end = .;
        Image$$CACHED_DATA$$ZI$$Limit = .;
        
    } > VRAM  AT> RAM
      
ASSERT(_bss_end <= (ORIGIN(VRAM) + LENGTH(VRAM)),"RAM expired")
    
    . = ALIGN(4);
      _sysram_data_load = LOADADDR(.sysram_data);

     .sysram_data  :
    {       
        _sysram_data_start = .;
        Image$$SYSRAM_DATA$$RW$$Base = .;
        *(.sysram_rodata)
        *(.sysram_rwdata)
        _sysram_data_end = .;          
       Image$$SYSRAM_DATA$$RW$$Limit = .;
    
        
    } > SYSRAM  AT> RAM
    

    . = ALIGN(4);
    .sysram_bss (NOLOAD) : 
    {
        _sysram_bss_start = .;
        Image$$SYSRAM_DATA$$ZI$$Base = .;
        
       *(.sysram_zidata)
        
        . = ALIGN(4);
        _sysram_bss_end = .;
        Image$$SYSRAM_DATA$$ZI$$Limit = .;
        
    } > SYSRAM  AT> RAM
    
    ASSERT(_sysram_bss_end <= (ORIGIN(SYSRAM) + LENGTH(SYSRAM)),"AP SYSRAM expired")

       . = ALIGN(4);
      _retsram_data_load = LOADADDR(.retsram_data);

     .retsram_data  :
    {       
        _retsram_data_start = .;
        Image$$RETSRAM_DATA$$RW$$Base = .;
        *(.retsram_rodata)
        *(.retsram_rwdata)
        *(.md_retsram_rodata)
        *(.md_retsram_rwdata)
        _retsram_data_end = .;          
       Image$$RETSRAM_DATA$$RW$$Limit = .;
    
        
    } > RETSRAM  AT> RAM
    

    . = ALIGN(4);
    .retsram_bss (NOLOAD) : 
    {
        _retsram_bss_start = .;
        Image$$RETSRAM_DATA$$ZI$$Base = .;
        
       *(.retsram_zidata)
       *(.md_retsram_zidata) 
        . = ALIGN(4);
        _retsram_bss_end = .;
        Image$$RETSRAM_DATA$$ZI$$Limit = .;
        
    } > RETSRAM  AT> RAM
    
    ASSERT(_retsram_bss_end <= (ORIGIN(RETSRAM) + LENGTH(RETSRAM)),"AP retsram expired")
  
   . = ALIGN(4);
    _tcm_text_load = LOADADDR(.tcm);

    .tcm :ALIGN(4)
    {
        
        _tcm_text_start = .;
        Image$$TCM$$RO$$Base = .;

        *(.tcm_code)
        *(.tcm_rodata)
        *(.md_tcm_code)
        *(.md_tcm_rodata)
        Image$$TCM$$RO$$Limit = .;
        Image$$TCM$$RW$$Base = .;
        
        *(.tcm_rwdata)
        *(.md_tcm_rwdata)
        . = ALIGN(4);
        Image$$TCM$$RW$$Limit = .;
        _tcm_text_end = .;
    }> TCM  AT> RAM

    . = ALIGN(4);
    .tcm_bss (NOLOAD) :
    {
        _tcm_zi_start = .;
        Image$$TCM$$ZI$$Base = .;
        
        *(.tcm_zidata)
        *(.md_tcm_zidata)
        _tcm_zi_end = .;
        Image$$TCM$$ZI$$Limit = .;
        
        
    }> TCM   
    
    /* use to check if the stack exceeds the total TCM size*/
    .stack :
    {
        . = ALIGN(4);
        
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + _stack_size;
        . = ALIGN(4);
        
    } > TCM

   /*MD PART*/

    .md_text :
    {
        _md_text_start = .;
        Image$$MD_TEXT$$Base = .;
       

        . = ALIGN(4);
        Image$$MD_TEXT$$Limit = .;
        _md_text_end = .;
        
    } > RAM_MD  AT> RAM_MD

   . = ALIGN(32);
    _md_ram_code_load = LOADADDR(.md_cached_ram_text);

    .md_cached_ram_text :
    {
        Image$$MD_CACHED_RAM_TEXT$$Base = .;
        _md_ram_code_start = .;

        
        
        Image$$MD_CACHED_RAM_TEXT$$Limit = .;
        _md_ram_code_end = .;
    } > VRAM_MD   AT> RAM_MD
  

    . = ALIGN(4);
    _md_data_load = LOADADDR(.md_cached_ram_data);

     .md_cached_ram_data :
    {       
        _md_data_start = .;
        Image$$MD_CACHED_RAM_DATA$$RW$$Base = .;

         

        _md_data_end = .;       
       Image$$MD_CACHED_RAM_DATA$$RW$$Limit = .;
    
        
    } > VRAM_MD  AT> RAM_MD
    
    
    . = ALIGN(4);
    .md_cached_ram_bss (NOLOAD) : 
    {
          _md_bss_start = .;
        Image$$MD_CACHED_RAM_DATA$$ZI$$Base = .;
        

        . = ALIGN(4);
        _md_bss_end = .;
        Image$$MD_CACHED_RAM_DATA$$ZI$$Limit = .;
        
    } > VRAM_MD  AT> RAM_MD
    
     
       . = ALIGN(32);
     _md_ram_noncached_rw_load = LOADADDR(.md_noncached_ram_data);
     . = . - ORIGIN(VRAM);

    .md_noncached_ram_data . :
    {
        Image$$MD_NONCACHED_RAM_DATA$$Base = .;
        _md_ram_noncached_rw_start = .;
        
       
           
        Image$$MD_NONCACHED_RAM_DATA$$Limit = .;
        _md_ram_noncached_rw_end = .; 
    } > RAM_MD  AT> RAM_MD
     
    . = ALIGN(4);
    .md_noncached_ram_bss . (NOLOAD) :
    {                
        Image$$MD_NONCACHED_RAM_ZI$$Base = .;
        _md_ram_noncached_zi_start = .;
        
        
        
        
        Image$$MD_NONCACHED_RAM_ZI$$Limit = .;
        _md_ram_noncached_zi_end = .;
       
    } > RAM_MD  AT> RAM_MD
    
    ASSERT(_ram_noncached_zi_end <= (ORIGIN(RAM_MD) + LENGTH(RAM_MD)),"MD PSRAM expired")

     . = ALIGN(4);
      _md_retsram_data_load = LOADADDR(.md_retsram_data);

     .md_retsram_data  :
    {       
        _md_retsram_data_start = .;
        Image$$MD_RETSRAM_DATA$$RW$$Base = .;

        _md_retsram_data_end = .;          
       Image$$MD_RETSRAM_DATA$$RW$$Limit = .;
    
        
    } > RETSRAM_MD  AT> RAM_MD
    

    . = ALIGN(4);
    .md_retsram_bss (NOLOAD) : 
    {
        _md_retsram_bss_start = .;
        Image$$MD_RETSRAM_DATA$$ZI$$Base = .;
        
       
        
        . = ALIGN(4);
        _md_retsram_bss_end = .;
        Image$$MD_RETSRAM_DATA$$ZI$$Limit = .;
        
    } > RETSRAM_MD  AT> RAM_MD
    
    ASSERT(_md_retsram_bss_end <= (ORIGIN(RETSRAM_MD) + LENGTH(RETSRAM_MD)),"MD retsram expired")

    . = ALIGN(4);
    _md_tcm_text_load = LOADADDR(.md_tcm);

    .md_tcm :
    {
        
        _md_tcm_text_start = .;
        Image$$MD_TCM$$RO$$Base = .;

        Image$$MD_TCM$$RO$$Limit = .;
        Image$$MD_TCM$$RW$$Base = .;  
        
        . = ALIGN(4);
        Image$$MD_TCM$$RW$$Limit = .;
        _md_tcm_text_end = .;
    }> TCM_MD  AT> RAM_MD

    . = ALIGN(4);
    .md_tcm_bss (NOLOAD) :
    {
        _md_tcm_zi_start = .;
        Image$$MD_TCM$$ZI$$Base = .;
        
        
        
        _md_tcm_zi_end = .;
        Image$$MD_TCM$$ZI$$Limit = .;
        
        
    }> TCM_MD 

    ASSERT(_md_tcm_zi_end <= (ORIGIN(TCM_MD) + LENGTH(TCM_MD)),"MD TCM_MD expired")

    /*linker symbol*/
    Image$$STACK$$ZI$$Base = _stack_end - _stack_size;
    Image$$STACK$$ZI$$Limit = _stack_end;
    
    /* provide nvdm start and length for NVDM management */
    Image$$NVDM$$ZI$$Base = ORIGIN(ROM_NVDM_RESERVED);
    Image$$NVDM$$ZI$$Length = LENGTH(ROM_NVDM_RESERVED);
    
    /* provide tcm start and end for Exception Handler */
    Image$$TCM_MD$$RO$$Base = _md_tcm_text_start;
    Image$$TCM_MD$$ZI$$Limit = _md_tcm_zi_end;
       
}
