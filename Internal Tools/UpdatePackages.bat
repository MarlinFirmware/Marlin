@echo off
set /p newufwver="Enter New Unified Version Number: "
set /p ufwver="Enter Old Unified Version Number: "

"C:\Program Files\7-Zip\7z.exe" d -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_CrealityV42X-UFW_%ufwver%.zip"
ping localhost -n 10 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_CrealityV42X-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\Firmware"
ping localhost -n 5 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_CrealityV42X-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\STL Files"
ping localhost -n 5 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_CrealityV42X-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\LCD Firmware"
ping localhost -n 5 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_CrealityV42X-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\TH3D Installation Guides"
ping localhost -n 5 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_CrealityV42X-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\README.md"
ping localhost -n 5 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_CrealityV42X-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\Board Configuration Files\CrealityV42X\Firmware"
ping localhost -n 10 >NUL
rename "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_CrealityV42X-UFW_%ufwver%.zip" TH3D_Unified2_CrealityV42X-UFW_%newufwver%.zip

"C:\Program Files\7-Zip\7z.exe" d -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_SKR_E3_Mini-UFW_%ufwver%.zip"
ping localhost -n 10 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_SKR_E3_Mini-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\Firmware"
ping localhost -n 5 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_SKR_E3_Mini-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\STL Files"
ping localhost -n 5 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_SKR_E3_Mini-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\TH3D Installation Guides"
ping localhost -n 5 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_SKR_E3_Mini-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\README.md"
ping localhost -n 5 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_SKR_E3_Mini-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\Board Configuration Files\SKR_E3_Mini\Firmware"
ping localhost -n 10 >NUL
rename "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_SKR_E3_Mini-UFW_%ufwver%.zip" TH3D_Unified2_SKR_E3_Mini-UFW_%newufwver%.zip

"C:\Program Files\7-Zip\7z.exe" d -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_TH3D_EZBoardLite-UFW_%ufwver%.zip"
ping localhost -n 10 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_TH3D_EZBoardLite-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\Firmware"
ping localhost -n 5 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_TH3D_EZBoardLite-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\STL Files"
ping localhost -n 5 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_TH3D_EZBoardLite-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\TH3D Installation Guides"
ping localhost -n 5 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_TH3D_EZBoardLite-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\EZBoard Installation Guide"
ping localhost -n 5 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_TH3D_EZBoardLite-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\Pre-Compiled Binaries\EZBoard Pre-Compiled Firmware"
ping localhost -n 5 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_TH3D_EZBoardLite-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\README.md"
ping localhost -n 5 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_TH3D_EZBoardLite-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\Board Configuration Files\TH3D_EZBoardLite\Firmware"
ping localhost -n 10 >NUL
rename "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_TH3D_EZBoardLite-UFW_%ufwver%.zip" TH3D_Unified2_TH3D_EZBoardLite-UFW_%newufwver%.zip

"C:\Program Files\7-Zip\7z.exe" d -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_MKS_Robin_Mini-UFW_%ufwver%.zip"
ping localhost -n 10 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_MKS_Robin_Mini-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\Firmware"
ping localhost -n 5 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_MKS_Robin_Mini-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\STL Files"
ping localhost -n 5 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_MKS_Robin_Mini-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\TH3D Installation Guides"
ping localhost -n 5 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_MKS_Robin_Mini-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\README.md"
ping localhost -n 5 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_MKS_Robin_Mini-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\Board Configuration Files\MKS_Robin_Mini\Firmware"
ping localhost -n 10 >NUL
rename "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_MKS_Robin_Mini-UFW_%ufwver%.zip" TH3D_Unified2_MKS_Robin_Mini-UFW_%newufwver%.zip

"C:\Program Files\7-Zip\7z.exe" d -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_Melzi_Boards-UFW_%ufwver%.zip"
ping localhost -n 10 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_Melzi_Boards-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\Firmware"
ping localhost -n 5 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_Melzi_Boards-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\STL Files"
ping localhost -n 5 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_Melzi_Boards-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\TH3D Installation Guides"
ping localhost -n 5 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_Melzi_Boards-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\README.md"
ping localhost -n 5 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_Melzi_Boards-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\Board Configuration Files\Melzi_Boards\Firmware"
ping localhost -n 10 >NUL
rename "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_Melzi_Boards-UFW_%ufwver%.zip" TH3D_Unified2_Melzi_Boards-UFW_%newufwver%.zip

"C:\Program Files\7-Zip\7z.exe" d -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_Ramps_Boards-UFW_%ufwver%.zip"
ping localhost -n 10 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_Ramps_Boards-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\Firmware"
ping localhost -n 5 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_Ramps_Boards-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\STL Files"
ping localhost -n 5 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_Ramps_Boards-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\TH3D Installation Guides"
ping localhost -n 5 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_Ramps_Boards-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\README.md"
ping localhost -n 5 >NUL
"C:\Program Files\7-Zip\7z.exe" a -tZip "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_Ramps_Boards-UFW_%ufwver%.zip" "D:\Personal\Documents\GitHub\UnifiedFirmware\Board Configuration Files\Ramps_Boards\Firmware"
ping localhost -n 10 >NUL
rename "D:\Work\TH3D Studio LLC\TH3D Internal Site - Shared Folders\Firmware\Web Downloads\Unified2\TH3D_Unified2_Ramps_Boards-UFW_%ufwver%.zip" TH3D_Unified2_Ramps_Boards-UFW_%newufwver%.zip