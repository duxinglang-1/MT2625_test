@echo off
set option=%1%
set ewspath=%2%
set currentpath=%~dp0
"%currentpath%\jre1.8_win\bin\java.exe" -jar "%currentpath%\EPT\ept_lib\ept_fwk.jar" %option% %ewspath%