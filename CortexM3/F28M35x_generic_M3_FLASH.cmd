/*
//###########################################################################
// FILE:    F28M35x_generic_M3_FLASH.cmd
// TITLE:   Linker Command File for F28M35x examples that run out of FLASH
//###########################################################################
// $TI Release: F28M35x Support Library v130 $
// $Release Date: Mon Apr 30 15:31:28 CDT 2012 $
//###########################################################################
*/

--retain=g_pfnVectors

/* The following command line options are set as part of the CCS project.    */
/* If you are building using the command line, or for some reason want to    */
/* define them here, you can uncomment and modify these lines as needed.     */
/* If you are using CCS for building, it is probably better to make any such */
/* modifications in your CCS project and leave this file alone.              */
/*                                                                           */
/* --heap_size=0                                                             */
/* --stack_size=256                                                          */
/* --library=rtsv7M3_T_le_eabi.lib                                           */


/* The following options allow the user to program Z1 and Z2 DCSM security   */
/* values, include CSM PSWD, ECSL PSWD, GRABSECT, GRABRAM, and FLASH EXEONLY */
/* The driverlib/dcsm_z1_secvalues.s and driverlib/dcsm_z2_secvalues.s files */
/* must be included in the Flash project for the below 2 lines to take       */
/* effect.                                                                   */
--retain=dcsm_z1_secvalues.obj(.z1secvalues,.z1_csm_rsvd)
--retain=dcsm_z2_secvalues.obj(.z2secvalues,.z2_csm_rsvd)

/* System memory map */

MEMORY
{
    CSM_ECSL_Z1     : origin = 0x00200000, length = 0x0024
    CSM_RSVD_Z1     : origin = 0x00200024, length = 0x000C
    RESETISR (RX)   : origin = 0x00200030, length = 0x0008   /* Reset ISR is mapped to boot to Flash location */
    INTVECS (RX)    : origin = 0x00201000, length = 0x01B0
    FLASHLOAD (RX)  : origin = 0x00201200, length = 0x2E00   /* For storing code in Flash to copy to RAM at runtime */
    FLASH1 (RX)     : origin = 0x00204000, length = 0x1C000
    FLASH2 (RX)     : origin = 0x00260000, length = 0x1FF00
    CSM_RSVD_Z2     : origin = 0x0027FF00, length = 0x00DC
    CSM_ECSL_Z2     : origin = 0x0027FFDC, length = 0x0024
    C0 (RWX)        : origin = 0x20000000, length = 0x3800
    C1 (RWX)        : origin = 0x20003800, length = 0x0800
    BOOT_RSVD (RX)  : origin = 0x20004000, length = 0x0900
    C2 (RWX)        : origin = 0x20004900, length = 0x1700
    C3 (RWX)        : origin = 0x20006000, length = 0x2000
    S0 (RWX)        : origin = 0x20008000, length = 0x2000
    S1 (RWX)        : origin = 0x2000A000, length = 0x2000
    S2 (RWX)        : origin = 0x2000C000, length = 0x2000
    S3 (RWX)        : origin = 0x2000E000, length = 0x2000
    S4 (RWX)        : origin = 0x20010000, length = 0x2000
    S5 (RWX)        : origin = 0x20012000, length = 0x2000
    S6 (RWX)        : origin = 0x20014000, length = 0x2000
    S7 (RWX)        : origin = 0x20016000, length = 0x2000
    CTOMRAM (RX)    : origin = 0x2007F000, length = 0x0800
    MTOCRAM (RWX)   : origin = 0x2007F800, length = 0x0800
}

/* Section allocation in memory */

SECTIONS
{
    .intvecs:   > INTVECS
    .resetisr:  > RESETISR
    .text   :   >> FLASH1 | FLASH2
    .const  :   >> FLASH1 | FLASH2
    .cinit  :   >  FLASH1 | FLASH2
    .pinit  :   >> FLASH1 | FLASH2

    .vtable :   >  C0 | C1 | C2 | C3
    .data   :   >  C2 | C3
    .bss    :   >> C2 | C3
    .sysmem :   >  C0 | C1 | C2 | C3
    .stack  :   >  C0 | C1 | C2 | C3
    
    .z1secvalues  :   >  CSM_ECSL_Z1
    .z1_csm_rsvd  :   >  CSM_RSVD_Z1
    .z2secvalues  :   >  CSM_ECSL_Z2
    .z2_csm_rsvd  :   >  CSM_RSVD_Z2
    
    
     ramfuncs            : LOAD = FLASHLOAD,
                           RUN = C0,
                           LOAD_START(RamfuncsLoadStart),
                           LOAD_SIZE(RamfuncsLoadSize),
                           RUN_START(RamfuncsRunStart),
                           PAGE = 0

    SHARERAMS0  : > S0
    SHARERAMS1  : > S1
    SHARERAMS2  : > S2
    SHARERAMS3  : > S3
    SHARERAMS4  : > S4
    SHARERAMS5  : > S5
    SHARERAMS6  : > S6
    SHARERAMS7  : > S7

    
    GROUP : > MTOCRAM
    {
        PUTBUFFER  
        PUTWRITEIDX
        GETREADIDX  
    }

     GROUP : > CTOMRAM 
    {
        GETBUFFER : TYPE = DSECT
        GETWRITEIDX : TYPE = DSECT
        PUTREADIDX : TYPE = DSECT
    }    
}






