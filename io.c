#include "./header_files/io.h"

void adc_config(void)
{
    ADCON0bits.CHS      = 0b00000;      // Canal zero (AN0)
    ADCON1bits.PVCFG    = 0b00;         // VDD
    ADCON1bits.NVCFG    = 0b00;         // VSS
    ADCON2bits.ADFM     = 1;            // Justifica��o do dado (direita)
    ADCON2bits.ACQT     = 0b110;        // Tempo de aquisi��o do sinal
    ADCON2bits.ADCS     = 0b100;        // Fosc/4
    TRISAbits.RA0       = 1;
    ANSELAbits.ANSA0    = 1;
}

void adc_on(void)
{
    ADCON0bits.ADON = 1;
}

uint16_t adc_read(void)
{
    uint16_t dados_adc;
    
    ADCON0bits.GO = 1;  // Inicia a amostragem do sinal anal�gico
    
    while (ADCON0bits.GODONE);
    
    dados_adc = ADRES;
    
    return dados_adc;
}

void pwm_config(void)
{
    // PWM em RC2 / CCP1 / P1A
    TRISCbits.RC2 = 0;
    ANSELCbits.ANSC2 = 0;

    PR2 = 0xFF;

    CCPR1L = 0;
    CCP1CONbits.DC1B = 0;
    CCP1CONbits.CCP1M = 0b1100;

    T2CONbits.T2CKPS = 0b11;
    T2CONbits.TMR2ON = 1;
}

void pwm_set_duty(uint16_t duty)
{
    if (duty > 1023)
        duty = 1023;

    CCPR1L = duty >> 2;
    CCP1CONbits.DC1B = duty & 0x03;
}

void pwm_off(void)
{
    pwm_set_duty(0);
}