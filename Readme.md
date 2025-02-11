# Enhanced Simple Network Driver with Custom Packet Filtering

## Overview

This project implements a simple Linux kernel network driver that registers a virtual network interface (e.g., `snet0`) and provides custom packet filtering. The driver applies multiple filtering rules:
- **IP Filtering:** Drops packets with source IP `192.168.1.1`
- **Protocol Filtering:** Drops UDP packets
- **MAC Filtering:** Drops packets with source MAC address `00:11:22:33:44:55`

This project provides hands-on experience in:
- Linux kernel module development
- Network interface registration and handling
- Real-time packet inspection and filtering

## Problem Definition

Many network drivers do not incorporate advanced packet filtering directly into the kernel, which is critical for enhancing security and performance. This project addresses that gap by implementing a virtual network driver with built-in filtering rules, allowing unwanted packets to be dropped before they reach the higher layers of the networking stack.

## Literature Review

1. **Design and Implementation of a Network Driver in Linux**  
   - Explores the structure and implementation of network drivers in the Linux kernel.

2. **A Survey on Network Packet Filtering in Linux Kernel**  
   - Provides an overview of various packet filtering techniques used within Linux.

3. **Kernel Module Programming and Development**  
   - Offers a comprehensive guide on writing and debugging Linux kernel modules.

4. **Customizing Network Stack in Linux for High-Performance Computing**  
   - Discusses performance optimizations, including packet filtering in high-traffic environments.

5. **Secure Network Programming in Linux**  
   - Examines the importance of packet filtering for secure network communications.

## Objectives

- **Develop a Basic Network Driver:**  
  Create a kernel module that registers a virtual network interface.

- **Implement Custom Packet Filtering:**  
  - Drop packets with source IP `192.168.1.1`.
  - Drop UDP packets.
  - Drop packets with source MAC address `00:11:22:33:44:55`.

- **Gain Practical Kernel Module Experience:**  
  Work with Linux kernel APIs, debugging tools, and packet processing routines.

- **Establish a Modular Base:**  
  Create a foundation that can be extended with additional filtering rules or enhancements.

## Methodology

1. **Environment Setup:**  
   Install required packages (build tools, kernel headers, and Scapy for testing) and create a working directory.

2. **Driver Implementation:**  
   Write the kernel module (`simple_net_driver.c`) to register a virtual network device and implement custom packet filtering inside the transmit function. Use `printk` for detailed logging.

3. **Compilation and Installation:**  
   Build the module using a Makefile and load it into the kernel.

4. **Testing:**  
   Use a Python test script with Scapy to send packets that trigger each filtering rule. Verify behavior by examining kernel logs with `dmesg`.

5. **Documentation and Future Work:**  
   Provide detailed instructions and outline potential future enhancements.

## Installation & Setup

### Prerequisites

1. **Operating System:** Lubuntu/Ubuntu  
2. **Packages:**  
    ```bash
    sudo apt-get update
    sudo apt-get install build-essential linux-headers-$(uname -r) python3-scapy
    ```
### Directory & Setup
1. **Directory-setup:**
    ```bash
    mkdir ~/simple_net_driver
    cd ~/simple_net_driver
    ```
2. **Code-files:**
    - simple_net_driver.c
    - Makefile
    - test.py

### Compilation & Installation
1. **Compile the Module:**
    In your project directory, run:
    ```bash
    make
    ```
    This will generate the simple_net_driver.ko file.
2. **Insert the Module:**
    Load the module into the kernel:
    ```bash
    sudo insmod simple_net_driver.ko
    ```
    Verify the module is loaded successfully by checking:
    ```bash
    dmesg | tail -n 20
    ```
    (Use sudo command if permission error occurs)
3. **Bring Up the Virtual Interface:**
    Bring up the network interface:
    ```bash
    sudo ip link set snet0 up
    ```
    or
    ```bash
    sudo ifconfig snet0 up
    ```
### Testing the driver
1. **Run the Test Script:**
    Make the test script executable:
    ```bash
    chmod +x test.py
    ```
    Then run it with root privileges:
    ```bash
    sudo ./test.py
    ```
2. **Monitor Kernel Logs:**
    In another terminal, check the kernel logs to see the filtering behavior:
    ```bash
    dmesg | tail -n 30
    ```
    You should see messages indicating which packets were dropped (due to IP, UDP, or MAC filters) and which were accepted.

### Cleanup
1. **Unload the module:**
    ```bash
    sudo rmmod simple_net_driver
    ```
2. **Verify the modules was removed:**
    ```bash
    dmesg | tail -n 20
    ```

## Conclusion
This project demonstrates an enhanced Linux network driver with custom packet filtering that:

- Drops packets with source IP 192.168.1.1
- Drops UDP packets
- Drops packets from source MAC 00:11:22:33:44:55
- Accepts valid packets