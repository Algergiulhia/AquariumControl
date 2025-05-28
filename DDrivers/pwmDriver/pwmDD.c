#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/init.h>
#include <asm/io.h>
#include <linux/timer.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/mm.h>
#include <linux/io.h>
#include <linux/types.h>
#include "utils.h"

#define DEVICE_NAME "pwmDD"
#define CLASS_NAME "pwmClass"

MODULE_LICENSE("GPL");


ssize_t pwm_device_write(struct file *pfile, const char __user *pbuff, size_t len, loff_t *off) { //tirar o static
	struct GpioRegisters *pdev; 
	unsigned int freq = 0;
	unsigned int duty = 0;

	pr_alert("%s: called (%u)\n",__FUNCTION__,len);
	
	if(unlikely(pfile->private_data == NULL))
		return -EFAULT;

	pdev = (struct GpioRegisters *)pfile->private_data;
	
	freq = (pbuff[0] - '0') * 10000 + (pbuff[1] - '0') * 1000 + (pbuff[2] - '0') * 100 + (pbuff[3] - '0') * 10 + (pbuff[4] - '0');
	duty = (pbuff[5] - '0') * 100 + (pbuff[6] - '0') * 10 + (pbuff[7] - '0');

	ConfigPwmSignal(freq,duty);

	return len;
}

ssize_t pwm_device_read(struct file *pfile, char __user *p_buff,size_t len, loff_t *poffset){
	pr_alert("%s: called (%u)\n",__FUNCTION__,len);
	return 0;
}

int pwm_device_close(struct inode *p_inode, struct file * pfile){
	
	pr_alert("%s: called\n",__FUNCTION__);
	pfile->private_data = NULL;
	return 0;
}


int pwm_device_open(struct inode* p_indode, struct file *p_file){

	pr_alert("%s: called\n",__FUNCTION__);
	p_file->private_data = (struct GpioRegisters *) s_pGpioRegisters;
	return 0;
	
}


static struct file_operations pwmDevice_fops = {
	.owner = THIS_MODULE,
	.write = pwm_device_write,
	.read = pwm_device_read,
	.release = pwm_device_close,
	.open = pwm_device_open,
};

static int __init pwmModule_init(void) {
	int ret;
	struct device *dev_ret;

	printk("init started");

	pr_alert("%s: called\n",__FUNCTION__);

	if ((ret = alloc_chrdev_region(&pwmDevice_majorminor, 0, 1, DEVICE_NAME)) < 0) {
		return ret;
	}

	if (IS_ERR(pwmDevice_class = class_create(THIS_MODULE, CLASS_NAME))) {
		unregister_chrdev_region(pwmDevice_majorminor, 1);
		return PTR_ERR(pwmDevice_class);
	}
	if (IS_ERR(dev_ret = device_create(pwmDevice_class, NULL, pwmDevice_majorminor, NULL, DEVICE_NAME))) {
		class_destroy(pwmDevice_class);
		unregister_chrdev_region(pwmDevice_majorminor, 1);
		return PTR_ERR(dev_ret);
	}

	cdev_init(&c_dev, &pwmDevice_fops);
	c_dev.owner = THIS_MODULE;
	if ((ret = cdev_add(&c_dev, pwmDevice_majorminor, 1)) < 0) {
		printk(KERN_NOTICE "Error %d adding device", ret);
		device_destroy(pwmDevice_class, pwmDevice_majorminor);
		class_destroy(pwmDevice_class);
		unregister_chrdev_region(pwmDevice_majorminor, 1);
		return ret;
	}

	s_pGpioRegisters = (struct GpioRegisters *)ioremap(GPIO_BASE, sizeof(struct GpioRegisters));
	pr_alert("map to virtual adresse: 0x%x\n", (unsigned)s_pGpioRegisters);
	s_pPwmCtlRegisters = (volatile unsigned int *)ioremap(PWM_CTL, sizeof(unsigned int));
	pr_alert("map to virtual adresse: 0x%x\n", (unsigned)s_pPwmCtlRegisters);
	s_pPwmStaRegisters = (volatile unsigned int *)ioremap(PWM_STA, sizeof(unsigned int));
	pr_alert("map to virtual adresse: 0x%x\n", (unsigned)s_pPwmStaRegisters);
	s_pPwmRng1Registers = (volatile unsigned int *)ioremap(PWM_RNG1, sizeof(unsigned int));
	pr_alert("map to virtual adresse: 0x%x\n", (unsigned)s_pPwmRng1Registers);
	s_pPwmDat1Registers = (volatile unsigned int *)ioremap(PWM_DAT1, sizeof(unsigned int));
	pr_alert("map to virtual adresse: 0x%x\n", (unsigned)s_pPwmDat1Registers);
	s_pPwmClkRegisters = (volatile unsigned int *)ioremap(CLK_BASE, sizeof(unsigned int));
	pr_alert("map to virtual adresse: 0x%x\n", (unsigned)s_pPwmClkRegisters);

	s_pPwmClkCtlRegisters = (volatile unsigned int *)ioremap(PWM_CLK_RNG1, sizeof(unsigned int));
	pr_alert("map to virtual adresse: 0x%x\n", (unsigned)s_pPwmDat1Registers);
	s_pPwmClkDivRegisters = (volatile unsigned int *)ioremap(PWM_DIV_DAT1, sizeof(unsigned int));
	pr_alert("map to virtual adresse: 0x%x\n", (unsigned)s_pPwmClkRegisters);

	SetGPIOFunction(s_pGpioRegisters, GPIO12, GPIO_ALTFUNC0);

    ConfigPwmChannel();
    SetPwmDivisor(BCM2711_PWM_CLOCK_DIVIDER_16); //CLK_DIV
    ConfigPwmSignal(50, 50);

	printk("init ended");

	return 0;
}

static void __exit pwmModule_exit(void) {
	
	pr_alert("%s: called\n",__FUNCTION__);
	
	SetGPIOFunction(s_pGpioRegisters, GPIO12, GPIO_INPUT);
	*(s_pPwmClkCtlRegisters) = 0x5A000020; /**< Kill the clock*/ 
	*(s_pPwmClkDivRegisters) = 0x5A000000;
	clearbit(*s_pPwmCtlRegisters, PWEN1);	/**< Disable PWM transmission*/ 

	iounmap(s_pGpioRegisters);
	iounmap(s_pPwmCtlRegisters);
	iounmap(s_pPwmStaRegisters);
	iounmap(s_pPwmRng1Registers);
	iounmap(s_pPwmDat1Registers);
	iounmap(s_pPwmClkRegisters);
	cdev_del(&c_dev);
	device_destroy(pwmDevice_class, pwmDevice_majorminor);
	class_destroy(pwmDevice_class);
	unregister_chrdev_region(pwmDevice_majorminor, 1);
}

module_init(pwmModule_init);
module_exit(pwmModule_exit);
