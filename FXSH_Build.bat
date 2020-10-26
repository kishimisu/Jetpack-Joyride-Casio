@echo off
rem Do not edit! This batch file is created by CASIO fx-9860G SDK.


if exist JETPACK.G1A  del JETPACK.G1A

cd debug
if exist FXADDINror.bin  del FXADDINror.bin
"E:\Program Files\SDK\OS\SH\Bin\Hmake.exe" Addin.mak
cd ..
if not exist debug\FXADDINror.bin  goto error

"E:\Program Files\SDK\Tools\MakeAddinHeader363.exe" "C:\Users\ALEX\Desktop\Jetpack Joyride Casio - LastIndev"
if not exist JETPACK.G1A  goto error
echo Build has completed.
goto end

:error
echo Build was not successful.

:end

