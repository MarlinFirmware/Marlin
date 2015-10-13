:: File:     Make.cmd
:: Author:   Ruy Garcia <ruy.garcia@bq.com>
:: Author:   Koldo Imanol de Miguel <koldo.demiguel@bq.com>
:: Purpose:  Configures Windows enviroment to execute bq Makefile
:: Revision: September 2015 - initial version

@ECHO OFF
SETLOCAL ENABLEEXTENSIONS
cls

:: variables
SET me=%~n0

IF NOT DEFINED ProgramFiles(x86) (
	SET "program_files=%ProgramFiles%"
) ELSE (
	SET "program_files=%ProgramFiles(x86)%"
)
ECHO %program_files%

SET "arduino_path=%program_files%\Arduino"
ECHO %arduino_path%

IF EXIST "%arduino_path%" (
	ECHO Arduino installation found: "%arduino_path%"
) ELSE (
	ECHO Arduino installation not found.
	@EXIT /b 1
)

SET "avr_tools=%arduino_path%\hardware\tools\avr\bin"
ECHO "%avr_tools%"

CD Marlin

:: Start of updater menu
:menu1
echo.
echo.
echo BUILD MENU FOR BQ 3D PRINTERS
echo     FOR USE WITH WINDOWS     
echo -----------------------------
echo.
echo	Choose the COM port of your 3d Printer:
echo.
echo	3- COM3
echo	4- COM4
echo.
echo	Q- Exit updater
choice /c:34Q>nul
if errorlevel 3 goto quit
if errorlevel 2 goto com4
if errorlevel 1 goto com3

:com3
SET COM=3
goto menu2

:com4
SET COM=4
goto menu2

:menu2
cls
echo BUILD MENU FOR BQ 3D PRINTERS
echo     FOR USE WITH WINDOWS
echo -----------------------------
echo.
echo	Choose your 3D printer model:
echo.
echo	1- Witbox
echo	2- Witbox 2
echo	3- Hephestos
echo	4- Hephestos XL
echo	5- Hephestos 2
echo.
echo	Q- Exit updater
echo.
choice /c:12345Q>nul
if errorlevel 6 goto quit
if errorlevel 5 goto hep2
if errorlevel 4 goto hepxl
if errorlevel 3 goto heph
if errorlevel 2 goto wit2
if errorlevel 1 goto wit

:hep2
SET TARGET=hephestos_2
goto make

:hepxl
SET TARGET=hephestos_xl
goto make

:heph
SET TARGET=hephestos
goto make

:wit2
SET TARGET=witbox_2
goto make

:wit
SET TARGET=witbox
goto make

:make
::remove applet for correct building after target change
@RD /S /Q ".\applet"
echo.
echo.
CALL "C:\Program Files (x86)\GnuWin32\bin\make.exe" %TARGET%
CALL "C:\Program Files (x86)\GnuWin32\bin\make.exe"
CALL "C:\Program Files (x86)\GnuWin32\bin\make.exe" upload COM=%COM%
goto exit

:quit
echo	Exiting updater
echo.

:exit
