///////////////////////////////////////////////////////////////////////////
// Simple PWM  - 10khz out on pin C0
// Analog input on A0 determines duty
// Remove while loop for a fixed 25%
///////////////////////////////////////////////////////////////////////////

#include <16f1789.h>
#use delay(osc=20mhz)


#define us(time)  (int16)(time*(getenv("CLOCK")/1000000))


void main(void)  {

    setup_psmc(1, PSMC_SINGLE,
               PSMC_EVENT_TIME | PSMC_SOURCE_FOSC, us(100),
               PSMC_EVENT_TIME, 0,
               PSMC_EVENT_TIME, us(25));
    
    psmc_pins(1, PSMC_A);
    
    setup_adc(ADC_CLOCK_INTERNAL);
    setup_adc_ports(sAN0);
    set_adc_channel(0);
    
    while(TRUE) {
       psmc_duty(1, us(((read_adc()*(int16)10)/25)) );  
    }

}

