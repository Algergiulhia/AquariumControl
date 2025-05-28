#ifndef UTILS_H
#define UTILS_H

#include <linux/types.h>
#include <linux/cdev.h> 

#define setbit(ptr,pos) ptr |= (1 << pos)
#define clearbit(ptr,pos) ptr &= ~(1 << pos)
#define togglebit(ptr,pos) ptr^=(1<<pos)
#define isbitset(ptr,pos) ((ptr>>pos) & 0x1)
#define isbitclear(ptr,pos) !((ptr>>pos) & 0x1)

#define BCM2711_PERI_BASE   0xfe000000
#define GPIO_BASE   (BCM2711_PERI_BASE + 0x200000)
#define PWM_BASE    (BCM2711_PERI_BASE + 0x20C000)  /**< PWM0*/
#define CLK_BASE    (BCM2711_PERI_BASE + 0x101000)
#define BCM2711_PWMCLK_CNTL 40
#define BCM2711_PWMCLK_DIV  41
#define BCM2711_PWM_CLOCK_DIVIDER_16 16

/* Defines for PWM Clock, word offsets (ie 4 byte multiples) */
#define BCM2711_PWMCLK_CNTL     40
#define BCM2711_PWMCLK_DIV      41
#define BCM2711_PWM_PASSWRD     (0x5A << 24)  
#define BCM2711_PWM_CNTL_KILL 5
#define BCM2711_PWM_CNTL_ENAB 4
#define BCM2711_PWM_CNTL_SRC  0
#define BCM2711_PWM_DIV_DIVI  12
#define CLK_SRC_OSCILLATOR 1

#define GPIO12 12

struct GpioRegisters
{
	uint32_t GPFSEL[6];
	uint32_t Reserved1;
	uint32_t GPSET[2];
	uint32_t Reserved2;
	uint32_t GPCLR[2];
};

typedef enum {
    GPIO_INPUT      = 0b000,									
    GPIO_OUTPUT     = 0b001,								
    GPIO_ALTFUNC5   = 0b010,								
    GPIO_ALTFUNC4   = 0b011,								
    GPIO_ALTFUNC0   = 0b100,								
    GPIO_ALTFUNC1   = 0b101,								
    GPIO_ALTFUNC2   = 0b110,								
    GPIO_ALTFUNC3   = 0b111,								
} GPIOMODE;


#define PWEN1   0 /**< Channel 1 enable*/
#define MODE1   1 /**< Channel 1 mode*/
#define RPTL1   2 /**< Channel 1 Repeat Last Data*/
#define SBIT1   3 /**< Channel 1 Silence Bit*/
#define POLA1   4 /**< Channel 1 Polarity*/
#define USEF1   5 /**< Channel 1 Use FIFO*/
#define CLRF    6 /**< Clear FIFO*/
#define MSEN1   7 /**< Channel 1 M/S Enable*/

#define STA1 9


extern volatile unsigned int PWM_CTL;
extern volatile unsigned int PWM_STA;
extern volatile unsigned int PWM_RNG1;
extern volatile unsigned int PWM_DAT1;
extern volatile unsigned int PWM_CLK_RNG1;
extern volatile unsigned int PWM_DIV_DAT1;


static struct class* pwmDevice_class = NULL;
static struct device* pwmDevice_device = NULL;
static dev_t pwmDevice_majorminor;
static struct cdev c_dev;  // Character device structure

static struct class *s_pDeviceClass;
static struct device *s_pDeviceObject;
extern struct GpioRegisters *s_pGpioRegisters;
extern volatile unsigned int *s_pPwmCtlRegisters;
extern volatile unsigned int *s_pPwmStaRegisters;
extern volatile unsigned int *s_pPwmRng1Registers;
extern volatile unsigned int *s_pPwmDat1Registers;
extern volatile unsigned int *s_pPwmClkRegisters;
extern volatile unsigned int *s_pPwmClkCtlRegisters;
extern volatile unsigned int *s_pPwmClkDivRegisters;

void SetGPIOFunction(struct GpioRegisters *s_pGpioRegisters, int GPIO, int functionCode);
void SetPwmDivisor(unsigned int div);
void ConfigPwmChannel(void);
void ConfigPwmSignal(unsigned int freq, unsigned int duty);

#endif