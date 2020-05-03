# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'interface/puzzlebox_synapse_interface_design.ui'
#
# Created: Tue Sep 21 06:34:27 2010
#      by: PyQt4 UI code generator 4.7.3
#
# WARNING! All changes made in this file will be lost!

try:
	import PySide as PyQt4
	from PySide import QtCore, QtGui
except:
	print "Using PyQt4 module"
	from PyQt4 import QtCore, QtGui
else:
	print "Using PySide module"

#from PyQt4 import QtCore, QtGui

class Ui_Form(object):
	def setupUi(self, Form):
		Form.setObjectName("Form")
		Form.resize(800, 424)
		Form.setMinimumSize(QtCore.QSize(800, 424))
		Form.setMaximumSize(QtCore.QSize(800, 424))
		icon = QtGui.QIcon()
		icon.addPixmap(QtGui.QPixmap("images/puzzlebox.ico"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
		Form.setWindowIcon(icon)
		self.tabWidget = QtGui.QTabWidget(Form)
		self.tabWidget.setGeometry(QtCore.QRect(0, 0, 800, 424))
		self.tabWidget.setLayoutDirection(QtCore.Qt.LeftToRight)
		self.tabWidget.setObjectName("tabWidget")
		self.tabThinkGearConnectServer = QtGui.QWidget()
		self.tabThinkGearConnectServer.setObjectName("tabThinkGearConnectServer")
		self.lineHorizontalTop = QtGui.QFrame(self.tabThinkGearConnectServer)
		self.lineHorizontalTop.setGeometry(QtCore.QRect(8, 68, 161, 20))
		self.lineHorizontalTop.setFrameShape(QtGui.QFrame.HLine)
		self.lineHorizontalTop.setFrameShadow(QtGui.QFrame.Sunken)
		self.lineHorizontalTop.setObjectName("lineHorizontalTop")
		self.labelPuzzleboxIcon = QtGui.QLabel(self.tabThinkGearConnectServer)
		self.labelPuzzleboxIcon.setGeometry(QtCore.QRect(8, 8, 60, 60))
		self.labelPuzzleboxIcon.setText("")
		self.labelPuzzleboxIcon.setPixmap(QtGui.QPixmap("images/puzzlebox_logo.png"))
		self.labelPuzzleboxIcon.setScaledContents(False)
		self.labelPuzzleboxIcon.setAlignment(QtCore.Qt.AlignCenter)
		self.labelPuzzleboxIcon.setTextInteractionFlags(QtCore.Qt.NoTextInteraction)
		self.labelPuzzleboxIcon.setObjectName("labelPuzzleboxIcon")
		self.textLabelPuzzleboxSynapse = QtGui.QLabel(self.tabThinkGearConnectServer)
		self.textLabelPuzzleboxSynapse.setGeometry(QtCore.QRect(69, 8, 100, 60))
		self.textLabelPuzzleboxSynapse.setTextFormat(QtCore.Qt.AutoText)
		self.textLabelPuzzleboxSynapse.setWordWrap(False)
		self.textLabelPuzzleboxSynapse.setOpenExternalLinks(True)
		self.textLabelPuzzleboxSynapse.setObjectName("textLabelPuzzleboxSynapse")
		self.labelServerDaemon = QtGui.QLabel(self.tabThinkGearConnectServer)
		self.labelServerDaemon.setGeometry(QtCore.QRect(10, 246, 161, 31))
		self.labelServerDaemon.setAlignment(QtCore.Qt.AlignCenter)
		self.labelServerDaemon.setObjectName("labelServerDaemon")
		self.textLabelThinkGearPort = QtGui.QLabel(self.tabThinkGearConnectServer)
		self.textLabelThinkGearPort.setGeometry(QtCore.QRect(8, 309, 41, 30))
		self.textLabelThinkGearPort.setAlignment(QtCore.Qt.AlignCenter)
		self.textLabelThinkGearPort.setWordWrap(False)
		self.textLabelThinkGearPort.setObjectName("textLabelThinkGearPort")
		self.textLabelBluetoothAddress = QtGui.QLabel(self.tabThinkGearConnectServer)
		self.textLabelBluetoothAddress.setGeometry(QtCore.QRect(10, 133, 161, 30))
		self.textLabelBluetoothAddress.setWordWrap(False)
		self.textLabelBluetoothAddress.setObjectName("textLabelBluetoothAddress")
		self.textLabelThinkGearHost = QtGui.QLabel(self.tabThinkGearConnectServer)
		self.textLabelThinkGearHost.setGeometry(QtCore.QRect(8, 279, 41, 31))
		self.textLabelThinkGearHost.setAlignment(QtCore.Qt.AlignCenter)
		self.textLabelThinkGearHost.setObjectName("textLabelThinkGearHost")
		self.lineEditThinkGearPort = QtGui.QLineEdit(self.tabThinkGearConnectServer)
		self.lineEditThinkGearPort.setGeometry(QtCore.QRect(50, 312, 119, 26))
		self.lineEditThinkGearPort.setAlignment(QtCore.Qt.AlignHCenter)
		self.lineEditThinkGearPort.setObjectName("lineEditThinkGearPort")
		self.pushButtonThinkGearConnect = QtGui.QPushButton(self.tabThinkGearConnectServer)
		self.pushButtonThinkGearConnect.setGeometry(QtCore.QRect(8, 349, 161, 31))
		self.pushButtonThinkGearConnect.setCheckable(True)
		self.pushButtonThinkGearConnect.setObjectName("pushButtonThinkGearConnect")
		self.textLabelBluetoothStatus = QtGui.QLabel(self.tabThinkGearConnectServer)
		self.textLabelBluetoothStatus.setGeometry(QtCore.QRect(8, 107, 161, 30))
		self.textLabelBluetoothStatus.setAlignment(QtCore.Qt.AlignCenter)
		self.textLabelBluetoothStatus.setWordWrap(False)
		self.textLabelBluetoothStatus.setObjectName("textLabelBluetoothStatus")
		self.pushButtonBluetoothConnect = QtGui.QPushButton(self.tabThinkGearConnectServer)
		self.pushButtonBluetoothConnect.setGeometry(QtCore.QRect(82, 196, 86, 31))
		self.pushButtonBluetoothConnect.setCheckable(True)
		self.pushButtonBluetoothConnect.setObjectName("pushButtonBluetoothConnect")
		self.lineEditThinkGearHost = QtGui.QLineEdit(self.tabThinkGearConnectServer)
		self.lineEditThinkGearHost.setGeometry(QtCore.QRect(50, 280, 119, 26))
		self.lineEditThinkGearHost.setAlignment(QtCore.Qt.AlignHCenter)
		self.lineEditThinkGearHost.setObjectName("lineEditThinkGearHost")
		self.lineHorizontalLower = QtGui.QFrame(self.tabThinkGearConnectServer)
		self.lineHorizontalLower.setGeometry(QtCore.QRect(8, 228, 161, 20))
		self.lineHorizontalLower.setFrameShape(QtGui.QFrame.HLine)
		self.lineHorizontalLower.setFrameShadow(QtGui.QFrame.Sunken)
		self.lineHorizontalLower.setObjectName("lineHorizontalLower")
		self.labelNeuroSkyMindSet = QtGui.QLabel(self.tabThinkGearConnectServer)
		self.labelNeuroSkyMindSet.setGeometry(QtCore.QRect(8, 87, 161, 21))
		self.labelNeuroSkyMindSet.setAlignment(QtCore.Qt.AlignCenter)
		self.labelNeuroSkyMindSet.setObjectName("labelNeuroSkyMindSet")
		self.lineHorizontalDivider = QtGui.QFrame(self.tabThinkGearConnectServer)
		self.lineHorizontalDivider.setGeometry(QtCore.QRect(169, 6, 20, 381))
		self.lineHorizontalDivider.setFrameShape(QtGui.QFrame.VLine)
		self.lineHorizontalDivider.setFrameShadow(QtGui.QFrame.Sunken)
		self.lineHorizontalDivider.setObjectName("lineHorizontalDivider")
		self.textLabelMeditation = QtGui.QLabel(self.tabThinkGearConnectServer)
		self.textLabelMeditation.setGeometry(QtCore.QRect(653, 37, 71, 20))
		self.textLabelMeditation.setWordWrap(False)
		self.textLabelMeditation.setObjectName("textLabelMeditation")
		self.textLabelAttention = QtGui.QLabel(self.tabThinkGearConnectServer)
		self.textLabelAttention.setGeometry(QtCore.QRect(587, 37, 61, 20))
		self.textLabelAttention.setWordWrap(False)
		self.textLabelAttention.setObjectName("textLabelAttention")
		self.progressBarMeditation = QtGui.QProgressBar(self.tabThinkGearConnectServer)
		self.progressBarMeditation.setGeometry(QtCore.QRect(658, 74, 60, 301))
		self.progressBarMeditation.setProperty("value", 0)
		self.progressBarMeditation.setOrientation(QtCore.Qt.Vertical)
		self.progressBarMeditation.setObjectName("progressBarMeditation")
		self.progressBarAttention = QtGui.QProgressBar(self.tabThinkGearConnectServer)
		self.progressBarAttention.setGeometry(QtCore.QRect(588, 74, 60, 301))
		self.progressBarAttention.setProperty("value", 0)
		self.progressBarAttention.setTextVisible(True)
		self.progressBarAttention.setOrientation(QtCore.Qt.Vertical)
		self.progressBarAttention.setObjectName("progressBarAttention")
		self.progressBarEEGDelta = QtGui.QProgressBar(self.tabThinkGearConnectServer)
		self.progressBarEEGDelta.setGeometry(QtCore.QRect(188, 74, 41, 301))
		self.progressBarEEGDelta.setLayoutDirection(QtCore.Qt.LeftToRight)
		self.progressBarEEGDelta.setProperty("value", 0)
		self.progressBarEEGDelta.setTextVisible(False)
		self.progressBarEEGDelta.setOrientation(QtCore.Qt.Vertical)
		self.progressBarEEGDelta.setInvertedAppearance(False)
		self.progressBarEEGDelta.setTextDirection(QtGui.QProgressBar.TopToBottom)
		self.progressBarEEGDelta.setObjectName("progressBarEEGDelta")
		self.progressBarEEGTheta = QtGui.QProgressBar(self.tabThinkGearConnectServer)
		self.progressBarEEGTheta.setGeometry(QtCore.QRect(238, 74, 41, 301))
		self.progressBarEEGTheta.setProperty("value", 0)
		self.progressBarEEGTheta.setTextVisible(False)
		self.progressBarEEGTheta.setOrientation(QtCore.Qt.Vertical)
		self.progressBarEEGTheta.setObjectName("progressBarEEGTheta")
		self.progressBarEEGLowAlpha = QtGui.QProgressBar(self.tabThinkGearConnectServer)
		self.progressBarEEGLowAlpha.setGeometry(QtCore.QRect(288, 74, 41, 301))
		self.progressBarEEGLowAlpha.setProperty("value", 0)
		self.progressBarEEGLowAlpha.setTextVisible(False)
		self.progressBarEEGLowAlpha.setOrientation(QtCore.Qt.Vertical)
		self.progressBarEEGLowAlpha.setObjectName("progressBarEEGLowAlpha")
		self.progressBarEEGHighAlpha = QtGui.QProgressBar(self.tabThinkGearConnectServer)
		self.progressBarEEGHighAlpha.setGeometry(QtCore.QRect(338, 74, 41, 301))
		self.progressBarEEGHighAlpha.setProperty("value", 0)
		self.progressBarEEGHighAlpha.setTextVisible(False)
		self.progressBarEEGHighAlpha.setOrientation(QtCore.Qt.Vertical)
		self.progressBarEEGHighAlpha.setObjectName("progressBarEEGHighAlpha")
		self.progressBarEEGLowBeta = QtGui.QProgressBar(self.tabThinkGearConnectServer)
		self.progressBarEEGLowBeta.setGeometry(QtCore.QRect(388, 74, 41, 301))
		self.progressBarEEGLowBeta.setProperty("value", 0)
		self.progressBarEEGLowBeta.setTextVisible(False)
		self.progressBarEEGLowBeta.setOrientation(QtCore.Qt.Vertical)
		self.progressBarEEGLowBeta.setObjectName("progressBarEEGLowBeta")
		self.progressBarEEGHighBeta = QtGui.QProgressBar(self.tabThinkGearConnectServer)
		self.progressBarEEGHighBeta.setGeometry(QtCore.QRect(438, 74, 41, 301))
		self.progressBarEEGHighBeta.setProperty("value", 0)
		self.progressBarEEGHighBeta.setTextVisible(False)
		self.progressBarEEGHighBeta.setOrientation(QtCore.Qt.Vertical)
		self.progressBarEEGHighBeta.setObjectName("progressBarEEGHighBeta")
		self.progressBarEEGLowGamma = QtGui.QProgressBar(self.tabThinkGearConnectServer)
		self.progressBarEEGLowGamma.setGeometry(QtCore.QRect(488, 74, 41, 301))
		self.progressBarEEGLowGamma.setProperty("value", 0)
		self.progressBarEEGLowGamma.setTextVisible(False)
		self.progressBarEEGLowGamma.setOrientation(QtCore.Qt.Vertical)
		self.progressBarEEGLowGamma.setObjectName("progressBarEEGLowGamma")
		self.progressBarEEGMidGamma = QtGui.QProgressBar(self.tabThinkGearConnectServer)
		self.progressBarEEGMidGamma.setGeometry(QtCore.QRect(538, 74, 41, 301))
		self.progressBarEEGMidGamma.setProperty("value", 0)
		self.progressBarEEGMidGamma.setTextVisible(False)
		self.progressBarEEGMidGamma.setOrientation(QtCore.Qt.Vertical)
		self.progressBarEEGMidGamma.setObjectName("progressBarEEGMidGamma")
		self.textLabelContactQuality = QtGui.QLabel(self.tabThinkGearConnectServer)
		self.textLabelContactQuality.setGeometry(QtCore.QRect(732, 39, 51, 30))
		self.textLabelContactQuality.setWordWrap(False)
		self.textLabelContactQuality.setObjectName("textLabelContactQuality")
		self.progressBarSignalContactQuality = QtGui.QProgressBar(self.tabThinkGearConnectServer)
		self.progressBarSignalContactQuality.setGeometry(QtCore.QRect(728, 74, 60, 301))
		self.progressBarSignalContactQuality.setProperty("value", 0)
		self.progressBarSignalContactQuality.setOrientation(QtCore.Qt.Vertical)
		self.progressBarSignalContactQuality.setObjectName("progressBarSignalContactQuality")
		self.labeleSense = QtGui.QLabel(self.tabThinkGearConnectServer)
		self.labeleSense.setGeometry(QtCore.QRect(620, 12, 71, 20))
		self.labeleSense.setObjectName("labeleSense")
		self.labelEEGBrainWaves = QtGui.QLabel(self.tabThinkGearConnectServer)
		self.labelEEGBrainWaves.setGeometry(QtCore.QRect(300, 12, 171, 20))
		self.labelEEGBrainWaves.setObjectName("labelEEGBrainWaves")
		self.labelDelta = QtGui.QLabel(self.tabThinkGearConnectServer)
		self.labelDelta.setGeometry(QtCore.QRect(189, 50, 41, 17))
		self.labelDelta.setObjectName("labelDelta")
		self.labelTheta = QtGui.QLabel(self.tabThinkGearConnectServer)
		self.labelTheta.setGeometry(QtCore.QRect(239, 50, 41, 17))
		self.labelTheta.setObjectName("labelTheta")
		self.labelLowAlpha = QtGui.QLabel(self.tabThinkGearConnectServer)
		self.labelLowAlpha.setGeometry(QtCore.QRect(289, 40, 41, 31))
		self.labelLowAlpha.setObjectName("labelLowAlpha")
		self.labelHighAlpha = QtGui.QLabel(self.tabThinkGearConnectServer)
		self.labelHighAlpha.setGeometry(QtCore.QRect(339, 40, 41, 31))
		self.labelHighAlpha.setObjectName("labelHighAlpha")
		self.labelLowBeta = QtGui.QLabel(self.tabThinkGearConnectServer)
		self.labelLowBeta.setGeometry(QtCore.QRect(388, 40, 41, 31))
		self.labelLowBeta.setObjectName("labelLowBeta")
		self.labelHighBeta = QtGui.QLabel(self.tabThinkGearConnectServer)
		self.labelHighBeta.setGeometry(QtCore.QRect(438, 40, 41, 31))
		self.labelHighBeta.setObjectName("labelHighBeta")
		self.labelLowGamma = QtGui.QLabel(self.tabThinkGearConnectServer)
		self.labelLowGamma.setGeometry(QtCore.QRect(486, 40, 44, 31))
		self.labelLowGamma.setObjectName("labelLowGamma")
		self.labelMidGamma = QtGui.QLabel(self.tabThinkGearConnectServer)
		self.labelMidGamma.setGeometry(QtCore.QRect(537, 40, 44, 31))
		self.labelMidGamma.setObjectName("labelMidGamma")
		self.labelSignal = QtGui.QLabel(self.tabThinkGearConnectServer)
		self.labelSignal.setGeometry(QtCore.QRect(727, 12, 60, 20))
		self.labelSignal.setObjectName("labelSignal")
		self.comboBoxDeviceSelect = QtGui.QComboBox(self.tabThinkGearConnectServer)
		self.comboBoxDeviceSelect.setGeometry(QtCore.QRect(8, 161, 160, 27))
		self.comboBoxDeviceSelect.setObjectName("comboBoxDeviceSelect")
		self.comboBoxDeviceSelect.addItem("")
		self.pushButtonBluetoothSearch = QtGui.QPushButton(self.tabThinkGearConnectServer)
		self.pushButtonBluetoothSearch.setGeometry(QtCore.QRect(8, 196, 66, 31))
		self.pushButtonBluetoothSearch.setCheckable(False)
		self.pushButtonBluetoothSearch.setObjectName("pushButtonBluetoothSearch")
		self.tabWidget.addTab(self.tabThinkGearConnectServer, "")
		self.tabEEGSignals = QtGui.QWidget()
		self.tabEEGSignals.setObjectName("tabEEGSignals")
		self.tabWidget.addTab(self.tabEEGSignals, "")
		self.tabCharts = QtGui.QWidget()
		self.tabCharts.setObjectName("tabCharts")
		self.tabWidget.addTab(self.tabCharts, "")
		self.tabControlPanel = QtGui.QWidget()
		self.tabControlPanel.setObjectName("tabControlPanel")
		self.textEditDebugConsole = QtGui.QTextEdit(self.tabControlPanel)
		self.textEditDebugConsole.setGeometry(QtCore.QRect(185, 3, 601, 386))
		self.textEditDebugConsole.setVerticalScrollBarPolicy(QtCore.Qt.ScrollBarAlwaysOn)
		self.textEditDebugConsole.setHorizontalScrollBarPolicy(QtCore.Qt.ScrollBarAlwaysOff)
		self.textEditDebugConsole.setReadOnly(True)
		self.textEditDebugConsole.setObjectName("textEditDebugConsole")
		self.pushButtonReset = QtGui.QPushButton(self.tabControlPanel)
		self.pushButtonReset.setGeometry(QtCore.QRect(43, 355, 77, 25))
		self.pushButtonReset.setObjectName("pushButtonReset")
		self.textLabelPuzzleboxSynapse_2 = QtGui.QLabel(self.tabControlPanel)
		self.textLabelPuzzleboxSynapse_2.setGeometry(QtCore.QRect(61, 2, 100, 60))
		self.textLabelPuzzleboxSynapse_2.setTextFormat(QtCore.Qt.AutoText)
		self.textLabelPuzzleboxSynapse_2.setWordWrap(False)
		self.textLabelPuzzleboxSynapse_2.setOpenExternalLinks(True)
		self.textLabelPuzzleboxSynapse_2.setObjectName("textLabelPuzzleboxSynapse_2")
		self.lineHorizontalDivider_2 = QtGui.QFrame(self.tabControlPanel)
		self.lineHorizontalDivider_2.setGeometry(QtCore.QRect(161, 0, 20, 381))
		self.lineHorizontalDivider_2.setFrameShape(QtGui.QFrame.VLine)
		self.lineHorizontalDivider_2.setFrameShadow(QtGui.QFrame.Sunken)
		self.lineHorizontalDivider_2.setObjectName("lineHorizontalDivider_2")
		self.lineHorizontalTop_2 = QtGui.QFrame(self.tabControlPanel)
		self.lineHorizontalTop_2.setGeometry(QtCore.QRect(0, 62, 161, 20))
		self.lineHorizontalTop_2.setFrameShape(QtGui.QFrame.HLine)
		self.lineHorizontalTop_2.setFrameShadow(QtGui.QFrame.Sunken)
		self.lineHorizontalTop_2.setObjectName("lineHorizontalTop_2")
		self.labelPuzzleboxIcon_2 = QtGui.QLabel(self.tabControlPanel)
		self.labelPuzzleboxIcon_2.setGeometry(QtCore.QRect(0, 2, 60, 60))
		self.labelPuzzleboxIcon_2.setText("")
		self.labelPuzzleboxIcon_2.setPixmap(QtGui.QPixmap("images/puzzlebox_logo.png"))
		self.labelPuzzleboxIcon_2.setScaledContents(False)
		self.labelPuzzleboxIcon_2.setAlignment(QtCore.Qt.AlignCenter)
		self.labelPuzzleboxIcon_2.setTextInteractionFlags(QtCore.Qt.NoTextInteraction)
		self.labelPuzzleboxIcon_2.setObjectName("labelPuzzleboxIcon_2")
		self.lineHorizontalTop_3 = QtGui.QFrame(self.tabControlPanel)
		self.lineHorizontalTop_3.setGeometry(QtCore.QRect(0, 250, 161, 20))
		self.lineHorizontalTop_3.setFrameShape(QtGui.QFrame.HLine)
		self.lineHorizontalTop_3.setFrameShadow(QtGui.QFrame.Sunken)
		self.lineHorizontalTop_3.setObjectName("lineHorizontalTop_3")
		self.pushButtonSave = QtGui.QPushButton(self.tabControlPanel)
		self.pushButtonSave.setGeometry(QtCore.QRect(43, 295, 77, 25))
		self.pushButtonSave.setObjectName("pushButtonSave")
		self.pushButtonExport = QtGui.QPushButton(self.tabControlPanel)
		self.pushButtonExport.setGeometry(QtCore.QRect(43, 325, 77, 25))
		self.pushButtonExport.setObjectName("pushButtonExport")
		self.textLabelProfile = QtGui.QLabel(self.tabControlPanel)
		self.textLabelProfile.setGeometry(QtCore.QRect(30, 80, 105, 21))
		self.textLabelProfile.setWordWrap(False)
		self.textLabelProfile.setObjectName("textLabelProfile")
		self.textLabelDataHistory = QtGui.QLabel(self.tabControlPanel)
		self.textLabelDataHistory.setGeometry(QtCore.QRect(30, 270, 105, 21))
		self.textLabelDataHistory.setWordWrap(False)
		self.textLabelDataHistory.setObjectName("textLabelDataHistory")
		self.textLabelSessionTimeTitle = QtGui.QLabel(self.tabControlPanel)
		self.textLabelSessionTimeTitle.setGeometry(QtCore.QRect(0, 135, 160, 20))
		self.textLabelSessionTimeTitle.setWordWrap(False)
		self.textLabelSessionTimeTitle.setObjectName("textLabelSessionTimeTitle")
		self.textLabelSessionTime = QtGui.QLabel(self.tabControlPanel)
		self.textLabelSessionTime.setGeometry(QtCore.QRect(50, 155, 62, 20))
		self.textLabelSessionTime.setWordWrap(False)
		self.textLabelSessionTime.setObjectName("textLabelSessionTime")
		self.textLabelPacketsReceivedTitle = QtGui.QLabel(self.tabControlPanel)
		self.textLabelPacketsReceivedTitle.setGeometry(QtCore.QRect(3, 175, 160, 20))
		self.textLabelPacketsReceivedTitle.setWordWrap(False)
		self.textLabelPacketsReceivedTitle.setObjectName("textLabelPacketsReceivedTitle")
		self.textLabelPacketsReceived = QtGui.QLabel(self.tabControlPanel)
		self.textLabelPacketsReceived.setGeometry(QtCore.QRect(50, 195, 60, 20))
		self.textLabelPacketsReceived.setWordWrap(False)
		self.textLabelPacketsReceived.setObjectName("textLabelPacketsReceived")
		self.textLabelPacketsDroppedTitle = QtGui.QLabel(self.tabControlPanel)
		self.textLabelPacketsDroppedTitle.setGeometry(QtCore.QRect(0, 215, 160, 20))
		self.textLabelPacketsDroppedTitle.setWordWrap(False)
		self.textLabelPacketsDroppedTitle.setObjectName("textLabelPacketsDroppedTitle")
		self.textLabelPacketsDropped = QtGui.QLabel(self.tabControlPanel)
		self.textLabelPacketsDropped.setGeometry(QtCore.QRect(50, 235, 60, 20))
		self.textLabelPacketsDropped.setWordWrap(False)
		self.textLabelPacketsDropped.setObjectName("textLabelPacketsDropped")
		self.lineEditSessionProfile = QtGui.QLineEdit(self.tabControlPanel)
		self.lineEditSessionProfile.setGeometry(QtCore.QRect(9, 105, 150, 25))
		self.lineEditSessionProfile.setAlignment(QtCore.Qt.AlignHCenter)
		self.lineEditSessionProfile.setObjectName("lineEditSessionProfile")
		self.tabWidget.addTab(self.tabControlPanel, "")

		self.retranslateUi(Form)
		self.tabWidget.setCurrentIndex(0)
		QtCore.QMetaObject.connectSlotsByName(Form)

	def retranslateUi(self, Form):
		Form.setWindowTitle(QtGui.QApplication.translate("Form", "Puzzlebox Synapse", None, QtGui.QApplication.UnicodeUTF8))
		self.textLabelPuzzleboxSynapse.setText(QtGui.QApplication.translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"http://brainstorms.puzzlebox.info\"><span style=\" font-size:11pt; text-decoration: none; color:#000000;\">Puzzlebox<br />Synapse</span></a></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
		self.labelServerDaemon.setText(QtGui.QApplication.translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Server Daemon</span></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
		self.textLabelThinkGearPort.setText(QtGui.QApplication.translate("Form", "<p align=\"center\">Port</p>", None, QtGui.QApplication.UnicodeUTF8))
		self.textLabelBluetoothAddress.setText(QtGui.QApplication.translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Bluetooth Address</p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
		self.textLabelThinkGearHost.setText(QtGui.QApplication.translate("Form", "Host", None, QtGui.QApplication.UnicodeUTF8))
		self.lineEditThinkGearPort.setText(QtGui.QApplication.translate("Form", "N/A", None, QtGui.QApplication.UnicodeUTF8))
		self.pushButtonThinkGearConnect.setText(QtGui.QApplication.translate("Form", "Start", None, QtGui.QApplication.UnicodeUTF8))
		self.textLabelBluetoothStatus.setText(QtGui.QApplication.translate("Form", "<p align=\"center\">Status: Disconnected</p>", None, QtGui.QApplication.UnicodeUTF8))
		self.pushButtonBluetoothConnect.setText(QtGui.QApplication.translate("Form", "Connect", None, QtGui.QApplication.UnicodeUTF8))
		self.lineEditThinkGearHost.setText(QtGui.QApplication.translate("Form", "N/A", None, QtGui.QApplication.UnicodeUTF8))
		self.labelNeuroSkyMindSet.setText(QtGui.QApplication.translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">NeuroSky MindSet</span></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
		self.textLabelMeditation.setText(QtGui.QApplication.translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:9pt;\">Meditation</span></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
		self.textLabelAttention.setText(QtGui.QApplication.translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:9pt;\">Attention</span></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
		self.textLabelContactQuality.setText(QtGui.QApplication.translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:9pt;\">Contact<br />Quality</span></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
		self.labeleSense.setText(QtGui.QApplication.translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"http://company.neurosky.com/files/neurosky_esense_whitepaper.pdf\"><span style=\" font-size:12pt; font-weight:600;text-decoration: none; color:#000000;\">eSense</span></a></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
		self.labelEEGBrainWaves.setText(QtGui.QApplication.translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"http://company.neurosky.com/files/neurosky_esense_whitepaper.pdf\"><span style=\" font-size:12pt; font-weight:600;text-decoration: none; color:#000000;\">EEG Brain Signals</span></a></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
		self.labelDelta.setText(QtGui.QApplication.translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"http://en.wikipedia.org/wiki/Delta_wave\"><span style=\" font-size:8pt; text-decoration: none; color:#000000;\">Delta</span></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
		self.labelTheta.setText(QtGui.QApplication.translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"http://en.wikipedia.org/wiki/Theta_rhythm\"><span style=\" font-size:8pt; text-decoration: none; color:#000000;\">Theta</span></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
		self.labelLowAlpha.setText(QtGui.QApplication.translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<a href=\"http://en.wikipedia.org/wiki/Alpha_wave\"><p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; text-decoration: none; color:#000000;\">Low</span></p><p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; text-decoration: none; color:#000000;\">Alpha</span></p></a></body></html>", None, QtGui.QApplication.UnicodeUTF8))
		self.labelHighAlpha.setText(QtGui.QApplication.translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<a href=\"http://en.wikipedia.org/wiki/Alpha_wave\"><p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; text-decoration: none; color:#000000;\">High</span></p><p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; text-decoration: none; color:#000000;\">Alpha</span></p></a></body></html>", None, QtGui.QApplication.UnicodeUTF8))
		self.labelLowBeta.setText(QtGui.QApplication.translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<a href=\"http://en.wikipedia.org/wiki/Beta_wave\"><p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; text-decoration: none; color:#000000;\">Low</span></p><p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; text-decoration: none; color:#000000;\">Beta</span></p></a></body></html>", None, QtGui.QApplication.UnicodeUTF8))
		self.labelHighBeta.setText(QtGui.QApplication.translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<a href=\"http://en.wikipedia.org/wiki/Beta_wave\"><p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; text-decoration: none; color:#000000;\">High</span></p><p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; text-decoration: none; color:#000000;\">Beta</span></p></a></body></html>", None, QtGui.QApplication.UnicodeUTF8))
		self.labelLowGamma.setText(QtGui.QApplication.translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<a href=\"http://en.wikipedia.org/wiki/Gamma_wave\"><p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; text-decoration: none; color:#000000;\">Low</span></p><p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; text-decoration: none; color:#000000;\">Gamma</span></p></a></body></html>", None, QtGui.QApplication.UnicodeUTF8))
		self.labelMidGamma.setText(QtGui.QApplication.translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<a href=\"http://en.wikipedia.org/wiki/Gamma_wave\"><p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; text-decoration: none; color:#000000;\">Mid</span></p><p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; text-decoration: none; color:#000000;\">Gamma</span></p></a></body></html>", None, QtGui.QApplication.UnicodeUTF8))
		self.labelSignal.setText(QtGui.QApplication.translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:12pt; font-weight:600;\">Signal</span></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
		self.comboBoxDeviceSelect.setItemText(0, QtGui.QApplication.translate("Form", "MindSet Emulator", None, QtGui.QApplication.UnicodeUTF8))
		self.pushButtonBluetoothSearch.setText(QtGui.QApplication.translate("Form", "Search", None, QtGui.QApplication.UnicodeUTF8))
		self.tabWidget.setTabText(self.tabWidget.indexOf(self.tabThinkGearConnectServer), QtGui.QApplication.translate("Form", "ThinkGear Connect Server", None, QtGui.QApplication.UnicodeUTF8))
		self.tabWidget.setTabText(self.tabWidget.indexOf(self.tabEEGSignals), QtGui.QApplication.translate("Form", "EEG Waves", None, QtGui.QApplication.UnicodeUTF8))
		self.tabWidget.setTabText(self.tabWidget.indexOf(self.tabCharts), QtGui.QApplication.translate("Form", "Charts", None, QtGui.QApplication.UnicodeUTF8))
		self.textEditDebugConsole.setHtml(QtGui.QApplication.translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
		self.pushButtonReset.setText(QtGui.QApplication.translate("Form", "Reset", None, QtGui.QApplication.UnicodeUTF8))
		self.textLabelPuzzleboxSynapse_2.setText(QtGui.QApplication.translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"http://brainstorms.puzzlebox.info\"><span style=\" font-size:11pt; text-decoration: none; color:#000000;\">Puzzlebox<br />Synapse</span></a></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
		self.pushButtonSave.setText(QtGui.QApplication.translate("Form", "Save", None, QtGui.QApplication.UnicodeUTF8))
		self.pushButtonExport.setText(QtGui.QApplication.translate("Form", "Export", None, QtGui.QApplication.UnicodeUTF8))
		self.textLabelProfile.setText(QtGui.QApplication.translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'MS Shell Dlg 2\'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:\'Sans\'; font-size:10pt; font-weight:600;\">Profile</span></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
		self.textLabelDataHistory.setText(QtGui.QApplication.translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'MS Shell Dlg 2\'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:\'Sans\'; font-size:10pt; font-weight:600;\">Data History</span></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
		self.textLabelSessionTimeTitle.setText(QtGui.QApplication.translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'MS Shell Dlg 2\'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:\'Sans\'; font-size:10pt; font-weight:600;\">Session Time</span></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
		self.textLabelSessionTime.setText(QtGui.QApplication.translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'MS Shell Dlg 2\'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:\'Sans\'; font-size:9pt;\">00:00:00</span></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
		self.textLabelPacketsReceivedTitle.setText(QtGui.QApplication.translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Packets Received</span></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
		self.textLabelPacketsReceived.setText(QtGui.QApplication.translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'MS Shell Dlg 2\'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:\'Sans\'; font-size:9pt;\">0</span></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
		self.textLabelPacketsDroppedTitle.setText(QtGui.QApplication.translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'Sans\'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Packets Dropped</span></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
		self.textLabelPacketsDropped.setText(QtGui.QApplication.translate("Form", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'MS Shell Dlg 2\'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:\'Sans\'; font-size:9pt;\">0</span></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
		self.lineEditSessionProfile.setText(QtGui.QApplication.translate("Form", "Untitled Session", None, QtGui.QApplication.UnicodeUTF8))
		self.tabWidget.setTabText(self.tabWidget.indexOf(self.tabControlPanel), QtGui.QApplication.translate("Form", "Control Panel", None, QtGui.QApplication.UnicodeUTF8))

