#include "ferrum/lib/netif.h"

#include <linux/init.h>

static int __init netif_init(void)
{
	int result = 0;
	printk(KERN_INFO "Trying to register char device region\n");
	dev_t dev = 0;
	// Выделяем регион для символьного устройства
	result = alloc_chrdev_region(&dev, DEVICE_FIRST, DEVICE_COUNT, DGROUP_NAME);
	major = MAJOR(dev);
	
	if ( result < 0 )
	{
		printk(KERN_INFO "Can not register char device region\n");
		goto err;
	}

	printk(KERN_INFO "Char device region created: %d:%d...%d\n", major, DEVICE_FIRST, DEVICE_COUNT);

	cdev_init(&netif_dev, &netif_fops);
	netif_dev.owner = THIS_MODULE;

	result = cdev_add(&netif_dev, dev, DEVICE_COUNT);
	if ( result < 0 )
	{
		unregister_chrdev_region(MKDEV(major, DEVICE_FIRST), DEVICE_COUNT);
		printk(KERN_INFO "Cannot add char device\n");
		goto err;
	}

	printk(KERN_INFO "Char device added\n");


err:
	return result;
}

static void __exit netif_exit(void)
{
	cdev_del(&netif_dev);
	unregister_chrdev_region(MKDEV(major, DEVICE_FIRST), DEVICE_COUNT);
	printk(KERN_INFO "Char device region %d:%d...%d destroyed\n", major, DEVICE_FIRST, DEVICE_COUNT);
}
module_init(netif_init);
module_exit(netif_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("netif_module");
