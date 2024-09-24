# Ethernet Packet Transfer Simulation

## Project Description

This project simulates the transfer of Ethernet packets over a network, incorporating key aspects such as packet size, Inter-Frame Gaps (IFGs), and CRC calculations. The simulation helps understand how Ethernet packets are structured and transmitted and visualizes the impact of different parameters on packet transfer.

## Features

- Simulate Ethernet packet transfer with customizable parameters.
- Calculate and validate CRC-32 checksums for packet integrity.
- Analyze the impact of packet size, IFGs, and other parameters on transfer performance.

## Prerequisites

Before running the simulation, ensure you have the following installed:

- **GCC** or another C++ compiler
- **Make** (for building the project)
- **zlib** (for CRC calculations)

## Installation

### Installing zlib

To install zlib on your system, follow these instructions:

** Ubuntu/Debian:**

```bash
sudo apt-get update
sudo apt-get install zlib1g-dev
```

### macOS

1. **Install zlib**:  
```bash
   brew install zlib
```
### Windows:

Download the zlib binaries or source from zlib's official site.
Follow the instructions in the zlib documentation to install or build the library.

# Simulation

This document describes the parameters used in the simulation configuration file (`config.txt`).

## Configuration File (`config.txt`)

The configuration file contains key-value pairs used to set up the simulation parameters. Each line specifies a parameter and its value.

### Parameters

- **Eth.LineRate**: 
  - **Description**: The line rate of the Ethernet connection in Gbps.
  - **Example Value**: `10`

- **Eth.CaptureSizeMs**: 
  - **Description**: The duration (in milliseconds) for which packets are captured.
  - **Example Value**: `10`

- **Eth.MinNumOfIFGsPerPacket**: 
  - **Description**: The minimum number of inter-frame gaps (IFGs) between packets.
  - **Example Value**: `3`

- **Eth.DestAddress**: 
  - **Description**: The destination MAC address in hexadecimal format.
  - **Example Value**: `0x010101010101`

- **Eth.SourceAddress**: 
  - **Description**: The source MAC address in hexadecimal format.
  - **Example Value**: `0x333333333333`

- **Eth.MaxPacketSize**: 
  - **Description**: The maximum size of a packet in bytes.
  - **Example Value**: `46`
    
**`Burst Mode`**

- **Eth.BurstSize**: 
  - **Description**: The size of the burst in packets.
  - **Example Value**: `3`

- **Eth.BurstPeriodicity_us**: 
  - **Description**: The periodicity of bursts in microseconds.
  - **Example Value**: `100`
 
  

**`ORAN Mode`**

- **Oran.SCS**: 
  - **Description**: Subcarrier Spacing.
  - **Example Value**: `30`

- **Oran.MaxNrb**: 
  - **Description**: Max Number Of Resource Blocks / Symbol.
  - **Example Value**: `273`
 
- **Oran.NrbPerPacket**: 
  - **Description**: Max Number Of Resource Blocks / Packet.
  - **Example Value**: `30`

- **Oran.Payload**: 
  - **Description**: name of the file which contains the payload's IQ samples.
  - **Example Value**: `iq_file.txt`

## Configuration File SetUp
Place the `config.txt` file in the root directory of the simulation project. The simulation program will read this file to configure its parameters accordingly.

## Running Project
Compile and Run the project using make:
```bash
   make run
```

## Output
- For all transmission modes, a 4-byte aligned output stream will be dumped into the `output.txt` file in the root directory of the simulation project.
- For **`ORAN Mode`**, a json file for the parsed packets, `parsedPackets.json`, will be generated in the root directory of the simulation project.
