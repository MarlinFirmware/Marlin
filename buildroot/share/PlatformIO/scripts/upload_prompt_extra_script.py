#
# upload_prompt_extra_script.py
# set the output_port
#
from __future__ import print_function

has_tkinter = False
try:
	import sys
	if sys.version_info[0] < 3:
		import Tkinter as tk
		import tkFileDialog as fileDialog
		from Tkinter import Tk
	else:
		import tkinter as tk
		from tkinter import Tk
		from tkinter import filedialog as fileDialog
	has_tkinter = True
except:
	pass

import pioutil
if has_tkinter and pioutil.is_pio_build():

	Import("env")

	def print_error(e):
		print('\nUnable to find destination disk (%s)\n' %( e ) )

	def before_upload(source, target, env):
		#
		# Find a disk for upload
		#
		upload_disk = ''

		root = Tk() # pointing root to Tk() to use it as Tk() in program.
		root.withdraw() # Hides small tkinter window.

		root.attributes('-topmost', True) # Opened windows will be active. above all windows despite of selection.

		upload_disk = fileDialog.askdirectory(title="Select the root of your SDCARD") # Returns opened path as str
		if not upload_disk:
			print_error('Canceled')
			return
		else:
			env.Replace(
				UPLOAD_FLAGS="-P$UPLOAD_PORT"
			)
			env.Replace(UPLOAD_PORT=upload_disk)
			print('\nUpload disk: ', upload_disk, '\n')

	env.AddPreAction("upload", before_upload)
