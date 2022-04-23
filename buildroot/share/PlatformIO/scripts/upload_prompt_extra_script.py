#
# upload_prompt_extra_script.py
# set the output_port
#
from __future__ import print_function

from tkinter import Tk, filedialog

import pioutil
if pioutil.is_pio_build():

	import os,getpass,platform

#	current_OS = platform.system()
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

		upload_disk = filedialog.askdirectory(title="Select the root of your SDCARD") # Returns opened path as str
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
