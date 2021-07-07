
/*
** @This file based on button.c of Derek Molloy and has modified
** @to control 4 buttons
** @Link: https://github.com/sdwuyawen/BeagleBoneBlack/blob/master/button/button.c
** @Author: vuhailongkl97@gmail.com
** 	    Tungnt58 (Add send signal to userspace functionality)
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

#define MY_SIGNAL		40

MODULE_LICENSE("GPL");
MODULE_AUTHOR("vuhailongkl97@gmail.com");
MODULE_DESCRIPTION("keyboard for snake ");
MODULE_VERSION("0.1");

static unsigned int gpioButton[4] = {44, 26, 46, 65};
//static unsigned int gpioButton[4] = {67,68,44, 26};
/* up down left right */
static unsigned int irqNumber[4];

static int button_status = 0;
static struct task_struct *t;

static irq_handler_t  button_irq_handler(unsigned int irq, void *dev_id,
						 struct pt_regs *regs);

static int dev_open(struct inode *, struct file *);
static int dev_close(struct inode *, struct file *);

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = dev_open,
	.release = dev_close,
};

static struct miscdevice btn_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "buttons",
	.fops = &fops,
};

/* This function to get user process id */
static int dev_open(struct inode *inodep, struct file *filep)
{
	struct pid *current_pid;
	pid_t user_pid;

	current_pid = get_task_pid(current, PIDTYPE_PID);
	t = pid_task(current_pid, PIDTYPE_PID);
	user_pid = pid_nr(current_pid);
	pr_info("user process id is %d\n", user_pid);

	return 0;
}

static int dev_close(struct inode *inodep, struct file *filep)
{
	/* Do Nothing */
	return 0;
}


/* Old way for polling on userspace*/
/*static ssize_t dev_read(struct file *filep, char __user *buf, size_t len,
			loff_t *offset)
{
	char temp_str[5];
	if (count)
		return 0;
	count++;
	sprintf(temp_str, "%d", button_status);
	copy_to_user(buf, temp_str, strlen(temp_str));
	return strlen(temp_str);
}*/

static int __init button_init(void)
{
	int result = 0, i = 0;
	pr_info("GPIO_TEST: Initializing the GPIO_TEST LKM\n");

	for (i = 0; i< 4; i++) {
		gpio_request(gpioButton[i], "sysfs");
		gpio_direction_input(gpioButton[i]);
		gpio_set_debounce(gpioButton[i], 100);
		gpio_export(gpioButton[i], false);

		pr_info("GPIO_TEST: The button state is currently: %d\n",
				gpio_get_value(gpioButton[i]));

		irqNumber[i] = gpio_to_irq(gpioButton[i]);
		pr_info("GPIO_TEST: The button is mapped to IRQ: %d\n",
							irqNumber[i]); 

		result = request_irq(irqNumber[i],
				(irq_handler_t) button_irq_handler,
				IRQF_TRIGGER_RISING,
				"ebb_gpio_handler",
				NULL);
 
		pr_info("GPIO_TEST: The interrupt request result is: %d\n",
							result);
		if (result != 0) {
			pr_info("error request irq\n");
			return -1;
		}
	}
	result = misc_register(&btn_dev);
	if (result) {
		pr_info("can't not register device\n");
		return result;
	}
	pr_info("register successfully\n");

	return 0;
}
 

static void __exit button_exit(void)
{
	int i = 0;

	pr_info("GPIO_TEST: The button state is currently: %d\n",
		gpio_get_value(gpioButton[i]));
	for (i = 0; i < 4; i++) {
		free_irq(irqNumber[i], NULL);             
		gpio_unexport(gpioButton[i]);              
		gpio_free(gpioButton[i]);                   
	}

	misc_deregister(&btn_dev);
	pr_info("GPIO_TEST: Goodbye from the LKM!\n");
}

/* Send signale to userspace via user process id */
static void send_sig_to_user(int sig_val)
{
	struct siginfo info;

	info.si_signo = MY_SIGNAL;
	info.si_code = SI_QUEUE;
	info.si_int = sig_val;

	if (t != NULL) {
		if (send_sig_info(MY_SIGNAL, &info, t) < 0)
			pr_err("send signal failed\n");
	} else {
		pr_err("pid_task error\n");
	}
}

static irq_handler_t button_irq_handler(unsigned int irq, void *dev_id,
					struct pt_regs *regs)
{
	pr_info("GPIO_TEST %d: Interrupt! \n", irq);

	pr_info("button %d", button_status);
	if (irq == irqNumber[0]) {
		button_status = 1;
		send_sig_to_user(button_status);
	} else if (irq == irqNumber[1]) {
		button_status = 2;
		send_sig_to_user(button_status);
	} else if (irq == irqNumber[2]) {
		button_status = 3;
		send_sig_to_user(button_status);
	} else if (irq == irqNumber[3]) {
		button_status = 4;
		send_sig_to_user(button_status);
	}
	
	return (irq_handler_t) IRQ_HANDLED;
}

module_init(button_init);
module_exit(button_exit);

