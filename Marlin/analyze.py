#!/usr/bin/env python

from __future__ import print_function
import subprocess
import sys
import json

def main():
  output = subprocess.check_output(["./calc", sys.argv[1]])
  progress = []
  result = {}
  first_filament = None
  last_filament = None
  for line in output.split("\n"):
    if not line:
      continue
    (filepos, filament, time) = map(float, line.split(","))
    if filament > 0 and not first_filament:
      first_filament = filepos
    if not last_filament or filament > last_filament:
      last_filament = filepos
    progress.append([filepos, time])
  result["firstFilament"] = first_filament
  result["lastFilament"] = last_filament
  total_time = progress[-1][1]
  most_recent_progress = float("-inf")
  result["progress"] = [[0,0]]
  for progress_entry in progress:
    if (most_recent_progress+60 < progress_entry[1] or
        progress_entry[0] == first_filament or
        progress_entry[0] == last_filament):
      most_recent_progress = progress_entry[1]
      result["progress"].append(
          [progress_entry[0],
           total_time-progress_entry[1]])
  print(json.dumps(result))

if __name__ == "__main__":
  main()
