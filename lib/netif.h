#pragma once

#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/uaccess.h>

#define DEVICE_FIRST 0
#define DEVICE_COUNT 3
#define DGROUP_NAME "ferrum_netif"
#define EOK 0

int netif_open(struct inode* n, struct file* f);
int netif_release(struct inode* n, struct file* f);
ssize_t netif_read(struct file* f, char __user* buf, size_t count, loff_t* ppos);

extern int major;
extern struct cdev netif_dev;
extern const struct file_operations netif_fops;
