/*
 * File:   main.c
 * Author: Icaro
 *
 * Created on 18 de Janeiro de 2017, 18:01
 */

// DSPIC33FJ12MC202 Configuration Bit Settings

// 'C' source line config statements

// FBS
#pragma config BWRP = WRPROTECT_OFF     // Boot Segment Write Protect (Boot Segment may be written)
#pragma config BSS = NO_FLASH           // Boot Segment Program Flash Code Protection (No Boot program Flash segment)

// FGS
#pragma config GWRP = OFF               // General Code Segment Write Protect (User program memory is not write-protected)
#pragma config GSS = OFF                // General Segment Code Protection (User program memory is not code-protected)

// FOSCSEL
#pragma config FNOSC = FRCPLL           // Oscillator Mode (Internal Fast RC (FRC) w/ PLL)
#pragma config IESO = OFF               // Internal External Switch Over Mode (Start-up device with user-selected oscillator source)

// FOSC
#pragma config POSCMD = NONE            // Primary Oscillator Source (Primary Oscillator Disabled)
#pragma config OSCIOFNC = ON            // OSC2 Pin Function (OSC2 pin has digital I/O function)
#pragma config IOL1WAY = OFF            // Peripheral Pin Select Configuration (Allow Multiple Re-configurations)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Both Clock Switching and Fail-Safe Clock Monitor are disabled)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler (1:32,768)
#pragma config WDTPRE = PR128           // WDT Prescaler (1:128)
#pragma config WINDIS = OFF             // Watchdog Timer Window (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog timer enabled/disabled by user software)

// FPOR
#pragma config FPWRT = PWR128           // POR Timer Value (128ms)
#pragma config ALTI2C = OFF             // Alternate I2C  pins (I2C mapped to SDA1/SCL1 pins)
#pragma config LPOL = OFF               // Motor Control PWM Low Side Polarity bit (PWM module low side output pins have active-low output polarity)
#pragma config HPOL = OFF               // Motor Control PWM High Side Polarity bit (PWM module high side output pins have active-low output polarity)
#pragma config PWMPIN = ON             // Motor Control PWM Module Pin Mode bit (PWM module pins controlled by PORT register at device Reset)

// FICD
#pragma config ICS = PGD1               // Comm Channel Select (Communicate on PGC1/EMUC1 and PGD1/EMUD1)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG is Disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define FCY 10009297ULL

#include "xc.h"
#include "libpic30.h"
#include "string.h"


// Functions
void initClock(void);
void UARTMap(void);
void UARTInit(long FCYx, int BAUD);
void UARTWrite(char* st, int len);
void UARTWriteChar(unsigned char c);
unsigned char UARTReadChar(void);
void I2CInit(void);
void I2CStart(void);
void I2CRestart(void);
void I2CStop(void);
void I2CWaitACK(void);
void I2CIdle(void);
void I2CAck(void);
void I2CNack(void);
void I2CWrite(unsigned char c);
void I2CRead(void);
void I2CWriteReg(char addr, char byteHigh, char byteLow, char value);
char I2CReadReg(char addr, char byteHigh, char byteLow);
void I2CSequentialWriteReg(char addr, char byteHigh, char byteLow, char* value, int length);
void I2CSequentialReadReg(char addr, char byteHigh, char byteLow, char* buff, int length);
//Global Vars
#define EEPROMaddr 80
#define BUFF_SIZE 32 // BUFFER_SIZE MUST ALWAYS BE 1 BYTE GREATER THAN MSG_SIZE
#define MSG_SIZE 27
char data;
char escrevendo[] = "escrevendo...\r\n";
char lendo[] = "lendo...\n\r";
// strings must be lesser than 64 bytes for eeprom page write
/*
char string1[] = "<html><head><title>iSo Setup</title><style>";
char string2[] = "input{width: 250px;margin:5px 0;display:block;}</style></head>";
char string3[] = "<body><h1>Configurar WiFi</h1><form method='POST'>";
char string4[] = "Nome da rede: <input type='text' name='ssid' maxlength='32'>";
char string5[] = "Senha da rede: <input type='password' name='pass' ";
char string6[] = "maxlength='32' required><input type='hidden' name='x'>";
char string7[] = "<input type='submit' value='Enviar'></form>";
char string8[] = "<input type='submit' value='Enviar'></form>";
char string9[] = "<h6>*O caractere '=' &eacute; proibido!</h5></body></html>\r\n";
*/
// 0x0000
// IP Server
// 0x0020
//char string[] = "<html><head><title>iSo Setup</title><style>input{width: 250px;margin:5px 0;display:block;}</style></head><body><h1>Configurar Tomada</h1><form method='POST'>Nome da rede: <input type='text' name='ssid' maxlength='32'>Senha da rede: <input type='password' name='pass' maxlength='32' required>IP do Servidor: <input type='text' name='ipserv' value='192.168.0.1'><input type='hidden' name='y'><input type='submit' value='Enviar'></form><h6>*O caractere '=' &eacute; proibido!</h5></body></html>\r\n";
// 0x03E8
//char string[] = "<html><head><title>iSo</title></head><body><h1>Dados gravados!</h1><br><h3>Aguarde a luz de <i>status</i> apagar.<br> Caso contr&aacute;rio, desligue a tomada e repita o procedimento.</h3></body></html>\r\n";
// 0x07D0
//char string[] = "SABE DE NADA INOCENTE;Cl@r@123;";


//
//int pageAddr = 0x0020;
int main(void) 
{
    initClock();
    UARTMap();
    UARTInit(FCY, 9600);
    //
    // Writing EEPROM
    //
    I2CInit();
    UARTWrite(escrevendo, strlen(escrevendo));
    
    //write
    /*
    int i, temp;
    temp = pageAddr;
    for(i = 0; i < strlen(string); i++ )
    {
        I2CWriteReg(EEPROMaddr, (temp&0xFF00)>>8, (temp&0x00FF), string[i]);
        temp++;
        __delay_ms(10);
    }
    */
    //read
    
    //temp = pageAddr;
    int i, temp = 0x0001;
    for(i = 0; i < 50; i++ )
    {
        data = I2CReadReg(EEPROMaddr, (temp&0xFF00)>>8, (temp&0x00FF));
        UARTWriteChar(data);
        temp++;
        __delay_ms(10);
    }
    while(1);
    return 0;
}

void initClock(void)
{
    CLKDIVbits.PLLPRE = 2;              // N1 = 4 - 0.8-8 MHz
    PLLFBDbits.PLLDIV = 85;             // M = 87 - 100-200 MHz
    CLKDIVbits.PLLPOST = 3;             // N2 = 2 - 12.8 - 80 MHz
}

void I2CInit(void)
{
    I2C1BRG = 23;           // @400kHz; (FCY/FSCL - FCY/1e7) - 1
    I2C1CONbits.I2CEN = 0;  // Disable I2C
    I2C1CONbits.DISSLW = 1;	// Disable slew rate control
    I2C1CONbits.A10M = 0;   // 7-bit slave addr
    I2C1CONbits.SCLREL = 1; // SCL release control
    I2C1CONbits.I2CEN = 1;  // Enable I2C
    IEC1bits.MI2C1IE = 0;   // Master I2C interrupt
    IFS1bits.MI2C1IF = 0;   // MI2C Flag
}

void I2CAck(void)
{
    I2C1CONbits.ACKDT = 0;      // Send ACK
    I2C1CONbits.ACKEN = 1;      // Initiate Acknowledge and transmit ACKDT
    while(I2CCONbits.ACKEN);
}

void I2CNack(void)
{
    I2C1CONbits.ACKDT = 1;      // Send NACK
    I2C1CONbits.ACKEN = 1;      // Initiate Acknowledge and transmit ACKDT
    while(I2CCONbits.ACKEN);  
}

void I2CStop(void)
{
    I2C1CONbits.RCEN = 0;       // receive mode not in progress
    I2C1CONbits.PEN = 1;        // Stop condition
    while(I2C1CONbits.PEN);
}

void I2CStart(void)
{
    I2C1CONbits.ACKDT = 0;      // Reset any ACK
    I2C1CONbits.SEN = 1;        // Start
    while(I2C1CONbits.SEN);
}

void I2CRestart(void)
{
    I2C1CONbits.RSEN = 1;       // Repeated Start Condition
    while(I2C1CONbits.RSEN);
    I2C1CONbits.ACKDT = 0;      // Send ACK
    I2C1STATbits.TBF = 0;       // I2C1TRN is empty
}

void I2CWaitACK(void)
{
    while(I2C1STATbits.ACKSTAT);
}

void I2CIdle(void)
{
    while(I2CSTATbits.TRSTAT);
}

void I2CWrite(unsigned char c)
{
    I2C1TRN = c;
    while(I2C1STATbits.TBF);
}

void I2CRead(void)
{
    I2CCONbits.RCEN = 1;
    Nop();
    while(!I2CSTATbits.RBF);
}

// IMPORTANT: EEPROM BUFFER IS 64 BYTES ONLY!!!
void I2CWriteReg(char addr, char byteHigh, char byteLow, char value)
{
    // Start Condition
    I2CStart();
    // EEPROM Addr
    I2CWrite((addr<<1)&0xFE);
    I2CIdle();
    // Addr High Byte
    I2CWrite(byteHigh);
    I2CIdle();
    // Addr Low Byte
    I2CWrite(byteLow);
    I2CIdle();
    // Value
    I2CWrite(value);
    I2CIdle();
    // Stop
    I2CStop();
}


char I2CReadReg(char addr, char byteHigh, char byteLow)
{
    char temp;
    // Start Condition
    I2CStart();
    // EEPROM Addr
    I2CWrite((addr<<1)&0xFE);
    I2CIdle();
    // Addr High Byte
    I2CWrite(byteHigh);
    I2CIdle();
    // Addr Low Byte
    I2CWrite(byteLow);
    I2CIdle();
    // Restart
    I2CRestart();
    I2CWrite((addr<<1)|0x01);
    I2CIdle();  
    I2CRead();
    I2CNack();
    I2CStop();
    temp = I2C1RCV;
    return temp;
}

void I2CSequentialWriteReg(char addr, char byteHigh, char byteLow, char* value, int length)
{
    int j;
    // Start Condition
    I2CStart();
    // EEPROM Addr
    I2CWrite((addr<<1)&0xFE);
    I2CIdle();
    // Addr High Byte
    I2CWrite(byteHigh);
    I2CIdle();
    // Addr Low Byte
    I2CWrite(byteLow);
    I2CIdle();
    // Value
    for(j = 0; j < length; j++)
    {
        I2CWrite(value[j]);
        I2CIdle();
        __delay_ms(1);
    }
    // Stop
    I2CStop();    
}

void I2CSequentialReadReg(char addr, char byteHigh, char byteLow, char* buff, int length)
{
    int j;
    // Start Condition
    I2CStart();
    // EEPROM Addr
    I2CWrite((addr<<1)&0xFE);
    I2CIdle();
    // Addr High Byte
    I2CWrite(byteHigh);
    I2CIdle();
    // Addr Low Byte
    I2CWrite(byteLow);
    I2CIdle();
    // Restart
    I2CRestart();
    I2CWrite((addr<<1)|0x01);
    I2CIdle();  
    for(j = 0; j < length; j++)
    {
        I2CRead();
        buff[j] = I2C1RCV;  
        I2CAck();
    }
    buff[j] = '\0';
    I2CNack();
    I2CStop();  
}

void UARTmap(void){
    TRISBbits.TRISB14 = 1;    // U1RX
    TRISBbits.TRISB15 = 0;    // U1TX
    RPINR18bits.U1RXR = 14;   // U1RX - pin 25
    //RPOR7bits.RP15R = 3;      // U1TX - pin 26
    RPOR6bits.RP13R = 3; // U1TX - pin 24
}

void UARTInit(long FCYx, int BAUD)
{
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
}

void UARTWrite(char* st, int len)
{
    int i;
    for(i = 0; i < len; i++){
        UARTWriteChar(st[i]);
    }
}

void UARTWriteChar(unsigned char c)
{
    while(U1STAbits.UTXBF);
    U1TXREG = c;
    while(!U1STAbits.TRMT);
}

unsigned char UARTReadChar(void)
{
    while(!IFS0bits.U1RXIF);
    IFS0bits.U1RXIF = 0;
    if(U1STAbits.OERR){
        U1STAbits.OERR = 0;
        return 0x00;
    }
    else{
        return U1RXREG; 
    }
}