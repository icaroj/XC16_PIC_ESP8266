/* 
 * File:   esp8266.h
 * Author: Icaro Jonas
 * Email: icarojonas[at]gmail[.]com
 *
 * Created on July 25, 2016, 8:12 PM
 */

// timeout is given in milliseconds

#ifndef _STRING_H_
#include <string.h>
#endif /* _STRING_H_ */

#ifndef __bool_true_and_false_are_defined
#include <stdbool.h>
#endif /* _STDBOOL_H_ */

#ifndef _STDIO_H_
#include <stdio.h>
#endif /* _STDIO_H_ */

#ifndef ESP8266_H
#define	ESP8266_H

#ifdef	__cplusplus
extern "C" {
#endif
/**************************************************************************/
//Timing
#define TIMEOUT_MAX_VALUE 600000 //10 min max.
//WiFi Mode***
#define ESP8266_STA_MODE 1
#define ESP8266_AP_MODE 2
#define ESP8266_AP_STA_MODE 3
//ECN***
#define ESP8266_OPEN 0
#define ESP8266_WPA_PSK 2
#define ESP8266_WPA2_PSK 3
#define ESP8266_WPA_WPA2_PSK 4
//Server mode***
#define ESP8266_CREATE_SERVER 1
#define ESP8266_DELETE_SERVER 0
//Returns***
#define ESP8266_OK 1
#define ESP8266_ALREADY_CONNECTED 2
#define ESP8266_READY 3
#define ESP8266_FAIL 4
#define ESP8266_ERROR 5
//Close all connections***
#define ESP8266_CLOSE_ALL 5
// [***] -> do not change these values

/* 
 * bool ESP8266_INIT(long FCY, int BAUD, char ALTIO);
 * timer2 (timeout function) and uart1 (communication)
 * must be used before any other function
 * return: true on sucess; false if PR2 > 65536;
 */
extern bool ESP8266_INIT(long FCYx, int BAUD);

//COMMANDS

/*
 * bool ESP8266_TEST(unsigned long timeout)
 * TEST STARTUP
 * return: true/false based on result
 */
extern bool ESP8266_TEST(unsigned long timeout);

/*
 * bool ESP8266_RESET(unsigned long timeout)
 * RESTART MODULE
 * return: true when ready; false on failure
 */
extern bool ESP8266_RESET(unsigned long timeout);


//WiFi Functions

/*
 * bool ESP8266_MODE(unsigned char mode, unsigned long timeout)
 * SET WiFi MODE
 * mode = ESP8266_STA_MODE || ESP8266_AP_MODE || ESP8266_AP_STA_MODE
 * return: true/false based on result
 */
extern bool ESP8266_MODE(unsigned char mode, bool save_to_flash, unsigned long timeout);

/*
 * bool ESP8266_CONNECT(char *ssid, char *pass, unsigned long timeout)
 * CONNECT TO AP
 * SSID, PASSWORD, save_to_flash (true or false), timeout 
 * return: true on success;
 */
extern bool ESP8266_CONNECT(char *ssid, char *pass, bool save_to_flash, unsigned long timeout);
/*
 * bool ESP8266_DISCONNECT(unsigned long timeout)
 * DISCONNECT FROM AP
 * return: true/false based on result
 */
extern bool ESP8266_DISCONNECT(unsigned long timeout);

/*
 * bool ESP8266_CONFIG_AP(char *ssid, char *pass, char channel, char ecn, bool save_to_flash, unsigned long timeout)
 * SSID, PASS (AT LEAST 8 CHARS ASCII), CHANNEL ID, ECN = OPEN | WPA_PSK | WPA2_PSK | WPA_WPA2_PSK, save to flash = true | false, timeout (milliseconds)
 * return: true/false based on result
 * NOTE: if enc != ESP8266_OPEN, save_to_flash must be set (reset after ecn)
 */
extern bool ESP8266_CONFIG_AP(char *ssid, char *pass, char channel, char ecn, bool save_to_flash, unsigned long timeout);

//TCP/IP Related

/*
 * bool ESP8266_MUX(char mode, unsigned long timeout)
 * ENABLE MULTIPLE CONNECTIONS 
 * mode = 0 - single connection ; 1 - multiple connections
 * return: true/false based on result
 */
extern bool ESP8266_MUX(char mode, unsigned long timeout);

/*
 * bool ESP8266_TCP_SERVER(char mode, char port, unsigned long timeout)
 * CONFIGURE AS TCP SERVER 
 * mode = ESP8266_CREATE_SERVER || ESP8266_DELETE_SERVER
 * return: true/false based on result
 * note: server can only be created when ESP8266_MUX is set to multiple connections
 */
extern bool ESP8266_TCP_SERVER(char mode, char port, unsigned long timeout);


extern bool ESP8266_SEND(bool mux, int linkId, char *data, unsigned long timeout);



/*
 * int ESP8266_FIND(char *target, unsigned long timeout)
 * READS DATA RECEIVED UNTIL TARGET IS FOUND. 
 * return: link ID or n -1 if times out;
 * IMPORTANT: BUG -> *target must be greater than 3 characters
 */
extern int ESP8266_FIND(char *target, unsigned long timeout);

/*
 * bool ESP8266_START(char *protocol, char *ip, char port, unsigned long timeout);
 * ESTABLISH TCP/UDP CONNECTION/TRANSMISSION OR SSL CONNECTION
 * return: true if ALREADY_CONNECTED OR OK, false in case of ERROR;
 */
extern bool ESP8266_START(char *protocol, char *ip, char port, unsigned long timeout);

/*
 * bool ESP8266_CLOSE(char linkID, unsigned long timeout);
 * CLOSE UDP OR TCP CONNECTION
 * linkID = connection ID | when linkID = ESP8266_CLOSE_ALL, all connections will be closed (no effect in server mode)
 * return: true/false based on result;
 */
extern bool ESP8266_CLOSE(int linkID, unsigned long timeout);

/**
 * void URLDecode(char *str, int length)
 * UTF8 -> ASCII
 **/
extern void URLDecode(char *str, int length);

extern bool ESPFind(char *target);
extern void ESPWrite(char* st, int len);
extern void ESPWriteChar(unsigned char c);
extern unsigned char ESPResponse(void);
extern unsigned char ESPReadChar(void);
extern void setTimeout(unsigned long timeout_value);
extern void __attribute__ ((interrupt, no_auto_psv)) _T2Interrupt(void);

/**************************************************************************/
#ifdef	__cplusplus
}
#endif

#endif	/* ESP8266_H */

/*
 * ESP8266_TEST -> Test AT startup; returns true or false;
 * 
 */