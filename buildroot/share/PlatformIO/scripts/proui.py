# ------------------------------------------------------------------------------
# VSCode script for the Professional Firmware
# URL: https://github.com/mriscoc/Marlin_Ender3v2/releases
# Version: 3.1
# Date: 2023/10/28
# Author: Miguel Risco-Castillo
# ------------------------------------------------------------------------------

import shutil
import os

libpath = "Marlin/lib/proui/"
libfile = libpath+'libproui.a'

Import("env")

def _GetMarlinEnv(marlinEnv, feature):
  if not marlinEnv: return None
  return any(item.startswith(feature) for item in marlinEnv)

# Get Marlin evironment vars
if "MARLIN_FEATURES" in env:
  print("Processing Professional Firmware requirements")
  MarlinEnv = env['MARLIN_FEATURES']
  dwin = _GetMarlinEnv(MarlinEnv, 'DWIN_LCD_PROUI')
  ultipanel = _GetMarlinEnv(MarlinEnv, 'IS_ULTIPANEL')
  prouiex = _GetMarlinEnv(MarlinEnv, 'PROUI_EX')
  marlin_manualmesh = _GetMarlinEnv(MarlinEnv, 'MESH_BED_LEVELING')
  marlin_abl = _GetMarlinEnv(MarlinEnv, 'AUTO_BED_LEVELING_BILINEAR')
  marlin_ubl = _GetMarlinEnv(MarlinEnv, 'AUTO_BED_LEVELING_UBL')
  stm32f1 = _GetMarlinEnv(MarlinEnv, 'MCU_STM32F1')
  stm32f4 = _GetMarlinEnv(MarlinEnv, 'MCU_STM32F4')
  stm32g0 = _GetMarlinEnv(MarlinEnv, 'MCU_STM32G0')
  simulat = _GetMarlinEnv(MarlinEnv, '__PLAT_NATIVE_SIM__')

# Select correct architecture
  if stm32f1:
    arch = 'stm32f1/'
    print ('STM32F1 Architecture detected')
  elif stm32f4:
    arch = 'stm32f4/'
    print ('STM32F4 Architecture detected')
  elif stm32g0:
    arch = 'stm32g0/'
    print ('STM32G0 Architecture detected')
  elif simulat:
    arch = 'simulat/'
    print ('Simulated Architecture detected')
  else:
    print("Error: can't detect the correct architecture")
    exit()

#Select correct ui
  if dwin:
    ui = 'dwin'
    print ('DWIN UI detected')
  elif ultipanel:
    ui = 'ultipanel'
    print ('Ultipanel UI detected')
  else:
    print("Error: can't detect the correct UI")
    exit()

#Select correct leveling system
  if marlin_manualmesh:
    lev = 'mbl'
    print("Manual Mesh Bed Leveling detected")
  elif marlin_abl:
    lev = 'abl'
    print("Auto Mesh Bed Leveling detected")
  elif marlin_ubl:
    lev = 'ubl'
    print("Unified Mesh Bed Leveling detected")
  else:
    print("Error: can't detect a supported leveling system")
    exit()

#Copy correct library file
  if prouiex or dwin:
    print('ProUI detected')
    shutil.copy(libpath+arch+'libproui_'+lev+'_'+ui+'.a', libfile)
