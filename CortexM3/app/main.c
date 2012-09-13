
#include "../Servidor/enet_uip.h"
#include "includes/commands.h"
#include "includes/shared_mem_arm.h"


extern void c28_config(void);

int main( void )
{
    c28_config();
    ethernet_config();
    shared_mem_init();
    
    while( 1 )
    {
        if( ethernet_main( &command.cmd, &command.length, command.data) )
            cmd_processCMD( &command );
    }
    
}
