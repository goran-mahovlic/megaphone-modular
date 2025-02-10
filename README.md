# megaphone-r5-modular
Modularised MEGAphone Design

This project is being carried out with the support of the NLnet Foundation under: https://nlnet.nl/project/MEGA65-MVP/

The goal of this project is to push the MEGAphone forward towards a Minimum Viable Product (MVP) in a cost-effective way by modularising the PCB
allowing for faster development and lower development costs as a result. Also, the modularisation helps to work around supply-chain problems by
allowing the replacement of hard to source components, without requiring a full PCB redesign.  Those modules can also be manufactured much more easily
in a domestic setting, as there is less cumulative chance of messing something up.  Alternatively, they can be manufactured in modest sized batches
using PCB fabrication places like PCBWay, who also offer an assembly option.

The following sections outline the tasks and their current state of progress, where appropriate.

## Task 1: Development and testing of core simple cellular telephony and text messaging software

The software aspect of this project relates to the software required to control and interact with the hardware modules, as well as to perform basic telephony functionality.

## Task 2: Modularisation and testing of the PCB design

The major hardware stream of this project is focused on modularising the MEGAphone PCB, so that the modules can be iterated and/or manufactured independently. This is to support both users self-manufacturing, so that the complexity of the fabrication process can be greatly simplified, as well as to allow for much faster and, including ad-hoc response to future supply chain challenges. 

### 2.1 High-efficiency DC-DC converter module to power the various sub-systems

High-current and high-efficiency modules are required for the main FPGA, cellular modem, auxilliary communications module, LCD panel, and external speaker/bluetooth modules.  It is desirable to come up with a single module that can be reused several times for these purposes.  The cellular modems are likely to be the highest draw item, and according to https://ww1.microchip.com/downloads/en/Appnotes/ANLPS300-APID.pdf, they can draw upto around 2A at 3.3V, which is the voltage we will most likely use, since it allows a common voltage for many of our sub-systems.

It is unlikely that any other device on the system will consume more power than this, so our DC-DC modules need to support 2A.

A further requirement is that it must be buck-boost, as the LiFePO4 cells can go below 3.3V, but when charged are >3.3V.

Some possible parts to consider:

STBBJ3 https://www.st.com/en/power-management/stbb3j.html  (2A in buck mode, i.e., where VIN > 3.3V, but only 800mA in boost mode, i.e., where VIN < 3.3V). 5x4 grid array, without thermal pad. Shutdown and soft-start. "typical" efficiency of 94%
LTC3113 https://www.analog.com/media/en/technical-documentation/data-sheets/3113f.pdf (3A in buck mode, 1.5A in boost mode). "up to" 96% efficiency. Choice of grid array and TSSOP -- but with a concealled thermal pad on the underside, which is not easy to hand-solder. AU$20 each for TTSOP!



### 2.2 Low-energy power-management module to minimise energy consumption of device, especially in low-power modes

### 2.3 Battery management and energy harvesting module to allow USB-C, integrated solar panel and external 12/24V vehicle battery power sources, and efficient management of the integrated rechargeable battery

### 2.4 Cellular modem communications module, including SIM card module

### 2.5 Internal microphone and speaker module

### 2.6 External speaker and bluetooth headset interface module

### 2.7 Primary FPGA carrier module, leveraging off-the-shelf FPGA modules if at all possible

### 2.8 LCD panel interface module

### 2.9 Auxiliary communications module, e.g., LoRa, ultra-sonic, infra-red or mm wave

Low-cost LoRa devices based on the SX1262 look to be an option, using open-source software like this: https://github.com/beegee-tokyo/SX126x-Mesh-Network
The goal would be for the module to be able to autonomously participate in the mesh while the main process is off.  ESP32s can go into low power modes with <1mA consumption according to https://www.arrow.com/en/research-and-events/articles/esp32-power-consumption-can-be-reduced-with-sleep-modes, and the SX1262 uses only about 5mA to listen for packets, and perhaps 70mA when active, so this looks plausible. The SX1262 when transmitting can draw upto about 120mA according to https://semtech.my.salesforce.com/sfc/p/#E0000000JelG/a/2R000000Un7F/yT.fKdAr9ZAo3cJLc4F2cBdUsMftpT2vsOICP7NmvMo

Thus the total power consumption of an auxilliary communications module (assuming we include an ESP32 in it, rather than using the power management FPGA to run the mesh) will be about 5mA while idle (~17mW) and upto ~200mA (~660mW).

### 2.10 Main carrier board and assembly, into which the various modules connect

## Task 3: Integration, Integration testing and remediation of the modules

Once the modules have been tested independently, this activity is focussed on testing the behaviour of the modules when connected to one another, including any VHDL/FPGA programming required to achieve this.

### 3.1 Requirements specification and analysis

### 3.2 Hardware integration and testing, including FPGA programming

### 3.3 Software integration and testing

### 3.4 Engage with and respond to security and accessibility audits

### 3.5 Remediation and contingency – for addressing any issues with the software and/or hardware components identified during the integration, and implementing or documenting appropriate solutions and/or work-arounds as appropriate

## Task 4: Preparation of Prototype Units

This activity focuses on taking the integrated hardware and software modules, and building a small number of units that are alpha-quality prototypes.  The specific objective is that they be integrated into units that are suitable for developers to use to continue development of both hardware and software of the system.  These units may be somewhat larger than the final obtainable footprint, depending on the costs of designing and fabricating the housings/enclosures.  The limited budget prevents the engagement of industrial designers who would be required for the completion of design for manufacture. Instead, we anticipate that the enclosures will be relatively simple 3D printed designs.
The remediation budget line is to allow for adjustment to the case design, the fabricated case and/or any of the hardware modules, so that the prototypes can be assembled and be as functional as possible.

### 4.1 Case Design

### 4.2 Case Fabrication

### 4.3 Assembly

### 4.4 Remediation / Mitigation, e.g., of any fitment issues, whether by adjustment of the case, reworking of a module or other means.