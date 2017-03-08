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

void ESP8266TelnetServer::began(Logger* logger) {
	telnet_srv.begin();
	telnet_srv.setNoDelay(true);
	logger->sendln("Please connect Telnet Client.");
}

void ESP8266TelnetServer::handle(Logger* logger) {

	// look for Client connect trial
	if (telnet_srv.hasClient()) {
		if (!serverClient || !serverClient.connected()) {
			disconnect(logger);
			serverClient = telnet_srv.available();
			logger->send("New Telnet client\n");
			serverClient.flush(); // clear input buffer, else you get strange characters
		}
	}

	while (serverClient.available()) { // get data from Client
		char charRX = serverClient.read();
		if (charRX == 'q') {
			disconnect(logger);
		}
		logger->send((String) charRX);
	}
}
void ESP8266TelnetServer::print(String msg) {
	if (serverClient && serverClient.connected()) {
		serverClient.print(msg);
	}
	delay(10);  // to avoid strange characters left in buffer
}
void ESP8266TelnetServer::println(String msg) {
	if (serverClient && serverClient.connected()) {
		serverClient.print(msg);
		serverClient.print("\n");
	}
	delay(10);  // to avoid strange characters left in buffer
}
size_t ESP8266TelnetServer::write(uint8_t c) {
	serverClient.write(c);
}
void ESP8266TelnetServer::disconnect(Logger* logger) {
	if (serverClient) {
		serverClient.stop();
		logger->send("Telnet Client Stop\n");
	}
}
