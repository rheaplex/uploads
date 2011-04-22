#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Puzzlebox - ThinkGear Emulator - Server
#
# Copyright Puzzlebox Productions, LLC (2010)
#
# This code is released under the GNU Pulic License (GPL) version 2
# For more information please refer to http://www.gnu.org/copyleft/gpl.html
#
# Last Update: 2010.07.08
#
#####################################################################

import math, os, signal, sys, time
import simplejson as json

from twisted.internet import reactor, protocol, defer, task
from twisted.protocols import basic

import puzzlebox_thinkgear_emulator_configuration as configuration
#import puzzlebox_logger

#####################################################################
# Globals
#####################################################################

DEBUG = 1

SERVER_INTERFACE = configuration.THINKGEAR_SERVER_INTERFACE
SERVER_PORT = configuration.THINKGEAR_SERVER_PORT

FLASH_POLICY_FILE_REQUEST = configuration.FLASH_POLICY_FILE_REQUEST
FLASH_SOCKET_POLICY_FILE = configuration.FLASH_SOCKET_POLICY_FILE

THINKGEAR_DELIMITER = '\r'

MESSAGE_FREQUENCY_TIMER = 1 # 1 Hz

ENABLE_SIMULATE_HEADSET_DATA = True

BLINK_FREQUENCY_TIMER = 6 # blink every 6 seconds
                          # (6 seconds is listed by Wikipedia
                          # as being the average number of times
                          # an adult blinks in a laboratory setting)

DEFAULT_SAMPLE_WAVELENGTH = 10 # number of seconds from 0 to max to 0 for
                               # any given detection value below

DEFAULT_AUTHORIZATION_MESSAGE = \
	{"isAuthorized": True}
		# Tells the client whether the server has authorized
		# access to the user's headset data. The value is
		# either true or false.

DEFAULT_SIGNAL_LEVEL_MESSAGE = \
	{"poorSignalLevel": 0}
		# A quantifier of the quality of the brainwave signal.
		# This is an integer value that is generally in the
		# range of 0 to 200, with 0 indicating a
		# good signal and 200 indicating an off-head state.

DEFAULT_EEG_POWER_MESSAGE = \
	{"eegPower": { \
		'delta': 0, \
		'theta': 0, \
		'lowAlpha': 0, \
		'highAlpha': 0, \
		'lowBeta': 0, \
		'highBeta': 0, \
		'lowGamma': 0, \
		'highGamma': 0, \
		}, \
	 } # A container for the EEG powers. These may
	   # be either integer or floating-point values.
	   # Maximum values are undocumented but assumed to be 65535

DEFAULT_ESENSE_MESSAGE = \
	{"eSense": { \
		'attention': 0, \
		'meditation': 0, \
		}, \
	} # A container for the eSense™ attributes.
	  # These are integer values between 0 and 100,
	  # where 0 is perceived as a lack of that attribute
	  # and 100 is an excess of that attribute.

DEFAULT_BLINK_MESSAGE = {"blinkStrength": 255}
	# The strength of a detected blink. This is
	# an integer in the range of 0-255.

DEFAULT_RAWEEG_MESSAGE = {"rawEeg": 255}
	# The raw data reading off the forehead sensor.
	# This may be either an integer or a floating-point value.

DEFAULT_PACKET = {}
DEFAULT_PACKET.update(DEFAULT_EEG_POWER_MESSAGE)
DEFAULT_PACKET.update(DEFAULT_SIGNAL_LEVEL_MESSAGE)
DEFAULT_PACKET.update(DEFAULT_ESENSE_MESSAGE)

DEFAULT_RESPONSE_MESSAGE = DEFAULT_SIGNAL_LEVEL_MESSAGE

#####################################################################
# Classes
#####################################################################

class puzzlebox_thinkgear_server(protocol.ServerFactory):
	
	def __init__(self, log, DEBUG=DEBUG):
		
		self.log = log
		self.DEBUG = DEBUG
		
		self.protocol = puzzlebox_thinkgear_server_protocol

		self.message_frequency_timer = MESSAGE_FREQUENCY_TIMER
		self.blink_frequency_timer = BLINK_FREQUENCY_TIMER

		self.client_connected = False
		self.connection_timestamp = time.time()
		self.blink_timestamp = time.time()

		self.packet_queue = []


	##################################################################

	def validate_checksum(self, checksum):

		'''The key used by the client application to identify 
itself. This must be 40 hexadecimal characters, ideally generated
using an SHA-1 digest. The appKey is an identifier that is unique
to each application, rather than each instance of an application.
It is used by the server to bypass the authorization process if a
user had previously authorized the requesting client. To reduce
the chance of overlap with the appKey of other applications, 
the appKey should be generated using an SHA-1 digest.'''

		is_valid = True

		hexadecimal_characters = '0123456789abcdef'

		if len(checksum) != 40:
			is_valid = False
		else:
			for character in checksum:
				if character not in hexadecimal_characters:
					is_valid = False
		
		return(is_valid)
	
	
	##################################################################
	
	def authorize_client(self, data):
	
		'''The client must initiate an authorization request
and the server must authorize the client before the
server will start transmitting any headset data.'''

		is_authorized = self.validate_checksum(data['appKey'])

		# A human-readable name identifying the client
		# application. This can be a maximum of 255 characters.
	
		if len(data['appName']) > 255:
			is_authorized = False


		return(is_authorized)
	

	##################################################################

	def calculate_wave_point(self, x, max_height=100, wave_length=10):

		# start at 0, increase to max value at half of one
		# wavelength, decrease to 0 by end of wavelength
		y = ( (max_height/2) * \
		      math.sin ((x-1) * ( math.pi / (wave_length / 2)))) + \
		      (max_height/2)

		# start at max value, decrease to 0 at half of one
		# wavelegnth, increase to max by end of wavelength
##		y = ( (max_height/2) * \
##		      math.cos (x * ( math.pi / (wave_length / 2)))) + \
##		      (max_height/2)


		return(y)


	##################################################################

	def simulate_headset_data(self):

		response = DEFAULT_PACKET

		time_value = self.connection_timestamp - time.time()

		for key in response.keys():

			if key == 'poorSignalLevel':
				pass

			elif key == 'eSense':
				plot = self.calculate_wave_point( \
					time_value, \
					max_height=100, \
					wave_length=DEFAULT_SAMPLE_WAVELENGTH)

				for each in response[key].keys():
					response[key][each] = plot

			elif key == 'eegPower':
				plot = self.calculate_wave_point( \
					time_value, \
					max_height=65535, \
					wave_length=DEFAULT_SAMPLE_WAVELENGTH)

				for each in response[key].keys():
					response[key][each] = plot


		return(response)


        ##################################################################

	def process_data(self, data):

		d = defer.Deferred()

		# Special socket handling for Flash applications
		if (data == FLASH_POLICY_FILE_REQUEST):
			self.packet_queue.insert(0, FLASH_SOCKET_POLICY_FILE)
			
		elif (type(data) == type({}) and \
		      data.has_key('appName') and \
		      data.has_key('appKey')):
			authorized = self.authorize_client(data)

			response = {}
			response['isAuthorized'] = authorized

			self.packet_queue.insert(0, response)

		else:
			# No specific information needs to be returned
			pass

		
		if self.DEBUG:
			print "<-- [ThinkGear Emulator] Received:",
			print data

		
		d.callback(self.message_frequency_timer)

		
		return d
	
	
	##################################################################
	
	def process_connection_lost(self):

		if self.DEBUG:
			print "--> [ThinkGear Emulator] Connection lost"

		self.client_connected = False

		try:
			self.looping_timer.stop()
		except Exception, e:
			if self.DEBUG:
				print "ERROR: Failed to stop looping timer:",
				print e


	##################################################################

##        def get_next_packet(self)
##
##                for packet in self.packet_queue:
##
##                        try:
##                                
##                                if packet.has_key('isAuthorized'):

	##################################################################

	def update_status(self):

		if ENABLE_SIMULATE_HEADSET_DATA:

			# Craft a simulated data packet
			self.packet_queue.append( \
				self.simulate_headset_data() )

			# Include simulated blinks at desired frequency
			if ((self.blink_frequency_timer != None) and \
			    (time.time() - self.blink_timestamp > \
			     self.blink_frequency_timer)):

				self.blink_timestamp = time.time()

				packet = DEFAULT_BLINK_MESSAGE
				self.packet_queue.append(packet)

		
	##################################################################

	def start_updating(self):

		self.client_connected = True
		self.connection_timestamp = time.time()
		self.looping_timer = task.LoopingCall(self.update_status)
		self.looping_timer.start(self.message_frequency_timer)


#####################################################################
# Protocol
#####################################################################

class puzzlebox_thinkgear_server_protocol(basic.LineReceiver):

	delimiter='\r'
	
	def __init__(self):
		
		self.DEBUG = DEBUG
		self.data_chunk = ""

	
	##################################################################
	
	def connectionMade(self):
		
		if self.DEBUG:
			print "<---- [ThinkGear Emulator] Client connected"
			
		self.factory.start_updating()


	##################################################################
	
	def noReply(self):
		
		try:
			self.factory.replyDefer.callback('NO_REPLY')
		except:
			if self.DEBUG:
				print "noReply failed to call callback"
		
		self.transport.loseConnection()

	
	##################################################################
	
	def dataReceived(self, data_received):

		data_to_process = None
		
		self.data_chunk += data_received
		
		try:
			data_to_process = json.loads(self.data_chunk)
		
		except Exception, e:

			# Special socket handling for Flash applications
			if (data_received == FLASH_POLICY_FILE_REQUEST):
                                
				if self.DEBUG:
					print "<-- [ThinkGear Emulator] Flash policy file requested"
                                        
				data_to_process = data_received
                                
                                
			else:
				if self.DEBUG:
					print "<-- [ThinkGear Emulator] Partial data received (or error:",
					print e
					print ")."

					print "data_chunk:",
					print self.data_chunk


		if (data_to_process != None):

			self.data_chunk = ""

			d = self.factory.process_data(data_to_process)
			d.addCallback(self.send_response)


	##################################################################
	
	def send_response(self, delay_timer):

		while (len(self.factory.packet_queue) > 0):

			packet = self.factory.packet_queue[0]
			del self.factory.packet_queue[0]
##                        packet = self.factory.packet_queue.pop()

			# Special socket handling for Flash applications
			if (packet != FLASH_SOCKET_POLICY_FILE):
				packet = json.dumps(packet)

			if self.DEBUG:
				print "--> [ThinkGear Emulator] Sending:",
				print packet

			self.sendLine(packet)


		if self.factory.client_connected:

			reactor.callLater(delay_timer, \
					  self.send_response, delay_timer)

				
	##################################################################
	
	def connectionLost(self, reason):
		
		self.factory.process_connection_lost()


#####################################################################
# Main
#####################################################################

if __name__ == '__main__':
	
	#log = puzzlebox_logger.puzzlebox_logger(logfile='thinkgear_server')
	log = None
	
	# Collect default settings and command line parameters
	server_interface = SERVER_INTERFACE
	server_port = SERVER_PORT
	
	for each in sys.argv:
		
		if each.startswith("--interface="):
			server_interface = each[ len("--interface="): ]
		if each.startswith("--port="):
			server_port = each[ len("--port="): ]
	
	
	thinkgear_server = puzzlebox_thinkgear_server(log, DEBUG=DEBUG)
	
	if DEBUG:
		print "----> [ThinkGear Emulator] Initializing server on %s:%i" % \
			(server_interface, server_port)
	
	reactor.listenTCP(interface=server_interface, \
			  port=server_port, \
			  factory=thinkgear_server)
	reactor.run()

