#include "ferrum/vnetint.h"


const struct proc_ops vnet_proc_ops = {
    .proc_open = vnet_proc_open,
    .proc_read = seq_read,
    .proc_write = vnet_proc_write,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};

const struct net_device_ops vnet_ops = { 
    .ndo_start_xmit = vnet_xmit, 
};

netdev_tx_t vnet_xmit(struct sk_buff* skb, struct net_device* dev)
{
    dev_kfree_skb( skb );
    return NETDEV_TX_OK;
}

void vnet_setup(struct net_device* dev)
{
    if ( dev )
    {
        ether_setup( dev );
        dev->netdev_ops = &vnet_ops;
    }
}
// Так как задачу делаю ради спортивного интереса то
// мне нужно было сбросить интерфейс а другого способа я не нашел
// поэтому так
bool vnetint_flush(void)
{
    char *envp[] = {
    "HOME=/",
    "PATH=/sbin:/bin:/usr/sbin:/usr/bin",
    NULL
    };
    
    char *argv_flush[] = {
        "/sbin/ip",
        "addr",
        "flush",
        "dev",
        DEV_NAME,
        NULL
    };

    int res = call_usermodehelper(argv_flush[0], argv_flush, envp, UMH_WAIT_PROC);
    if ( res != 0 )
    {
        return false;
    }
    return true;
}

int vnetint_update_interface_ip(__be32 new_ip, __be32 mask, int prefixlen)
{
    struct in_device* in_dev;
    struct in_ifaddr* ifa;
    struct in_ifaddr* old_ifa;
    int ret = 0;
    
    if ( !vnetint_flush() )
    {
        return -1;
    }

    rtnl_lock();
    
    in_dev = __in_dev_get_rtnl( vnet_dev );
    if ( !in_dev ) 
    {
        rtnl_unlock();
        return -ENODEV;
    }
    
    while ( in_dev->ifa_list ) 
    {
        old_ifa = in_dev->ifa_list;
        in_dev->ifa_list = old_ifa->ifa_next;
        kfree(old_ifa);
    }

    ifa = kmalloc( sizeof(struct in_ifaddr), GFP_KERNEL );
    if ( !ifa ) 
    {
        rtnl_unlock();
        return -ENOMEM;
    }
    
    memset( ifa, 0, sizeof(struct in_ifaddr) );
    strncpy(ifa->ifa_label, vnet_dev->name, IFNAMSIZ - 1);
    ifa->ifa_local = new_ip;
    ifa->ifa_address = new_ip;
    ifa->ifa_prefixlen = prefixlen;
    ifa->ifa_mask = mask;
    ifa->ifa_scope = RT_SCOPE_UNIVERSE;
    ifa->ifa_flags = IFA_F_PERMANENT;
    ifa->ifa_dev = in_dev;
    ifa->ifa_next = NULL;
    in_dev->ifa_list = ifa;
    
    call_netdevice_notifiers(NETDEV_CHANGEADDR, vnet_dev);
    
    if ( netif_running(vnet_dev) ) 
    {
        dev_close( vnet_dev );
        dev_open( vnet_dev, NULL );
    }
    
    vnet_ip_addr = new_ip;
    rtnl_unlock();
    return ret;
    return 0;

}

int vnet_proc_show(struct seq_file* m, void* v)
{
    seq_printf(m, "%d.%d.%d.%d\n", ip_a, ip_b, ip_c, ip_d);
    return 0;
}

ssize_t vnet_proc_write(struct file* file, const char __user* buffer, size_t count, loff_t* pos)
{
    char buf[32];
    int new_ip_a, new_ip_b, new_ip_c, new_ip_d;
    __be32 new_ip_hex;
    __be32 new_mask;
    int ret;
    
    if ( count > 31 )
    {
        count = 31;
    }
    
    if ( copy_from_user(buf, buffer, count) )
    {
        return -EFAULT;
    }
        
    buf[count] = '\0';
    if (count > 0 && buf[count-1] == '\n')
    {
        buf[count-1] = '\0';
    }
    
    if ( sscanf(buf, "%d.%d.%d.%d", &new_ip_a, &new_ip_b, &new_ip_c, &new_ip_d) == 4 ) 
    {
        ip_a = new_ip_a;
        ip_b = new_ip_b;
        ip_c = new_ip_c;
        ip_d = new_ip_d;
        
        new_ip_hex = IP_TO_HEX(ip_a, ip_b, ip_c, ip_d);
        vnet_ip_addr = new_ip_hex;
        new_mask = inet_make_mask(24);
        
        ret = vnetint_update_interface_ip( new_ip_hex, new_mask, 24 );
        if ( ret == 0 ) 
        {
            printk(KERN_INFO "IP successfully changed to %d.%d.%d.%d\n", ip_a, ip_b, ip_c, ip_d);
        } 
        else 
        {
            printk(KERN_ERR "Failed to change IP to %d.%d.%d.%d\n", ip_a, ip_b, ip_c, ip_d);
            return -EIO;
        }
    } 
    else 
    {
        printk(KERN_WARNING "Invalid IP format: %s\n", buf);
        return -EINVAL;
    }
    return count;
}

int vnet_proc_open(struct inode* inode, struct file* file)
{
    return single_open(file, vnet_proc_show, NULL);
}


