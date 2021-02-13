#
# pre-compile-checks.py
# Script to check for common issues prior to compiling
#
import os
import re

try:
	import configparser
except ImportError:
	import ConfigParser as configparser

Import("env")
config = configparser.ConfigParser()

#print(env.Dump())

#
# Load Marlin features
#
def load_marlin_features():
  if 'MARLIN_FEATURES' in env:
    return
  else:
    raise SystemExit("MARLIN_FEATURES not in env:")

#
# extract first environment name found after the start position
#   return: environment name and position to start the next search from
def get_env_from_line(line, start_position):
  env = ''
  next_position = -1
  env_position = line.find('env:', start_position)
  if 0 < env_position:
    next_position = line.find(' ', env_position + 4)
    if 0 < next_position:
      env = line[env_position + 4:next_position]
    else:
      env = line[env_position + 4:]  # at the end of the line
  return env, next_position

#
# scan pins.h for board name and return the environment(s) found (up to 9 environments)
#
def get_starting_env(board_name_full):
  # get environment starting point

  path = 'Marlin/src/pins/pins.h'
  with open(path, 'r') as myfile:
    pins_h = myfile.read()

  env_A, env_B, env_C, env_D, env_E, env_F, env_G, env_H, env_I = ('',)*9

  board_name = "[( ]"+board_name_full[6:]+"[,)]" # only use the part after "BOARD_" since we're searching the pins.h file
  pins_h = pins_h.split('\n')
  environment = ''
  board_line = ''
  cpu_A = ''
  cpu_B = ''
  i = 0
  list_start_found = False
  for lines in pins_h:
    i = i + 1  # i is always one ahead of the index into pins_h
    if 0 < lines.find("Unknown MOTHERBOARD value set in Configuration.h"):
      break  #  no more
    if 0 < lines.find('1280'):
      list_start_found = True
    if list_start_found == False:  # skip lines until find start of CPU list
      continue
    comment_start = lines.find('// ')
    cpu_A_loc = comment_start
    cpu_B_loc = 0
    if re.search(board_name,lines):
      cpu_line = pins_h[i]
      comment_start = cpu_line.find('// ')
      env_A, next_position = get_env_from_line(cpu_line, comment_start)  # get name of environment & start of search for next
      env_B, next_position = get_env_from_line(cpu_line, next_position)  # get next environment, if it exists
      env_C, next_position = get_env_from_line(cpu_line, next_position)  # get next environment, if it exists
      env_D, next_position = get_env_from_line(cpu_line, next_position)  # get next environment, if it exists
      env_E, next_position = get_env_from_line(cpu_line, next_position)  # get next environment, if it exists
      env_F, next_position = get_env_from_line(cpu_line, next_position)  # get next environment, if it exists
      env_G, next_position = get_env_from_line(cpu_line, next_position)  # get next environment, if it exists
      env_H, next_position = get_env_from_line(cpu_line, next_position)  # get next environment, if it exists
      env_I, next_position = get_env_from_line(cpu_line, next_position)  # get next environment, if it exists
      break
  return env_A, env_B, env_C, env_D, env_E, env_F, env_G, env_H, env_I

#
# Revove the prefix from the text
#
def remove_prefix(text, prefix):
    if text.startswith(prefix):
        return text[len(prefix):]

#
# Resursivly check the environments are ok for for the board name
#
def check_build_env(board_name,env_name):
  #print(env_name)
  env_A, env_B, env_C, env_D, env_E, env_F, env_G, env_H, env_I = get_starting_env(board_name)
  if env_name in [env_A, env_B, env_C, env_D, env_E, env_F, env_G, env_H, env_I]:
    return True
  else:
    if (config['env:'+env_name]['extends'].find('env:') != -1):  #not done yet
      check_build_env(board_name,remove_prefix(config['env:'+env_name]['extends'],'env:'))
    else:
      err = 'You have MOTHERBOARD set to ' + str(board_name) + ', please use default_envs = ' + str(env_A)
      if not env_B == '':
        err += ', or default_envs = ' + str(env_B)
      if not env_C == '':
        err += ', or default_envs = ' + str(env_C)
      if not env_D == '':
        err += ', or default_envs = ' + str(env_D)
      if not env_E == '':
        err += ', or default_envs = ' + str(env_E)
      if not env_F == '':
        err += ', or default_envs = ' + str(env_F)
      if not env_G == '':
        err += ', or default_envs = ' + str(env_G)
      if not env_H == '':
        err += ', or default_envs = ' + str(env_H)
      if not env_I == '':
        err += ', or default_envs = ' + str(env_I)
      raise SystemExit(err)

#
# Start checking env is corect for board name
#
def check_suitable_env():
  board_name = env['MARLIN_FEATURES']['MOTHERBOARD']
  env_name = env['PIOENV']
  check_build_env(board_name,env_name)

#
# Check for Configfiles in two common incorrect places
#
def check_configfile_locations():
	for p in [ env['PROJECT_DIR'], os.path.join(env['PROJECT_DIR'], "config") ]:
		for f in [ "Configuration.h", "Configuration_adv.h" ]:
			if os.path.isfile(os.path.join(p, f)):
				err = 'ERROR: Config files found in directory ' + str(p) + '. Please move them into the Marlin subdirectory.'
				raise SystemExit(err)

#
# Setup
#
config.read("platformio.ini")
load_marlin_features()

#
# Do these checks
#
check_configfile_locations()
check_suitable_env()

