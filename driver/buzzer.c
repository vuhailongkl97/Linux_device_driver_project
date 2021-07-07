#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("vuhailongkl97@gmail.com");
MODULE_DESCRIPTION("buzzer score");
MODULE_VERSION("0.1");

#define BUZZER_PIN 49

static int dev_open(struct inode *, struct file *);
static int dev_close(struct inode *, struct file *);

static ssize_t dev_write(struct file *fp, const char *buf, size_t len, loff_t *pos) {
	
	char cmd;
	pr_info("my misc write\n");
	copy_from_user(&cmd, buf, 1);	

	if (cmd == '1'){
		pr_info("turn on buzzer\n");
		gpio_set_value(BUZZER_PIN, 1);
	}
	else if (cmd == '0') {
		pr_info("turn off buzzer\n");
		gpio_set_value(BUZZER_PIN, 0);
	}
	else {
		pr_err("cmd incorrect\n");
	}
	return len;
}

static ssize_t dev_read(struct file *fp, char *buf, size_t len, loff_t *pos) {

	pr_info("my misc read\n");
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = dev_open,
	.release = dev_close,
	.read = dev_read,
	.write = dev_write,
};
static struct miscdevice buzzer_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "buzzer",
	.fops = &fops,
};

static int dev_open(struct inode *inodep, struct file *filep)
{
	pr_info("dev open\n");

	return 0;
}

static int dev_close(struct inode *inodep, struct file *filep)
{
	pr_info("dev close\n");
	
	return 0;
}
static int __init button_init(void)
{
	int result;

	pr_info("GPIO_TEST: Initializing the GPIO_TEST LKM\n");
	if ( !gpio_is_valid(BUZZER_PIN)) {
		pr_err("gpio is not valid\n");
		return -ENODEV;
	}

	gpio_request(BUZZER_PIN, "sysfs");
	gpio_direction_output(BUZZER_PIN, 0 );
	gpio_export(BUZZER_PIN, false);

	result = misc_register(&buzzer_dev);
	if (result) {
		pr_info("can't not register device\n");
		return result;
	}
	pr_info("register successfully\n");

	return 0;
}
 

static void __exit button_exit(void)
{

	gpio_unexport(BUZZER_PIN);              
	gpio_free(BUZZER_PIN);                   
	misc_deregister(&buzzer_dev);

	pr_info("GPIO_TEST: Goodbye from the LKM!\n");
}

module_init(button_init);
module_exit(button_exit);

