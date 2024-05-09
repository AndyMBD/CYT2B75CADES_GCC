##############################################################################################################################
# This file is used to configure paths and certain options for tools and add-on SW (compilers, debuggers, programmers, Tresos,
# MCAL, etc.) which may differ from user to user.
# 
# Two options exist for adjusting them:
#
# 1) Adjust paths and option values directly in this file
# 2) A mechanism is implemented that will check for the presence of corresponding environment variables which will then take
#    precedence over the configured settings in this file. The environment variables need to have the same name like the CMake
#    variable but prefixed with 'SDL_CMAKE_'.
#    Example:
#             CMake variable name:                 GHS_COMPILER_ROOT_DIR
#             Environment variable name: SDL_CMAKE_GHS_COMPILER_ROOT_DIR
#
##############################################################################################################################



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


# Specify the hardware interface to be used with AFU/OOCD, typically 'kitprog3.cfg' for Cypress/Infineon MiniProg4/KitProg3 or 'jlink.cfg' for SEGGER J-Link
if(DEFINED ENV{SDL_CMAKE_AFU_OOCD_INTERFACE_CFG_FILE})
    set(defaultAfuOocdInterfaceCfgFile $ENV{SDL_CMAKE_AFU_OOCD_INTERFACE_CFG_FILE})
else()
    set(defaultAfuOocdInterfaceCfgFile "kitprog3.cfg")
endif()
set(AFU_OOCD_INTERFACE_CFG_FILE ${defaultAfuOocdInterfaceCfgFile} CACHE STRING "Infineon Auto Flash Utility / OpenOCD interface configuration file name (typically kitprog3.cfg or jlink.cfg)")


######################################################################################
# MCAL / Tresos (only relevant if USE_MCAL is true)
######################################################################################
# Tresos/MCAL directory (having 'bin', 'plugins', etc. subfolders)
if(DEFINED ENV{SDL_CMAKE_MCAL_VAR_TRESOS_DIR})
    set(defaultMcalVarTresosDir $ENV{SDL_CMAKE_MCAL_VAR_TRESOS_DIR})
else()
    set(defaultMcalVarTresosDir "C:/INFINEON_ESDB/Tresos26_2_0")
endif()
set(MCAL_VAR_TRESOS_DIR ${defaultMcalVarTresosDir} CACHE PATH "Path to Tresos root folder")


# Tresos workspace can be a common folder for multiple projects, if it does not exist, it will be created automatically (Note: it will not be cleaned by the "clean" target)
if(DEFINED ENV{SDL_CMAKE_MCAL_VAR_TRESOS_WORKSPACE_DIR})
    set(defaultMcalVarTresosWorkspaceDir $ENV{SDL_CMAKE_MCAL_VAR_TRESOS_WORKSPACE_DIR})
else()
    set(defaultMcalVarTresosWorkspaceDir "${CMAKE_SOURCE_DIR}/tresos_workspace")
endif()
set(MCAL_VAR_TRESOS_WORKSPACE_DIR ${defaultMcalVarTresosWorkspaceDir} CACHE PATH "Path to Tresos workspace folder")


# Tresos project directory (can be placed anywhere, does not need to be inside the workspace). The default value assumes that you copy the MCAL example folder contents to the DIE's source directory.
if(DEFINED ENV{SDL_CMAKE_MCAL_VAR_TRESOS_PROJECT_DIR})
    set(defaultMcalVarTresosProjectDir $ENV{SDL_CMAKE_MCAL_VAR_TRESOS_PROJECT_DIR})
else()
    set(defaultMcalVarTresosProjectDir "${CMAKE_SOURCE_DIR}/${DIE}/src/${MCAL_VAR_TRESOS_PROJECT_NAME}")
endif()
set(MCAL_VAR_TRESOS_PROJECT_DIR ${defaultMcalVarTresosProjectDir} CACHE PATH "Path to Tresos project folder")


# If Tresos projects not delivered with SDL are used, this option can be helpful if those projects do not adhere to the expected output generation path
if(DEFINED ENV{SDL_CMAKE_AUTOFIX_TRESOS_GEN_PATH})
    set(defaultAutoFixTresosGenPath $ENV{SDL_CMAKE_AUTOFIX_TRESOS_GEN_PATH})
else()
    set(defaultAutoFixTresosGenPath OFF)
endif()
option(AUTOFIX_TRESOS_GEN_PATH "Updates the GenerationPath in pref_general.xdm to expected value (output\\generated)" ${defaultAutoFixTresosGenPath})

