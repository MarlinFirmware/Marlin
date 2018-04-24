
import sys
import os

num_args = len(sys.argv)
if num_args > 1:
  build_type = str(sys.argv[1])
else:
  print 'Please specify build type'
  exit()

print '\nWorking\n'

python_ver = sys.version_info[0] # major version - 2 or 3

print python_ver
#print "python version " + chr(sys.version_info[0] + 48) + "." + chr(sys.version_info[1] + 48) + "." + chr(sys.version_info[2] + 48)

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


'''The SuperText class inherits from Tkinter's Text class and provides added
functionality to a standard text widget:

- Option to turn scrollbar on/off
- Right-click pop-up menu
- Two themes included (terminal and typewriter)

Compliant with Python 2.5-2.7

Author: @ifthisthenbreak
'''
#import Tkinter as tk
from Tkinter import Tk, Frame, Text, Scrollbar, Menu
from tkMessageBox import askokcancel
import tkFileDialog

class SuperText(Text):
    global write_to_screen




    def __init__(self, parent, scrollbar=True, **kw):

        self.parent = parent

        frame = Frame(parent)
        frame.pack(fill='both', expand=True)

        # text widget
        Text.__init__(self, frame, borderwidth=3, relief="sunken", **kw)
#        tab_width = self.font.measure(' ' * 40)  # compute desired width of tabs
        self.config(tabs=(400,))  # configure Text widget tab stops
        self.pack(side='left', fill='both', expand=True)

        self.tag_config('normal', foreground = 'white')
        self.tag_config('warning', foreground = 'yellow' )
        self.tag_config('error', foreground = 'red')
        self.tag_config('highlight_green', foreground = 'green')
        self.tag_config('highlight_blue', foreground = 'cyan')

#        self.bind('<Control-a>', self.select_all)  # the event happens but the action doesn't

        # scrollbar
        if scrollbar:
            scrb = Scrollbar(frame, orient='vertical', command=self.yview)
            self.config(yscrollcommand=scrb.set)
            scrb.pack(side='right', fill='y')



        # pop-up menu
        self.popup = Menu(self, tearoff=0)
        self.popup.add_command(label='Cut', command=self._cut)
        self.popup.add_command(label='Copy', command=self._copy)
        self.popup.add_command(label='Paste', command=self._paste)
        self.popup.add_separator()
        self.popup.add_command(label='Select All', command=self._select_all)
        self.popup.add_command(label='Clear All', command=self._clear_all)
        self.popup.add_separator()
        self.popup.add_command(label='Save As', command=self._file_save_as)
        self.bind('<Button-3>', self._show_popup)

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

        if self.parent.focus_get() != self:
            self.focus_set()

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


    def _clear_all(self):
        '''erases all text'''

        isok = askokcancel('Clear All', 'Erase all text?', parent=self,
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

  # helper function
    def write_to_screen(self, text, format_tag = 'normal'):
        self.insert('end', text, format_tag)
# end - SuperText



#
# popup to get input from user
#

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


#  env:teensy35
#  env:teensy20
#  env:sanguino_atmega644p
#  env:sanguino_atmega1284p
#  env:rambo
#  env:melzi_optiboot
#  env:melzi
#  env:megaatmega2560
#  env:megaatmega1280
#  env:STM32F1
#  env:LPC1768_debug_and_upload
#  env:LPC1768
#  env:DUE_debug
#  env:DUE_USB
#  env:DUE

def env_name_check(argument):
      name_check = {
        'teensy35'                   :  True,
        'teensy20'                   :  True,
        'sanguino_atmega644p'        :  True,
        'sanguino_atmega1284p'       :  True,
        'rambo'                      :  True,
        'melzi_optiboot'             :  True,
        'melzi'                      :  True,
        'megaatmega2560'             :  True,
        'megaatmega1280'             :  True,
        'STM32F1'                    :  True,
        'LPC1768_debug_and_upload'   :  True,
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
      print 'get_starting_env Marln version: ', version, '\npath: ', path
      with open(path, 'r') as myfile:
        pins_h = myfile.read()

      board_name = board_name_full[ 6 : ]  # only use the part after "BOARD_" since we're searching the pins.h file
      print board_name_full, board_name
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
          print lines + '\n' + cpu_line
          comment_start = cpu_line.find('// ')
          env_A, next_position = get_env_from_line(cpu_line, comment_start)  # get name of environment & start of search for next
          env_B, next_position = get_env_from_line(cpu_line, next_position)  # get next environment, if it exists
          env_C, next_position = get_env_from_line(cpu_line, next_position)  # get next environment, if it exists
          break
      print '\nenv_A: ', env_A, '\nenv_B: ', env_B, '\nenv_C: ', env_C
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
          print 'get_env Marlin version: ', ver_Marlin
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

              print 'get_answer_val: ' , get_answer_val
              if 2 == get_answer_val:
                target_env = env_B
              else:
                target_env = env_A
              print 'target_env: ' , target_env
              return target_env

          if env_A == 'LPC1768':
              if build_type == 'traceback' or (build_type == 'clean' and get_build_last() == 'LPC1768_debug_and_upload'):
                target_env = 'LPC1768_debug_and_upload'
              else:
                target_env = 'LPC1768'
          elif env_A == 'DUE':
              target_env = 'DUE'
              print 'get_build_last: ', get_build_last()
              if build_type == 'traceback' or (build_type == 'clean' and get_build_last() == 'DUE_debug'):
                  target_env = 'DUE_debug'
              elif env_B == 'DUE_USB':
                get_answer(board_name, "DUE: need download port", "USB (native USB) port", "Programming port       ")
                print 'get_answer_val: ', get_answer_val
                if 1 == get_answer_val:
                  target_env = 'DUE_USB'
                else:
                  target_env = 'DUE'
                print 'DUE env after USB choice: ', target_env

              print 'DUE env: ', target_env
          else:
              invalid_board()

      if build_type == 'traceback' and not(target_env == 'LPC1768_debug_and_upload' or target_env == 'DUE_debug')  and Marlin_ver == 2:
          print "ERROR - this board isn't setup for traceback"
          print 'board_name: ', board_name
          print 'target_env: ', target_env
          raise SystemExit(0)

      return target_env
# end - get_env


def run_Platformio(target_env):

      ##########################################################################
      #                                                                        #
      # run Platformio                                                         #
      #                                                                        #
      ##########################################################################


      #  build      platformio run -e  target_env
      #  clean      platformio run --target clean -e  target_env
      #  upload     platformio run --target upload -e  target_env
      #  program    platformio run --target program -e  target_env
      #  test       platformio test upload -e  target_env
      #  remote     platformio remote run --target upload -e  target_env
      #  debug      platformio debug -e  target_env



    def Platformio_run(dummy):  # this function is run as a separate thread so that a progress bar can be displayed
      import subprocess         # getting the results from the thread requires the use of the queue module
      import re
      import os

      print 'starting platformio'
      pio_subprocess = ''
      if build_type == 'build':
            # combine stdout & stderr so all compile messages are included
            pio_subprocess = subprocess.Popen(['platformio', 'run', '-e', target_env], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)




      if build_type == 'clean':
            #platformio run --target clean -e  target_env
            # combine stdout & stderr so all compile messages are included
            pio_subprocess = subprocess.Popen(['platformio', 'run', '--target', 'clean', '-e', target_env], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)


      if build_type == 'upload':
            #platformio run --target upload -e  target_env
            # combine stdout & stderr so all compile messages are included
            pio_subprocess = subprocess.Popen(['platformio', 'run', '--target', 'upload', '-e',  target_env], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)



      if build_type == 'traceback':
            #platformio run --target upload -e  target_env  - select the debug environment if there is one
            # combine stdout & stderr so all compile messages are included
            pio_subprocess = subprocess.Popen(['platformio', 'run', '--target', 'upload', '-e', target_env], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)



      if build_type == 'program':
            #platformio run --target program -e  target_env
            # combine stdout & stderr so all compile messages are included
            pio_subprocess = subprocess.Popen(['platformio', 'run', '--target', 'program', '-e',  target_env], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)



      if build_type == 'test':
            #platformio test upload -e  target_env
            # combine stdout & stderr so all compile messages are included
            pio_subprocess = subprocess.Popen(['platformio', 'test', 'upload', '-e', target_env], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)



      if build_type == 'remote':
            #platformio remote run --target upload -e  target_env
            # combine stdout & stderr so all compile messages are included
            pio_subprocess = subprocess.Popen(['platformio', 'remote', 'run', '--target', 'program', '-e',  target_env], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)



      if build_type == 'debug':
            #platformio debug -e  target_env
            # combine stdout & stderr so all compile messages are included
            pio_subprocess = subprocess.Popen(['platformio', 'debug', '-e', target_env], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)


      pio_subprocess = pio_subprocess.communicate()[0]  # turn it into a text/string object

      return pio_subprocess

    #
    # starts a subprocess in a different thread and displays a progress bar while it executes
    #   must manually enter the name of the subprocess
    #

    import time
    import threading
    import Tkinter as tk
    import ttk
    import Queue
    que = Queue.Queue()
    time_start = time.time()

    class start_subprocess_with_progress_bar(object):

        def __init__(self):
            self.root = tk.Tk()

            self.int_var = tk.IntVar()
            self.progbar = ttk.Progressbar(self.root, maximum=400,)
            # associate self.int_var with the progress value
            self.i = 0
            self.progbar['variable'] = self.i
            self.progbar.pack()
            self.elapsed_time = tk.IntVar()
            self.elapsed_time.set('0:00')
            self.label_time = ttk.Label(self.root, textvariable=self.elapsed_time, foreground = 'blue', font = ("consolas", 12, 'bold'))
            self.label_time.pack()
            self.label = ttk.Label(self.root, text='PLEASE BE PATIENT - no further output until Platformio finishes (could be minutes)', foreground = 'blue', font = ("consolas", 12, 'bold'))
            self.label.pack()

        def start_thread(self):
            self.int_var.set(0) # empty the Progressbar
            # create then start a secondary thread to run arbitrary()
            #  must have at least one argument
#                    self.secondary_thread = threading.Thread(target = lambda q, arg1: q.put(arbitrary(arg1)), args=(que, ''))
            self.secondary_thread = threading.Thread(target = lambda q, arg1: q.put(Platformio_run(arg1)), args=(que, ''))
            self.secondary_thread.start()
            # check the Queue in 50ms
            self.root.after(50, self.check_thread)
            self.root.after(50, self.update)

        def check_thread(self):
            if self.secondary_thread.is_alive():
              self.root.after(50, self.check_thread)
            else:
                self.root.destroy()

        def update(self):
            #Updates the progressbar
            self.progbar["value"] = self.i
            self.i = self.i + 1
            if self.i > 395:
              self.i = 0
            if self.secondary_thread.is_alive():
                self.root.after(50, self.update)#method is called all 50ms
            time_dif = time.time() - time_start

            self.elapsed_time.set(time.strftime("%M:%S", time.gmtime(time_dif)))
    # end - progress bar

    pio = start_subprocess_with_progress_bar()
    pio.start_thread()
    pio.root.mainloop()

    pio_log = que.get()  # get value returned by threaded subprocess
    return  pio_log
# end -   run_Platformio


#
#  send the output to the terminal screen with syntax highlighting
#

def format_print_log(log, board_name, target_env):

      import re
      import os

    # open output window
      root = Tk()
      auto_build = SuperText(root, scrollbar=True)
      auto_build.pack(fill='both', expand=True)
      options = {'bg': 'black', 'fg': 'white', 'font': ("consolas", 12), 'wrap' : 'word', 'undo' : 'True'}
      auto_build.config(options)

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
              write_to_screen(auto_build,text[            : found_1 + 1      ])
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
                  write_to_screen(auto_build,text[found_1 + 1 : found_space + 1     ], highlight[2])
                  write_to_screen(auto_build,text[found_space + 1 : found + 1     ])
                  write_to_screen(auto_build,text[found + 1   : found_right], highlight_2[2])
                  write_to_screen(auto_build,text[found_right :                ] + '\n')
                  break
              break
            if 1 == highlight[1]:
              found_right = text.find(']', found + 1)
              write_to_screen(auto_build,text[               : found + 1   ])
              write_to_screen(auto_build,text[found + 1      : found_right ], highlight[2])
              write_to_screen(auto_build,text[found_right :                ] + '\n')
            break
        if did_something == False:
          write_to_screen(auto_build,text + '\n')
      # end - write_to_screen_with_replace


      first_write = True

      log_split = log.split('\n')
      log_split_len = len(log_split)

      warning = False
      warning_FROM = False
      error = False
      standard = True
      prev_line_COM = False
      next_line_warning = False
      warning_continue = False
      for i in xrange(0,log_split_len):
        max_search = len(log_split[i])
        if max_search > 3 :
          max_search = 3
        beginning = log_split[i][:max_search]

        # set flags
        if 0 < log_split[i].find(': warning: '): # start of warning block
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
        elif 0 < log_split[i].find(': error:') or \
          0 < log_split[i].find(': fatal error:'):       # start of warning /error block
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
          write_to_screen_with_replace(log_split[i], platformio_highlights)   #print white on black with substitutions
        if warning == True:
          write_to_screen(auto_build, log_split[i] + '\n', 'warning')
        if error == True:
          write_to_screen(auto_build, log_split[i] + '\n', 'error')

      write_to_screen(auto_build, '\nBoard name: ' + board_name  + '\n')  # put build info at the bottom of the screen
      write_to_screen(auto_build, 'Build type: ' + build_type  + '\n')
      write_to_screen(auto_build, 'Environment used: ' + target_env  + '\n')

      root.mainloop()
# end - format_print_log


def auto_build():


  ##########################################################################
  #                                                                        #
  # main program                                                           #
  #                                                                        #
  ##########################################################################


        board_name, Marlin_ver = get_board_name()

        target_env = get_env(board_name, Marlin_ver)

        print 'board_name: ', board_name
        print 'target_env: ', target_env
        print '\n' + 'PLEASE BE PATIENT - no further output until Platformio finishes (could be minutes)' + '\n'

        format_print_log(run_Platformio(target_env), board_name, target_env)


if __name__ == '__main__':

    auto_build()
