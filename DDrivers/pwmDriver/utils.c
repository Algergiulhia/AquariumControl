#include "utils.h"
#include <linux/module.h>


struct GpioRegisters *s_pGpioRegisters;
volatile unsigned int * s_pPwmCtlRegisters;
volatile unsigned int * s_pPwmStaRegisters;
volatile unsigned int * s_pPwmRng1Registers;
volatile unsigned int * s_pPwmDat1Registers;
volatile unsigned int * s_pPwmClkRegisters;
volatile unsigned int * s_pPwmClkCtlRegisters;
volatile unsigned int * s_pPwmClkDivRegisters;

volatile unsigned int PWM_CTL = PWM_BASE + 0x00;
volatile unsigned int PWM_STA = PWM_BASE + 0x04;
volatile unsigned int PWM_RNG1 = PWM_BASE + 0x10;
volatile unsigned int PWM_DAT1 = PWM_BASE + 0x14;
volatile unsigned int PWM_CLK_RNG1 = CLK_BASE + 0x70;
volatile unsigned int PWM_DIV_DAT1 = CLK_BASE + 0x74;

void SetGPIOFunction(struct GpioRegisters *s_pGpioRegisters, int GPIO, int functionCode) {
	int registerIndex = GPIO / 10;
	int bit = (GPIO % 10) * 3;

	unsigned oldValue = s_pGpioRegisters->GPFSEL[registerIndex];
	unsigned mask = 0b111 << bit;

	pr_alert("%s: register index is %d\n",__FUNCTION__,registerIndex);
	pr_alert("%s: mask is 0x%x\n",__FUNCTION__,mask);
	pr_alert("%s: update value is 0x%x\n",__FUNCTION__,((functionCode << bit) & mask));
	
	s_pGpioRegisters->GPFSEL[registerIndex] = (oldValue & ~mask) | ((functionCode << bit) & mask);
}

void SetPwmDivisor(unsigned int div)
{	
	*(s_pPwmClkRegisters + BCM2711_PWMCLK_CNTL) = (BCM2711_PWM_PASSWRD | 1 << BCM2711_PWM_CNTL_KILL); /**< Kill the clock*/
	
	clearbit(*s_pPwmCtlRegisters, PWEN1);	/**< Disable PWM transmission*/ 
	
	if ( div < 1 ) {
		div = 1;				/**< Lowest divisor */
	} else if ( div >= 0x1000 ) {
		div = 0xFFF;			/**< Highest divisor */
	}
	
	*(s_pPwmClkRegisters + BCM2711_PWMCLK_DIV) = (BCM2711_PWM_PASSWRD | (div << BCM2711_PWM_DIV_DIVI));  /**< Configure the Divisor */ 
	*(s_pPwmClkRegisters + BCM2711_PWMCLK_CNTL) = (BCM2711_PWM_PASSWRD | (1 << BCM2711_PWM_CNTL_ENAB) | 
		(CLK_SRC_OSCILLATOR << BCM2711_PWM_CNTL_SRC)); /**< bits[3:0]: 1-Source oscillator, bit[4]: Enable Clock Generator*/
	setbit(*s_pPwmCtlRegisters, PWEN1);

	printk("Configure PWM frequency clk");
}

void ConfigPwmChannel(){
	*s_pPwmCtlRegisters = 0;
    setbit(*s_pPwmCtlRegisters, MSEN1);
    setbit(*s_pPwmCtlRegisters, CLRF);
	printk("Configure PWM channels");
}

void ConfigPwmSignal(unsigned int freq, unsigned int duty){

	unsigned int s, m;

	if (freq > 20000)	
		freq = 20000;
	if (freq < 10)
		freq = 10;

	s = (54000000 / (freq * 16)); 	/**< freq = oscillator / (divisor * s); */

	m = s*duty/100;		/**< m/s = duty cycle */

	clearbit(*s_pPwmCtlRegisters, PWEN1);	/**< Disable PWM transmission*/ 
	*(s_pPwmRng1Registers) = s;
	*(s_pPwmDat1Registers) = m;
	if(*(s_pPwmDat1Registers + STA1) == 0){
		printk("channel1 Disable");
	}
	setbit(*s_pPwmCtlRegisters, PWEN1);		/**< Enable PWM transmission*/ 
	printk("Configure PWM Signal form (m/n)");
}