#ifndef FERRUM_NETIF_H
#define FERRUM_NETIF_H

#include <linux/types.h>
#include <linux/netdevice.h>

/*Вызывается при изменении состояния sysfs-атрибута enable*/
typedef void (*fr_netif_enable_cb_t)(struct fr_netif* vif, bool enable);

struct fr_netif 
{
	struct net_device* dev;
	char ip[16];
	u16 port;
	bool enabled;
	fr_netif_enable_cb_t on_enable;
};
/*Создает виртуальный сетевой интефейс*/
int fr_netif_create(struct fr_netif* vif, const char* name, fr_netif_enable_cb_t on_enable);
/*Остановка и удаление интерфейса*/
void fr_netif_destroy(struct fr_netif* vif);

#endif /* FERRUM_NETIF_H */