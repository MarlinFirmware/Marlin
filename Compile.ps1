cls
Write-Output 'Clean Up' 
Set-Location -Path 'D:\Users\Miguel\Documents\Trabajos\VSCode\Marlin_Ender3v2-Development'
if (Test-Path -Path '.\.pio\build') { Rename-Item -Path '.\.pio\build' -NewName 'build-' -Force }
if (Test-Path -Path '.\.pio\build-') { Remove-Item '.\.pio\build-' -Recurse }
. $env:USERPROFILE\.platformio\penv\Scripts\platformio.exe run --target clean -e STM32F103RET6_creality
Write-Output ' '
Write-Output ' '
#-------------------------------
Write-Output 'Compiling 422 BLTouch ...'
copy ".\configurations\v4.2.2\BLTouch\*.*" ".\Marlin"
. $env:USERPROFILE\.platformio\penv\Scripts\platformio.exe run -e STM32F103RET6_creality
Write-Output 'Rename and move firmware'
ls '.pio\build\STM32F103RET6_creality\*.bin' | Rename-Item -NewName {$_.name -replace "firmware","Experimental-422-BLTouch5x5"}
Move-Item -Path '.pio\build\STM32F103RET6_creality\*.bin' -Destination '..\Releases'
Write-Output ' '
Write-Output ' '
#-------------------------------
Write-Output 'Compiling 422 ManualMesh ...'
copy ".\configurations\v4.2.2\ManualMesh\*.*" ".\Marlin"
. $env:USERPROFILE\.platformio\penv\Scripts\platformio.exe run -e STM32F103RET6_creality
Write-Output 'Rename and move firmware'
ls '.pio\build\STM32F103RET6_creality\*.bin' | Rename-Item -NewName {$_.name -replace "firmware","Experimental-422-ManualMesh5x5"}
Move-Item -Path '.pio\build\STM32F103RET6_creality\*.bin' -Destination '..\Releases'
Write-Output ' '
Write-Output ' '
#-------------------------------
Write-Output 'Clean Up'
Start-Sleep -Seconds 2
if (Test-Path -Path '.\.pio\build') { Rename-Item -Path '.\.pio\build' -NewName 'build-' -Force }
if (Test-Path -Path '.\.pio\build-') { Remove-Item '.\.pio\build-' -Recurse }
. $env:USERPROFILE\.platformio\penv\Scripts\platformio.exe run --target clean -e STM32F103RET6_creality
Write-Output ' '
Write-Output ' '
#-------------------------------
Write-Output 'Compiling 427 BLTouch ...'
copy ".\configurations\v4.2.7\BLTouch\*.*" ".\Marlin"
. $env:USERPROFILE\.platformio\penv\Scripts\platformio.exe run -e STM32F103RET6_creality
Write-Output 'Rename and move firmware'
ls '.pio\build\STM32F103RET6_creality\*.bin' | Rename-Item -NewName {$_.name -replace "firmware","Experimental-427-BLTouch5x5"}
Move-Item -Path '.pio\build\STM32F103RET6_creality\*.bin' -Destination '..\Releases'
Write-Output ' '
Write-Output ' '
#-------------------------------
Write-Output 'Compiling 427 ManualMesh ...'
copy ".\configurations\v4.2.7\ManualMesh\*.*" ".\Marlin"
. $env:USERPROFILE\.platformio\penv\Scripts\platformio.exe run -e STM32F103RET6_creality
Write-Output 'Rename and move firmware'
ls '.pio\build\STM32F103RET6_creality\*.bin' | Rename-Item -NewName {$_.name -replace "firmware","Experimental-427-ManualMesh5x5"}
Move-Item -Path '.pio\build\STM32F103RET6_creality\*.bin' -Destination '..\Releases'
#-------------------------------
