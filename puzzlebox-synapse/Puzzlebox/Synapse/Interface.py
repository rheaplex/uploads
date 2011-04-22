# -*- coding: utf-8 -*-

# Copyright Puzzlebox Productions, LLC (2010)
#
# This code is released under the GNU Pulic License (GPL) version 2
# For more information please refer to http://www.gnu.org/copyleft/gpl.html

# Old Class Names:
#       puzzlebox_synapse_interface = QtUI

__changelog__ = """\
Last Update: 2011.03.01

"""

__todo__ = """
- update configuration.ini file with settings entered into interface

"""

### IMPORTS ###
import os, sys, time
import simplejson as json

try:
	from Interface_Plot import *
	MATPLOTLIB_AVAILABLE = True
except:
	MATPLOTLIB_AVAILABLE = False

if (sys.platform != 'win32'):
	import bluetooth
	DEFAULT_IMAGE_PATH = '/usr/share/puzzlebox_synapse/images'
else:
	import _winreg as winreg
	import itertools
	import re
	import serial
	DEFAULT_IMAGE_PATH = 'images'

try:
	import PySide as PyQt4
	from PySide import QtCore, QtGui
except:
	print "Using PyQt4 module"
	from PyQt4 import QtCore, QtGui
else:
	print "Using PySide module"

try:
	import cPickle as pickle
except:
	import pickle

# from puzzlebox_synapse_interface_design import Ui_Form
from Interface_Design import Ui_Form as Design

import Configuration as configuration
import Server as synapse_server
import Client as thinkgear_client
#import puzzlebox_logger

### GLOBALS ###

DEBUG = 1

THINKGEAR_SERVER_HOST = configuration.THINKGEAR_SERVER_HOST
THINKGEAR_SERVER_PORT = configuration.THINKGEAR_SERVER_PORT

THINKGEAR_EEG_POWER_BAND_ORDER = configuration.THINKGEAR_EEG_POWER_BAND_ORDER

THINKGEAR_EMULATION_MAX_ESENSE_VALUE = \
	configuration.THINKGEAR_EMULATION_MAX_ESENSE_VALUE
THINKGEAR_EMULATION_MAX_EEG_POWER_VALUE = \
	configuration.THINKGEAR_EMULATION_MAX_EEG_POWER_VALUE

PATH_TO_HCITOOL = '/usr/bin/hcitool'

#UPDATE_INTERFACE_VIA_TIMER = True # Alternative is to establish a
                                  ## ThinkGear Connect client which
                                  ## updates the interface on demand
                                  ## as packets are received

UPDATE_INTERFACE_VIA_TIMER = False

#INTERFACE_UPDATE_FREQUENCY = (1 / 512) * 1000 # ms (512 Hz)
INTERFACE_UPDATE_FREQUENCY = 1000 # ms

INTERFACE_RAW_EEG_UPDATE_FREQUENCY = 512

### CLASSES ###

class QtUI(QtGui.QWidget, Design):
	
	def __init__(self, log, server=None, DEBUG=DEBUG, parent = None):
		
		self.log = log
		self.DEBUG = DEBUG
		
		QtGui.QWidget.__init__(self, parent)
		self.setupUi(self)
		
		self.configureSettings()
		self.connectWidgets()
		
		self.name = "Synapse Interface"
		
		self.thinkGearConnectServer = None
		self.thinkgearConnectClient = None
		
		self.maxEEGPower = THINKGEAR_EMULATION_MAX_EEG_POWER_VALUE
		
		self.debug_console_buffer = ''
		
		self.packets = {}
		self.packets['rawEeg'] = []
		self.packets['signals'] = []
		
		if UPDATE_INTERFACE_VIA_TIMER:
			self.updateInterfaceTimer = QtCore.QTimer()
			QtCore.QObject.connect(self.updateInterfaceTimer, \
				                    QtCore.SIGNAL("timeout()"), \
				                    self.updateInterface)
	
	
	##################################################################
	
	def configureSettings(self):
		
		# Synapse Interface
		image_path = "puzzlebox.ico"
		if not os.path.exists(image_path):
			image_path = os.path.join(DEFAULT_IMAGE_PATH, image_path)
		
		if os.path.exists(image_path):
			icon = QtGui.QIcon()
			icon.addPixmap(QtGui.QPixmap(image_path), \
				            QtGui.QIcon.Normal, \
				            QtGui.QIcon.Off)
			self.setWindowIcon(icon)
		
		image_path = "puzzlebox_logo.png"
		if not os.path.exists(image_path):
			image_path = os.path.join(DEFAULT_IMAGE_PATH, image_path)
		if os.path.exists(image_path):
			self.labelPuzzleboxIcon.setPixmap(QtGui.QPixmap(image_path))
		
		
		if configuration.INTERFACE_TAB_POSITION == 'South':
			self.tabWidget.setTabPosition(QtGui.QTabWidget.South)
		else:
			self.tabWidget.setTabPosition(QtGui.QTabWidget.North)
		
		
		# ThinkGear Device
		self.searchForThinkGearDevices()
		
		
		# ThinkGear Connect Server
		self.textLabelBluetoothStatus.setText("Status: Disconnected")
		
		# Display Host for ThinkGear Connect Socket Server
		self.lineEditThinkGearHost.setText(THINKGEAR_SERVER_HOST)
		
		# Display Port for ThinkGear Connect Socket Server
		self.lineEditThinkGearPort.setText('%i' % THINKGEAR_SERVER_PORT)
		
		
		# ThinkgGear Progress Bars
		self.progressBarEEGDelta.setMaximum(THINKGEAR_EMULATION_MAX_EEG_POWER_VALUE)
		self.progressBarEEGTheta.setMaximum(THINKGEAR_EMULATION_MAX_EEG_POWER_VALUE)
		self.progressBarEEGLowAlpha.setMaximum(THINKGEAR_EMULATION_MAX_EEG_POWER_VALUE)
		self.progressBarEEGHighAlpha.setMaximum(THINKGEAR_EMULATION_MAX_EEG_POWER_VALUE)
		self.progressBarEEGLowBeta.setMaximum(THINKGEAR_EMULATION_MAX_EEG_POWER_VALUE)
		self.progressBarEEGHighBeta.setMaximum(THINKGEAR_EMULATION_MAX_EEG_POWER_VALUE)
		self.progressBarEEGLowGamma.setMaximum(THINKGEAR_EMULATION_MAX_EEG_POWER_VALUE)
		self.progressBarEEGMidGamma.setMaximum(THINKGEAR_EMULATION_MAX_EEG_POWER_VALUE)
		
		self.progressBarAttention.setMaximum(THINKGEAR_EMULATION_MAX_ESENSE_VALUE)
		self.progressBarMeditation.setMaximum(THINKGEAR_EMULATION_MAX_ESENSE_VALUE)
		
		self.progressBarSignalContactQuality.setMaximum(200)
		
		
		if MATPLOTLIB_AVAILABLE:
			self.rawEEGMatplot = rawEEGMatplotlibCanvas( \
			                        self.tabEEGSignals, \
			                        width=8, \
			                        height=4, \
			                        dpi=100, \
			                        title='Raw EEG Waves')
			self.chartEEGMatplot = chartEEGMatplotlibCanvas( \
			                        self.tabCharts, \
			                        width=8, \
			                        height=4, \
			                        dpi=100, \
			                        title='EEG Brain Signals')
		
		else:
			self.tabWidget.removeTab(self.tabWidget.indexOf(self.tabEEGSignals))
			self.tabWidget.removeTab(self.tabWidget.indexOf(self.tabCharts))
	
	
	##################################################################
	
	def connectWidgets(self):
		
		self.connect(self.pushButtonBluetoothSearch, \
			          QtCore.SIGNAL("clicked()"), \
			          self.searchForThinkGearDevices)
		
		self.connect(self.pushButtonBluetoothConnect, \
			          QtCore.SIGNAL("clicked()"), \
			          self.connectToThinkGearDevice)
		
		self.connect(self.pushButtonThinkGearConnect, \
			          QtCore.SIGNAL("clicked()"), \
			          self.startThinkGearConnectServer)
		
		self.connect(self.pushButtonSave, \
			          QtCore.SIGNAL("clicked()"), \
			          self.saveData)
		
		self.connect(self.pushButtonExport, \
			          QtCore.SIGNAL("clicked()"), \
			          self.exportData)
		
		self.connect(self.pushButtonReset, \
			          QtCore.SIGNAL("clicked()"), \
			          self.resetData)
	
	
	##################################################################
	
	def connectToThinkGearDevice(self):
		
		device_selection = self.comboBoxDeviceSelect.currentText()
		
		self.disconnect(self.pushButtonBluetoothConnect, \
			             QtCore.SIGNAL("clicked()"), \
			             self.connectToThinkGearDevice)
		
		self.connect(self.pushButtonBluetoothConnect, \
			          QtCore.SIGNAL("clicked()"), \
			          self.disconnectFromThinkGearDevice)
		
		self.textLabelBluetoothStatus.setText("Status: Connected")
		
		self.pushButtonBluetoothSearch.setEnabled(False)
		
		self.pushButtonBluetoothConnect.setText('Disconnect')
		self.pushButtonBluetoothConnect.setChecked(True)
		
		self.comboBoxDeviceSelect.setEnabled(False)
	
	
	##################################################################
	
	def disconnectFromThinkGearDevice(self):
		
		self.disconnect(self.pushButtonBluetoothConnect, \
			             QtCore.SIGNAL("clicked()"), \
			             self.disconnectFromThinkGearDevice)
		
		self.connect(self.pushButtonBluetoothConnect, \
			          QtCore.SIGNAL("clicked()"), \
			          self.connectToThinkGearDevice)
		
		self.textLabelBluetoothStatus.setText("Status: Disconnected")
		
		self.pushButtonBluetoothSearch.setEnabled(True)
		
		self.pushButtonBluetoothConnect.setText('Connect')
		self.pushButtonBluetoothConnect.setChecked(False)
		
		self.comboBoxDeviceSelect.setEnabled(True)
		
		
		self.progressBarEEGDelta.setValue(0)
		self.progressBarEEGTheta.setValue(0)
		self.progressBarEEGLowAlpha.setValue(0)
		self.progressBarEEGHighAlpha.setValue(0)
		self.progressBarEEGLowBeta.setValue(0)
		self.progressBarEEGHighBeta.setValue(0)
		self.progressBarEEGLowGamma.setValue(0)
		self.progressBarEEGMidGamma.setValue(0)
		
		self.progressBarAttention.setValue(0)
		self.progressBarMeditation.setValue(0)
		
		self.progressBarSignalContactQuality.setValue(0)
		
		self.maxEEGPower = THINKGEAR_EMULATION_MAX_EEG_POWER_VALUE
		
		# In case the user connects to a MindSet, then disconnects
		# and re-connects to a MindSet Emulator,
		# we need to reset the max power values
		self.progressBarEEGDelta.setMaximum(self.maxEEGPower)
		self.progressBarEEGTheta.setMaximum(self.maxEEGPower)
		self.progressBarEEGLowAlpha.setMaximum(self.maxEEGPower)
		self.progressBarEEGHighAlpha.setMaximum(self.maxEEGPower)
		self.progressBarEEGLowBeta.setMaximum(self.maxEEGPower)
		self.progressBarEEGHighBeta.setMaximum(self.maxEEGPower)
		self.progressBarEEGLowGamma.setMaximum(self.maxEEGPower)
		self.progressBarEEGMidGamma.setMaximum(self.maxEEGPower)
	
	
	##################################################################
	
	def startThinkGearConnectServer(self):
		
		# Ensure EEG device is connected first
		
		if not self.pushButtonBluetoothConnect.isChecked():
			self.connectToThinkGearDevice()
		
		
		self.pushButtonBluetoothSearch.setEnabled(False)
		self.pushButtonBluetoothConnect.setEnabled(False)
		
		server_interface = str(self.lineEditThinkGearHost.text())
		server_port = int(self.lineEditThinkGearPort.text())
		device_address = str(self.comboBoxDeviceSelect.currentText())
		emulate_headset_data = (device_address == 'MindSet Emulator')
		
		
		self.thinkGearConnectServer = \
			synapse_server.ThinkgearServer( \
				self.log, \
				server_interface=server_interface, \
				server_port=server_port, \
				device_address=device_address, \
				emulate_headset_data=emulate_headset_data, \
				DEBUG=DEBUG, \
				parent=self)
		
		self.thinkGearConnectServer.start()
		
		
		if UPDATE_INTERFACE_VIA_TIMER:
			self.updateInterfaceTimer.start(INTERFACE_UPDATE_FREQUENCY)
		
		else:
			self.thinkgearConnectClient = \
				thinkgear_client.QtClient( \
					self.log, \
					server_host=server_interface, \
					server_port=server_port, \
					DEBUG=0, \
					parent=self)
			
			self.thinkgearConnectClient.start()
		
		
		self.disconnect(self.pushButtonThinkGearConnect, \
			             QtCore.SIGNAL("clicked()"), \
			             self.startThinkGearConnectServer)
		
		self.connect(self.pushButtonThinkGearConnect, \
			          QtCore.SIGNAL("clicked()"), \
			          self.stopThinkGearConnectServer)
		
		self.lineEditThinkGearHost.setEnabled(False)
		self.lineEditThinkGearPort.setEnabled(False)
		
		self.pushButtonThinkGearConnect.setText('Stop')
	
	
	##################################################################
	
	def stopThinkGearConnectServer(self):
		
		if UPDATE_INTERFACE_VIA_TIMER:
			self.updateInterfaceTimer.stop()
		else:
			try:
				self.thinkgearConnectClient.disconnectFromHost()
			except Exception, e:
				if self.DEBUG:
					print "Call failed to self.thinkgearConnectClient.disconnectFromHost():",
					print e
			
			try:
				self.thinkGearConnectServer.exitThread()
			except Exception, e:
				if self.DEBUG:
					print "Call failed to self.thinkGearConnectServer.exitThread():",
					print e
		
		self.disconnect(self.pushButtonThinkGearConnect, \
		                QtCore.SIGNAL("clicked()"), \
		                self.stopThinkGearConnectServer)
		
		self.connect(self.pushButtonThinkGearConnect, \
			          QtCore.SIGNAL("clicked()"), \
			          self.startThinkGearConnectServer)
		
		self.lineEditThinkGearHost.setEnabled(True)
		self.lineEditThinkGearPort.setEnabled(True)
		
		self.pushButtonThinkGearConnect.setText('Start')
		
		#self.pushButtonBluetoothSearch.setEnabled(True)
		self.pushButtonBluetoothConnect.setEnabled(True)
		
		self.pushButtonThinkGearConnect.setChecked(False)
	
	
	##################################################################
	
	def updateInterface(self):
		
		if not self.thinkGearConnectServer.emulate_headset_data:
			self.processPacketThinkGear( \
				self.thinkGearConnectServer.protocol.data_packet)
	
	
	##################################################################
	
	def parseTimeStamp(self, timestamp, local_version=False):
		
		try:
			decimal = '%f' % timestamp
			decimal = decimal.split('.')[1]
		except:
			decimal = '0'
		
		localtime = time.localtime(timestamp)
		
		if local_version:
			date = time.strftime('%x', localtime)
			localtime = time.strftime('%X', localtime)
		
		else:
			date = time.strftime('%Y-%m-%d', localtime)
			localtime = time.strftime('%H:%M:%S', localtime)
			localtime = '%s.%s %s' % (localtime, decimal, \
			               time.strftime('%Z', time.localtime(timestamp)))
		
		
		return(date, localtime)
	
	
	##################################################################
	
	def processPacketThinkGear(self, packet):
		
		if self.DEBUG > 2:
			print packet
		
		
		if ('rawEeg' in packet.keys()):
			self.packets['rawEeg'].append(packet['rawEeg'])
			value = packet['rawEeg']
			if MATPLOTLIB_AVAILABLE and \
				(self.tabWidget.currentIndex() == \
				 self.tabWidget.indexOf(self.tabEEGSignals)):
				self.rawEEGMatplot.update_figure(value)
		else:
			self.packets['signals'].append(packet)
		
		
		if ('poorSignalLevel' in packet.keys()):
			value = 200 - packet['poorSignalLevel']
			self.progressBarSignalContactQuality.setValue(value)
			self.textEditDebugConsole.append("")
			try:
				(date, localtime) = self.parseTimeStamp(packet['timestamp'])
				self.textEditDebugConsole.append("Timestamp: %s %s" % (date, localtime))
			except:
				pass
			self.textEditDebugConsole.append("poorSignalLevel: %i" % \
			                                 packet['poorSignalLevel'])
		
		
		if ('eSense' in packet.keys()):
			
			if ('attention' in packet['eSense'].keys()):
				value = packet['eSense']['attention']
				self.progressBarAttention.setValue(value)
				self.textEditDebugConsole.append("eSense attention: %i" % value)
			
			if ('meditation' in packet['eSense'].keys()):
				value = packet['eSense']['meditation']
				self.progressBarMeditation.setValue(value)
				self.textEditDebugConsole.append("eSense meditation: %i" % value)
			
			
			if MATPLOTLIB_AVAILABLE:
				self.chartEEGMatplot.update_values('eSense', packet['eSense'])
				if (self.tabWidget.currentIndex() == \
				    self.tabWidget.indexOf(self.tabCharts)):
					self.chartEEGMatplot.update_figure('eSense', packet['eSense'])
		
		
		if ('eegPower' in packet.keys()):
			
			# If we are not emulating packets we'll set the maximum EEG Power value
			# threshold to the default (or maximum value found within this packet)
			if not self.thinkGearConnectServer.emulate_headset_data:
				self.maxEEGPower = THINKGEAR_EMULATION_MAX_EEG_POWER_VALUE
			
			for value in packet['eegPower'].keys():
				if packet['eegPower'][value] > self.maxEEGPower:
					self.maxEEGPower = packet['eegPower'][value]
			
			
			if ('delta' in packet['eegPower'].keys()):
				value = packet['eegPower']['delta']
				self.progressBarEEGDelta.setMaximum(self.maxEEGPower)
				self.progressBarEEGDelta.setValue(value)
				self.textEditDebugConsole.append("delta: %i" % value)
			
			if ('theta' in packet['eegPower'].keys()):
				value = packet['eegPower']['theta']
				self.progressBarEEGTheta.setMaximum(self.maxEEGPower)
				self.progressBarEEGTheta.setValue(value)
				self.textEditDebugConsole.append("theta: %i" % value)
			
			if ('lowAlpha' in packet['eegPower'].keys()):
				value = packet['eegPower']['lowAlpha']
				self.progressBarEEGLowAlpha.setMaximum(self.maxEEGPower)
				self.progressBarEEGLowAlpha.setValue(value)
				self.textEditDebugConsole.append("lowAlpha: %i" % value)
			
			if ('highAlpha' in packet['eegPower'].keys()):
				value = packet['eegPower']['highAlpha']
				self.progressBarEEGHighAlpha.setMaximum(self.maxEEGPower)
				self.progressBarEEGHighAlpha.setValue(value)
				self.textEditDebugConsole.append("highAlpha: %i" % value)
			
			if ('lowBeta' in packet['eegPower'].keys()):
				value = packet['eegPower']['lowBeta']
				self.progressBarEEGLowBeta.setMaximum(self.maxEEGPower)
				self.progressBarEEGLowBeta.setValue(value)
				self.textEditDebugConsole.append("lowBeta: %i" % value)
			
			if ('highBeta' in packet['eegPower'].keys()):
				value = packet['eegPower']['highBeta']
				self.progressBarEEGHighBeta.setMaximum(self.maxEEGPower)
				self.progressBarEEGHighBeta.setValue(value)
				self.textEditDebugConsole.append("highBeta: %i" % value)
			
			if ('lowGamma' in packet['eegPower'].keys()):
				value = packet['eegPower']['lowGamma']
				self.progressBarEEGLowGamma.setMaximum(self.maxEEGPower)
				self.progressBarEEGLowGamma.setValue(value)
				self.textEditDebugConsole.append("lowGamma: %i" % value)
			
			if ('highGamma' in packet['eegPower'].keys()):
				value = packet['eegPower']['highGamma']
				self.progressBarEEGMidGamma.setMaximum(self.maxEEGPower)
				self.progressBarEEGMidGamma.setValue(value)
				self.textEditDebugConsole.append("highGamma: %i" % value)
			
			
			if MATPLOTLIB_AVAILABLE:
				self.chartEEGMatplot.update_values('eegPower', packet['eegPower'])
				if (self.tabWidget.currentIndex() == \
				    self.tabWidget.indexOf(self.tabCharts)):
					self.chartEEGMatplot.update_figure('eegPower', packet['eegPower'])
		
		
		if ((self.thinkGearConnectServer.protocol != None) and
		    (self.tabWidget.currentIndex() == \
		     self.tabWidget.indexOf(self.tabControlPanel))):
			
			self.updateProfileSessionStatus()
	
	
	##################################################################
	
	def updateProfileSessionStatus(self):
		
		session_time = self.calculateSessionTime()
		
		self.textLabelSessionTime.setText(session_time)
		
		self.textLabelPacketsReceived.setText( "%i" % \
			self.thinkGearConnectServer.protocol.packet_count)
		self.textLabelPacketsDropped.setText( "%i" % \
			self.thinkGearConnectServer.protocol.bad_packets)
	
	
	##################################################################
	
	def calculateSessionTime(self):
		
		session_time = time.time() - \
			self.thinkGearConnectServer.protocol.session_start_timestamp
		
		session_time = int(session_time)
		
		session_time = self.convert_seconds_to_datetime(session_time)
		
		return(session_time)
	
	
	##################################################################
	
	def enumerateSerialPorts(self):
		
		""" Uses the Win32 registry to return an
		iterator of serial (COM) ports
		existing on this computer.
		
		from http://eli.thegreenplace.net/2009/07/31/listing-all-serial-ports-on-windows-with-python/
		"""
	 
		path = 'HARDWARE\\DEVICEMAP\\SERIALCOMM'
		try:
			key = winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, path)
		except WindowsError:
			#raise IterationError
			return
		
		for i in itertools.count():
			try:
				val = winreg.EnumValue(key, i)
				yield str(val[1])
			except EnvironmentError:
				break
	
	
	##################################################################
	
	def fullPortName(self, portname):
		
		""" Given a port-name (of the form COM7,
		COM12, CNCA0, etc.) returns a full
		name suitable for opening with the
		Serial class.
		"""
		
		m = re.match('^COM(\d+)$', portname)
		if m and int(m.group(1)) < 10:
			return portname
		
		return '\\\\.\\' + portname
	
	
	##################################################################
	
	def searchForThinkGearDevices(self):
		
		#self.pushButtonBluetoothSearch.setText('Searching')
		
		mindset_devices = []
		
		if (sys.platform == 'win32'):
			
			for portname in self.enumerateSerialPorts():
				
				if portname not in mindset_devices:
					#portname = self.fullPortName(portname)
					mindset_devices.append(portname)
		
		
		else:
			
			# Bluetooth module doesn't compile properly under Windows
			
			bluetooth_devices = []
			
			#bluetooth_devices = bluetooth.discover_devices( \
				                    #duration=5, \
				                    #flush_cache=True, \
				                    #lookup_names=True)
			
			command = '%s con' % PATH_TO_HCITOOL
			
			output = os.popen(command, 'r')
			
			for line in output.readlines():
				try:
					address = line.split(' ')[2]
				except:
					pass
				else:
					bluetooth_devices.append(address)
			
			for address in bluetooth_devices:
				device_name = bluetooth.lookup_name(address)
				if ((device_name == 'MindSet') and \
				    (address not in mindset_devices)):
					mindset_devices.append(address)
			
			
			if self.DEBUG:
				print "Bluetooth MindSet devices found:",
				print mindset_devices
			
			
			self.comboBoxDeviceSelect.clear()
			
			self.comboBoxDeviceSelect.addItem('MindSet Emulator')
			
			if os.path.exists('/dev/ttyUSB0'):
				self.comboBoxDeviceSelect.addItem('/dev/ttyUSB0')
			if os.path.exists('/dev/ttyUSB1'):
				self.comboBoxDeviceSelect.addItem('/dev/ttyUSB1')
			if os.path.exists('/dev/ttyUSB2'):
				self.comboBoxDeviceSelect.addItem('/dev/ttyUSB2')
			if os.path.exists('/dev/ttyUSB3'):
				self.comboBoxDeviceSelect.addItem('/dev/ttyUSB3')
			if os.path.exists('/dev/ttyUSB4'):
				self.comboBoxDeviceSelect.addItem('/dev/ttyUSB4')
		
		
		for mindset_device in mindset_devices:
			self.comboBoxDeviceSelect.addItem(mindset_device)
		
		
		#self.pushButtonBluetoothSearch.setText('Search')
	
	
	##################################################################
	
	def collectData(self):
		
		data = {}
		
		data['rawEeg'] = self.packets['rawEeg']
		data['signals'] = self.packets['signals']
		
		data['sessionTime'] = self.calculateSessionTime()
		
		data['profileName'] = str(self.lineEditSessionProfile.text())
		
		return(data)
	
	
	##################################################################
	
	def saveData(self):
		
		data = self.collectData()
		
		#output_file = 'data.synapse'
		
		(date, localtime) = self.parseTimeStamp(time.time())
		
		default_filename = '%s %s.synapse' % (date, \
		                      self.lineEditSessionProfile.text())
		
		output_file = QtGui.QFileDialog.getSaveFileName(self, \
		                 "Save Synapse Data File", \
		                 default_filename, \
		                 "Synapse Data File (*.synapse)")
		
		
		file = open(output_file, 'w')
		pickle.dump(data, file)
		file.close()
	
	
	##################################################################
	
	def exportData(self):
		
		#output_file = 'text.synapse'
		
		(date, localtime) = self.parseTimeStamp(time.time())
		
		default_filename = '%s %s.csv' % (date, \
		                      self.lineEditSessionProfile.text())
		
		output_file = QtGui.QFileDialog.getSaveFileName(self, \
		                 "Export Synapse Data to File", \
		                 default_filename, \
		                 "CSV File (*.csv);;Text File (*.txt)")
		
		print output_file
		
		if str(output_file).endswith('.csv'):
			
			outputData = self.exportDataToCSV()
		
		
		else:
			
			outputData = self.textEditDebugConsole.toPlainText()
			
			
		file = open(output_file, 'w')
		file.write(outputData)
		file.close()
	
	
	##################################################################
	
	def exportDataToCSV(self):
		
		header = 'Date,Time,Delta,Theta,Low Alpha,High Alpha,Low Beta,High Beta,Low Gamma,Mid Gamma,Attention,Meditation,Signal Level\n'
		
		csv = {}
		
		for packet in self.packets['signals']:
			
			if 'rawEeg' in packet.keys():
				continue
			
			if packet['timestamp'] not in csv.keys():
				
				print packet
				timestamp = packet['timestamp']
				(date, localtime) = self.parseTimeStamp(timestamp)
				
				csv[timestamp] = {}
				csv[timestamp]['Date'] = date
				csv[timestamp]['Time'] = localtime
				csv[timestamp]['Delta'] = ''
				csv[timestamp]['Theta'] = ''
				csv[timestamp]['Low Alpha'] = ''
				csv[timestamp]['High Alpha'] = ''
				csv[timestamp]['Low Beta'] = ''
				csv[timestamp]['High Beta'] = ''
				csv[timestamp]['Low Gamma'] = ''
				csv[timestamp]['Mid Gamma'] = ''
				csv[timestamp]['Attention'] = ''
				csv[timestamp]['Meditation'] = ''
				csv[timestamp]['Signal Level'] = ''
			
			
			if 'eSense' in packet.keys():
				if 'attention' in packet['eSense'].keys():
					csv[timestamp]['Attention'] = packet['eSense']['attention']
				if 'meditation' in packet['eSense'].keys():
					csv[timestamp]['Meditation'] = packet['eSense']['meditation']
			
			if 'eegPower' in packet.keys():
				if 'delta' in packet['eegPower'].keys():
					csv[timestamp]['Delta'] = packet['eegPower']['delta']
				if 'theta' in packet['eegPower'].keys():
					csv[timestamp]['Theta'] = packet['eegPower']['theta']
				if 'lowAlpha' in packet['eegPower'].keys():
					csv[timestamp]['Low Alpha'] = packet['eegPower']['lowAlpha']
				if 'highAlpha' in packet['eegPower'].keys():
					csv[timestamp]['High Alpha'] = packet['eegPower']['highAlpha']
				if 'lowBeta' in packet['eegPower'].keys():
					csv[timestamp]['Low Beta'] = packet['eegPower']['lowBeta']
				if 'highBeta' in packet['eegPower'].keys():
					csv[timestamp]['High Beta'] = packet['eegPower']['highBeta']
				if 'lowGamma' in packet['eegPower'].keys():
					csv[timestamp]['Low Gamma'] = packet['eegPower']['lowGamma']
				if 'highGamma' in packet['eegPower'].keys():
					csv[timestamp]['Mid Gamma'] = packet['eegPower']['highGamma']
			
			if 'poorSignalLevel' in packet.keys():
				csv[timestamp]['Signal Level'] = packet['poorSignalLevel']
		
		
		output = header
		
		csv_keys = csv.keys()
		csv_keys.sort()
		
		for key in csv_keys:
			
			row = '%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n' % \
			      (csv[key]['Date'], \
			       csv[key]['Time'], \
			       csv[key]['Delta'], \
			       csv[key]['Theta'], \
			       csv[key]['Low Alpha'], \
			       csv[key]['High Alpha'], \
			       csv[key]['Low Beta'], \
			       csv[key]['High Beta'], \
			       csv[key]['Low Gamma'], \
			       csv[key]['Mid Gamma'], \
			       csv[key]['Attention'], \
			       csv[key]['Meditation'], \
			       csv[key]['Signal Level'])
			
			
			output = output + row
		
		
		return(output)
	
	
	##################################################################
	
	def resetData(self):
		
		self.packets['rawEeg'] = []
		self.packets['signals'] = []
		
		self.thinkGearConnectServer.protocol.session_start_timestamp = \
			time.time()
		
		self.thinkGearConnectServer.protocol.packet_count = 0
		self.thinkGearConnectServer.protocol.bad_packets = 0
		
		self.updateProfileSessionStatus()
		
		self.textEditDebugConsole.setText("")
	
	
	#####################################################################
	
	def convert_seconds_to_datetime(self, duration):
		
		duration_hours = duration / (60 * 60)
		duration_minutes = (duration - (duration_hours * (60 * 60))) / 60
		duration_seconds = (duration - (duration_hours * (60 * 60)) - (duration_minutes * 60))
		
		duration_hours = '%i' % duration_hours
		if (len(duration_hours) == 1):
			duration_hours = "0%s" % duration_hours
		
		duration_minutes = '%i' % duration_minutes
		if (len(duration_minutes) == 1):
			duration_minutes = "0%s" % duration_minutes
		
		duration_seconds = '%i' % duration_seconds
		if (len(duration_seconds) == 1):
			duration_seconds = "0%s" % duration_seconds
		
		datetime = '%s:%s:%s' % (duration_hours, duration_minutes, duration_seconds)
		
		return(datetime)
	
	
	##################################################################
	
	def closeEvent(self, event):
		
		quit_message = "Are you sure you want to exit the program?"
		
		reply = QtGui.QMessageBox.question( \
		           self, \
		          'Message', \
		           quit_message, \
		           QtGui.QMessageBox.Yes, \
		           QtGui.QMessageBox.No)
		
		if reply == QtGui.QMessageBox.Yes:
			
			if UPDATE_INTERFACE_VIA_TIMER:
				self.updateInterfaceTimer.stop()
			else:
				if self.thinkgearConnectClient != None:
					self.thinkgearConnectClient.disconnectFromHost()
			
			if self.thinkGearConnectServer != None:
				self.thinkGearConnectServer.exitThread()
			
			event.accept()
		
		else:
			event.ignore()
