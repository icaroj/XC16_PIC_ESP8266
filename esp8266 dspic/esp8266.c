/** v0.001 with .h and examples at: http://gist.github.com/icaroj/42c4a6160fbb0ddf151be70d5e821035 **/

/* 
 * File:   esp8266.c
 * Author: Icaro Jonas
 * Email: icarojonas[at]gmail[.]com
 *
 * Created on July 25, 2016, 8:12 PM
 * 
 * REGISTERS YOU MAY CONSIDER TO CHANGE:
 * U1BRG <- default: FCY/(16*BAUDRATE) - 1; 
 * U1MODE
 * U1STA
 * U1TXREG
 * U1RXREG
 * IFS0
 * IEC0
 * 
 * setTimeout must be declared before ESPRead()!
 */
#include <xc.h>
#include "esp8266.h"
/* GLOBAL VARS */

//timeout
bool _timeoutFlag = false; 
unsigned long _timeout = TIMEOUT_MAX_VALUE;
unsigned long _t2counter = 0;

//buffers
char sbuf[64];
/* ESP8266 FUNCTIONS */

// Configure Timer2 in 1 sec (timeout function)
bool ESP8266_INIT(long FCYx, int BAUD){
    //uart1
    U1MODE = 0;
    U1STA = 0;
    U1BRG = ((FCYx/16)/BAUD) - 1;
    U1MODEbits.UARTEN = 1; 
    U1STAbits.UTXEN = 1;  
    IFS0bits.U1RXIF = 0;
    IFS0bits.U1TXIF = 0;
    IEC0bits.U1TXIE = 0; 
    IEC0bits.U1RXIE = 0; 
    //timer2
    int period = FCYx/256000; // 1 sec = FCYx/PCKPS/1000
    if (period <= 65536){
        TMR2 = 0;
        PR2 = period;
        IFS0bits.T2IF = 0;
        IEC0bits.T2IE = 1;
        T2CONbits.TCKPS = 0b11;
        T2CONbits.TON = 1;
        return true;
    }else{
        return false;
    }
}

bool ESP8266_TEST(unsigned long timeout){
    ESPWrite("AT\r\n", 4);
    setTimeout(timeout);
    if(ESPResponse() == ESP8266_OK){
        return true;
    }
    else{
        return false;
    }
}

bool ESP8266_RESET(unsigned long timeout){
    ESPWrite("AT+RST\r\n", 8);
    setTimeout(timeout);
    if(ESPResponse() == ESP8266_OK){
        if(ESPResponse() == ESP8266_READY)
            return true;
    }
    return false;
}

bool ESP8266_MODE(unsigned char mode, bool save_to_flash, unsigned long timeout){
    if(save_to_flash == true)
         sprintf(sbuf, "AT+CWMODE_DEF=%d\r\n", mode);
    else
        sprintf(sbuf, "AT+CWMODE_CUR=%d\r\n", mode);
    ESPWrite(sbuf, strlen(sbuf));
    setTimeout(timeout);
    if(ESPResponse() == ESP8266_OK)
        return true;
    else
        return false;
}

bool ESP8266_CONNECT(char *ssid, char *pass, bool save_to_flash, unsigned long timeout){
    if(save_to_flash == true)
        sprintf(sbuf, "AT+CWJAP_DEF=\"%s\",\"%s\"\r\n", ssid, pass);
    else
        sprintf(sbuf, "AT+CWJAP_CUR=\"%s\",\"%s\"\r\n", ssid, pass);
    ESPWrite(sbuf, strlen(sbuf));
    setTimeout(timeout);
    char resp;
    while((resp = ESPResponse()) != 0){
        switch(resp){
            case ESP8266_OK:
                return true;
            break;
            case ESP8266_ERROR:
                return false;
            break;
            case ESP8266_FAIL:
                return false;
            break;
        }
    }
    return false;
}

bool ESP8266_DISCONNECT(unsigned long timeout){
    ESPWrite("AT+CWQAP\r\n", 10);
    setTimeout(timeout);
    if(ESPResponse() == ESP8266_OK)
        return true;
    else
        return false;
}

bool ESP8266_CONFIG_AP(char *ssid, char *pass, char channel, char ecn, bool save_to_flash, unsigned long timeout){
    if(save_to_flash == true)
        sprintf(sbuf, "AT+CWSAP_DEF=\"%s\",\"%s\",%d,%d\r\n", ssid, pass, channel, ecn);
    else
        sprintf(sbuf, "AT+CWSAP_CUR=\"%s\",\"%s\",%d,%d\r\n", ssid, pass, channel, ecn);
    ESPWrite(sbuf, strlen(sbuf));
    setTimeout(timeout);
    char resp;
    while((resp = ESPResponse()) != 0){
        switch(resp){
            case ESP8266_OK:
                return true;
            break;
            case ESP8266_ERROR:
                return false;
            break;
        }
    }
    return false;
}

bool ESP8266_MUX(char mode, unsigned long timeout){
    sprintf(sbuf, "AT+CIPMUX=%d\r\n", mode);
    ESPWrite(sbuf, strlen(sbuf));
    setTimeout(timeout);
    if(ESPResponse() == ESP8266_OK){
        return true;
    }else{
        return false;
    }
}

bool ESP8266_TCP_SERVER(char mode, char port, unsigned long timeout){
    sprintf(sbuf, "AT+CIPSERVER=%d,%d\r\n", mode, port);
    ESPWrite(sbuf, strlen(sbuf));
    setTimeout(timeout);
    if(ESPResponse() == ESP8266_OK){
        return true;
    }else{
        return false;
    }
}

bool ESP8266_SEND(bool mux, int linkId, char *data, unsigned long timeout){
//    strcat(data, "\r\n");
    int len = strlen(data);
    if(mux == true)
        sprintf(sbuf, "AT+CIPSEND=%d,%d\r\n", linkId, len);
    else
        sprintf(sbuf, "AT+CIPSEND=%d\r\n", len);
    ESPWrite(sbuf, strlen(sbuf));
    setTimeout(timeout);
    while(ESPReadChar() != '>' && !_timeoutFlag);
    if(!_timeoutFlag){
        ESPWrite(data, len);
        if(ESPResponse() == ESP8266_OK)
            return true;
        else
            return false;

    }else{
        return false;
    }
}

int ESP8266_FIND(char *target, unsigned long timeout){
    int res;
    setTimeout(timeout);
    if(ESPFind("+IPD,")){
        res = ESPReadChar()-48;
        ESPFind(target);
    }else{
        res = -1;
    }
    return res;
}

bool ESP8266_START(char *protocol, char *ip, char port, unsigned long timeout){
    sprintf(sbuf, "AT+CIPSTART=0,\"%s\",\"%s\",%d\r\n", protocol, ip, port);
    ESPWrite(sbuf, strlen(sbuf));
    setTimeout(timeout);
    if(ESPResponse() == ESP8266_ERROR){
        return false;
    }else{
        return true;
    }
}


bool ESP8266_CLOSE(int linkID, unsigned long timeout){
    sprintf(sbuf, "AT+CIPCLOSE=%d\r\n", linkID);
    ESPWrite(sbuf, strlen(sbuf));
    setTimeout(timeout);
    if(ESPResponse() == ESP8266_OK){
        return true;
    }else{
        return false;
    }
}


void URLDecode(char *str, int length){
    char buff[33]; //max length of ssid 32 char + \0
    char *ptr = buff;
    unsigned char c0,c1;
    unsigned int i = 0;
    unsigned char c;
    memset(buff, 0, sizeof(buff));
    while((i) < length){
        c = str[i];
        if(c == '+') c = ' ';
        if(c == '%'){
            c0 = str[i+1];
            c1 = str[i+2];
            if(c0 >= '0' && c0 <= '9')
                c0 = c0 - '0';
            if(c0 >= 'A' && c0 <= 'F')
                c0 = c0 - 'A';
            if(c1 >= '0' && c1 <= '9')
            c1 = c1 - '0';
            if(c1 >= 'A' && c1 <= 'F')
                c1 = c1 - 'A';
            //------------------
            c = c0*16 + c1; //utf8 to ascii
            i = i + 2;
        }
        *ptr++ = c;
        i++;
    }
    *ptr = '\0';
    strcpy(str, buff);
}

/* UART1 */

bool ESPFind(char *target){
    unsigned int i = 0;
    unsigned int len = strlen(target);
    unsigned char rxBuffer = 0x00;
    while(true){
        rxBuffer = ESPReadChar();
        while(rxBuffer == target[i]){
            i++;
            if(i == len){
                return true;
            }
            rxBuffer = ESPReadChar();
        }
        i = 0;
        if(_timeoutFlag){
            return false;
        }
    }
}

void ESPWrite(char* st, int len){
    int i;
    for(i = 0; i < len; i++){
        ESPWriteChar(st[i]);
    }
}

void ESPWriteChar(unsigned char c){
    while(U1STAbits.UTXBF);
    U1TXREG = c;
    while(!U1STAbits.TRMT);
}

unsigned char ESPResponse(void){
    unsigned char rxBuffer = 0x00;
    char *response[5] = {"OK", "ALREADY", "ready", "FAIL", "ERROR"};
    unsigned int i = 0;
    unsigned int j;
    unsigned int len = 0;
    while(true){
        rxBuffer = ESPReadChar();
        for(i = 0; i < 5; i++){
            len = strlen(response[i]); 
            j = 0; 
            while(rxBuffer == response[i][j]){
                j++;
                if(j == len){
                    return (i + 1); // array index + 1 = ESP8266_RESPONSE
                }
                rxBuffer = ESPReadChar();
            }
        }
        if(_timeoutFlag){
            return 0;
        }
    }
}

unsigned char ESPReadChar(void){
    while(!IFS0bits.U1RXIF && !_timeoutFlag);
    IFS0bits.U1RXIF = 0;
    if(_timeoutFlag || U1STAbits.OERR){
        U1STAbits.OERR = 0;
        return 0x00;
    }
    else{
        return U1RXREG; 
    }
}
/* TIMEOUT IMPLEMENTATION (TIMER2) */

void setTimeout(unsigned long timeout_value){
    _timeoutFlag = 0;
    _t2counter = 0;
    _timeout = timeout_value;
    T2CONbits.TON = 1;
}

void __attribute__ ((interrupt, no_auto_psv)) _T2Interrupt(void){
    IFS0bits.T2IF = 0;
    if(_t2counter < _timeout && _t2counter < TIMEOUT_MAX_VALUE){
        _t2counter++;
    }
    else{
        _t2counter = 0;
        _timeoutFlag = true;            
        T2CONbits.TON = 0;
    }
}