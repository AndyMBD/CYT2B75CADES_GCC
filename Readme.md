
## Change point

1. Change the cmake\tool_config.cmake<br>

1.1. **defaultGccCompilerRootDir** to the *arm-gnu-toolchain* path in file cmake\tool_config.cmake<br>
```cmake
######################################################################################
# ARM GNU Toolchain (e.g. GCC for building, GDB for debugging with VS Code, ...)
######################################################################################
set(ENV{SDL_CMAKE_GCC_COMPILER_ROOT_DIR} "C:/SW_Tool/arm-gnu-toolchain-13.2.rel1-mingw-w64-i686-arm-none-eabi")
if(DEFINED ENV{SDL_CMAKE_GCC_COMPILER_ROOT_DIR})
set(defaultGccCompilerRootDir $ENV{SDL_CMAKE_GCC_COMPILER_ROOT_DIR})
else()
set(defaultGccCompilerRootDir "C:/SW_Tool/arm-gnu-toolchain-13.2.Rel1-mingw-w64-i686-arm-none-eabi")
endif()
set(GCC_COMPILER_ROOT_DIR ${defaultGccCompilerRootDir} CACHE PATH "Path to GNU C Compiler root folder used for compiling/debugging")

```

1.2 Change the Infineon Auto Flash Utility dir under cmake\tool_config.cmake

```cmake
######################################################################################
# Infineon Auto Flash Utility (AFU) / OpenOCD (OOCD)
######################################################################################
set(ENV{SDL_CMAKE_AFU_OOCD_ROOT_DIR} "C:/SW_Tool/Cypress Auto Flash Utility 1.0")
if(DEFINED ENV{SDL_CMAKE_AFU_OOCD_ROOT_DIR})
    set(defaultAfuOocdRootDir $ENV{SDL_CMAKE_AFU_OOCD_ROOT_DIR})
else()
    set(defaultAfuOocdRootDir "C:/SW_Tool/Cypress Auto Flash Utility 1.0")
endif()
set(AFU_OOCD_ROOT_DIR ${defaultAfuOocdRootDir} CACHE PATH "Path to Infineon Auto Flash Utility / OpenOCD")
```

2. LiveWatch implement cmake/targets/debugger_vsc.cmake     
***but looks not work good***
```json
"svdFile": "${workspaceFolder}/misc/tools/svd/@DIE@/@MCU_REV_STRING@/@SERIES@.svd",
"servertype": "openocd",
// LiveWatch
"liveWatch":{"enable": true, "samplesPerSecond": 4},
"serverpath": "@AFU_OOCD_ROOT_DIR@/bin/openocd.exe",
"searchDir": [
    "@AFU_OOCD_ROOT_DIR@/scripts"
],
```
3. Add multiplite project switch

3.1.  Change on CMakePresets.json

3.1.1. add ***PROJECT_SRC_PATH*** configuration

```json
 {
 "name": "led_blink",
 "hidden": true,
 "cacheVariables": {
     "PROJECT_SRC_PATH": "led_blink"
 }
 },
 {
 "name": "adc_DieTempCalculation",
 "hidden": true,
 "cacheVariables": {
     "PROJECT_SRC_PATH": "adc_DieTempCalculation"
 }
 },
```

3.1.2. add Project build configuration Preset with update ***PROJECT_SRC_PATH*** <br>
```json
 {
   "name": "tviibe1m-sk-gcc_led_blink",
   "inherits": [ "default", "tviibe1m-sk", "toolchain-gcc", "led_blink" ],
   "displayName": "CYTVII-B-E-1M-SK (GCC) led_blink",
   "description": "Configure T2G-B-E-1M Starter Kit 'CYTVII-B-E-1M-SK' led_blink (latest rev, flash linking) using GCC compiler and Ninja Multi-Config generator"
 },
 {
   "name": "tviibe1m-sk-gcc_adc_DieTempCalculation",
   "inherits": [ "default", "tviibe1m-sk", "toolchain-gcc","adc_DieTempCalculation" ],
   "displayName": "CYTVII-B-E-1M-SK_ADC (GCC) adc_DieTempCalculation",
   "description": "Configure T2G-B-E-1M Starter Kit 'CYTVII-B-E-1M-SK' adc_DieTempCalculation (latest rev, flash linking) using GCC compiler and Ninja Multi-Config generator"
 }
```

3.1.3. add **debug** **release** **RelWithDebInfo** configuration preset <br>
```json
 {
   "name": "tviibe1m-sk-gcc-debug_led_blink",
   "configurePreset": "tviibe1m-sk-gcc_led_blink",
   "inherits": [ "default", "cfg-debug" ],
   "displayName": "CYTVII-B-E-1M-SK (GCC) led_blink, 'Debug' Cfg",
   "description": "Build 'Debug' configuration of T2G-B-E-1M Starter Kit 'CYTVII-B-E-1M-SK' (latest rev, flash linking) using GCC compiler and Ninja"
 },
 {
   "name": "tviibe1m-sk-gcc-debug_adc_DieTempCalculation",
   "configurePreset":  "tviibe1m-sk-gcc_adc_DieTempCalculation" ,
   "inherits": [ "default", "cfg-debug" ],
   "displayName": "CYTVII-B-E-1M-SK (GCC) adc_DieTempCalculation, 'Debug' Cfg",
   "description": "Build 'Debug' configuration of T2G-B-E-1M Starter Kit 'CYTVII-B-E-1M-SK' (latest rev, flash linking) using GCC compiler and Ninja"
 },

 {
   "name": "tviibe1m-sk-gcc-release_led_blink",
   "configurePreset": "tviibe1m-sk-gcc_led_blink",
   "inherits": [ "default", "cfg-release" ],
   "displayName": "CYTVII-B-E-1M-SK (GCC) led_blink, 'Release' Cfg",
   "description": "Build 'Release' configuration of T2G-B-E-1M Starter Kit 'CYTVII-B-E-1M-SK' (latest rev, flash linking) using GCC compiler and Ninja"
 },
 {
   "name": "tviibe1m-sk-gcc-release_adc_DieTempCalculation",
   "configurePreset": "tviibe1m-sk-gcc_adc_DieTempCalculation",
   "inherits": [ "default", "cfg-release" ],
   "displayName": "CYTVII-B-E-1M-SK (GCC) adc_DieTempCalculation, 'Release' Cfg",
   "description": "Build 'Release' configuration of T2G-B-E-1M Starter Kit 'CYTVII-B-E-1M-SK' (latest rev, flash linking) using GCC compiler and Ninja"
 },

 {
   "name": "tviibe1m-sk-gcc-relwithdebinfo_led_blink",
   "configurePreset": "tviibe1m-sk-gcc_led_blink",
   "inherits": [ "default", "cfg-relwithdebinfo" ],
   "displayName": "CYTVII-B-E-1M-SK (GCC) led_blink, 'Release w/ Debug Info' Cfg",
   "description": "Build 'Release w/ Debug Info' configuration of T2G-B-E-1M Starter Kit 'CYTVII-B-E-1M-SK' (latest rev, flash linking) using GCC compiler and Ninja"
 },
 {
   "name": "tviibe1m-sk-gcc-relwithdebinfo_adc_DieTempCalculation",
   "configurePreset": "tviibe1m-sk-gcc_adc_DieTempCalculation",
   "inherits": [ "default", "cfg-relwithdebinfo" ],
   "displayName": "CYTVII-B-E-1M-SK (GCC) adc_DieTempCalculation, 'Release w/ Debug Info' Cfg",
   "description": "Build 'Release w/ Debug Info' configuration of T2G-B-E-1M Starter Kit 'CYTVII-B-E-1M-SK' (latest rev, flash linking) using GCC compiler and Ninja"
 }
```
3.2. update ***PROJECT_SRC_PATH*** in CMakefileLists.txt

```cmake
 ###################################################################################################
 # Common variables needed to build all/most of the target libraries/executables
 ###################################################################################################

 # Frequently used paths
 set(PATH_COMMON_SRC                  ${CMAKE_SOURCE_DIR}/common/src)
 set(PATH_COMMON_SRC_DRIVERS          ${CMAKE_SOURCE_DIR}/common/src/drivers)
 set(PATH_COMMON_SRC_MW               ${CMAKE_SOURCE_DIR}/common/src/mw)
 set(PATH_DIE_SRC                     ${CMAKE_SOURCE_DIR}/${DIE}/${PROJECT_SRC_PATH})
 set(PATH_DIE_SRC_DRIVERS             ${CMAKE_SOURCE_DIR}/${DIE}/${PROJECT_SRC_PATH}/drivers)
 set(PATH_DIE_SRC_MW                  ${CMAKE_SOURCE_DIR}/${DIE}/${PROJECT_SRC_PATH}/mw)
 if(USE_RTOS)
     set(PATH_COMMON_SRC_RTOS         ${CMAKE_SOURCE_DIR}/common/src/rtos)
 endif()
 if(USE_ETHSTACK)
     set(PATH_COMMON_SRC_ETHSTACK     ${CMAKE_SOURCE_DIR}/common/src/ethstack)
 endif()
 if(USE_MCAL)
     set(PATH_COMMON_SRC_MCALSTUBS    ${CMAKE_SOURCE_DIR}/common/src/mcalstubs)
 endif()

 # Generic compiler/assember include directories (check CMake files that create executable or library targets for further specific include dirs)
 set(INCLUDE_DIRS
     ${TOOLCHAIN_INCLUDE_DIRS} # Explicitly adding the toolchain include dirs may help code indexers in 3rd party apps like VS Code
     ${DIE}/hdr/${MCU_REV_STRING}
     ${DIE}/hdr/${MCU_REV_STRING}/ip
     ${DIE}/hdr/${MCU_REV_STRING}/mcureg
     ${DIE}/${PROJECT_SRC_PATH}
     ${DIE}/${PROJECT_SRC_PATH}/drivers
     ${DIE}/${PROJECT_SRC_PATH}/mw
     ${DIE}/${PROJECT_SRC_PATH}/interrupts/${MCU_REV_STRING}
     ${DIE}/${PROJECT_SRC_PATH}/system/${MCU_REV_STRING}
     common/hdr
     common/hdr/cmsis/include
     common/src/drivers
     common/src/mw
 )

```
3.3. update Project **adc_DieTempCalculation** and **led_blink** folder ***tviibe1m***
```c
├─adc_DieTempCalculation
│  ├─drivers
│  │  ├─canfd
│  │  ├─cpu
│  │  ├─crypto
│  │  │  └─libs
│  │  ├─sysclk
│  │  └─sysflt
│  ├─interrupts
│  │  └─rev_d
│  └─system
│      └─rev_d
├─hdr
│  └─rev_d
│      ├─ip
│      └─mcureg
├─led_blink
│  ├─drivers
│  │  ├─canfd
│  │  ├─cpu
│  │  ├─crypto
│  │  │  └─libs
│  │  ├─sysclk
│  │  └─sysflt
│  ├─interrupts
│  │  └─rev_d
│  └─system
│      └─rev_d
└─tools
 └─gcc
```
3.4. Further add more project into workspace, Copy source from tviibe1m_examples to tviibe1m and follow step 3.1->3.3 