
::  Under Windows, starts two instances (nodes) of the default
::  solution in lab2 of TNM086, using the configuration that sets up
::  VRPN tracking,

call C:\VRSystem\setenv-tnm086-lab2.bat

rem start .\build_win\main.exe --config urn:gramods:config/se.liu.vortex.workbench-secondary-3DTV.xml
rem start .\build_win\main.exe --config urn:gramods:config/se.liu.vortex.workbench-master.xml
.\build_win\Release\main.exe --config urn:gramods:config/se.liu.vortex.workbench-3DTV-single.xml

pause
