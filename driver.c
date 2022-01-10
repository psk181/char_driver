#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/slab.h>

#include "ioctl.h"

static dev_t first; // Global variable for the first device number
static struct cdev c_dev; // Global variable for the character device structure
static struct class *cl; // Global variable for the device class

static int my_open(struct inode *i, struct file *f)
{
    printk(KERN_INFO "Driver: open()\n");
    return 0;
}
static int my_close(struct inode *i, struct file *f)
{
    printk(KERN_INFO "Driver: close()\n");
    return 0;
}
static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Driver: read()\n");
    return 0;
}
static ssize_t my_write(struct file *f, const char __user *buf, size_t len,
    loff_t *off)
{
    printk(KERN_INFO "Driver: write()\n");
    return len;
}

static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {

	struct data_read kcs_data, k_buff;
	int i = 0;

	switch(cmd){
		case DATA_READ:
			printk("IOCTL : Data Read\n");

			if(copy_from_user(&kcs_data,(void*) arg,sizeof(struct data_read)))
				return -EFAULT;

			printk("Data Len %d\n", kcs_data.len);
			
			k_buff.buffer = (char *)kmalloc(kcs_data.len * sizeof(char), GFP_KERNEL);
			//copy some data to kernel buffer
			
				for(i = 0; i < kcs_data.len; i++)
					k_buff.buffer[i] = 'a';
				
				for(i = 0; i < kcs_data.len; i++)
					printk("Kernel Data : %c\n", k_buff.buffer[i]);

				i = copy_to_user((void*) arg,&k_buff, sizeof(struct data_read));
				printk("End Switch case : %d\n", i);
				break;

		default:
			printk("NO COMMAND FOUND \n");
			break;
		}

	//kfree(k_buff.buffer);
	return i;
}


static struct file_operations pugs_fops =
{
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_close,
    .read = my_read,
    .write = my_write,
    .unlocked_ioctl = my_ioctl
};

static int __init ofcd_init(void) /* Constructor */
{
    int ret;
    struct device *dev_ret;

    printk(KERN_INFO "Namaskar: ofcd registered");

  if ((ret = alloc_chrdev_region(&first, 0, 1, "customdevice")) < 0)
    {
        return ret;
    }
    if (IS_ERR(cl = class_create(THIS_MODULE, "customdevice")))
    {
        unregister_chrdev_region(first, 1);
        return PTR_ERR(cl);
    }
    if (IS_ERR(dev_ret = device_create(cl, NULL, first, NULL, "customdevice")))
    {
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return PTR_ERR(dev_ret);
    }

    cdev_init(&c_dev, &pugs_fops);
    if ((ret = cdev_add(&c_dev, first, 1)) < 0)
    {
        device_destroy(cl, first);
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return ret;
    }

	printk("Registed !!!!!!!\n");
    return 0;
}

static void __exit ofcd_exit(void) /* Destructor */
{
    cdev_del(&c_dev);
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    printk(KERN_INFO "Alvida: ofcd unregistered");
}

module_init(ofcd_init);
module_exit(ofcd_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Our First Character Driver");
