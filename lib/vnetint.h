#pragma once
/*
 * Модуль для создания виртуального сетевого интерфейса
 *
 *
 *
 *
*/

#include <linux/inetdevice.h>
#include <linux/etherdevice.h>

extern struct net_device* vnet_dev;
extern __be32 vnet_ip_addr;

// определение функций по порядку как в структуре для удобства
int vnet_init(struct net_device* dev);
int vnet_stop(struct net_device* dev);
netdev_tx_t vnet_start_xmit(struct sk_buff* skb, struct net_device* dev);
net_device_stats* vnet_get_stats(struct net_device* dev);



