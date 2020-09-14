@echo off
set /p ufwver="Enter Unified Version Number: "

"C:\Program Files\7-Zip\7z.exe" a -tZip D:\Work\UF2Packages\TH3D_Unified2_CrealityV422-UFW_%ufwver%.zip D:\Personal\Documents\GitHub\UnifiedFirmware\Firmware
"C:\Program Files\7-Zip\7z.exe" a -tZip D:\Work\UF2Packages\TH3D_Unified2_CrealityV422-UFW_%ufwver%.zip "D:\Personal\Documents\GitHub\UnifiedFirmware\Board Configuration Files\CrealityV422\Firmware"

"C:\Program Files\7-Zip\7z.exe" a -tZip D:\Work\UF2Packages\TH3D_Unified2_CrealityV427-UFW_%ufwver%.zip D:\Personal\Documents\GitHub\UnifiedFirmware\Firmware
"C:\Program Files\7-Zip\7z.exe" a -tZip D:\Work\UF2Packages\TH3D_Unified2_CrealityV427-UFW_%ufwver%.zip "D:\Personal\Documents\GitHub\UnifiedFirmware\Board Configuration Files\CrealityV427\Firmware"

"C:\Program Files\7-Zip\7z.exe" a -tZip D:\Work\UF2Packages\TH3D_Unified2_SKR_E3_Mini-UFW_%ufwver%.zip D:\Personal\Documents\GitHub\UnifiedFirmware\Firmware
"C:\Program Files\7-Zip\7z.exe" a -tZip D:\Work\UF2Packages\TH3D_Unified2_SKR_E3_Mini-UFW_%ufwver%.zip "D:\Personal\Documents\GitHub\UnifiedFirmware\Board Configuration Files\SKR_E3_Mini\Firmware"

"C:\Program Files\7-Zip\7z.exe" a -tZip D:\Work\UF2Packages\TH3D_Unified2_TH3D_EZBoardLite-UFW_%ufwver%.zip D:\Personal\Documents\GitHub\UnifiedFirmware\Firmware
"C:\Program Files\7-Zip\7z.exe" a -tZip D:\Work\UF2Packages\TH3D_Unified2_TH3D_EZBoardLite-UFW_%ufwver%.zip "D:\Personal\Documents\GitHub\UnifiedFirmware\Board Configuration Files\TH3D_EZBoardLite\Firmware"