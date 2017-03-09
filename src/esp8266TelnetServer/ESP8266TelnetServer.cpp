/*
 * ESP8266TelnetServer.cpp
 *
 *  Created on: 1 mars 2017
 *      Author: Bruno
 */

#include "ESP8266TelnetServer.h"

WiFiServer telnet_srv(23);

ESP8266TelnetServer::ESP8266TelnetServer() {
}
ESP8266TelnetServer::~ESP8266TelnetServer() {
}

void ESP8266TelnetServer::begin() {
	telnet_srv.begin();
	telnet_srv.setNoDelay(true);
	DEBUG_INIT_PRINTLN("Please connect Telnet Client.");
}
bool ESP8266TelnetServer::hasConnectedClient() {
	return (serverClient && serverClient.connected());
}

void ESP8266TelnetServer::handleClient() {

	// look for Client connect trial
	if (telnet_srv.hasClient()) {
		if (hasConnectedClient()) {
			disconnect();
		}
		serverClient = telnet_srv.available();
		DEBUG_PRINTLN("New Telnet client");
		serverClient.flush(); // clear input buffer, else you get strange characters
	}

	while (serverClient.available()) { // get data from Client
		char charRX = serverClient.read();
		if (charRX == 'q') {
			disconnect();
		}
		Serial.print((String) charRX);
	}
}
size_t ESP8266TelnetServer::write(uint8_t c) {
	if (hasConnectedClient()) {
		serverClient.write(c);
	}
	delay(10);  // to avoid strange characters left in buffer
}

size_t ESP8266TelnetServer::write(const uint8_t *buffer, size_t size) {
	if (hasConnectedClient()) {
		serverClient.write(buffer, size);
	}
	delay(10);  // to avoid strange characters left in buffer

}
void ESP8266TelnetServer::disconnect() {
	if (hasConnectedClient()) {
		serverClient.stop();
		DEBUG_PRINTLN("Telnet Client Stop");
	}
}
