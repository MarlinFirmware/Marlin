#!/usr/bin/env python
"""
    Patch utility to apply unified diffs

    Brute-force line-by-line non-recursive parsing 

    Copyright (c) 2008-2016 anatoly techtonik
    Available under the terms of MIT license

"""
from __future__ import print_function

__author__ = "anatoly techtonik <techtonik@gmail.com>"
__version__ = "1.16"
__license__ = "MIT"
__url__ = "https://github.com/techtonik/python-patch"

import copy
import logging
import re

# cStringIO doesn't support unicode in 2.5
try:
  from StringIO import StringIO
except ImportError:
  from io import BytesIO as StringIO # python 3
try:
  import urllib2 as urllib_request
except ImportError:
  import urllib.request as urllib_request

from os.path import exists, isfile, abspath
import os
import posixpath
import shutil
import sys


PY3K = sys.version_info >= (3, 0)

# PEP 3114
if not PY3K:
  compat_next = lambda gen: gen.next()
else:
  compat_next = lambda gen: gen.__next__()

def tostr(b):
  """ Python 3 bytes encoder. Used to print filename in
      diffstat output. Assumes that filenames are in utf-8.
  """
  if not PY3K:
    return b

  # [ ] figure out how to print non-utf-8 filenames without
  #     information loss
  return b.decode('utf-8')    


#------------------------------------------------
# Logging is controlled by logger named after the
# module name (e.g. 'patch' for patch.py module)

logger = logging.getLogger(__name__)

debug = logger.debug
info = logger.info
warning = logger.warning

class NullHandler(logging.Handler):
  """ Copied from Python 2.7 to avoid getting
      `No handlers could be found for logger "patch"`
      http://bugs.python.org/issue16539
  """
  def handle(self, record):
    pass
  def emit(self, record):
    pass
  def createLock(self):
    self.lock = None

streamhandler = logging.StreamHandler()

# initialize logger itself
logger.addHandler(NullHandler())

debugmode = False

def setdebug():
  global debugmode, streamhandler

  debugmode = True
  loglevel = logging.DEBUG
  logformat = "%(levelname)8s %(message)s"
  logger.setLevel(loglevel)

  if streamhandler not in logger.handlers:
    # when used as a library, streamhandler is not added
    # by default
    logger.addHandler(streamhandler)

  streamhandler.setFormatter(logging.Formatter(logformat))


#------------------------------------------------
# Constants for Patch/PatchSet types

DIFF = PLAIN = "plain"
GIT = "git"
HG = MERCURIAL = "mercurial"
SVN = SUBVERSION = "svn"
# mixed type is only actual when PatchSet contains
# Patches of different type
MIXED = MIXED = "mixed"


#------------------------------------------------
# Helpers (these could come with Python stdlib)

# x...() function are used to work with paths in
# cross-platform manner - all paths use forward
# slashes even on Windows.

def xisabs(filename):
  """ Cross-platform version of `os.path.isabs()`
      Returns True if `filename` is absolute on
      Linux, OS X or Windows.
  """
  if filename.startswith(b'/'):     # Linux/Unix
    return True
  elif filename.startswith(b'\\'):  # Windows
    return True
  elif re.match(b'\\w:[\\\\/]', filename): # Windows
    return True
  return False

def xnormpath(path):
  """ Cross-platform version of os.path.normpath """
  # replace escapes and Windows slashes
  normalized = posixpath.normpath(path).replace(b'\\', b'/')
  # fold the result
  return posixpath.normpath(normalized)

def xstrip(filename):
  """ Make relative path out of absolute by stripping
      prefixes used on Linux, OS X and Windows.

      This function is critical for security.
  """
  while xisabs(filename):
    # strip windows drive with all slashes
    if re.match(b'\\w:[\\\\/]', filename):
      filename = re.sub(b'^\\w+:[\\\\/]+', b'', filename)
    # strip all slashes
    elif re.match(b'[\\\\/]', filename):
      filename = re.sub(b'^[\\\\/]+', b'', filename)
  return filename

#-----------------------------------------------
# Main API functions

def fromfile(filename):
  """ Parse patch file. If successful, returns
      PatchSet() object. Otherwise returns False.
  """
  patchset = PatchSet()
  debug("reading %s" % filename)
  fp = open(filename, "rb")
  res = patchset.parse(fp)
  fp.close()
  if res == True:
    return patchset
  return False


def fromstring(s):
  """ Parse text string and return PatchSet()
      object (or False if parsing fails)
  """
  ps = PatchSet( StringIO(s) )
  if ps.errors == 0:
    return ps
  return False


def fromurl(url):
  """ Parse patch from an URL, return False
      if an error occured. Note that this also
      can throw urlopen() exceptions.
  """
  ps = PatchSet( urllib_request.urlopen(url) )
  if ps.errors == 0:
    return ps
  return False


# --- Utility functions ---
# [ ] reuse more universal pathsplit()
def pathstrip(path, n):
  """ Strip n leading components from the given path """
  pathlist = [path]
  while os.path.dirname(pathlist[0]) != b'':
    pathlist[0:1] = os.path.split(pathlist[0])
  return b'/'.join(pathlist[n:])
# --- /Utility function ---


class Hunk(object):
  """ Parsed hunk data container (hunk starts with @@ -R +R @@) """

  def __init__(self):
    self.startsrc=None #: line count starts with 1
    self.linessrc=None
    self.starttgt=None
    self.linestgt=None
    self.invalid=False
    self.desc=''
    self.text=[]

#  def apply(self, estream):
#    """ write hunk data into enumerable stream
#        return strings one by one until hunk is
#        over
#
#        enumerable stream are tuples (lineno, line)
#        where lineno starts with 0
#    """
#    pass


class Patch(object):
  """ Patch for a single file.
      If used as an iterable, returns hunks.
  """
  def __init__(self):
    self.source = None 
    self.target = None
    self.hunks = []
    self.hunkends = []
    self.header = []

    self.type = None

  def __iter__(self):
    for h in self.hunks:
      yield h


class PatchSet(object):
  """ PatchSet is a patch parser and container.
      When used as an iterable, returns patches.
  """

  def __init__(self, stream=None):
    # --- API accessible fields ---

    # name of the PatchSet (filename or ...)
    self.name = None
    # patch set type - one of constants
    self.type = None

    # list of Patch objects
    self.items = []

    self.errors = 0    # fatal parsing errors
    self.warnings = 0  # non-critical warnings
    # --- /API ---

    if stream:
      self.parse(stream)

  def __len__(self):
    return len(self.items)

  def __iter__(self):
    for i in self.items:
      yield i

  def parse(self, stream):
    """ parse unified diff
        return True on success
    """
    lineends = dict(lf=0, crlf=0, cr=0)
    nexthunkno = 0    #: even if index starts with 0 user messages number hunks from 1

    p = None
    hunk = None
    # hunkactual variable is used to calculate hunk lines for comparison
    hunkactual = dict(linessrc=None, linestgt=None)


    class wrapumerate(enumerate):
      """Enumerate wrapper that uses boolean end of stream status instead of
      StopIteration exception, and properties to access line information.
      """

      def __init__(self, *args, **kwargs):
        # we don't call parent, it is magically created by __new__ method

        self._exhausted = False
        self._lineno = False     # after end of stream equal to the num of lines
        self._line = False       # will be reset to False after end of stream

      def next(self):
        """Try to read the next line and return True if it is available,
           False if end of stream is reached."""
        if self._exhausted:
          return False

        try:
          self._lineno, self._line = compat_next(super(wrapumerate, self))
        except StopIteration:
          self._exhausted = True
          self._line = False
          return False
        return True

      @property
      def is_empty(self):
        return self._exhausted

      @property
      def line(self):
        return self._line

      @property
      def lineno(self):
        return self._lineno

    # define states (possible file regions) that direct parse flow
    headscan  = True  # start with scanning header
    filenames = False # lines starting with --- and +++

    hunkhead = False  # @@ -R +R @@ sequence
    hunkbody = False  #
    hunkskip = False  # skipping invalid hunk mode

    hunkparsed = False # state after successfully parsed hunk

    # regexp to match start of hunk, used groups - 1,3,4,6
    re_hunk_start = re.compile(br"^@@ -(\d+)(,(\d+))? \+(\d+)(,(\d+))? @@")
    
    self.errors = 0
    # temp buffers for header and filenames info
    header = []
    srcname = None
    tgtname = None

    # start of main cycle
    # each parsing block already has line available in fe.line
    fe = wrapumerate(stream)
    while fe.next():

      # -- deciders: these only switch state to decide who should process
      # --           line fetched at the start of this cycle
      if hunkparsed:
        hunkparsed = False
        if re_hunk_start.match(fe.line):
            hunkhead = True
        elif fe.line.startswith(b"--- "):
            filenames = True
        else:
            headscan = True
      # -- ------------------------------------

      # read out header
      if headscan:
        while not fe.is_empty and not fe.line.startswith(b"--- "):
            header.append(fe.line)
            fe.next()
        if fe.is_empty:
            if p == None:
              debug("no patch data found")  # error is shown later
              self.errors += 1
            else:
              info("%d unparsed bytes left at the end of stream" % len(b''.join(header)))
              self.warnings += 1
              # TODO check for \No new line at the end.. 
              # TODO test for unparsed bytes
              # otherwise error += 1
            # this is actually a loop exit
            continue

        headscan = False
        # switch to filenames state
        filenames = True

      line = fe.line
      lineno = fe.lineno


      # hunkskip and hunkbody code skipped until definition of hunkhead is parsed
      if hunkbody:
        # [x] treat empty lines inside hunks as containing single space
        #     (this happens when diff is saved by copy/pasting to editor
        #      that strips trailing whitespace)
        if line.strip(b"\r\n") == b"":
            debug("expanding empty line in a middle of hunk body")
            self.warnings += 1
            line = b' ' + line

        # process line first
        if re.match(b"^[- \\+\\\\]", line):
            # gather stats about line endings
            if line.endswith(b"\r\n"):
              p.hunkends["crlf"] += 1
            elif line.endswith(b"\n"):
              p.hunkends["lf"] += 1
            elif line.endswith(b"\r"):
              p.hunkends["cr"] += 1
              
            if line.startswith(b"-"):
              hunkactual["linessrc"] += 1
            elif line.startswith(b"+"):
              hunkactual["linestgt"] += 1
            elif not line.startswith(b"\\"):
              hunkactual["linessrc"] += 1
              hunkactual["linestgt"] += 1
            hunk.text.append(line)
            # todo: handle \ No newline cases
        else:
            warning("invalid hunk no.%d at %d for target file %s" % (nexthunkno, lineno+1, p.target))
            # add hunk status node
            hunk.invalid = True
            p.hunks.append(hunk)
            self.errors += 1
            # switch to hunkskip state
            hunkbody = False
            hunkskip = True

        # check exit conditions
        if hunkactual["linessrc"] > hunk.linessrc or hunkactual["linestgt"] > hunk.linestgt:
            warning("extra lines for hunk no.%d at %d for target %s" % (nexthunkno, lineno+1, p.target))
            # add hunk status node
            hunk.invalid = True
            p.hunks.append(hunk)
            self.errors += 1
            # switch to hunkskip state
            hunkbody = False
            hunkskip = True
        elif hunk.linessrc == hunkactual["linessrc"] and hunk.linestgt == hunkactual["linestgt"]:
            # hunk parsed successfully
            p.hunks.append(hunk)
            # switch to hunkparsed state
            hunkbody = False
            hunkparsed = True

            # detect mixed window/unix line ends
            ends = p.hunkends
            if ((ends["cr"]!=0) + (ends["crlf"]!=0) + (ends["lf"]!=0)) > 1:
              warning("inconsistent line ends in patch hunks for %s" % p.source)
              self.warnings += 1
            if debugmode:
              debuglines = dict(ends)
              debuglines.update(file=p.target, hunk=nexthunkno)
              debug("crlf: %(crlf)d  lf: %(lf)d  cr: %(cr)d\t - file: %(file)s hunk: %(hunk)d" % debuglines)
            # fetch next line
            continue

      if hunkskip:
        if re_hunk_start.match(line):
          # switch to hunkhead state
          hunkskip = False
          hunkhead = True
        elif line.startswith(b"--- "):
          # switch to filenames state
          hunkskip = False
          filenames = True
          if debugmode and len(self.items) > 0:
            debug("- %2d hunks for %s" % (len(p.hunks), p.source))

      if filenames:
        if line.startswith(b"--- "):
          if srcname != None:
            # XXX testcase
            warning("skipping false patch for %s" % srcname)
            srcname = None
            # XXX header += srcname
            # double source filename line is encountered
            # attempt to restart from this second line
          re_filename = b"^--- ([^\t]+)"
          match = re.match(re_filename, line)
          # todo: support spaces in filenames
          if match:
            srcname = match.group(1).strip()
          else:
            warning("skipping invalid filename at line %d" % (lineno+1))
            self.errors += 1
            # XXX p.header += line
            # switch back to headscan state
            filenames = False
            headscan = True
        elif not line.startswith(b"+++ "):
          if srcname != None:
            warning("skipping invalid patch with no target for %s" % srcname)
            self.errors += 1
            srcname = None
            # XXX header += srcname
            # XXX header += line
          else:
            # this should be unreachable
            warning("skipping invalid target patch")
          filenames = False
          headscan = True
        else:
          if tgtname != None:
            # XXX seems to be a dead branch  
            warning("skipping invalid patch - double target at line %d" % (lineno+1))
            self.errors += 1
            srcname = None
            tgtname = None
            # XXX header += srcname
            # XXX header += tgtname
            # XXX header += line
            # double target filename line is encountered
            # switch back to headscan state
            filenames = False
            headscan = True
          else:
            re_filename = br"^\+\+\+ ([^\t]+)"
            match = re.match(re_filename, line)
            if not match:
              warning("skipping invalid patch - no target filename at line %d" % (lineno+1))
              self.errors += 1
              srcname = None
              # switch back to headscan state
              filenames = False
              headscan = True
            else:
              if p: # for the first run p is None
                self.items.append(p)
              p = Patch()
              p.source = srcname
              srcname = None
              p.target = match.group(1).strip()
              p.header = header
              header = []
              # switch to hunkhead state
              filenames = False
              hunkhead = True
              nexthunkno = 0
              p.hunkends = lineends.copy()
              continue

      if hunkhead:
        match = re.match(br"^@@ -(\d+)(,(\d+))? \+(\d+)(,(\d+))? @@(.*)", line)
        if not match:
          if not p.hunks:
            warning("skipping invalid patch with no hunks for file %s" % p.source)
            self.errors += 1
            # XXX review switch
            # switch to headscan state
            hunkhead = False
            headscan = True
            continue
          else:
            # TODO review condition case
            # switch to headscan state
            hunkhead = False
            headscan = True
        else:
          hunk = Hunk()
          hunk.startsrc = int(match.group(1))
          hunk.linessrc = 1
          if match.group(3): hunk.linessrc = int(match.group(3))
          hunk.starttgt = int(match.group(4))
          hunk.linestgt = 1
          if match.group(6): hunk.linestgt = int(match.group(6))
          hunk.invalid = False
          hunk.desc = match.group(7)[1:].rstrip()
          hunk.text = []

          hunkactual["linessrc"] = hunkactual["linestgt"] = 0

          # switch to hunkbody state
          hunkhead = False
          hunkbody = True
          nexthunkno += 1
          continue

    # /while fe.next()

    if p:
      self.items.append(p)

    if not hunkparsed:
      if hunkskip:
        warning("warning: finished with errors, some hunks may be invalid")
      elif headscan:
        if len(self.items) == 0:
          warning("error: no patch data found!")
          return False
        else: # extra data at the end of file
          pass 
      else:
        warning("error: patch stream is incomplete!")
        self.errors += 1
        if len(self.items) == 0:
          return False

    if debugmode and len(self.items) > 0:
        debug("- %2d hunks for %s" % (len(p.hunks), p.source))

    # XXX fix total hunks calculation
    debug("total files: %d  total hunks: %d" % (len(self.items),
        sum(len(p.hunks) for p in self.items)))

    # ---- detect patch and patchset types ----
    for idx, p in enumerate(self.items):
      self.items[idx].type = self._detect_type(p)

    types = set([p.type for p in self.items])
    if len(types) > 1:
      self.type = MIXED
    else:
      self.type = types.pop()
    # --------

    self._normalize_filenames()
    
    return (self.errors == 0)

  def _detect_type(self, p):
    """ detect and return type for the specified Patch object
        analyzes header and filenames info

        NOTE: must be run before filenames are normalized
    """

    # check for SVN
    #  - header starts with Index:
    #  - next line is ===... delimiter
    #  - filename is followed by revision number
    # TODO add SVN revision
    if (len(p.header) > 1 and p.header[-2].startswith(b"Index: ")
          and p.header[-1].startswith(b"="*67)):
        return SVN

    # common checks for both HG and GIT
    DVCS = ((p.source.startswith(b'a/') or p.source == b'/dev/null')
        and (p.target.startswith(b'b/') or p.target == b'/dev/null'))

    # GIT type check
    #  - header[-2] is like "diff --git a/oldname b/newname"
    #  - header[-1] is like "index <hash>..<hash> <mode>"
    # TODO add git rename diffs and add/remove diffs
    #      add git diff with spaced filename
    # TODO http://www.kernel.org/pub/software/scm/git/docs/git-diff.html

    # Git patch header len is 2 min
    if len(p.header) > 1:
      # detect the start of diff header - there might be some comments before
      for idx in reversed(range(len(p.header))):
        if p.header[idx].startswith(b"diff --git"):
          break
      if p.header[idx].startswith(b'diff --git a/'):
        if (idx+1 < len(p.header)
            and re.match(b'index \\w{7}..\\w{7} \\d{6}', p.header[idx+1])):
          if DVCS:
            return GIT

    # HG check
    # 
    #  - for plain HG format header is like "diff -r b2d9961ff1f5 filename"
    #  - for Git-style HG patches it is "diff --git a/oldname b/newname"
    #  - filename starts with a/, b/ or is equal to /dev/null
    #  - exported changesets also contain the header
    #    # HG changeset patch
    #    # User name@example.com
    #    ...   
    # TODO add MQ
    # TODO add revision info
    if len(p.header) > 0:
      if DVCS and re.match(b'diff -r \\w{12} .*', p.header[-1]):
        return HG
      if DVCS and p.header[-1].startswith(b'diff --git a/'):
        if len(p.header) == 1:  # native Git patch header len is 2
          return HG
        elif p.header[0].startswith(b'# HG changeset patch'):
          return HG

    return PLAIN


  def _normalize_filenames(self):
    """ sanitize filenames, normalizing paths, i.e.:
        1. strip a/ and b/ prefixes from GIT and HG style patches
        2. remove all references to parent directories (with warning)
        3. translate any absolute paths to relative (with warning)

        [x] always use forward slashes to be crossplatform
            (diff/patch were born as a unix utility after all)
        
        return None
    """
    if debugmode:
      debug("normalize filenames")
    for i,p in enumerate(self.items):
      if debugmode:
        debug("    patch type = " + p.type)
        debug("    source = " + p.source)
        debug("    target = " + p.target)
      if p.type in (HG, GIT):
        # TODO: figure out how to deal with /dev/null entries
        debug("stripping a/ and b/ prefixes")
        if p.source != '/dev/null':
          if not p.source.startswith(b"a/"):
            warning("invalid source filename")
          else:
            p.source = p.source[2:]
        if p.target != '/dev/null':
          if not p.target.startswith(b"b/"):
            warning("invalid target filename")
          else:
            p.target = p.target[2:]

      p.source = xnormpath(p.source)
      p.target = xnormpath(p.target)

      sep = b'/'  # sep value can be hardcoded, but it looks nice this way

      # references to parent are not allowed
      if p.source.startswith(b".." + sep):
        warning("error: stripping parent path for source file patch no.%d" % (i+1))
        self.warnings += 1
        while p.source.startswith(b".." + sep):
          p.source = p.source.partition(sep)[2]
      if p.target.startswith(b".." + sep):
        warning("error: stripping parent path for target file patch no.%d" % (i+1))
        self.warnings += 1
        while p.target.startswith(b".." + sep):
          p.target = p.target.partition(sep)[2]
      # absolute paths are not allowed
      if xisabs(p.source) or xisabs(p.target):
        warning("error: absolute paths are not allowed - file no.%d" % (i+1))
        self.warnings += 1
        if xisabs(p.source):
          warning("stripping absolute path from source name '%s'" % p.source)
          p.source = xstrip(p.source)
        if xisabs(p.target):
          warning("stripping absolute path from target name '%s'" % p.target)
          p.target = xstrip(p.target)
    
      self.items[i].source = p.source
      self.items[i].target = p.target


  def diffstat(self):
    """ calculate diffstat and return as a string
        Notes:
          - original diffstat ouputs target filename
          - single + or - shouldn't escape histogram
    """
    names = []
    insert = []
    delete = []
    delta = 0    # size change in bytes
    namelen = 0
    maxdiff = 0  # max number of changes for single file
                 # (for histogram width calculation)
    for patch in self.items:
      i,d = 0,0
      for hunk in patch.hunks:
        for line in hunk.text:
          if line.startswith(b'+'):
            i += 1
            delta += len(line)-1
          elif line.startswith(b'-'):
            d += 1
            delta -= len(line)-1
      names.append(patch.target)
      insert.append(i)
      delete.append(d)
      namelen = max(namelen, len(patch.target))
      maxdiff = max(maxdiff, i+d)
    output = ''
    statlen = len(str(maxdiff))  # stats column width
    for i,n in enumerate(names):
      # %-19s | %-4d %s
      format = " %-" + str(namelen) + "s | %" + str(statlen) + "s %s\n"

      hist = ''
      # -- calculating histogram --
      width = len(format % ('', '', ''))
      histwidth = max(2, 80 - width)
      if maxdiff < histwidth:
        hist = "+"*insert[i] + "-"*delete[i]
      else:
        iratio = (float(insert[i]) / maxdiff) * histwidth
        dratio = (float(delete[i]) / maxdiff) * histwidth

        # make sure every entry gets at least one + or -
        iwidth = 1 if 0 < iratio < 1 else int(iratio)
        dwidth = 1 if 0 < dratio < 1 else int(dratio)
        #print(iratio, dratio, iwidth, dwidth, histwidth)
        hist = "+"*int(iwidth) + "-"*int(dwidth)
      # -- /calculating +- histogram --
      output += (format % (tostr(names[i]), str(insert[i] + delete[i]), hist))
 
    output += (" %d files changed, %d insertions(+), %d deletions(-), %+d bytes"
               % (len(names), sum(insert), sum(delete), delta))
    return output


  def findfile(self, old, new):
    """ return name of file to be patched or None """
    if exists(old):
      return old
    elif exists(new):
      return new
    else:
      # [w] Google Code generates broken patches with its online editor
      debug("broken patch from Google Code, stripping prefixes..")
      if old.startswith(b'a/') and new.startswith(b'b/'):
        old, new = old[2:], new[2:]
        debug("   %s" % old)
        debug("   %s" % new)
        if exists(old):
          return old
        elif exists(new):
          return new
      return None


  def apply(self, strip=0, root=None):
    """ Apply parsed patch, optionally stripping leading components
        from file paths. `root` parameter specifies working dir.
        return True on success
    """
    if root:
      prevdir = os.getcwd()
      os.chdir(root)

    total = len(self.items)
    errors = 0
    if strip:
      # [ ] test strip level exceeds nesting level
      #   [ ] test the same only for selected files
      #     [ ] test if files end up being on the same level
      try:
        strip = int(strip)
      except ValueError:
        errors += 1
        warning("error: strip parameter '%s' must be an integer" % strip)
        strip = 0

    #for fileno, filename in enumerate(self.source):
    for i,p in enumerate(self.items):
      if strip:
        debug("stripping %s leading component(s) from:" % strip)
        debug("   %s" % p.source)
        debug("   %s" % p.target)
        old = pathstrip(p.source, strip)
        new = pathstrip(p.target, strip)
      else:
        old, new = p.source, p.target

      filename = self.findfile(old, new)

      if not filename:
          warning("source/target file does not exist:\n  --- %s\n  +++ %s" % (old, new))
          errors += 1
          continue
      if not isfile(filename):
        warning("not a file - %s" % filename)
        errors += 1
        continue

      # [ ] check absolute paths security here
      debug("processing %d/%d:\t %s" % (i+1, total, filename))

      # validate before patching
      f2fp = open(filename, 'rb')
      hunkno = 0
      hunk = p.hunks[hunkno]
      hunkfind = []
      hunkreplace = []
      validhunks = 0
      canpatch = False
      for lineno, line in enumerate(f2fp):
        if lineno+1 < hunk.startsrc:
          continue
        elif lineno+1 == hunk.startsrc:
          hunkfind = [x[1:].rstrip(b"\r\n") for x in hunk.text if x[0] in b" -"]
          hunkreplace = [x[1:].rstrip(b"\r\n") for x in hunk.text if x[0] in b" +"]
          #pprint(hunkreplace)
          hunklineno = 0

          # todo \ No newline at end of file

        # check hunks in source file
        if lineno+1 < hunk.startsrc+len(hunkfind)-1:
          if line.rstrip(b"\r\n") == hunkfind[hunklineno]:
            hunklineno+=1
          else:
            info("file %d/%d:\t %s" % (i+1, total, filename))
            info(" hunk no.%d doesn't match source file at line %d" % (hunkno+1, lineno+1))
            info("  expected: %s" % hunkfind[hunklineno])
            info("  actual  : %s" % line.rstrip(b"\r\n"))
            # not counting this as error, because file may already be patched.
            # check if file is already patched is done after the number of
            # invalid hunks if found
            # TODO: check hunks against source/target file in one pass
            #   API - check(stream, srchunks, tgthunks)
            #           return tuple (srcerrs, tgterrs)

            # continue to check other hunks for completeness
            hunkno += 1
            if hunkno < len(p.hunks):
              hunk = p.hunks[hunkno]
              continue
            else:
              break

        # check if processed line is the last line
        if lineno+1 == hunk.startsrc+len(hunkfind)-1:
          debug(" hunk no.%d for file %s  -- is ready to be patched" % (hunkno+1, filename))
          hunkno+=1
          validhunks+=1
          if hunkno < len(p.hunks):
            hunk = p.hunks[hunkno]
          else:
            if validhunks == len(p.hunks):
              # patch file
              canpatch = True
              break
      else:
        if hunkno < len(p.hunks):
          warning("premature end of source file %s at hunk %d" % (filename, hunkno+1))
          errors += 1

      f2fp.close()

      if validhunks < len(p.hunks):
        if self._match_file_hunks(filename, p.hunks):
          warning("already patched  %s" % filename)
        else:
          warning("source file is different - %s" % filename)
          errors += 1
      if canpatch:
        backupname = filename+b".orig"
        if exists(backupname):
          warning("can't backup original file to %s - aborting" % backupname)
        else:
          import shutil
          shutil.move(filename, backupname)
          if self.write_hunks(backupname, filename, p.hunks):
            info("successfully patched %d/%d:\t %s" % (i+1, total, filename))
            os.unlink(backupname)
          else:
            errors += 1
            warning("error patching file %s" % filename)
            shutil.copy(filename, filename+".invalid")
            warning("invalid version is saved to %s" % filename+".invalid")
            # todo: proper rejects
            shutil.move(backupname, filename)

    if root:
      os.chdir(prevdir)

    # todo: check for premature eof
    return (errors == 0)


  def _reverse(self):
    """ reverse patch direction (this doesn't touch filenames) """
    for p in self.items:
      for h in p.hunks:
        h.startsrc, h.starttgt = h.starttgt, h.startsrc
        h.linessrc, h.linestgt = h.linestgt, h.linessrc
        for i,line in enumerate(h.text):
          # need to use line[0:1] here, because line[0]
          # returns int instead of bytes on Python 3
          if line[0:1] == b'+':
            h.text[i] = b'-' + line[1:]
          elif line[0:1] == b'-':
            h.text[i] = b'+' +line[1:]

  def revert(self, strip=0, root=None):
    """ apply patch in reverse order """
    reverted = copy.deepcopy(self)
    reverted._reverse()
    return reverted.apply(strip, root)


  def can_patch(self, filename):
    """ Check if specified filename can be patched. Returns None if file can
    not be found among source filenames. False if patch can not be applied
    clearly. True otherwise.

    :returns: True, False or None
    """
    filename = abspath(filename)
    for p in self.items:
      if filename == abspath(p.source):
        return self._match_file_hunks(filename, p.hunks)
    return None


  def _match_file_hunks(self, filepath, hunks):
    matched = True
    fp = open(abspath(filepath), 'rb')

    class NoMatch(Exception):
      pass

    lineno = 1
    line = fp.readline()
    hno = None
    try:
      for hno, h in enumerate(hunks):
        # skip to first line of the hunk
        while lineno < h.starttgt:
          if not len(line): # eof
            debug("check failed - premature eof before hunk: %d" % (hno+1))
            raise NoMatch
          line = fp.readline()
          lineno += 1
        for hline in h.text:
          if hline.startswith(b"-"):
            continue
          if not len(line):
            debug("check failed - premature eof on hunk: %d" % (hno+1))
            # todo: \ No newline at the end of file
            raise NoMatch
          if line.rstrip(b"\r\n") != hline[1:].rstrip(b"\r\n"):
            debug("file is not patched - failed hunk: %d" % (hno+1))
            raise NoMatch
          line = fp.readline()
          lineno += 1

    except NoMatch:
      matched = False
      # todo: display failed hunk, i.e. expected/found

    fp.close()
    return matched


  def patch_stream(self, instream, hunks):
    """ Generator that yields stream patched with hunks iterable
    
        Converts lineends in hunk lines to the best suitable format
        autodetected from input
    """

    # todo: At the moment substituted lineends may not be the same
    #       at the start and at the end of patching. Also issue a
    #       warning/throw about mixed lineends (is it really needed?)

    hunks = iter(hunks)

    srclineno = 1

    lineends = {b'\n':0, b'\r\n':0, b'\r':0}
    def get_line():
      """
      local utility function - return line from source stream
      collecting line end statistics on the way
      """
      line = instream.readline()
        # 'U' mode works only with text files
      if line.endswith(b"\r\n"):
        lineends[b"\r\n"] += 1
      elif line.endswith(b"\n"):
        lineends[b"\n"] += 1
      elif line.endswith(b"\r"):
        lineends[b"\r"] += 1
      return line

    for hno, h in enumerate(hunks):
      debug("hunk %d" % (hno+1))
      # skip to line just before hunk starts
      while srclineno < h.startsrc:
        yield get_line()
        srclineno += 1

      for hline in h.text:
        # todo: check \ No newline at the end of file
        if hline.startswith(b"-") or hline.startswith(b"\\"):
          get_line()
          srclineno += 1
          continue
        else:
          if not hline.startswith(b"+"):
            get_line()
            srclineno += 1
          line2write = hline[1:]
          # detect if line ends are consistent in source file
          if sum([bool(lineends[x]) for x in lineends]) == 1:
            newline = [x for x in lineends if lineends[x] != 0][0]
            yield line2write.rstrip(b"\r\n")+newline
          else: # newlines are mixed
            yield line2write
     
    for line in instream:
      yield line


  def write_hunks(self, srcname, tgtname, hunks):
    src = open(srcname, "rb")
    tgt = open(tgtname, "wb")

    debug("processing target file %s" % tgtname)

    tgt.writelines(self.patch_stream(src, hunks))

    tgt.close()
    src.close()
    # [ ] TODO: add test for permission copy
    shutil.copymode(srcname, tgtname)
    return True


  def dump(self):
    for p in self.items:
      for headline in p.header:
        print(headline.rstrip('\n'))
      print('--- ' + p.source)
      print('+++ ' + p.target)
      for h in p.hunks:
        print('@@ -%s,%s +%s,%s @@' % (h.startsrc, h.linessrc, h.starttgt, h.linestgt))
        for line in h.text:
          print(line.rstrip('\n'))


def main():
  from optparse import OptionParser
  from os.path import exists
  import sys

  opt = OptionParser(usage="1. %prog [options] unified.diff\n"
                    "       2. %prog [options] http://host/patch\n"
                    "       3. %prog [options] -- < unified.diff",
                     version="python-patch %s" % __version__)
  opt.add_option("-q", "--quiet", action="store_const", dest="verbosity",
                                  const=0, help="print only warnings and errors", default=1)
  opt.add_option("-v", "--verbose", action="store_const", dest="verbosity",
                                  const=2, help="be verbose")
  opt.add_option("--debug", action="store_true", dest="debugmode", help="debug mode")
  opt.add_option("--diffstat", action="store_true", dest="diffstat",
                                           help="print diffstat and exit")
  opt.add_option("-d", "--directory", metavar='DIR',
                                           help="specify root directory for applying patch")
  opt.add_option("-p", "--strip", type="int", metavar='N', default=0,
                                           help="strip N path components from filenames")
  opt.add_option("--revert", action="store_true",
                                           help="apply patch in reverse order (unpatch)")
  (options, args) = opt.parse_args()

  if not args and sys.argv[-1:] != ['--']:
    opt.print_version()
    opt.print_help()
    sys.exit()
  readstdin = (sys.argv[-1:] == ['--'] and not args)

  verbosity_levels = {0:logging.WARNING, 1:logging.INFO, 2:logging.DEBUG}
  loglevel = verbosity_levels[options.verbosity]
  logformat = "%(message)s"
  logger.setLevel(loglevel)
  streamhandler.setFormatter(logging.Formatter(logformat))

  if options.debugmode:
    setdebug()  # this sets global debugmode variable

  if readstdin:
    patch = PatchSet(sys.stdin)
  else:
    patchfile = args[0]
    urltest = patchfile.split(':')[0]
    if (':' in patchfile and urltest.isalpha()
        and len(urltest) > 1): # one char before : is a windows drive letter
      patch = fromurl(patchfile)
    else:
      if not exists(patchfile) or not isfile(patchfile):
        sys.exit("patch file does not exist - %s" % patchfile)
      patch = fromfile(patchfile)

  if options.diffstat:
    print(patch.diffstat())
    sys.exit(0)

  #pprint(patch)
  if options.revert:
    patch.revert(options.strip, root=options.directory) or sys.exit(-1)
  else:
    patch.apply(options.strip, root=options.directory) or sys.exit(-1)

  # todo: document and test line ends handling logic - patch.py detects proper line-endings
  #       for inserted hunks and issues a warning if patched file has incosistent line ends


if __name__ == "__main__":
  main()

# Legend:
# [ ]  - some thing to be done
# [w]  - official wart, external or internal that is unlikely to be fixed

# [ ] API break (2.x) wishlist
# PatchSet.items  -->  PatchSet.patches

# [ ] run --revert test for all dataset items
# [ ] run .parse() / .dump() test for dataset
