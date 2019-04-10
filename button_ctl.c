#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>                 // Required for the GPIO functions
#include <linux/interrupt.h>            // Required for the IRQ code
#include <linux/miscdevice.h>            // Required for the IRQ code
#include <linux/uaccess.h>            // Required for the IRQ code
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("longvh");
MODULE_DESCRIPTION("keyboard for snake ");
MODULE_VERSION("0.1");
 
static unsigned int gpioButton[4] = {44,26,46,65} ;   
// up down left right
static unsigned int irqNumber[4];

static int count;    
static int button_status = 0;    
 
static irq_handler_t  ebbgpio_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs);

static int dev_open(struct inode *, struct file *);
static int dev_close(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char __user *, size_t, loff_t *);

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = dev_open,
	.read = dev_read,
	.release = dev_close,
};

static struct miscdevice btn_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "buttons",
	.fops = &fops,
};

static int dev_open(struct inode *inodep, struct file *filep)
{
	count = 0;
	//pr_info("open file\n");
	return 0;
}

static int dev_close(struct inode *inodep, struct file *filep)
{
	return 0;
}

static ssize_t dev_read(struct file *filep, char __user *buf, size_t len,
			loff_t *offset)
{
	char temp_str[5];
	if (count)
		return 0;
	count++;
	sprintf(temp_str, "%d",button_status);
	copy_to_user(buf, temp_str, strlen(temp_str));
	return strlen(temp_str);
}
static int __init ebbgpio_init(void){
   int result = 0, i = 0;
   printk(KERN_INFO "GPIO_TEST: Initializing the GPIO_TEST LKM\n");

   for (i = 0; i< 4; i++) {
   	gpio_request(gpioButton[i], "sysfs");       
   	gpio_direction_input(gpioButton[i]);
   	gpio_set_debounce(gpioButton[i], 100);
   	gpio_export(gpioButton[i], false);
   	printk(KERN_INFO "GPIO_TEST: The button state is currently: %d\n", gpio_get_value(gpioButton[i]));
   	
   	irqNumber[i] = gpio_to_irq(gpioButton[i]);
   	printk(KERN_INFO "GPIO_TEST: The button is mapped to IRQ: %d\n", irqNumber[i]);
 
   	result = request_irq(irqNumber[i],             // The interrupt number requested
   	                     (irq_handler_t) ebbgpio_irq_handler, // The pointer to the handler function below
   	                     IRQF_TRIGGER_FALLING,   // Interrupt on rising edge (button press, not release)
   	                     "ebb_gpio_handler",    // Used in /proc/interrupts to identify the owner
   	                     NULL);                 // The *dev_id for shared interrupt lines, NULL is okay
 
   	printk(KERN_INFO "GPIO_TEST: The interrupt request result is: %d\n", result);
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
 

static void __exit ebbgpio_exit(void){
   int i = 0;
   printk(KERN_INFO "GPIO_TEST: The button state is currently: %d\n", gpio_get_value(gpioButton[i]));
   for (i =0;i < 4; i++) {
	   free_irq(irqNumber[i], NULL);             
   	   gpio_unexport(gpioButton[i]);              
   	   gpio_free(gpioButton[i]);                   
   }
   misc_deregister(&btn_dev);
   printk(KERN_INFO "GPIO_TEST: Goodbye from the LKM!\n");
}
 
static irq_handler_t ebbgpio_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs){

   printk(KERN_INFO "GPIO_TEST %d: Interrupt! \n",irq);
   if (irq == irqNumber[0]) 
	button_status = 1;
   else if (irq == irqNumber[1]) 
	button_status = 2;
   else if (irq == irqNumber[2]) 
	button_status = 3;
   else if (irq == irqNumber[3]) 
	button_status = 4;
	
   return (irq_handler_t) IRQ_HANDLED;
}

module_init(ebbgpio_init);
module_exit(ebbgpio_exit);

