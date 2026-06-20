#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/net.h>

#include "ferrum/ferrum.h"

#define VIF_NAME "ferrum0"

static struct fr_netif vif;
static struct task_struct* listener_thread;
static struct fr_tcp_conn listener_conn;
static bool stop_requested;

static int listener_fn(void* arg)
{
	int ret;

	ret = fr_tcp_listen(&listener_conn, vif.ip, vif.port, 8);
	if ( ret != FR_OK ) 
    {
		fr_err("failed to start listener on %s:%u: %d\n", vif.ip, vif.port, ret);
		return ret;
	}

	while ( !kthread_should_stop() && !stop_requested ) 
    {
		struct fr_tcp_conn client;
		char buf[256];
		int n;

		ret = fr_tcp_accept(&listener_conn, &client);
		if ( ret != FR_OK ) 
        {
			if ( kthread_should_stop() || stop_requested )
            {
                break;
            }
			msleep(100);
			continue;
		}

		fr_info("client connected on %s:%u\n", vif.ip, vif.port);

		while ( (n = fr_tcp_recv(&client, buf, sizeof(buf))) > 0 )
        {
            fr_tcp_send(&client, buf, n);
        }
		fr_tcp_close(&client);
		fr_info("client disconnected\n");
	}
	fr_tcp_close(&listener_conn);
	return 0;
}

static void on_vif_enable(struct fr_netif* v, bool enable)
{
	if ( enable ) 
    {
		if ( v->ip[0] == '\0' || v->port == 0 ) 
        {
			fr_err("ip/port not configured (ip='%s' port=%u), refusing to start listener\n", v->ip, v->port);
			return;
		}

		stop_requested = false;
		listener_thread = kthread_run(listener_fn, NULL, "ferrum_listener");
		if ( IS_ERR(listener_thread) ) 
        {
			fr_err("failed to start listener thread\n");
			listener_thread = NULL;
		}
	} 
    else 
    {
		if ( listener_thread ) 
        {
			stop_requested = true;

			if ( listener_conn.sock )
            {
                kernel_sock_shutdown(listener_conn.sock, SHUT_RDWR);
            }
			kthread_stop(listener_thread);
			listener_thread = NULL;
		}
	}
}

static int __init vif_module_init(void)
{
	int ret;
	ret = fr_netif_create(&vif, VIF_NAME, on_vif_enable);
	if ( ret != FR_OK )
    {
        return ret;
    }
	fr_info("interface %s created. Configure with:\n", VIF_NAME);
	fr_info("  echo <ip>   > /sys/class/net/%s/ferrum/ip\n", VIF_NAME);
	fr_info("  echo <port> > /sys/class/net/%s/ferrum/port\n", VIF_NAME);
	fr_info("  ip addr add <ip>/24 dev %s\n", VIF_NAME);
	fr_info("  echo 1      > /sys/class/net/%s/ferrum/enable\n", VIF_NAME);
	return 0;
}

static void __exit vif_module_exit(void)
{
	fr_netif_destroy(&vif);
}

module_init(vif_module_init);
module_exit(vif_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("A");
MODULE_DESCRIPTION("Virtual network interface with sysfs-controlled TCP listener");