#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Puzzlebox - Client - ThinkGear Connector
#
# Copyright Puzzlebox Productions, LLC (2010)
#
# This code is released under the GNU Pulic License (GPL) version 2
# For more information please refer to http://www.gnu.org/copyleft/gpl.html
#
# Last Update: 2010.06.22
#
#####################################################################

import os, sys
import simplejson as json

from twisted.internet import reactor, protocol, defer
from twisted.protocols import basic

import puzzlebox_thinkgear_emulator_configuration as configuration

#####################################################################
# Globals
#####################################################################

DEBUG = 1

SERVER_HOST = configuration.THINKGEAR_SERVER_HOST
SERVER_PORT = configuration.THINKGEAR_SERVER_PORT

MAX_CONNECTION_ATTEMPTS = configuration.MAX_CONNECTION_ATTEMPTS
NO_REPLY_WAIT = configuration.NO_REPLY_WAIT

THINKGEAR_DELIMITER = '\r'

ENABLE_THINKGEAR_AUTHORIZATION = configuration.ENABLE_THINKGEAR_AUTHORIZATION
AUTHORIZATION_REQUEST = configuration.THINKGEAR_AUTHORIZATION_REQUEST

THINKGEAR_PARAMETERS = {"enableRawOutput": False, "format": "Json"}
#THINKGEAR_PARAMETERS = {"enableRawOutput": True, "format": "Json"}
#THINKGEAR_PARAMETERS = configuration.FLASH_POLICY_FILE_REQUEST

#####################################################################
# Classes
#####################################################################

class puzzlebox_thinkgear_client:
	
	def __init__(self, log, \
		          server_host=SERVER_HOST, \
		          server_port=SERVER_PORT, \
		          DEBUG=DEBUG):
		
		self.log = log
		self.DEBUG=DEBUG
		
		self.server_host = server_host
		self.server_port = server_port
		self.max_connection_attempts = MAX_CONNECTION_ATTEMPTS
		
		self.is_authorized = True
	
	
	##################################################################
	
	def authorize_and_send_parameters(self,\
					  thinkgear_parameters, \
					  authorization_request):
		
		if self.DEBUG:
			print "--> [ThinkGear Client] Sending Authorization Request:",
			print authorization_request
		
		
		d = self.send_parameters(thinkgear_parameters, authorization_request)
		d.addCallback(self.update_authorization)
	
	
	##################################################################
	
	def update_authorization(self, authorization):
		
		if self.DEBUG:
			print "--> [ThinkGear Client] Authorization:",
			print authorization
		
		self.is_authorized = authorization
	
	
	##################################################################
	
	def send_parameters(self, \
		                 parameters, \
		                 authentication=None, \
		                 max_connection_attempts=MAX_CONNECTION_ATTEMPTS):
		
		self.factory = puzzlebox_thinkgear_client_factory( \
		                  self.log, \
		                  parameters, \
		                  authentication, \
		                  self.server_host, \
		                  self.server_port, \
		                  max_connection_attempts, \
		                  self.DEBUG)
		
		reactor.connectTCP(self.server_host, self.server_port, self.factory)
		
		return self.factory.replyDefer
	
	
	##################################################################
	
	def send_parameters_and_print_response(self, parameters):
		
		if self.DEBUG:
			print "----> [ThinkGear Client] Sending:",
			print parameters
		
		
		d = self.send_parameters(parameters)
		d.addCallback(self.print_response)
	
	
	##################################################################
	
	def print_response(self, response):
		
		if self.DEBUG:
			print "--> [ThinkGear Client] Server Response:",
			print response


#####################################################################
# ThinkGear Client Protocol class
#####################################################################

class puzzlebox_thinkgear_client_protocol(basic.LineReceiver):
	
	delimiter = THINKGEAR_DELIMITER
	
	def __init__(self):
		
		self.DEBUG = DEBUG
	
	
	##################################################################
	
	def connectionMade(self):
		
		if self.DEBUG:
					print "----> [ThinkGear Client] Connected to %s:%i" % \
							(self.factory.server_host, \
							 self.factory.server_port)
		
		
		if (self.factory.authentication != None):
			self.send_data(self.factory.authentication)
		
		else:
			self.send_data(self.factory.parameters)
	
	
	##################################################################
	
	def send_data(self, data):
		
		packet = json.dumps(data)
		
		if self.DEBUG:
			print "----> [ThinkGear Client] Sending:",
			print packet
		
		
		self.sendLine(packet)
		
		#self.factory.noReply = reactor.callLater(NO_REPLY_WAIT, self.noReply)
	
	
	##################################################################
	
	def noReply(self):
		
		try:
			self.factory.replyDefer.callback('NO_REPLY')
		except:
			if self.DEBUG:
				print "noReply failed to call callback"
	
	
	##################################################################
	
	def lineReceived(self, line):
	
		# Ignore blank lines
		if not line:
			return
		
		try:
			data = json.loads(line)
		except Exception, e:
			if self.DEBUG:
				print "Partial data received (or error:",
				print e
				print ")."
				
				print "line:",
				print line
				
				# We've received back an unrecognized response
				# so we'll attempt to reset the server's
				# parameters for JSON communication
				self.send_data(self.factory.parameters)
		
		else:
			
			if (type(data) == type({})):
			
				self.factory.process_response(data)
				#self.factory.replyDefer.callback(data)
				
			else:
				if self.DEBUG:
					print "data:",
					print data
	
	
	##################################################################
	
	def connectionLost(self, reason):
		
		if self.DEBUG > 1:
			print "Connection lost:",
			print reason
	
	
	##################################################################
	
	def connectionDone(self, reason):
		
		if self.DEBUG > 1:
			print "Connection done:",
			print reason


#####################################################################
# ThinkGear Client Factory class
#####################################################################

class puzzlebox_thinkgear_client_factory(protocol.ClientFactory):
	
	def __init__(self, log, \
		     parameters, \
		     authentication, \
		     server_host=SERVER_HOST, \
		     server_port=SERVER_PORT, \
		     max_connection_attempts=MAX_CONNECTION_ATTEMPTS, \
		     DEBUG=DEBUG):
		
		self.log = log
		self.DEBUG = DEBUG
		self.server_host = server_host
		self.server_port = server_port
		self.parameters = parameters
		self.authentication = authentication
		
		self.max_connection_attempts = max_connection_attempts
		self.connection_attempt = 1
		
		self.protocol = \
		   puzzlebox_thinkgear_client_protocol
		
		self.replyDefer = defer.Deferred()
	
	
	##################################################################
	
	def process_response(self, response):
		
		if self.DEBUG:
			if (('rawEeg' not in response.keys()) or \
			    (self.DEBUG > 1)):
				print "<-- [ThinkGear Client] Received:",
				print response
		
		
		if response.has_key('isAuthorized'):
			
			if (response['isAuthorized'] == True):
				self.replyDefer.callback(True)
			else:
				self.replyDefer.callback(False)
	
	
	##################################################################
	
	def clientConnectionFailed(self, connector, reason):
		
		if self.DEBUG:
			print "Client failed to connect to remote component at %s:%i" % \
			   (self.server_host, self.server_port)
		
		reply = 'FAILED_TO_CONNECT'
		
		self.connection_attempt = self.connection_attempt + 1
		
		if ((self.max_connection_attempts == None) or \
			 (self.connection_attempt <= self.max_connection_attempts)):
			
			# If connection failed retry after one second
			reactor.callLater(1, connector.connect)
		
		else:
			if self.DEBUG:
				print "Maximum connection retries reached, aborting"
			
			
			self.replyDefer.callback(reply)
	
	
	##################################################################
	
	def clientConnectionLost(self, connector, reason):
		
		# Losing Connection is expected after data exchange is complete
		try:
			self.replyDefer.callback(reason)
		except:
			pass


#####################################################################
# Main
#####################################################################

if __name__ == '__main__':
	
	#log = puzzlebox_logger.puzzlebox_logger(logfile='thinkgear_client')
	log = None
	
	authorization_request = AUTHORIZATION_REQUEST
	thinkgear_parameters = THINKGEAR_PARAMETERS
	
	thinkgear_client = puzzlebox_thinkgear_client( \
	            log, \
	            server_host=SERVER_HOST, \
	            server_port=SERVER_PORT, \
	            DEBUG=DEBUG)
	
	
	if ENABLE_THINKGEAR_AUTHORIZATION:
		# Use ThinkGear authentication
		reactor.callWhenRunning( \
			thinkgear_client.authorize_and_send_parameters, \
			thinkgear_parameters, \
			authorization_request)
	
	else:	
		# Do not use ThinkGear authentication
		authorization_request = None
		reactor.callWhenRunning( \
			thinkgear_client.send_parameters, \
			thinkgear_parameters, \
			authorization_request)
	
	
	reactor.run()

