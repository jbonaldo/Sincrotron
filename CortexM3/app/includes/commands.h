#ifndef COMMANDS_H_
#define COMMANDS_H_


#define CMD_GET_I_MEAS   50
#define CMD_SET_I_REF    0

#define DATA_BYTES_MAX  128

typedef struct {
    int  cmd;
    int  length;
    char data[DATA_BYTES_MAX];
} commands_t;

extern commands_t command;

int cmd_processCMD(commands_t *cmd);

#endif /*COMMANDS_H_*/
