///////////////////////////////////////////////////////////////////////////
// Complementary PWM  - 20khz out on pin C0 and C1
// Analog input on A0 determines duty
// Remove while loop for a fixed 50%
///////////////////////////////////////////////////////////////////////////
#include <16f1789.h>
#use delay(osc=20mhz)


#define us(time)  (int16)(time*(getenv("CLOCK")/1000000))


void main(void)  {

    setup_psmc(1, PSMC_SINGLE | PSMC_COMPLEMENTARY,
               PSMC_EVENT_TIME | PSMC_SOURCE_FOSC, us(50),
               PSMC_EVENT_TIME, 0,
               PSMC_EVENT_TIME, us(25));
    
    psmc_pins(1, PSMC_A | PSMC_B);
    
    setup_adc(ADC_CLOCK_INTERNAL);
    setup_adc_ports(sAN0);
    set_adc_channel(0);
    
    while(TRUE) {
       psmc_duty(1, us(((read_adc()*(int16)5)/25)) );  
    }


}

