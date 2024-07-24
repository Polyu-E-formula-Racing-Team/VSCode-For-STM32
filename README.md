# VSCode STM32 Development Setup Guide
This guide is written to help new EEE member to quickly setup development environment on their machine.

>Please noted that this guide is first written on 19/12/2022, if any updated is made to the softwares that will be used. 
>Please reference the latest software instruction for configuration details.

## Required Software
* [VS Code](#vs-code)
* [GNU GCC](#gnu-gcc)
* [ARM-none-eabi-gcc](#arm-none-eabi-gcc)
* [OpenOCD](#openocd)
* [STM32 Cube MX](#stm32-cube-mx)

## Installation

### VS Code
Download: <https://code.visualstudio.com/>

**Required Extension**
* C/C++
* Cortex-Debug
* IntelliCode

### GNU GCC
The GNU Compiler Collection (GCC) is an optimizing compiler produced by the GNU Project supporting various programming languages, hardware architectures and operating systems.

Windows
1. Download Minimum GCC for Windows (MinGW) : <https://sourceforge.net/projects/mingw/>
2. Copy the `mingw32-make.exe` file and rename it to `make.exe`. 
3. Add the installation path to system path, i.e. `<Path to your installation>/MinGW/bin/` (See [Appendix](#appendix) for detail)

Linux 
1. Type `apt install gcc` in terminal

Mac (Homebrew/Brew)
1. Type `brew install gcc` in termial


**Minimum required modules**
* gcc
* g++
* make

### ARM-none-eabi-gcc
Arm Embeded ToolChain
Download: <https://developer.arm.com/downloads/-/gnu-rm>

Windows 
1. Add the installation path to system path, i.e. `<Path to your installation>/GNU Arm Embedded Toolchain/bin`
>Noted: There are two `bin` directory in your installation, **DO NOT** use the `bin` inside `arm-none-eabi` folder

### OpenOCD
Open On Chip Debugger

Offical page: <https://openocd.org/>

3rd-party build: <https://github.com/xpack-dev-tools/openocd-xpack/releases>

It is highly recommanded to setup the debugger for troubleshooting.
If you don't need a debugger, than you can skip this part.

Windows 
1. Add the installation path to system path, i.e. `<Path to your installation>/OpenOCD/bin`

Linux
1. Run script `openocdInstall` for install instruction.

### STM32 Cube MX
STM32 Cube eco system tool for setting hardware parameter and build the project template.

Download: <https://www.st.com/en/development-tools/stm32cubemx.html>
>Noted: This software required an vaild email to download, after filling the form, the download link will send to your email.

### Install vaildation
Type the following command to terminal, if you can see the version information than you are fine.
```
make -v
gcc -v
arm-none-eabi-gcc -v
openocd -v
```
## Configuration

### STM32 Cube MX Config
Configuration is stored inside `<ProjectName>.ioc`, do not edit it directly, we only recommended edit it through CubeMX.
Under the Project Manager tab, select Toolchain/IDE as Makefile

### VS Code Config
There are serval files need to be set. Templates are readied for you. [Github]()

**Files**
```
ProjectRoot
|-.vscode
|  |-c_cppproperties.json (Manual create)
|  |-task.json (Manual create)
|  |-launch.json (Manual create)
|
|-build
|-Core
|-Drivers
|-Makefile
|-OpenOCD.cfg (Manual create)
|-...
|-<ProjectName>.ioc
```

#### c_cppproperties.json (filename maybe wrong)
The `c_cppproperties.json` file define the IDE highlight and hints setting, it is not a compousery setting, but better set it properly to have a good experience.

There are many item missed here, most of them can find from the `Makefile` generated from STM32CubeMX.

`IncludePath` and `defines` can find in the `Makefile`, while `CompilerPath` is your ARM-none-eabi-gcc, not regular gcc, otherwise you will miss some header.

**Generic template**
``` json
{
    "configurations": [
        {
            "name": "STM32",
            "includePath": [
                "${workspaceFolder}/**",
                "<Copy the include path from Makefile>"
            ],
            "defines": [
                "_DEBUG",
                "UNICODE",
                "_UNICODE",
                "<Copy the Marco defined in Makefile>"
            ],
            "compilerPath": "<Path to your ARM-none-eabi-gcc>/GNU Arm Embedded Toolchain/bin/arm-none-eabi-gcc.exe",
            "cStandard": "c11",
            "cppStandard": "gnu++14",
            "intelliSenseMode": "windows-gcc-arm"
        }
    ],
    "version": 4
}
```
**Template for STM32H7x**
``` json
{
    "configurations": [
        {
            "name": "STM32",
            "includePath": [
                "${workspaceFolder}/**",
                "Core/Inc",
                "Drivers/STM32H7xx_HAL_Driver/Inc",
                "Drivers/STM32H7xx_HAL_Driver/Inc/Legacy",
                "Drivers/CMSIS/Device/ST/STM32H7xx/Include"
                //"<Other path listed inside Makefile>"
            ],
            "defines": [
                "_DEBUG",
                "UNICODE",
                "_UNICODE",
                "USE_HAL_DRIVER",
                "<STM32 model defined in Makefile>"
            ],
            "compilerPath": "<Path to your ARM-none-eabi-gcc>/GNU Arm Embedded Toolchain/bin/arm-none-eabi-gcc.exe",
            "cStandard": "c11",
            "cppStandard": "gnu++14",
            "intelliSenseMode": "windows-gcc-arm"
        }
    ],
    "version": 4
}
```

#### task.json (filename maybe wrong)
The `task.json` file define the task that vscode automated for you.
```json
{
    // See https://go.microsoft.com/fwlink/?LinkId=733558
    // for the documentation about the tasks.json format
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Build",
            "type": "shell",
            "command": "make",
            "args": [],
            "group": "build",
            
            "detail": "compilerPath: <Path to your ARM-none-eabi-gcc>/GNU Arm Embedded Toolchain/bin/arm-none-eabi-gcc.exe",
            "problemMatcher": "$gcc"
        },
        {
            "label": "Download",
            "type": "shell",
            "command": "openocd",
            "args": [
                "-f",
                "OpenOCD.cfg",
                "-c program ./build/${workspaceFolderBasename}.elf verify reset exit"
            ],
            "group": "build",
            "dependsOn":[
                "Build"
            ],
            "detail": "openocd profile: OpenOCD.cfg",
            "problemMatcher": "$gcc"
        }
    ]
}
```


#### launch.json
The `launch.json` define debug action.
```json
{
    // Use IntelliSense to learn about possible attributes.
    // Hover to view descriptions of existing attributes.
    // For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "cwd": "${workspaceRoot}",
            "executable": "./build/${workspaceFolderBasename}.elf",
            "name": "Debug with OpenOCD",
            "request": "launch",
            "type": "cortex-debug",
            "servertype": "openocd",
            "configFiles": ["OpenOCD.cfg"],
            "searchDir": [],
            "runToEntryPoint": "main",
            "showDevDebugOutput": "both",
            "preLaunchTask": "Build"
        },

    ]
}
```

#### OpenOCD.cfg

```
#debug probe / debug interface
source [find interface/stlink.cfg]
#target mcu / development board
source [find target/stm32<series>x.cfg]
```



## Appendix
* [Add Tool Path to System Path](https://learn.microsoft.com/en-us/previous-versions/office/developer/sharepoint-2010/ee537574(v=office.14))






