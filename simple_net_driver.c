/**
 * simple_net_driver.c - Enhanced network driver with multiple custom packet filtering
 *
 * This module creates a virtual network interface (e.g., snet0) and implements
 * a custom packet filtering function that applies three types of filtering:
 *   1. IP Filtering: Drops packets with a source IP of 192.168.1.1.
 *   2. Protocol Filtering: Drops UDP packets.
 *   3. MAC Filtering: Drops packets with a source MAC address matching 00:11:22:33:44:55.
 *
 * All filtering decisions are logged to the kernel log.
 */

 #include <linux/module.h>
 #include <linux/kernel.h>
 #include <linux/init.h>
 #include <linux/netdevice.h>
 #include <linux/etherdevice.h>
 #include <linux/ip.h>
 #include <linux/if_ether.h>
 #include <linux/skbuff.h>
 #include <linux/string.h>
 
 #define DRIVER_NAME "simple_net_driver"
 #define FILTERED_IP 0xC0A80101  /* 192.168.1.1 in hex */
 #define FILTERED_PROTO IPPROTO_UDP  /* Filter out UDP packets */
 
 // Define a filtered MAC address: 00:11:22:33:44:55
 static unsigned char filtered_mac[ETH_ALEN] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
 
 static struct net_device *simple_net_dev;
 
 static int custom_packet_filter(struct sk_buff *skb)
 {
     struct iphdr *ip_header;
     struct ethhdr *eth_header;
     __be32 src_ip;
 
     /* Only process IP packets */
     if (skb->protocol != htons(ETH_P_IP)) {
         printk(KERN_INFO DRIVER_NAME ": [Filter] Non-IP packet received, allowing\n");
         return NET_RX_SUCCESS;
     }
 
     ip_header = ip_hdr(skb);
     if (!ip_header) {
         printk(KERN_WARNING DRIVER_NAME ": [Filter] Unable to retrieve IP header\n");
         return NET_RX_SUCCESS;
     }
 
     eth_header = eth_hdr(skb);
     src_ip = ip_header->saddr;
 
     printk(KERN_INFO DRIVER_NAME ": [Filter] Processing IP packet from %pI4, Protocol: %d, Source MAC: %pM\n",
            &src_ip, ip_header->protocol, eth_header->h_source);
 
     /* IP Filtering: Drop packet if source IP is 192.168.1.1 */
     if (src_ip == htonl(FILTERED_IP)) {
         printk(KERN_INFO DRIVER_NAME ": [Filter] Dropping packet from 192.168.1.1\n");
         return NET_RX_DROP;
     }
 
     /* Protocol Filtering: Drop packet if the protocol is UDP */
     if (ip_header->protocol == FILTERED_PROTO) {
         printk(KERN_INFO DRIVER_NAME ": [Filter] Dropping UDP packet\n");
         return NET_RX_DROP;
     }
 
     /* MAC Filtering: Drop packet if source MAC matches filtered_mac */
     if (memcmp(eth_header->h_source, filtered_mac, ETH_ALEN) == 0) {
         printk(KERN_INFO DRIVER_NAME ": [Filter] Dropping packet from filtered MAC address: %pM\n",
                eth_header->h_source);
         return NET_RX_DROP;
     }
 
     printk(KERN_INFO DRIVER_NAME ": [Filter] Packet accepted\n");
     return NET_RX_SUCCESS;
 }
 
 static int simple_net_open(struct net_device *dev)
 {
     netif_start_queue(dev);
     printk(KERN_INFO DRIVER_NAME ": Device opened\n");
     return 0;
 }
 
 static int simple_net_stop(struct net_device *dev)
 {
     netif_stop_queue(dev);
     printk(KERN_INFO DRIVER_NAME ": Device closed\n");
     return 0;
 }
 
 static netdev_tx_t simple_net_xmit(struct sk_buff *skb, struct net_device *dev)
 {
     int filter_result;
 
     printk(KERN_INFO DRIVER_NAME ": Transmit function called\n");
 
     filter_result = custom_packet_filter(skb);
     if (filter_result == NET_RX_DROP) {
         printk(KERN_INFO DRIVER_NAME ": Packet dropped by filter\n");
         dev_kfree_skb(skb);
         return NETDEV_TX_OK;
     }
 
     skb->dev = dev;
     skb->protocol = eth_type_trans(skb, dev);
     netif_rx(skb);
 
     printk(KERN_INFO DRIVER_NAME ": Packet transmitted and looped back\n");
     return NETDEV_TX_OK;
 }
 
 static const struct net_device_ops simple_netdev_ops = {
     .ndo_open       = simple_net_open,
     .ndo_stop       = simple_net_stop,
     .ndo_start_xmit = simple_net_xmit,
 };
 
 static void simple_net_setup(struct net_device *dev)
 {
     ether_setup(dev);
     dev->netdev_ops = &simple_netdev_ops;
     dev->flags |= IFF_NOARP;
     strcpy(dev->name, "snet%d");
 }
 
 static int __init simple_net_init(void)
 {
     int result;
 
     printk(KERN_INFO DRIVER_NAME ": Initializing enhanced network driver module\n");
     printk(KERN_INFO DRIVER_NAME ": Custom Packet Filtering Features:\n");
     printk(KERN_INFO DRIVER_NAME ":   - IP Filter: Drops packets from 192.168.1.1\n");
     printk(KERN_INFO DRIVER_NAME ":   - Protocol Filter: Drops UDP packets\n");
     printk(KERN_INFO DRIVER_NAME ":   - MAC Filter: Drops packets from MAC 00:11:22:33:44:55\n");
 
     simple_net_dev = alloc_netdev(0, "snet%d", NET_NAME_UNKNOWN, simple_net_setup);
     if (!simple_net_dev) {
         printk(KERN_ERR DRIVER_NAME ": Failed to allocate net device\n");
         return -ENOMEM;
     }
 
     result = register_netdev(simple_net_dev);
     if (result) {
         printk(KERN_ERR DRIVER_NAME ": Error %d registering net device\n", result);
         free_netdev(simple_net_dev);
         return result;
     }
 
     printk(KERN_INFO DRIVER_NAME ": Enhanced network driver module loaded successfully\n");
     return 0;
 }
 
 static void __exit simple_net_exit(void)
 {
     unregister_netdev(simple_net_dev);
     free_netdev(simple_net_dev);
     printk(KERN_INFO DRIVER_NAME ": Enhanced network driver module unloaded\n");
 }
 
 module_init(simple_net_init);
 module_exit(simple_net_exit);
 
 MODULE_LICENSE("GPL");
 MODULE_AUTHOR("Your Name");
 MODULE_DESCRIPTION("Enhanced Simple Network Driver with Multiple Custom Packet Filtering");
 