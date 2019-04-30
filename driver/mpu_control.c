#include "mpu6050.h"
#include <linux/init.h>
#include <linux/module.h> 
#include <linux/kernel.h>  
#include <linux/kdev_t.h>  // for macro MAJOR MINOR 
#include <linux/fs.h>      // for function register major , minor
#include <linux/types.h>   // for dev_t type
#include <linux/device.h>   // for dev_t type
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

static dev_t first;
static struct cdev *c_dev;
static struct class *cl;
static struct i2c_client *client;


static int mpu_i2c_probe(struct i2c_client *i2c_dev, const struct i2c_device_id *i2c_dev_id) {
	int ret = 0;
	struct mpu_xyz mpu;
	pr_info("jump to probe\n");

	client = i2c_dev;
	
	pr_info("address of device : %d\n", i2c_dev->addr);
	pr_info("adapter bus       : %s\n", i2c_dev->adapter->name);
	pr_info("get address of client %d\n", mpu_read_value(i2c_dev, 0x75));
	
	ret = mpu_init(i2c_dev, MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G) ;
	if (ret < 0) {
		pr_info("mpu_init fail\n");
		return -1;
	}
	
	ret =  read_raw_accel(i2c_dev, &mpu);
	if (ret < 0) {
		pr_info("error read raw_accel\n");
		return -1;
	}
	pr_info("get mpu x :%d\ny : %d\nz : %d\n",mpu.x_asis, mpu.y_asis, mpu.z_asis);

	return 0;
} 
static int mpu_i2c_remove(struct i2c_client *i2c_dev) {
	pr_info("remove driver\n");
	return 0;
}
static const struct of_device_id test_id[] = {
	{ .compatible ="hehe,longvh",  },
	{},
};

static struct i2c_driver mpu_i2c_driver = {
	.driver = {
		.owner = THIS_MODULE,
		.name  = "mpu_driver",
		.of_match_table = test_id,
	},	
	.probe    = mpu_i2c_probe,
	.remove   = mpu_i2c_remove,
};

static int my_open(struct inode *i , struct file *f)
{
	return 0;
}
static ssize_t my_read(struct file  *f , char __user *buf ,size_t len ,  loff_t *off )
{
	int ret = 0;
	struct mpu_xyz mpu;

	ret =  read_raw_accel(client, &mpu);
	if (ret < 0) {
		pr_info("error read raw_accel\n");
	}
	
	ret = copy_to_user(buf, &mpu, sizeof(struct mpu_xyz));
	if (ret < 0) {
		pr_info("copy to user error\n");
	}
	return sizeof(mpu);
}

static ssize_t my_write(struct file  *f , const char __user *buf ,size_t len ,  loff_t *off )
{
	printk(KERN_ALERT "Device : my_write()\n");
	return len;
}
static int my_close(struct inode *i , struct file *f)
{
	return 0;
}
static struct file_operations fops = 
{
		.owner = THIS_MODULE ,
		.read  = my_read,
		.write  = my_write,
		.open  = my_open,
		.release  = my_close

};

static int __init hello_init(void)
{
	printk(KERN_EMERG "Device driver file example \n");
	if( alloc_chrdev_region(&first , 0 , 1 , "my_ddfile") < 0)
	{
		goto chrdev_fail;
	}

	if( ( cl = class_create(THIS_MODULE , "my_char_driver_cls")) == NULL)
	{
		goto cre_class_fail;
	}
 	if( device_create(cl , NULL , MKDEV(MAJOR(first)  , MINOR(first)) , NULL , "mpu6050") == NULL)
	{
		goto device_fail;
	}
	c_dev = cdev_alloc();
	cdev_init(c_dev ,&fops); 
	if( (cdev_add(c_dev , first , 1))  == -1)
	{
		goto cdev_fail;
	}
	printk(KERN_EMERG "<major , minor > : <%d ,%d>\n",MAJOR(first) , MINOR(first));

	return i2c_add_driver(&mpu_i2c_driver);

	cdev_fail : device_destroy(cl , first);	
	device_fail : class_destroy(cl); 
	cre_class_fail:unregister_chrdev_region(first , 1);
	chrdev_fail: return -1;
}

static void __exit hello_exit(void)
{
	printk(KERN_EMERG "Goodbye\n");
	device_destroy(cl , first);		
	class_destroy(cl);
	cdev_del(c_dev);
	unregister_chrdev_region(first , 1);
	i2c_del_driver(&mpu_i2c_driver);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Longvh");
