#include "../drivers/ethernet.h"
#include "../kernel/types.h"

static EthernetDevice* eth_device = 0;

/* Initialize Ethernet */
void ethernet_init(void) {
    /* TODO: Initialize Ethernet layer */
}

/* Register Ethernet device */
void ethernet_register_device(EthernetDevice* dev) {
    eth_device = dev;
}

/* Send Ethernet frame */
void ethernet_send_frame(EthernetFrame* frame) {
    if (!eth_device || !eth_device->send_frame) return;
    
    eth_device->send_frame(frame);
}

/* Receive Ethernet frame */
EthernetFrame* ethernet_receive_frame(void) {
    if (!eth_device || !eth_device->receive_frame) return 0;
    
    return eth_device->receive_frame();
}

/* Set MAC address */
void ethernet_set_mac_address(uint8_t* mac) {
    if (!eth_device) return;
    
    int i;
    for (i = 0; i < 6; i++) {
        eth_device->mac_address.addr[i] = mac[i];
    }
}

/* Get MAC address */
void ethernet_get_mac_address(uint8_t* mac) {
    if (!eth_device) return;
    
    int i;
    for (i = 0; i < 6; i++) {
        mac[i] = eth_device->mac_address.addr[i];
    }
}
