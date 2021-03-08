/*
 * File:   I2C_Adafruit.c
 * Author: santi
 *
 * Created on March 5, 2021, 6:09 PM
 */


#include <xc.h>
#include <stdint.h>
#include <pic16f887.h>
#include "I2C.h"
#include "EUSART.h"
//******************************************************************************
//Configuration Bits
//******************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_CLKOUT       // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

//******************************************************************************
//Variables
//******************************************************************************
#define _XTAL_FREQ 8000000
#define S_Add_W 0b11010000 
#define S_Add_R 0b11010001

uint8_t s;
uint8_t h;
uint8_t m;


//******************************************************************************
//Prototipos de Funciones
//******************************************************************************
void setup(void);
void Get_time(void);
void RTC_conf(void);
void Start(void);
void Stop(void);
//******************************************************************************
//Loop Principal
//******************************************************************************

void main(void) {
    setup();
    while (1) {
        Get_time();
        SendString("Segundos: ");
        SendChar(s+48);
        SendChar(0x0D);
        SendString("Minutos: ");
        SendChar(m+48);
        SendChar(0x0D);
        SendString("Horas: ");
        SendChar(h+48);
        SendChar(0x0D);

        PORTD = s;
    }
}


//******************************************************************************
//Setup
//******************************************************************************

void setup(void) {
    ANSEL = 0;
    ANSELH = 0;

    TRISA = 0;
    TRISB = 0;
    // TRISC = 0;
    TRISD = 0;

    PORTA = 0;
    PORTB = 0;
    ///  PORTC = 0;
    PORTD = 0;
    EUSART_conf();
    I2C_Master_Init(100000);
    RTC_conf();

}

//******************************************************************************
// Otras Funciones
//******************************************************************************

void RTC_conf(void) {
    I2C_Master_Start();
    I2C_Master_Write(S_Add_W);
    I2C_Master_Write(0x07);
    // I2C_Master_RepeatedStart();
    I2C_Master_Write(0x13);
    I2C_Master_Stop();
}

void Get_time(void) {
    //Start();
    I2C_Master_Start();
    I2C_Master_Write(S_Add_W); //Le mando el byte de direccion del RTC en modo Write
    I2C_Master_Write(0x00); /* Le envio la direccion a la cual deseo apuntar, que 
                               en este caso es la 0 ya que ahi se guarda el dato de 
                               los segundos segun la datasheet*/

    I2C_Master_RepeatedStart();
    I2C_Master_Write(S_Add_R); //Le mando el byte de direccion del RTC en modo Read para leer los datos

    s = I2C_Master_Read(1);
    s &= 0b01111111;
    m = I2C_Master_Read(1);
    m &= 0b01111111;
    h = I2C_Master_Read(0);
    h &= 0b01111111;
    I2C_Master_Stop();
    __delay_ms(200);
}

/*00100000
  00100010
  00100011
  00100100
  00100110
  00100111
  00101000
  00111001
 */ 