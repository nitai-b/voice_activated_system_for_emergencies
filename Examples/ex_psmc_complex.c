///////////////////////////////////////////////////////////////////////////
// 10khz out on pin C0,C1,C2,C3
// Signal is delayed 10us into period
// 2us deadband on rise and 4us deadband on fall
// Active only when B0 is high
// Analog input on A0 determines duty
// Remove while loop for a fixed 25%
///////////////////////////////////////////////////////////////////////////

#include <16f1789.h>
#use delay(osc=20mhz)


#define us(time)  (int16)(time*(getenv("CLOCK")/1000000))


void main(void)  {


    setup_psmc(1, PSMC_ECCP_BRIDGE_FORWARD,
               PSMC_EVENT_TIME | PSMC_SOURCE_FOSC | PSMC_DIV_2, us(100),
               PSMC_EVENT_TIME, us(10),
               PSMC_EVENT_TIME, us(35));
               
    psmc_deadband(1, us(2), us(4));
    psmc_modulation(1, PSMC_MOD_IN_PIN);
    psmc_pins(1, PSMC_A | PSMC_B | PSMC_C | PSMC_D);
    
    setup_adc(ADC_CLOCK_INTERNAL);
    setup_adc_ports(sAN0);
    set_adc_channel(0);
    
    while(TRUE) {
       psmc_duty(1, us(((read_adc()*(int16)10)/25)) );  
    }

}

