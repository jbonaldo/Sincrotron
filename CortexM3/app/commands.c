#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "includes/commands.h"
#include "includes/shared_mem_arm.h"
#include "../Servidor/includes/httpd.h"

char buffer_value[128];
float valor_recebido = 0;
float valor_a_enviar = 0.123;

commands_t command;
char string_output[DATA_BYTES_MAX];

int cmd_processCMD(commands_t *cmd)
{
    int nCharWrote = 0;
    
    switch(cmd->cmd)
    {
    case CMD_SET_I_REF:
        valor_recebido = atof( cmd->data );
        shm_setPiRef( valor_recebido / 10.0);
        break;


    case CMD_GET_I_MEAS:
        valor_a_enviar = 10.0 * shm_getIout();
        nCharWrote = sprintf(string_output, "%2.6f", valor_a_enviar ); 
        httpd_insert_response(nCharWrote,(char *)string_output);   

    default:

        break;
    }
    
    return 0;
}