# STM32_EPD_Driver
This is a project for driving the ePaper display panel with STM32, here I'm using the development board from Good Display's DESPI-M02, which is based on STM32F103VET6 microcontroller. 

# Hardware connections
| DESPI-M02 | EPD |
|----|----|
| PE13 | EPD_BUSY |
| PE14 | EPD_RES |
| PE15 | EPD_DC |
| PD8 | EPD_CS |
| PD9 | EPD_SCK |
| PD10 | EPD_SDI |

# Supported ePaper Display Panels
1. DEPG0420BNS, 4.2 inch black/white ePaper display panel
2. Unknown 3.71 inch black/white/red ePaper display panel,FPC labeled as "FPC-2303", which compactable with gooddisplay GDEY037Z03


# Change Log
20221001 Kimi Jin: 

    1. Initial Project
    2. Initial code structure
    3. Adding support for DEPG0420BNS(not tested) and Unknown 3.71(tested) epd
    

