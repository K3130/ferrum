#include "ferrum/netif.h"
#include "ferrum/core.h"

#undef FERRUM_LOG_TAG
#define FERRUM_LOG_TAG "ferrum-netif"
#include "ferrum/log.h"

#include <linux/etherdevice.h>
#include <linux/kernel.h>
#include <linux/sysfs.h>
#include <linux/string.h>

struct fr_netif_priv 
{
	struct fr_netif* vif;
};

static int fr_netif_open(struct net_device* dev)
{
	netif_start_queue(dev);
	return 0;
}

static int fr_netif_stop(struct net_device* dev)
{
	netif_stop_queue(dev);
	return 0;
}

static netdev_tx_t fr_netif_xmit(struct sk_buff* skb, struct net_device* dev)
{
	dev->stats.tx_packets++;
	dev->stats.tx_bytes += skb->len;
	consume_skb(skb);
	return NETDEV_TX_OK;
}

static const struct net_device_ops fr_netif_ops = 
{
	.ndo_open       = fr_netif_open,
	.ndo_stop       = fr_netif_stop,
	.ndo_start_xmit = fr_netif_xmit,
};

static void fr_netif_setup(struct net_device* dev)
{
	ether_setup(dev);
	dev->netdev_ops = &fr_netif_ops;
	dev->flags |= IFF_NOARP;
	eth_hw_addr_random(dev);
}

/* ---- sysfs: ip / port / enable ---- */
static struct fr_netif* priv_vif(struct device* d)
{
	struct net_device* dev = to_net_dev(d);
	struct fr_netif_priv* priv = netdev_priv(dev);
	return priv->vif;
}

static ssize_t ip_show(struct device* d, struct device_attribute* attr, char* buf)
{
	struct fr_netif* vif = priv_vif(d);
	return sysfs_emit(buf, "%s\n", vif->ip);
}

static ssize_t ip_store(struct device* d, struct device_attribute* attr, const char* buf, size_t count)
{
	struct fr_netif* vif = priv_vif(d);
	size_t len = count;

	if ( len >= sizeof(vif->ip) )
    {
        return -EINVAL;
    }
		
	memcpy(vif->ip, buf, len);
	vif->ip[len] = '\0';

	if ( len > 0 && vif->ip[len - 1] == '\n' )
    {
        vif->ip[len - 1] = '\0';
    }
	return count;
}

static ssize_t port_show(struct device* d, struct device_attribute* attr, char* buf)
{
	struct fr_netif* vif = priv_vif(d);
	return sysfs_emit(buf, "%u\n", vif->port);
}

static ssize_t port_store(struct device* d, struct device_attribute* attr, const char* buf, size_t count)
{
	struct fr_netif* vif = priv_vif(d);
	u16 port;
	int ret;

	ret = kstrtou16(buf, 10, &port);
	if ( ret < 0 )
    {
        return ret;
    }
	vif->port = port;
	return count;
}

static ssize_t enable_show(struct device* d, struct device_attribute* attr, char* buf)
{
	struct fr_netif* vif = priv_vif(d);
	return sysfs_emit(buf, "%d\n", vif->enabled ? 1 : 0);
}

static ssize_t enable_store(struct device* d, struct device_attribute* attr, const char* buf, size_t count)
{
	struct net_device* dev = to_net_dev(d);
	struct fr_netif* vif = priv_vif(d);
	bool want;
	int ret;

	if ( count == 0 )
    {
        return -EINVAL;
    }
		
	want = (buf[0] == '1');

	if ( want && !vif->enabled ) 
    {
		ret = dev_open(dev, NULL);
		if ( ret < 0 ) 
        {
			fr_err("dev_open failed for %s: %d\n", dev->name, ret);
			return ret;
		}
		vif->enabled = true;
		fr_info("%s enabled (ip=%s port=%u)\n", dev->name, vif->ip, vif->port);
		if ( vif->on_enable )
        {
            vif->on_enable(vif, true);
        }		
	} 
    else if ( !want && vif->enabled ) 
    {
		if (vif->on_enable)
        {
            vif->on_enable(vif, false);
        }
		dev_close(dev);
		vif->enabled = false;
		fr_info("%s disabled\n", dev->name);
	}
	return count;
}

static DEVICE_ATTR_RW(ip);
static DEVICE_ATTR_RW(port);
static DEVICE_ATTR_RW(enable);

static struct attribute* fr_netif_attrs[] = {
	&dev_attr_ip.attr,
	&dev_attr_port.attr,
	&dev_attr_enable.attr,
	NULL,
};

static const struct attribute_group fr_netif_attr_group = {
	.name  = "ferrum",
	.attrs = fr_netif_attrs,
};


int fr_netif_create(struct fr_netif* vif, const char* name, fr_netif_enable_cb_t on_enable)
{
	struct net_device* dev;
	struct fr_netif_priv* priv;
	int ret;

	if ( !vif || !name )
    {
        return FR_ERR_INVAL;
    }
	
	memset(vif, 0, sizeof(*vif));
	vif->on_enable = on_enable;

	dev = alloc_netdev(sizeof(struct fr_netif_priv), name, NET_NAME_UNKNOWN, fr_netif_setup);
	if ( !dev )
    {
        return FR_ERR_NOMEM;
    }
	
	priv = netdev_priv(dev);
	priv->vif = vif;
	vif->dev = dev;

	dev->sysfs_groups[0] = &fr_netif_attr_group;

	ret = register_netdev(dev);
	if ( ret < 0 ) 
    {
		fr_err("register_netdev failed: %d\n", ret);
		free_netdev(dev);
		vif->dev = NULL;
		return ret;
	}

	fr_info("created virtual interface %s\n", dev->name);
	return FR_OK;
}

void fr_netif_destroy(struct fr_netif* vif)
{
	if ( !vif || !vif->dev )
    {
        return;
    }
		
	if ( vif->enabled ) 
    {
		if ( vif->on_enable )
        {
            vif->on_enable(vif, false);
        }	
		dev_close(vif->dev);
		vif->enabled = false;
	}
	unregister_netdev(vif->dev);
	free_netdev(vif->dev);
	vif->dev = NULL;
}