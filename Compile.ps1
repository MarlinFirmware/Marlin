cls
Write-Output 'Clean Up' 
Set-Location -Path 'D:\Users\Miguel\Documents\Trabajos\VSCode\Marlin_Ender3v2-Development'
Rename-Item -Path '.\.pio\build' -NewName 'build-' -Force
Remove-Item '.\.pio\build-' -Recurse
C:\Users\Miguel\.platformio\penv\Scripts\platformio run --target clean -e STM32F103RET6_creality
Write-Output ' '
Write-Output ' '
#-------------------------------
Write-Output 'Compiling 422 BLTouch ...'
copy ".\configurations\v4.2.2\BLTouch\*.*" ".\Marlin"
C:\Users\Miguel\.platformio\penv\Scripts\platformio run -e STM32F103RET6_creality
Write-Output 'Rename and move firmware'
ls '.pio\build\STM32F103RET6_creality\*.bin' | Rename-Item -NewName {$_.name -replace "firmware","Experimental-422-BLTouch5x5"}
Move-Item -Path '.pio\build\STM32F103RET6_creality\*.bin' -Destination '..\Releases'
Write-Output ' '
Write-Output ' '
#-------------------------------
Write-Output 'Compiling 422 ManualMesh ...'
copy ".\configurations\v4.2.2\ManualMesh\*.*" ".\Marlin"
C:\Users\Miguel\.platformio\penv\Scripts\platformio run -e STM32F103RET6_creality
Write-Output 'Rename and move firmware'
ls '.pio\build\STM32F103RET6_creality\*.bin' | Rename-Item -NewName {$_.name -replace "firmware","Experimental-422-ManualMesh5x5"}
Move-Item -Path '.pio\build\STM32F103RET6_creality\*.bin' -Destination '..\Releases'
Write-Output ' '
Write-Output ' '
#-------------------------------
Write-Output 'Clean Up'
Start-Sleep -Seconds 2
Rename-Item -Path '.\.pio\build' -NewName 'build-' -Force
Remove-Item '.\.pio\build-' -Recurse
C:\Users\Miguel\.platformio\penv\Scripts\platformio run --target clean -e STM32F103RET6_creality
Write-Output ' '
Write-Output ' '
#-------------------------------
Write-Output 'Compiling 427 BLTouch ...'
copy ".\configurations\v4.2.7\BLTouch\*.*" ".\Marlin"
C:\Users\Miguel\.platformio\penv\Scripts\platformio run -e STM32F103RET6_creality
Write-Output 'Rename and move firmware'
ls '.pio\build\STM32F103RET6_creality\*.bin' | Rename-Item -NewName {$_.name -replace "firmware","Experimental-427-BLTouch5x5"}
Move-Item -Path '.pio\build\STM32F103RET6_creality\*.bin' -Destination '..\Releases'
Write-Output ' '
Write-Output ' '
#-------------------------------
Write-Output 'Compiling 427 ManualMesh ...'
copy ".\configurations\v4.2.7\ManualMesh\*.*" ".\Marlin"
C:\Users\Miguel\.platformio\penv\Scripts\platformio run -e STM32F103RET6_creality
Write-Output 'Rename and move firmware'
ls '.pio\build\STM32F103RET6_creality\*.bin' | Rename-Item -NewName {$_.name -replace "firmware","Experimental-427-ManualMesh5x5"}
Move-Item -Path '.pio\build\STM32F103RET6_creality\*.bin' -Destination '..\Releases'
#-------------------------------
