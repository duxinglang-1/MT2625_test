MediaTek LinkIt(TM) Development Platform for RTOS provides a comprehensive software solution for devices based
on the MT2625.

1. Getting started
   The SDK package supports GCC tool chains. Follow the instructions at <sdk_root>/doc/LinkIt_for_RTOS_Get_Started_Guide.pdf
   to build your first project and run it. Release notes are also under the <sdk_root>/doc folder.

2. Folder structure
  \doc                      ->  SDK documents, including get started guide, developer's guide, software and environment guide, 
                            ->  API reference manual, and SDK release notes.
  \driver\board             ->  Drivers for the development board
  \driver\chip              ->  Chipset modules, such as GPIO, I2C, Bluetooth Low Energy.
  \driver\CMSIS             ->  CMSIS interface drivers.
  \kernel                   ->  OS / system service
  \middleware\MTK           ->  MediaTek middleware. Read readme.txt in each module for details. 
  \middleware\third_party   ->  Open source software, such as cjson, fatfs, httpd. Read readme.txt in each module for details.
  \project\<board>          ->  Example projects of the current SDK. Read <sdk_root>/project/readme.txt for more details.
  \tools                    ->  Script, generation script, gcc compiler. If there isn't a gcc compiler under the /tools/gcc folder,
                            ->  extract the tool package to the root folder of the SDK with the following command.
                            ->       7za x SDK_VX.Y.Z_tool_chain.7z -o<sdk_root>
                            **  Please make sure you have /tools/gcc before you build the SDK under linux environment. **


