
# ILPS22QS I2C/SPI Driver

This is frameworkless I2C/3-wire SPI driver for ILPS22QS absolute pressure & temperature sensor. It is written in C++ using CRTP pattern. 
Driver does not depend on framework (eg., Arduino or nRF5 SDK). External handlers for interface operations have to be provided during object construction.

Driver documentation is available at `Documentation/html/index.html`
Example applications are available at `Examples` folder. All examples are made with STM HAL framework and tested with `ILPS22QSTR` sensor(on sTPMS_SEN1 PCB) and BluePill(`STM32F103C8`).

**The driver is developing by TPMS DIY project needs. Not every feature is supported and/or tested.**

# Driver configuration

Timeout for interface operations can be redefined with `ILPS22QS_I2C_READ_TIMEOUT` and `ILPS22QS_I2C_WRITE_TIMEOUT` or `ILPS22QS_SPI_READ_TIMEOUT` and `ILPS22QS_SPI_WRITE_TIMEOUT`.
Default timeout is 10ms.

# Debug mode

To enable debug mode, define `ILPS22QS_DEBUG` for verbose debug, `ILPS22QS_DEBUG_INFO` for info debug and `ILPS22QS_DEBUG_ERROR` for error debug.
Name of debug output handlers must be defined with `DEBUG_PRINT` and `DEBUG_PRINTF`.

# License

Copyright (c) 2025, silvio3105 (www.github.com/silvio3105)

Access and use of this Project and its contents are granted free of charge to any Person.
The Person is allowed to copy, modify and use The Project and its contents only for non-commercial use.
Commercial use of this Project and its contents is prohibited.
Modifying this License and/or sublicensing is prohibited.

THE PROJECT AND ITS CONTENT ARE PROVIDED "AS IS" WITH ALL FAULTS AND WITHOUT EXPRESSED OR IMPLIED WARRANTY.
THE AUTHOR KEEPS ALL RIGHTS TO CHANGE OR REMOVE THE CONTENTS OF THIS PROJECT WITHOUT PREVIOUS NOTICE.
THE AUTHOR IS NOT RESPONSIBLE FOR DAMAGE OF ANY KIND OR LIABILITY CAUSED BY USING THE CONTENTS OF THIS PROJECT.

This License shall be included in all functional textual files.

---

Copyright (c) 2025, silvio3105