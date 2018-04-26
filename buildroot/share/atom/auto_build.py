#######################################
#
# Marlin 3D Printer Firmware
# Copyright (C) 2018 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
#
# Based on Sprinter and grbl.
# Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#######################################

#######################################
#
# Description: script to automate PlatformIO builds
# CLI:  python auto_build.py build_option
#    build_option (required)
#        build      executes ->  platformio run -e  target_env
#        clean      executes ->  platformio run --target clean -e  target_env
#        upload     executes ->  platformio run --target upload -e  target_env
#        traceback  executes ->  platformio run --target upload -e  target_env
#        program    executes ->  platformio run --target program -e  target_env
#        test       executes ->  platformio test upload -e  target_env
#        remote     executes ->  platformio remote run --target upload -e  target_env
#        debug      executes ->  platformio debug -e  target_env
#
# 'traceback' just uses the debug variant of the target environment if one exists
#
#######################################

#######################################
#
# General program flow
#
#  1. Scans Configuration.h for the motherboard name and Marlin version.
#  2. Scans pins.h for the motherboard.
#       returns the CPU(s) and platformio environment(s) used by the motherboard
#  3. If further info is needed then a popup gets it from the user.
#  4. The OUTPUT_WINDOW class creates a window to display the output of the PlatformIO program.
#  5. A thread is created by the OUTPUT_WINDOW class in order to execute the RUN_PIO function.
#  6. The RUN_PIO function uses a subprocess to run the CLI version of PlatformIO.
#  7. The "iter(pio_subprocess.stdout.readline, '')" function is used to stream the output of
#     PlatformIO back to the RUN_PIO function.
#  8. Each line returned from PlatformIO is formatted to match the color coding seen in the
#     PlatformIO GUI.
#  9. If there is a color change within a line then the line is broken at each color change
#     and sent separately.
# 10. Each formatted segment (could be a full line or a split line) is put into the queue
#     IO_queue as it arrives from the platformio subprocess.
# 11. The OUTPUT_WINDOW class periodically samples IO_queue.  If data is available then it
#     is written to the window.
# 12. The window stays open until the user closes it.
# 13. The OUTPUT_WINDOW class continues to execute as long as the window is open.  This allows
#     copying, saving, scrolling of the window.  A right click popup is available.
#
#######################################

import sys
import os

num_args = len(sys.argv)
if num_args > 1:
  build_type = str(sys.argv[1])
else:
  print 'Please specify build type'
  exit()

print'build_type:  ', build_type

print '\nWorking\n'

python_ver = sys.version_info[0] # major version - 2 or 3

if python_ver == 2:
  print "python version " + str(sys.version_info[0]) + "." + str(sys.version_info[1]) + "." + str(sys.version_info[2])
else:
  print "python version " + str(sys.version_info[0])
  print "This script only runs under python 2"
  exit()

#########
#  Python 2 error messages:
#    Can't find a usable init.tcl in the following directories ...
#    error "invalid command name "tcl_findLibrary""
#
#  Fix for the above errors on my Win10 system:
#    search all init.tcl files for the line "package require -exact Tcl" that has the highest 8.5.x number
#    copy it into the first directory listed in the error messages
#    set the environmental variables TCLLIBPATH and TCL_LIBRARY to the directory where you found the init.tcl file
#    reboot
#########

#globals
target_env = ''
board_name = ''



##########################################################################################
#
# popup to get input from user
#
##########################################################################################

def get_answer(board_name, cpu_label_txt, cpu_a_txt, cpu_b_txt):


        if python_ver == 2:
          import Tkinter as tk
        else:
          import tkinter as tk

        def CPU_exit_3():   # forward declare functions

          CPU_exit_3_()
        def CPU_exit_4():

          CPU_exit_4_()
        def kill_session():
          kill_session_()

        root_get_answer = tk.Tk()

        root_get_answer.chk_state_1 = 1   # declare variables used by TK and enable

        chk_state_1 = 0   # set initial state of check boxes


        global get_answer_val
        get_answer_val = 2       # return get_answer_val, set default to match chk_state_1 default


        l1 = tk.Label(text=board_name,
              fg = "light green",
              bg = "dark green",
              font = "Helvetica 12 bold").grid(row=1)

        l2 = tk.Label(text=cpu_label_txt,
              fg = "light green",
              bg = "dark green",
              font = "Helvetica 16 bold italic").grid(row=2)

        b4 = tk.Checkbutton(text=cpu_a_txt,
              fg = "black",
              font = "Times 20 bold ",
                variable=chk_state_1, onvalue=1, offvalue=0,

                command = CPU_exit_3).grid(row=3)

        b5 = tk.Checkbutton(text=cpu_b_txt,
              fg = "black",
              font = "Times 20 bold ",
                variable=chk_state_1, onvalue=0, offvalue=1,

                command = CPU_exit_4).grid(row=4)  # use same variable but inverted so they will track
        b6 = tk.Button(text="CONFIRM",
              fg = "blue",
              font = "Times 20 bold ",
                command = root_get_answer.destroy).grid(row=5, pady=4)

        b7 = tk.Button(text="CANCEL",
              fg = "red",
              font = "Times 12 bold ",
                command = kill_session).grid(row=6, pady=4)


        def CPU_exit_3_():
                global get_answer_val
                get_answer_val = 1

        def CPU_exit_4_():
                global get_answer_val
                get_answer_val = 2

        def kill_session_():
                raise SystemExit(0)     # kill everything

        root_get_answer.mainloop()

# end - get answer



def env_name_check(argument):
      name_check = {
        'teensy35'                   :  True,
        'teensy20'                   :  True,
        'STM32F4'                    :  True,
        'STM32F1'                    :  True,
        'sanguino_atmega644p'        :  True,
        'sanguino_atmega1284p'       :  True,
        'rambo'                      :  True,
        'melzi_optiboot'             :  True,
        'melzi'                      :  True,
        'megaatmega2560'             :  True,
        'megaatmega1280'             :  True,
        'malyanm200'                 :  True,
        'LPC1768'                    :  True,
        'DUE_debug'                  :  True,
        'DUE_USB'                    :  True,
        'DUE'                        :  True
      }

      return name_check.get(argument, False)


# gets the last build environment
def get_build_last():
      env_last = ''
      DIR_PWD = os.listdir('.')
      if '.pioenvs' in DIR_PWD:
        date_last = 0.0
        DIR__pioenvs = os.listdir('.pioenvs')
        for name in DIR__pioenvs:
          if env_name_check(name):
            DIR_temp = os.listdir('.pioenvs/' + name)
            for names_temp in DIR_temp:
              if 0 == names_temp.find('firmware.'):
                date_temp = os.path.getmtime('.pioenvs/' + name + '/' + names_temp)
                if date_temp > date_last:
                  date_last = date_temp
                  env_last = name
      return env_last


# gets the board being built from the Configuration.h file
#   returns: board name, major version of Marlin being used (1 or 2)
def get_board_name():
      board_name = ''
      # get board name

      with open('Marlin/Configuration.h', 'r') as myfile:
        Configuration_h = myfile.read()

      Configuration_h = Configuration_h.split('\n')
      Marlin_ver = 0  # set version to invalid number
      for lines in Configuration_h:
        if 0 == lines.find('#define CONFIGURATION_H_VERSION 01'):
          Marlin_ver = 1
        if 0 == lines.find('#define CONFIGURATION_H_VERSION 02'):
          Marlin_ver = 2
        board = lines.find(' BOARD_') + 1
        motherboard = lines.find(' MOTHERBOARD ') + 1
        define = lines.find('#define ')
        comment = lines.find('//')
        if (comment == -1 or comment > board) and \
          board > motherboard and \
          motherboard > define and \
          define >= 0 :
          spaces = lines.find(' ', board)  # find the end of the board substring
          if spaces == -1:
            board_name = lines[board : ]
          else:
            board_name = lines[board : spaces]
          break


      return board_name, Marlin_ver


# extract first environment name it finds after the start position
#   returns: environment name and position to start the next search from
def get_env_from_line(line, start_position):
      env = ''
      next_position = -1
      env_position = line.find('env:', start_position)
      if 0 < env_position:
        next_position = line.find(' ', env_position + 4)
        if 0 < next_position:
          env = line[env_position + 4 : next_position]
        else:
          env = line[env_position + 4 :              ]    # at the end of the line
      return env, next_position



#scans pins.h for board name and returns the environment(s) it finds
def get_starting_env(board_name_full, version):
      # get environment starting point

      if version == 1:
        path = 'Marlin/pins.h'
      if version == 2:
        path = 'Marlin/src/pins/pins.h'
      with open(path, 'r') as myfile:
        pins_h = myfile.read()

      board_name = board_name_full[ 6 : ]  # only use the part after "BOARD_" since we're searching the pins.h file
      pins_h = pins_h.split('\n')
      environment = ''
      board_line = ''
      cpu_A = ''
      cpu_B = ''
      i = 0
      list_start_found = False
      for lines in pins_h:
        i = i + 1   # i is always one ahead of the index into pins_h
        if 0 < lines.find("Unknown MOTHERBOARD value set in Configuration.h"):
          break   #  no more
        if 0 < lines.find('1280'):
          list_start_found = True
        if list_start_found == False:  # skip lines until find start of CPU list
          continue
        board = lines.find(board_name)
        comment_start = lines.find('// ')
        cpu_A_loc = comment_start
        cpu_B_loc = 0
        if board > 0:  # need to look at the next line for environment info
          cpu_line = pins_h[i]
          comment_start = cpu_line.find('// ')
          env_A, next_position = get_env_from_line(cpu_line, comment_start)  # get name of environment & start of search for next
          env_B, next_position = get_env_from_line(cpu_line, next_position)  # get next environment, if it exists
          env_C, next_position = get_env_from_line(cpu_line, next_position)  # get next environment, if it exists
          break
      return env_A, env_B, env_C


# scans input string for CPUs that the users may need to select from
#   returns: CPU name
def get_CPU_name(environment):
          CPU_list = ('1280', '2560','644', '1284', 'LPC1768', 'DUE')
          CPU_name = ''
          for CPU in CPU_list:
            if 0 < environment.find(CPU):
              return CPU


# get environment to be used for the build
#  returns: environment
def get_env(board_name, ver_Marlin):
      def no_environment():
            print 'ERROR - no environment for this board'
            print board_name
            raise SystemExit(0)                          # no environment so quit

      def invalid_board():
            print 'ERROR - invalid board'
            print board_name
            raise SystemExit(0)                          # quit if unable to find board


      CPU_question = ( ('1280', '2560', "1280 or 2560 CPU?"), ('644', '1284', "644 or 1284 CPU?") )

      if 0 < board_name.find('MELZI') :
          get_answer(board_name, "Which flavor of Melzi?", "Melzi (Optiboot bootloader)", "Melzi                                      ")
          if 1 == get_answer_val:
            target_env = 'melzi_optiboot'
          else:
            target_env = 'melzi'
      else:
          env_A, env_B, env_C = get_starting_env(board_name, ver_Marlin)

          if env_A == '':
            no_environment()
          if env_B == '':
            return env_A      # only one environment so finished

          CPU_A = get_CPU_name(env_A)
          CPU_B = get_CPU_name(env_B)

          for item in CPU_question:
            if CPU_A == item[0]:
              get_answer(board_name, item[2], item[0], item[1])
              if 2 == get_answer_val:
                target_env = env_B
              else:
                target_env = env_A
              return target_env

          if env_A == 'LPC1768':
              if build_type == 'traceback' or (build_type == 'clean' and get_build_last() == 'LPC1768_debug_and_upload'):
                target_env = 'LPC1768_debug_and_upload'
              else:
                target_env = 'LPC1768'
          elif env_A == 'DUE':
              target_env = 'DUE'
              if build_type == 'traceback' or (build_type == 'clean' and get_build_last() == 'DUE_debug'):
                  target_env = 'DUE_debug'
              elif env_B == 'DUE_USB':
                get_answer(board_name, "DUE: need download port", "USB (native USB) port", "Programming port       ")
                if 1 == get_answer_val:
                  target_env = 'DUE_USB'
                else:
                  target_env = 'DUE'
          else:
              invalid_board()

      if build_type == 'traceback' and not(target_env == 'LPC1768_debug_and_upload' or target_env == 'DUE_debug')  and Marlin_ver == 2:
          print "ERROR - this board isn't setup for traceback"
          print 'board_name: ', board_name
          print 'target_env: ', target_env
          raise SystemExit(0)

      return target_env
# end - get_env

# puts screen text into queue so that the parent thread can fetch the data from this thread
import Queue
IO_queue = Queue.Queue()
def write_to_screen_queue(text, format_tag = 'normal'):
      double_in = [text, format_tag]
      IO_queue.put(double_in, block = False)


#
#  send one line to the terminal screen with syntax highlighting
#
# input: unformatted text, flags from previous run
# returns: formatted text ready to go to the terminal, flags from this run
#
# This routine remembers the status from call to call because previous
# lines can affect how the current line is highlighted
#

# 'static' variables - init here and then keep updating them from within print_line
warning = False
warning_FROM = False
error = False
standard = True
prev_line_COM = False
next_line_warning = False
warning_continue = False

def line_print(line_input):

      global warning
      global warning_FROM
      global error
      global standard
      global prev_line_COM
      global next_line_warning
      global warning_continue




      # all '0' elements must precede all '1' elements or they'll be skipped
      platformio_highlights = [
              ['Environment', 0, 'highlight_blue'],
              ['[SKIP]', 1, 'warning'],
              ['[ERROR]', 1, 'error'],
              ['[SUCCESS]', 1, 'highlight_green']
      ]

      def write_to_screen_with_replace(text, highlights):  # search for highlights & split line accordingly
        did_something = False
        for highlight in highlights:
          found = text.find(highlight[0])
          if did_something == True:
            break
          if found >= 0 :
            did_something = True
            if 0 == highlight[1]:
              found_1 = text.find(' ')
              found_tab = text.find('\t')
              if found_1 < 0 or found_1 > found_tab:
                found_1 = found_tab
              write_to_screen_queue(text[            : found_1 + 1      ])
              for highlight_2 in highlights:
                if  highlight[0] == highlight_2[0] :
                  continue
                found = text.find(highlight_2[0])
                if found >= 0 :
                  found_space = text.find(' ', found_1 + 1)
                  found_tab = text.find('\t', found_1 + 1)
                  if found_space < 0 or found_space > found_tab:
                    found_space = found_tab
                  found_right = text.find(']', found + 1)
                  write_to_screen_queue(text[found_1 + 1 : found_space + 1     ], highlight[2])
                  write_to_screen_queue(text[found_space + 1 : found + 1     ])
                  write_to_screen_queue(text[found + 1   : found_right], highlight_2[2])
                  write_to_screen_queue(text[found_right :                ] + '\n')
                  break
              break
            if 1 == highlight[1]:
              found_right = text.find(']', found + 1)
              write_to_screen_queue(text[               : found + 1   ])
              write_to_screen_queue(text[found + 1      : found_right ], highlight[2])
              write_to_screen_queue(text[found_right :                ] + '\n')
            break
        if did_something == False:
          write_to_screen_queue(text + '\n')
      # end - write_to_screen_with_replace



    # scan the line
      max_search = len(line_input)
      if max_search > 3 :
        max_search = 3
      beginning = line_input[:max_search]

      # set flags
      if 0 < line_input.find(': warning: '): # start of warning block
        warning = True
        warning_FROM = False
        error = False
        standard = False
        prev_line_COM = False
        prev_line_COM = False
        warning_continue = True
      if beginning == 'War' or \
        beginning == '#er' or \
        beginning == 'In ' or \
        (beginning != 'Com' and prev_line_COM == True and not(beginning == 'Arc' or beginning == 'Lin'  or beginning == 'Ind') or \
        next_line_warning == True):
        warning = True                #warning found
        warning_FROM = False
        error = False
        standard = False
        prev_line_COM = False
      elif beginning == 'Com' or \
        beginning == 'Ver' or \
        beginning == ' [E' or \
        beginning == 'Rem' or \
        beginning == 'Bui' or \
        beginning == 'Ind' or \
        beginning == 'PLA':
        warning = False               #standard line found
        warning_FROM = False
        error = False
        standard = True
        prev_line_COM = False
        warning_continue = False
      elif beginning == '***':
        warning = False               # error found
        warning_FROM = False
        error = True
        standard = False
        prev_line_COM = False

      elif beginning == 'fro' and warning == True :  # start of warning /error block
        warning_FROM = True
        prev_line_COM = False
        warning_continue = True
      elif 0 < line_input.find(': error:') or \
        0 < line_input.find(': fatal error:'):       # start of warning /error block
        warning = False                                 # error found
        warning_FROM = False
        error = True
        standard = False
        prev_line_COM = False
        warning_continue = True
      elif warning_continue == True:
        warning = True
        warning_FROM = False          # keep the warning status going until find a standard line
        error = False
        standard = False
        prev_line_COM = False
        warning_continue = True

      else:
        warning = False               # unknown so assume standard line
        warning_FROM = False
        error = False
        standard = True
        prev_line_COM = False
        warning_continue = False

      if beginning == 'Com':
        prev_line_COM = True

    # print based on flags
      if standard == True:
        write_to_screen_with_replace(line_input, platformio_highlights)   #print white on black with substitutions
      if warning == True:
        write_to_screen_queue(line_input + '\n', 'warning')
      if error == True:
        write_to_screen_queue(line_input + '\n', 'error')
# end - line_print



def run_PIO(dummy):

    ##########################################################################
    #                                                                        #
    # run Platformio                                                         #
    #                                                                        #
    ##########################################################################


    #  build      platformio run -e  target_env
    #  clean      platformio run --target clean -e  target_env
    #  upload     platformio run --target upload -e  target_env
    #  traceback  platformio run --target upload -e  target_env
    #  program    platformio run --target program -e  target_env
    #  test       platformio test upload -e  target_env
    #  remote     platformio remote run --target upload -e  target_env
    #  debug      platformio debug -e  target_env


    global build_type
    global target_env
    global board_name
    print 'build_type:  ', build_type

    import subprocess
    import sys
    print 'starting platformio'

    if   build_type == 'build':
          # platformio run -e  target_env
          # combine stdout & stderr so all compile messages are included
          pio_subprocess = subprocess.Popen(['platformio', 'run', '-e', target_env], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)


    elif build_type == 'clean':
          # platformio run --target clean -e  target_env
          # combine stdout & stderr so all compile messages are included
          pio_subprocess = subprocess.Popen(['platformio', 'run', '--target', 'clean', '-e', target_env], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)


    elif build_type == 'upload':
          # platformio run --target upload -e  target_env
          # combine stdout & stderr so all compile messages are included
          pio_subprocess = subprocess.Popen(['platformio', 'run', '--target', 'upload', '-e',  target_env], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)


    elif build_type == 'traceback':
          # platformio run --target upload -e  target_env  - select the debug environment if there is one
          # combine stdout & stderr so all compile messages are included
          pio_subprocess = subprocess.Popen(['platformio', 'run', '--target', 'upload', '-e', target_env], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)


    elif build_type == 'program':
          # platformio run --target program -e  target_env
          # combine stdout & stderr so all compile messages are included
          pio_subprocess = subprocess.Popen(['platformio', 'run', '--target', 'program', '-e',  target_env], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)


    elif build_type == 'test':
          #platformio test upload -e  target_env
          # combine stdout & stderr so all compile messages are included
          pio_subprocess = subprocess.Popen(['platformio', 'test', 'upload', '-e', target_env], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)


    elif build_type == 'remote':
          # platformio remote run --target upload -e  target_env
          # combine stdout & stderr so all compile messages are included
          pio_subprocess = subprocess.Popen(['platformio', 'remote', 'run', '--target', 'program', '-e',  target_env], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)


    elif build_type == 'debug':
          # platformio debug -e  target_env
          # combine stdout & stderr so all compile messages are included
          pio_subprocess = subprocess.Popen(['platformio', 'debug', '-e', target_env], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)


    else:
          print 'ERROR - unknown build type:  ', build_type
          raise SystemExit(0)     # kill everything

  # stream output from subprocess and split it into lines
    for line in iter(pio_subprocess.stdout.readline, ''):
        line_print(line.replace('\n', ''))


  # append info used to run PlatformIO
    write_to_screen_queue('\nBoard name: ' + board_name  + '\n')  # put build info at the bottom of the screen
    write_to_screen_queue('Build type: ' + build_type  + '\n')
    write_to_screen_queue('Environment used: ' + target_env  + '\n')
# end - run_PIO


########################################################################

import time
import threading
import Tkinter as tk
import ttk
import Queue
import subprocess
import sys
que = Queue.Queue()
#IO_queue = Queue.Queue()

from Tkinter import Tk, Frame, Text, Scrollbar, Menu
from tkMessageBox import askokcancel

import tkFileDialog
from tkMessageBox import askokcancel
import tkFileDialog



class output_window(Text):

    global continue_updates
    continue_updates = True


    def  __init__(self):


        self.root = tk.Tk()
        self.frame = tk.Frame(self.root)
        self.frame.pack(fill='both', expand=True)

        # text widget
        #self.text = tk.Text(self.frame, borderwidth=3, relief="sunken")
        Text.__init__(self, self.frame, borderwidth=3, relief="sunken")
        self.config(tabs=(400,))  # configure Text widget tab stops
        self.config(background = 'black', foreground = 'white', font= ("consolas", 12), wrap = 'word', undo = 'True')
        self.config(height  = 24, width = 120)
        self.pack(side='left', fill='both', expand=True)

        self.tag_config('normal', foreground = 'white')
        self.tag_config('warning', foreground = 'yellow' )
        self.tag_config('error', foreground = 'red')
        self.tag_config('highlight_green', foreground = 'green')
        self.tag_config('highlight_blue', foreground = 'cyan')

#        self.bind('<Control-Key-a>', self.select_all)  # the event happens but the action doesn't

        # scrollbar

        scrb = tk.Scrollbar(self.frame, orient='vertical', command=self.yview)
        self.config(yscrollcommand=scrb.set)
        scrb.pack(side='right', fill='y')


        # pop-up menu
        self.popup = tk.Menu(self, tearoff=0)
        self.popup.add_command(label='Cut', command=self._cut)
        self.popup.add_command(label='Copy', command=self._copy)
        self.popup.add_command(label='Paste', command=self._paste)
        self.popup.add_separator()
        self.popup.add_command(label='Select All', command=self._select_all)
        self.popup.add_command(label='Clear All', command=self._clear_all)
        self.popup.add_separator()
        self.popup.add_command(label='Save As', command=self._file_save_as)
        self.bind('<Button-3>', self._show_popup)


  # threading & subprocess section

    def start_thread(self, ):
        global continue_updates
        # create then start a secondary thread to run an arbitrary function
        #  must have at least one argument
        self.secondary_thread = threading.Thread(target = lambda q, arg1: q.put(run_PIO(arg1)), args=(que, ''))
        self.secondary_thread.start()
        continue_updates = True
        # check the Queue in 50ms
        self.root.after(50, self.check_thread)
        self.root.after(50, self.update)


    def check_thread(self):  # wait for user to kill the window
        global continue_updates
        if continue_updates == True:
          self.root.after(20, self.check_thread)


    def update(self):
        global continue_updates
        if continue_updates == True:
           self.root.after(20, self.update)#method is called every 50ms
        temp_text = ['0','0']
        if IO_queue.empty():
          if not(self.secondary_thread.is_alive()):
            continue_updates = False  # queue is exhausted and thread is dead so no need for further updates
            self.tag_add('sel', '1.0', 'end')
        else:
          try:
              temp_text = IO_queue.get(block = False)
          except Queue.Empty:
              continue_updates = False  # queue is exhausted so no need for further updates
          else:
              self.insert('end', temp_text[0], temp_text[1])
              self.see("end")  # make the last line visible (scroll text off the top)


  # text editing section

    def _file_save_as(self):
        self.filename = tkFileDialog.asksaveasfilename(defaultextension = '.txt')
        f = open(self.filename, 'w')
        f.write(self.get('1.0', 'end'))
        f.close()



    def copy(self, event):
        try:
            selection = self.get(*self.tag_ranges('sel'))
            self.clipboard_clear()
            self.clipboard_append(selection)
        except TypeError:
            pass

    def cut(self, event):

        try:
            selection = self.get(*self.tag_ranges('sel'))
            self.clipboard_clear()
            self.clipboard_append(selection)
            self.delete(*self.tag_ranges('sel'))
        except TypeError:
            pass

    def _show_popup(self, event):
        '''right-click popup menu'''

        if self.root.focus_get() != self:
            self.root.focus_set()

        try:
            self.popup.tk_popup(event.x_root, event.y_root, 0)
        finally:
            self.popup.grab_release()

    def _cut(self):

        try:
            selection = self.get(*self.tag_ranges('sel'))
            self.clipboard_clear()
            self.clipboard_append(selection)
            self.delete(*self.tag_ranges('sel'))
        except TypeError:
            pass

    def cut(self, event):
        _cut(self)

    def _copy(self):

        try:
            selection = self.get(*self.tag_ranges('sel'))
            self.clipboard_clear()
            self.clipboard_append(selection)
        except TypeError:
            pass

    def copy(self, event):
        _copy(self)

    def _paste(self):

        self.insert('insert', self.selection_get(selection='CLIPBOARD'))

    def _select_all(self):
        self.tag_add('sel', '1.0', 'end')


    def select_all(self, event):
        self.tag_add('sel', '1.0', 'end')


    def _clear_all(self):
        '''erases all text'''

        isok = askokcancel('Clear All', 'Erase all text?', frame=self,
                           default='ok')
        if isok:
            self.delete('1.0', 'end')

    def _place_cursor(self): # theme: terminal
        '''check the position of the cursor against the last known position
        every 15ms and update the cursorblock tag as needed'''

        current_index = self.index('insert')

        if self.cursor != current_index:
            self.cursor = current_index
            self.tag_delete('cursorblock')

            start = self.index('insert')
            end = self.index('insert+1c')

            if start[0] != end[0]:
                self.insert(start, ' ')
                end = self.index('insert')

            self.tag_add('cursorblock', start, end)
            self.mark_set('insert', self.cursor)

        self.after(15, self._place_cursor)

    def _blink_cursor(self): # theme: terminal
        '''alternate the background color of the cursorblock tagged text
        every 600 milliseconds'''

        if self.switch == self.fg:
            self.switch = self.bg
        else:
            self.switch = self.fg

        self.tag_config('cursorblock', background=self.switch)

        self.after(600, self._blink_cursor)
# end - output_window



def main():


  ##########################################################################
  #                                                                        #
  # main program                                                           #
  #                                                                        #
  ##########################################################################

        global build_type
        global target_env
        global board_name

        board_name, Marlin_ver = get_board_name()

        target_env = get_env(board_name, Marlin_ver)

        auto_build = output_window()
        auto_build.start_thread()  # executes the "run_PIO" function

        auto_build.root.mainloop()




if __name__ == '__main__':

    main()
