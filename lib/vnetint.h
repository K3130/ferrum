#pragma once

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/inetdevice.h>

#define IP_TO_HEX(a,b,c,d) ((d << 24) | (c << 16) | (b << 8) | a)
#define DEV_NAME "vnetint0"

extern struct net_device* vnet_dev;
extern __be32 vnet_ip_addr;
extern proc_dir_entry* vnet_proc_file;

extern int ip_a;
extern int ip_b;
extern int ip_c;
extern int ip_d;

module_param(ip_a, int, 0644);
module_param(ip_b, int, 0644);
module_param(ip_c, int, 0644);
module_param(ip_d, int, 0644);

bool vnetint_flush(void);
int vnetint_update_interface_ip(__be32 new_ip, __be32 mask, int prefixlen);

netdev_tx_t vnet_xmit(struct sk_buff* skb, struct net_device* dev);
void vnet_setup(struct net_device* dev);
int vnet_proc_show(struct seq_file* m, void* v);
ssize_t vnet_proc_write(struct file* file, const char __user* buffer, size_t count, loff_t* pos);
int vnet_proc_open(struct inode* inode, struct file* file);
