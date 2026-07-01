#include "../lib/vnetint.h"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Virtual net device");

const struct net_device_ops vnet_dev = {
    .ndo_init                = vnet_init,
    .ndo_uninit              = NULL,
    .ndo_open                = NULL,
    .ndo_stop                = vnet_stop,
    .ndo_start_xmit          = vnet_start_xmit,
    .ndo_get_stats           = vnet_get_stats,
    .ndo_get_stats64         = NULL,
    .ndo_set_rx_mode         = NULL,
    .ndo_set_mac_address     = NULL,
    .ndo_validate_addr       = NULL,
    .ndo_do_ioctl            = NULL,
    .ndo_siocdevprivate      = NULL,
    .ndo_set_config          = NULL,
    .ndo_change_mtu          = NULL,
    .ndo_neigh_setup         = NULL,
    .ndo_tx_timeout          = NULL,
    .ndo_get_xmit_slave      = NULL,
    .ndo_hard_start_xmit     = NULL,
    .ndo_fix_features        = NULL,
    .ndo_set_features        = NULL,
    .ndo_vlan_rx_add_vid     = NULL,
    .ndo_vlan_rx_kill_vid    = NULL,
    .ndo_set_vf_mac          = NULL,
    .ndo_set_vf_vlan         = NULL,
    .ndo_set_vf_rate         = NULL,
    .ndo_set_vf_spoofchk     = NULL,
    .ndo_set_vf_trust        = NULL,
    .ndo_set_vf_link_state   = NULL,
    .ndo_set_vf_rss_query_en = NULL,
    .ndo_get_vf_config       = NULL,
    .ndo_set_vf_guid         = NULL,
    .ndo_get_vf_stats        = NULL,
    .ndo_setup_tc            = NULL,
    .ndo_fdb_add             = NULL,
    .ndo_fdb_del             = NULL,
    .ndo_fdb_dump            = NULL,
    .ndo_bridge_setlink      = NULL,
    .ndo_bridge_getlink      = NULL,
    .ndo_bridge_dellink      = NULL,
    .ndo_get_phys_port_id    = NULL,
    .ndo_get_phys_port_name  = NULL,
    .ndo_get_phys_switch_id  = NULL,
    .ndo_set_tx_maxrate      = NULL,
    .ndo_get_iflink          = NULL,
    .ndo_get_ts_stats        = NULL,
    .ndo_get_timestamping    = NULL,
    .ndo_gets_stats          = NULL,
    .ndo_set_time_method     = NULL,
    .ndo_ptp_settime64       = NULL,
    .ndo_ptp_gettimex64      = NULL,
    .ndo_ptp_enable          = NULL,
    .ndo_ptp_adjtime         = NULL,
    .ndo_ptp_adjfreq         = NULL,
    .ndo_udp_tunnel_add      = NULL,
    .ndo_udp_tunnel_del      = NULL,
    .ndo_has_offload_stats   = NULL,
    .ndo_get_offload_stats   = NULL,
    .ndo_set_vxlan_port      = NULL,
    .ndo_set_geneve_port     = NULL,
    .ndo_xdp_xmit            = NULL,
    .ndo_xsk_wakeup          = NULL,
    .ndo_xdp_get_info        = NULL,
    .ndo_xdp_attachment_info = NULL,
    .ndo_get_devlink_port    = NULL,
    .ndo_get_group           = NULL,
    .ndo_set_group           = NULL,
    .ndo_get_link_ksettings  = NULL,
    .ndo_set_link_ksettings  = NULL,
    .ndo_get_fecparam        = NULL,
    .ndo_set_fecparam        = NULL,
    .ndo_get_eeprom_len      = NULL,
    .ndo_get_eeprom          = NULL,
    .ndo_set_eeprom          = NULL,
    .ndo_get_module_info     = NULL,
    .ndo_get_module_eeprom   = NULL,
    .ndo_eth_ioctl           = NULL,
    .ndo_set_rss_hash        = NULL,
    .ndo_get_rss_hash        = NULL,
    .ndo_flow_rule_create    = NULL,
    .ndo_flow_rule_destroy   = NULL,
    .ndo_set_ringparam       = NULL,
    .ndo_get_ringparam       = NULL,
    .ndo_set_channels        = NULL,
    .ndo_get_channels        = NULL,
    .ndo_set_qos             = NULL,
    .ndo_get_qos             = NULL,
    .ndo_set_prio_tc_map     = NULL,
    .ndo_get_tc_map          = NULL,
    .ndo_set_rx_flow_steer   = NULL,
    .ndo_get_rx_flow_steer   = NULL,
    .ndo_get_rxnfc           = NULL,
    .ndo_set_rxnfc           = NULL,
    .ndo_get_rmon_stats      = NULL,
    .ndo_get_stats_counters  = NULL,
    .ndo_get_phc_vclocks     = NULL,
    .ndo_set_pauseparam      = NULL,
    .ndo_get_pauseparam      = NULL,
    .ndo_set_tunable         = NULL,
    .ndo_get_tunable         = NULL,
    .ndo_get_hw_stats        = NULL,
    .ndo_nsi_open            = NULL,
    .ndo_nsi_close           = NULL,
    .ndo_nsi_start_xmit      = NULL,
    .ndo_offload_skb         = NULL,
    .ndo_fill_metadata_dst   = NULL,
    .ndo_get_port_parent_id  = NULL,
    .ndo_get_dev_id          = NULL,
    .ndo_get_port_id         = NULL,
    .ndo_get_xstats          = NULL,
    .ndo_set_xstats          = NULL,
    .ndo_flow_block_bind     = NULL,
    .ndo_flow_block_unbind   = NULL,
    .ndo_l3_dev_bind         = NULL,
    .ndo_l3_dev_unbind       = NULL,
};

int vnet_init(net_device* dev)
{
  //  рандомный мак
  eth_hw_addr_random(dev);

  netif_start_queue(dev);
  return 0;
}

int vnet_stop(net_device* dev)
{
  netif_stop_queue(dev);
  return 0;
}

netdev_tx_t vnet_start_xmit(sk_buff* skb, net_device* dev)
{

}

net_device_stats *vnet_get_stats(net_device* dev)
{

}
