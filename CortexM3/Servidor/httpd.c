//*****************************************************************************
// HTTP server.
// Adam Dunkels <adam@dunkels.com>
// Copyright (c) 2001, Adam Dunkels.
// All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. The name of the author may not be used to endorse or promote
//    products derived from this software without specific prior
//    written permission.
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// This file is part of the uIP TCP/IP stack.
//*****************************************************************************
// This file has been modified from its original uIP distribution to add
// functionality needed for the enet_uip example.
//*****************************************************************************

#include "uip.h"
#include "httpd.h"
#include <string.h>
#include <stdlib.h>

//*****************************************************************************
// Macro for easy access to buffer data
//*****************************************************************************
#define BUF_APPDATA ((u8_t *)uip_appdata)

//*****************************************************************************
// Definitions of HTTP Server States
//*****************************************************************************
#define HTTP_NOGET      0
#define HTTP_FILE       1
#define HTTP_TEXT       2
#define HTTP_FUNC       3
#define HTTP_END        4

//*****************************************************************************
// Global for keeping up with web server state.
//*****************************************************************************
struct httpd_state *hs;

int index;

//*****************************************************************************
// command words
//*****************************************************************************
#define TOGGLE_LED_6     0xA1
#define TOGGLE_LED_7     0xA2
#define GET_LED6_STATUS  0xA3
#define GET_LED7_STATUS  0xA4
#define INVALID_CMD      0xA5
#define INVALID_INPUT    0xA6
#define NO_CMD           0xA6
#define CMD_GET_I_MEAS   0xAA
#define CMD_SET_I_REF    0xAB

#define LED_6            0
#define LED_7            1

static int static_web_page_fully_sent = 0;
static int command;
static int selected_LED;

//*****************************************************************************
// Default Web Page - allocated in three segments to allow easy update of a
// counter that is incremented each time the page is sent.
//*****************************************************************************
static const char page_not_found[] =
    "HTTP/1.0 404 OK\r\n"
    "Server: UIP/1.0 (http://www.sics.se/~adam/uip/)\r\n"
    "Content-type: text/html\r\n\r\n"
    "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN"
    "http://www.w3.org/TR/html4/loose.dtd\">"
    "<html>"
    "<head>"
    "<title>Page Not Found!</title>"
    "</head>"
    "<body>"
    "Page Not Found!"
    "</body>"
    "</html>";

//*****************************************************************************
// Default Web Page - allocated in three segments to allow easy update of a
// counter that is incremented each time the page is sent.
//*****************************************************************************
static const char default_page_buf1of3[] =
    "HTTP/1.0 200 OK\r\n"
    "Server: UIP/1.0 (http://www.sics.se/~adam/uip/)\r\n"
    "Content-type: text/html\r\n\r\n"
    "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN"
    "http://www.w3.org/TR/html4/loose.dtd\">"
    "<html>"
    "<head>"
    "<title>Projeto LNLS/LCEE</title>"
    "</head>"
    "<script language=\"JavaScript\">"
    "var Rx = false;"
    "var http = false;"
    "function Rcv()"
    "{"
    "if(http.readyState == 4)"
    "{"
    "if(http.status == 200)"
    "{"
    "resp = http.responseText;"
    "if(Rx)"
    "{"
    "var v2 = document.getElementById(\"I2\");"
    "v2.value = resp;"
    "}"
    "}"
    "}"
    "}"
    "function cmd(input)"
    "{"
    "if(input == 'C00')"
    "{"
    "Rx = false;"
    "}"
    "else if(input == 'C50')"
    "{"
    "Rx = true;"
    "}"
    "var v1 = document.getElementById(\"I1\");"
    "if(window.XMLHttpRequest)"
    "{"
    "http = new XMLHttpRequest();"
    "}"
    "else if(window.ActiveXObject)"
    "{"
    "http = new ActiveXObject(\"Microsoft.XMLHTTP\");"
    "}"
    "if(http)"
    "{"
    "http.open(\"GET\", \"cmd?=\" + input + v1.value + \"&id\" + Math.random(), true);"
    "http.onreadystatechange = Rcv;"
    "http.send(null);"
    "}"

    "}"
    "</script>";
static const char default_page_buf2of3[] =
    "<html>"
    "<head>"
    "<title>Projeto LNLS/LCEE</title>"
    "</head>"
    "<body>"
    "<center>"
    "<h1>Ajuste de parametros da fonte</h1>"
    "<p>&nbsp;"
    "<hr size=\"3\" width=\"1000\">"
    "<p>&nbsp;"
    "<table>"
    "<tr><td width=\"3000\">Referencia de corrente:</td><td width=\"3000\">Corrente medida na saida</td></tr>"
    "<tr>"
    "<td><br/><input id=\"B1\" value=\"IRef\" onclick=\"cmd('C00');\"type=\"button\"><input maxlength=\"9\" size=\"4\" id=\"I1\" type=\"text\"></td>"
    "<td><input id=\"B2\" value=\"Iout\" onclick=\"cmd('C50');\" type=\"button\"><input maxlength=\"10\" size=\"10\" id=\"I2\" type=\"text\"></td>"
    "</tr>"
    "</table>"
    "<br/><br/><br/><br/>";
static const char default_page_buf3of3[] =
    "</center>"
    "</body>"
    "</html>";
char empty_char[] = " ";
typedef struct
{
    char *response_data;
    int length;
}response;
response response_to_client;

typedef struct {
    char data[128];
    int length;   
} buffer_t;

buffer_t buffer_rx;

//*****************************************************************************
// Initialize the web server.
// Starts to listen for incoming connection requests on TCP port 80.
//*****************************************************************************
void
httpd_init(void)
{
    // Listen to port 80.
    uip_listen(HTONS(80));
}

//*****************************************************************************
// Parse command, 'C0' to toggle LED, 'C1' to get status of the LED
//****************************************************************************
void
httpd_parse_command_word(void)
{
    char temp[3] = {0, 0, 0};
    
    // Toggle command
    if( (BUF_APPDATA[10] == 'C') && (BUF_APPDATA[11] == '0'))
    {
        temp[0] = BUF_APPDATA[11];
        temp[1] = BUF_APPDATA[12];            
        command = atoi( temp );
        
        strncpy(&(buffer_rx.data[0]), (char*)&BUF_APPDATA[13], 6);
        buffer_rx.length = 6;
        /*
        switch(BUF_APPDATA[12])
        {
        case '6':
            command = TOGGLE_LED_6;
            selected_LED = 0;
            break;

        case '7':
            command = CMD_SET_I_REF;
            strncpy(&(buffer_rx.data[0]), (char*)&BUF_APPDATA[13], 6);
            buffer_rx.length = 6;
            break;

        default:
            command = INVALID_INPUT;
            selected_LED = 2;

            break;
        }
        */

    }

    // Get status command
    else if((BUF_APPDATA[10] == 'C')&& (BUF_APPDATA[11] == '5'))
    {
        temp[0] = BUF_APPDATA[11];
        temp[1] = BUF_APPDATA[12];            
        command = atoi( temp );
        
        /*
        switch(selected_LED)
        {
        case LED_6:
            command = GET_LED6_STATUS;
            break;

        case LED_7:
            command = CMD_GET_I_MEAS;
            break;

        default:
            command = INVALID_INPUT;
            break;
        }
        */

    }

    // Invalid command
    else
    {
        command = INVALID_CMD;
    }

}

//*****************************************************************************
// return the status along with the command
//*****************************************************************************
int
httpd_get_command(int *command_word, int *buffer_lenth, char *buffer)
{
    if((command != INVALID_CMD) && (command != INVALID_INPUT) &&
       (command != NO_CMD))
    {
        *command_word = command;
        memmove(buffer, &(buffer_rx.data[0]), buffer_rx.length );
        *buffer_lenth = buffer_rx.length;
        return(1);
    }
    else
    {
        return(0);
    }
}

//*****************************************************************************
// clear the command, so that we won't execute it again
//*****************************************************************************
void
httpd_clear_command(void)
{
    command = NO_CMD;
    memset( &(buffer_rx.data[0]), 0, 10 );
}

//*****************************************************************************
// insert a response
//*****************************************************************************

void
httpd_insert_response(int data_length,char *data)
{
    response_to_client.response_data = data;
    response_to_client.length = data_length;
}

//*****************************************************************************
// HTTP Application Callback Function
//*****************************************************************************
void
httpd_appcall(void)
{
    switch(uip_conn->lport)
    {
    // This is the web server:
    case HTONS(80):
    {

        // Pick out the application state from the uip_conn structure.
        hs = (struct httpd_state *)&(uip_conn->appstate);

        // We use the uip_ test functions to deduce why we were
        // called. If uip_connected() is non-zero, we were called
        // because a remote host has connected to us. If
        // uip_newdata() is non-zero, we were called because the
        // remote host has sent us new data, and if uip_acked() is
        // non-zero, the remote host has acknowledged the data we
        // previously sent to it. */
        if(uip_connected())
        {
            // Since we have just been connected with the remote host, we
            // reset the state for this connection. The ->count variable
            // contains the amount of data that is yet to be sent to the
            // remote host, and the ->state is set to HTTP_NOGET to signal
            // that we haven't received any HTTP GET request for this
            // connection yet.
            hs->state = HTTP_NOGET;
            hs->count = 0;
            return;
        }
        else if(uip_poll())
        {
            // If we are polled ten times, we abort the connection. This is
            // because we don't want connections lingering indefinately in
            // the system.
            if(hs->count++ >= 10)
            {
                uip_abort();
            }
            return;
        }
        else if(uip_newdata() && hs->state == HTTP_NOGET)
        {

            if(BUF_APPDATA[0] != 'G' || BUF_APPDATA[1] != 'E' ||
               BUF_APPDATA[2] != 'T' || BUF_APPDATA[3] != ' ')
            {
                uip_abort();
                return;
            }

            // Check to see what we should send.
            if((BUF_APPDATA[4] == '/') && (BUF_APPDATA[5] == ' '))
            {
                static_web_page_fully_sent = 0;
                uip_send(default_page_buf1of3,sizeof(default_page_buf1of3) - 1);
            }
            else if((BUF_APPDATA[4] == '/') && (BUF_APPDATA[5] == 'c')&&
                    (BUF_APPDATA[6] == 'm')&&(BUF_APPDATA[7] == 'd'))
            {
                httpd_parse_command_word();

                // send an empty character. when this function exits the values
                // will be updated. The response will be
                // sent to client when client acks the empty character
                uip_send(empty_char,sizeof(empty_char)-1);
            }
            else
            {
                uip_send(page_not_found,sizeof(page_not_found) - 1);
                hs->count = 3;
            }
        }
        else if(uip_acked())
        {

            hs->count++;
            if(static_web_page_fully_sent == 0)
            {
                if(hs->count == 1)
                {
                    uip_send(default_page_buf2of3,
                             sizeof(default_page_buf2of3) - 1);
                }
                else if(hs->count == 2)
                {
                    uip_send(default_page_buf3of3,
                             sizeof(default_page_buf3of3) - 1);
                    static_web_page_fully_sent = 1;
                }
                else
                {
                    uip_close();
                }
            }
            else
            {
                if(hs->count == 1)
                {
                    uip_send(response_to_client.response_data,
                             response_to_client.length);
                }
                else
                {
                    uip_close();
                }

            }
        }

        // Finally, return to uIP. Our outgoing packet will soon be on its
        // way...
        return;
    }

    default:
    {
        // Should never happen.
        uip_abort();
        break;
    }
    }
}



