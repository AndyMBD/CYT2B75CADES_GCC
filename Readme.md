
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
            "runToEntryPoint": "main",
            //liveWatch
            "liveWatch": { "enabled": true, "samplesPerSecond": 4 },
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

4. Add Jlink debug support
   1. Add jlinkscript for CM0 and CM4 to let Jlink known which core it shall connect
```jlinkscript
void InitTarget(void) {
  Report("********************************************");
  Report("InitTarget for PSoC6 Cortex-M0+ script");
  CORESIGHT_AddAP(0, CORESIGHT_AHB_AP);  // SYSAP
  CORESIGHT_AddAP(1, CORESIGHT_AHB_AP);  // AHB-AP used to connect to M0+ core
  CORESIGHT_AddAP(2, CORESIGHT_AHB_AP);  // AHB-AP used to connect to M4 core
  CORESIGHT_IndexAHBAPToUse = 1;
  CPU=CORTEX_M0;
  Report("********************************************");
}
```
```jlinkscript
void InitTarget(void) {
  Report("********************************************");
  Report("InitTarget for PSoC6 Cortex-M4 script");
  CORESIGHT_AddAP(0, CORESIGHT_AHB_AP);  // SYSAP
  CORESIGHT_AddAP(1, CORESIGHT_AHB_AP);  // AHB-AP used to connect to M0+ core
  CORESIGHT_AddAP(2, CORESIGHT_AHB_AP);  // AHB-AP used to connect to M4 core
  CORESIGHT_IndexAHBAPToUse = 2;
  CPU=CORTEX_M4;
  Report("********************************************");
}
```
   2. Add Jlinkscript support in launch.json on script debugger_vsc.cmake
```cmake
# Jlink launch.json setting template
set(jsonMainCpuTemplateString
[==[
{
    "name": "@TEMPLATE_VAR_MAIN_CFG_NAME@ (jlink)",
    "type": "cortex-debug",
    "cwd": "${workspaceFolder}",
    "armToolchainPath": "@GCC_COMPILER_ROOT_DIR@/bin",
    "svdFile": "${workspaceFolder}/misc/tools/svd/@DIE@/@MCU_REV_STRING@/@SERIES@.svd",
    "servertype": "jlink",
    "serverpath": "@JLINK_DIR@/JLinkGDBServerCL.exe",
    "device": "@DEVICE@",
    "interface": "@JLINK_INTERFACE@",
    "serverArgs": ["-speed", "8000"],
    "showDevDebugOutput": "raw",
    //Default start from M0
    "jlinkscript":"@CMAKE_SOURCE_DIR@/@DIE@/tools/jlink/TRAVEO2_1M_@ARG_EXE_NAME_CM0PLUS@.JLinkScript",
    "numberOfProcessors": @TEMPLATE_VAR_NR_OF_CPUS@,
    "targetProcessor": 0,            
    "executable": "${command:cmake.getLaunchTargetDirectory}/@ARG_EXE_NAME_CM0PLUS@@CMAKE_EXECUTABLE_SUFFIX@",
    "request": "launch",
    "loadFiles": [
    "${command:cmake.getLaunchTargetDirectory}/@ARG_EXE_NAME_CM0PLUS@@CMAKE_EXECUTABLE_SUFFIX@",
    @TEMPLATE_VAR_ADDL_LOAD_FILES@
    ],
    "runToEntryPoint": "main",
    "liveWatch": { "enabled": true, "samplesPerSecond": 4 },
    @TEMPLATE_VAR_CHAINED_CFG@            
},
]==])


set(jsonChainedConfigurationsTemplateString
[==[
"chainedConfigurations": {
    "enabled": true,
    "waitOnEvent": "postInit",
    "detached": true, // jlink requires true
    "lifecycleManagedByParent": true,
    "launches": [
    @TEMPLATE_VAR_LAUNCH_ENTRIES@
    ]
},
]==])

set(jsonAppCpuTemplateString
[==[
{
    "name": "@TEMPLATE_VAR_SUB_CFG_NAME@ (jlink)",
    "type": "cortex-debug",
    "presentation": {
        "hidden": true,
    },
    "cwd": "${workspaceFolder}",
    "servertype": "jlink",
    "targetProcessor": @TEMPLATE_VAR_TARGET_CPU_INDEX@,            
    "jlinkscript":"@CMAKE_SOURCE_DIR@/@DIE@/tools/jlink/TRAVEO2_1M_@ARG_EXE_NAME_CM4@.JLinkScript",
    "executable": "${command:cmake.getLaunchTargetDirectory}/@TEMPLATE_VAR_SUB_CFG_EXE_NAME@@CMAKE_EXECUTABLE_SUFFIX@",
    "request": "launch", // 'attach' does not work properly, therefore use 'launch' and override the launch commands 
    "overrideLaunchCommands": [],
    "runToEntryPoint": "main",
    "liveWatch": { "enabled": true, "samplesPerSecond": 4 },
    "device": "@DEVICE@",
    "showDevDebugOutput": "none",
},
]==])


set(jsonLoadFileEntryTemplateString
[==[
"${command:cmake.getLaunchTargetDirectory}/@TEMPLATE_VAR_LOAD_FILE_EXE_NAME@@CMAKE_EXECUTABLE_SUFFIX@",
]==])

set(jsonLaunchEntryTemplateString
[==[
{
    "name": "@TEMPLATE_VAR_LAUNCH_CFG_NAME@ (jlink)",
},
]==])
    
```
5. add elf size information output after build in executable.cmake
```cmake
            COMMAND     echo "-----------${ARG_EXE_NAME}${CMAKE_EXECUTABLE_SUFFIX}--------------------"
            COMMAND     ${TOOLCHAIN_EXE_INFO} ${CMAKE_BINARY_DIR}/$<CONFIG>/${ARG_EXE_NAME}${CMAKE_EXECUTABLE_SUFFIX}
```
7. ***CY_USE_PSVP*** **tviibe1m** ***STARTER_KIT_REV3*** ***CYT2B75CAE*** is define from CMakeLists.txt and CMakePresets.json
   1. CY_MCU_${MCU_REV_STRING}
   2. ${DIE} **tviibe1m** 
   ```c
    // in cy_project.h
      #elif defined (tviibe1m)
      #include "bb_bsp_tviibe1m.h"
   ```
   3. ${BOARD_REV} **STARTER_KIT_REV3**
```c
    //in bb_bsp_tviibe1m.h
    #elif defined (STARTER_KIT_REV3)
    #include "bb_bsp_tviibe1m_starter_kit.h"
```
   1. CY_USE_PSVP define and device define <br>
      1. CY_USE_PSVP
```cmake
option(PSVP "Targets are built for PSVP" OFF) # PSVP: FPGA based pre-silicon validation platform

 CY_USE_PSVP=$<BOOL:${PSVP}>
```
```c
   //in bb_bsp_tviibe1m_starter_kit.h
  // tviibe1m - 100-LQFP Package devices  
  #if (CY_USE_PSVP == 0) && ( defined(CYT2B75BAS) || defined(CYT2B75BAE) || \
                              defined(CYT2B75CAS) || defined(CYT2B75CAE) )
```
   1. DIE define as ***CYT2B75CAE***
      
```json
 {
      "name": "tviibe1m-sk",
      "hidden": true,
      "cacheVariables": {
        "DIE": "tviibe1m",
        "BOARD_REV": "STARTER_KIT_REV3",
        "DEVICE": "CYT2B75CAE"
      }
    },
```
```cmake
# Generic compiler/assember defines (check CMake files that create executable or library targets for further specific defines)
set(COMPILER_DEFINES
    ${DIE}
    ${SERIES}
    ${DEVICE}
    CY_MCU_${MCU_REV_STRING}
    CY_USE_PSVP=$<BOOL:${PSVP}>
    ${BOARD}
    ${BOARD_REV}
    _LINK_${LINK}_
    $<$<BOOL:${USE_RTOS}>:USE_RTOS>
    $<$<BOOL:${USE_ETHSTACK}>:USE_ETHSTACK>
    $<$<BOOL:${USE_MCAL}>:USE_MCAL>
)
```
```cmake
[cmake] -- Latest MCU revision detected: D
[cmake] -- ============================================================
[cmake] -- T2G SDL - CMake Configuration
[cmake] -- ------------------------------------------------------------
[cmake] -- Toolchain         = gcc
[cmake] -- DIE               = tviibe1m
[cmake] -- MCU revision      = D
[cmake] -- Device            = CYT2B75CAE
[cmake] -- Link location     = flash
[cmake] -- PSVP              = OFF
[cmake] -- Board             = 
[cmake] -- Board revision    = STARTER_KIT_REV3
[cmake] -- Use RTOS          = ON
[cmake] -- Use ETH Stack     = OFF
[cmake] -- Use MCAL          = OFF
[cmake] -- ============================================================
```
TODO:
1. Jlink debug Attach
2. Jlink erase_all
3. Jlink prog_all
4. Jlink Prog_cm0plus
5. Jlink Prog_cm4