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

### 2.2 Low-energy power-management module to minimise energy consumption of device, especially in low-power modes

### 2.3 Battery management and energy harvesting module to allow USB-C, integrated solar panel and external 12/24V vehicle battery power sources, and efficient management of the integrated rechargeable battery

### 2.4 Cellular modem communications module, including SIM card module

### 2.5 Internal microphone and speaker module

### 2.6 External speaker and bluetooth headset interface module

### 2.7 Primary FPGA carrier module, leveraging off-the-shelf FPGA modules if at all possible

### 2.8 LCD panel interface module

### 2.9 Auxiliary communications module, e.g., LoRa, ultra-sonic, infra-red or mm wave

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