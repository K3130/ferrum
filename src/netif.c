#include "ferrum/netif.h"

int major = 0;
struct cdev netif_dev;
static int device_open = 0;

int netif_open(struct inode* n, struct file* f)
{
	if ( device_open )
	{
		return -EBUSY;
	}
	device_open++;
	return EOK;
}

int netif_release(struct inode* n, struct file* f)
{
	device_open--;
	return EOK;
}

ssize_t netif_read(struct file* f, char __user* buf, size_t count, loff_t* ppos)
{
	const char* reply = "Test answer.\n";
	size_t len = strlen(reply);
	if( *ppos >= len )
	{
		return 0;
	}
	if ( count > len - *ppos )
	{
		count = len - *ppos;
	}
	if ( copy_to_user(buf, reply, len) )
	{
		return -EINVAL;
	}
	*ppos += count;
	return count;
}

const struct file_operations netif_fops = {
	.owner 	 = THIS_MODULE,
	.open 	 = netif_open,
	.release = netif_release,
	.read    = netif_read,
};



