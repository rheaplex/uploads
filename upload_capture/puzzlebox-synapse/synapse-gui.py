#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright Puzzlebox Productions, LLC (2010-2011)
#
# This code is released under the GNU Pulic License (GPL) version 2
# For more information please refer to http://www.gnu.org/copyleft/gpl.html

import Puzzlebox.Synapse.Interface as tgInterface
import sys

try:
	import PySide as PyQt4
	from PySide import QtCore, QtGui
except:
	print "Using PyQt4 module"
	from PyQt4 import QtCore, QtGui
else:
	print "Using PySide module"

DEBUG = 1
log = None
app = QtGui.QApplication(sys.argv)
window = tgInterface.QtUI(log, DEBUG)
window.show()
sys.exit(app.exec_())