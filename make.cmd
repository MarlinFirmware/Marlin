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

SET "arduino_path=c:\Arduino"

IF EXIST "%arduino_path%" (
	ECHO Arduino installation found: "%arduino_path%"
) ELSE (
	ECHO Arduino installation not found in "%arduino_path%".
	goto exit
)

SET "avr_tools=%arduino_path%\hardware\tools\avr\bin"
ECHO "%avr_tools%"

CD Marlin

:: Start of updater menu
:menu_device
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
echo	5- Hephestos ZUM
echo	6- Hephestos ZUM XL
echo	7- Hephestos 2
echo.
echo	Q- Exit updater
echo.
choice /c:1234567Q>nul
if errorlevel 8 goto quit
if errorlevel 7 goto hep2
if errorlevel 6 goto hepzxl
if errorlevel 5 goto hepz
if errorlevel 4 goto hepxl
if errorlevel 3 goto heph
if errorlevel 2 goto wit2
if errorlevel 1 goto wit

:menu_language
cls
echo BUILD MENU FOR BQ 3D PRINTERS
echo     FOR USE WITH WINDOWS
echo -----------------------------
echo.
echo	Choose the language:
echo.
echo	1- DE - German
echo	2- EN - English
echo	3- ES - Spanish
echo	4- FI - Finnish
echo	5- FR - French
echo	6- IT - Italian
echo	7- NL - Dutch
echo	8- PL - Polish
echo	9- PT - Portuguse
echo.
echo	Q- Exit updater
echo.
choice /c:123456789Q>nul
if errorlevel 10 goto quit
if errorlevel 9 goto language_pt
if errorlevel 8 goto language_pl
if errorlevel 7 goto language_nl
if errorlevel 6 goto language_it
if errorlevel 5 goto language_fr
if errorlevel 4 goto language_fi
if errorlevel 3 goto language_es
if errorlevel 2 goto language_en
if errorlevel 1 goto language_de

:: Device targets
:hep2
SET TARGET=hephestos_2
goto language_en

:hepzxl
SET TARGET=hephestos_zum_xl
goto menu_language

:hepz
SET TARGET=hephestos_zum
goto menu_language

:hepxl
SET TARGET=hephestos_xl
goto menu_language

:heph
SET TARGET=hephestos
goto menu_language

:wit2
SET TARGET=witbox_2
goto language_en

:wit
SET TARGET=witbox
goto menu_language

:: Language targets
:language_de
SET LANGUAGE=DE
goto make

:language_en
SET LANGUAGE=EN
goto make

:language_es
SET LANGUAGE=ES
goto make

:language_fi:
SET LANGUAGE=FI
goto make

:language_fr
SET LANGUAGE=FR
goto make

:language_it
SET LANGUAGE=IT
goto make

:language_nl
SET LANGUAGE=NL
goto make

:language_pl
SET LANGUAGE=PL
goto make

:language_pt
SET LANGUAGE=PT
goto make

:make
echo.
echo.
SET /p COM=Enter the COM port for your arduino board: COM
CALL "C:\Program Files (x86)\GnuWin32\bin\make.exe" cleanall
CALL "C:\Program Files (x86)\GnuWin32\bin\make.exe" %TARGET% %LANGUAGE%
CALL "C:\Program Files (x86)\GnuWin32\bin\make.exe"
CALL "C:\Program Files (x86)\GnuWin32\bin\make.exe" upload COM=%COM%
CALL "C:\Program Files (x86)\GnuWin32\bin\make.exe" cleanall
goto exit

:quit
echo	Exiting updater

:exit
echo.
set /p EXCH=Press enter to exit...
