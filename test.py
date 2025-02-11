#!/usr/bin/env python3
"""
test.py

This script uses Scapy to send test packets on the virtual network interface (snet0)
to exercise the enhanced filtering rules:

1. IP Filter: Drops TCP packets with source IP 192.168.1.1.
2. UDP Filter: Drops UDP packets.
3. MAC Filter: Drops packets with source MAC 00:11:22:33:44:55.
4. Valid Packet: A TCP packet with valid parameters that should be accepted.
"""

from scapy.all import Ether, IP, TCP, UDP, sendp
import time

INTERFACE = "snet0"

def test_ip_filter():
    print("[Test IP Filter] Sending TCP packet with source IP 192.168.1.1 (should be dropped)")
    packet = (
        Ether() /
        IP(src="192.168.1.1", dst="192.168.1.200") /
        TCP(sport=12345, dport=80)
    )
    sendp(packet, iface=INTERFACE, verbose=True)
    time.sleep(1)

def test_udp_filter():
    print("[Test UDP Filter] Sending UDP packet with source IP 192.168.1.100 (should be dropped due to UDP filtering)")
    packet = (
        Ether() /
        IP(src="192.168.1.100", dst="192.168.1.200") /
        UDP(sport=12345, dport=53)
    )
    sendp(packet, iface=INTERFACE, verbose=True)
    time.sleep(1)

def test_mac_filter():
    print("[Test MAC Filter] Sending TCP packet with source MAC 00:11:22:33:44:55 (should be dropped)")
    packet = (
        Ether(src="00:11:22:33:44:55") /
        IP(src="192.168.1.100", dst="192.168.1.200") /
        TCP(sport=12345, dport=80)
    )
    sendp(packet, iface=INTERFACE, verbose=True)
    time.sleep(1)

def test_valid_packet():
    print("[Test Valid Packet] Sending TCP packet with valid parameters (should be accepted)")
    packet = (
        Ether(src="00:aa:bb:cc:dd:ee") /
        IP(src="192.168.1.100", dst="192.168.1.200") /
        TCP(sport=12345, dport=80)
    )
    sendp(packet, iface=INTERFACE, verbose=True)
    time.sleep(1)

def main():
    print("Starting packet simulation tests on interface:", INTERFACE)
    print("-------------------------------------------------------------\n")
    
    test_ip_filter()
    test_udp_filter()
    test_mac_filter()
    test_valid_packet()
    
    print("\nPacket simulation tests completed.")
    print("Please check kernel logs using: dmesg | tail -n 30")

if __name__ == "__main__":
    main()
