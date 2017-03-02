/*
 * ESP8266TelnetServer.cpp
 *
 *  Created on: 1 mars 2017
 *      Author: Bruno
 */

#include "ESP8266TelnetServer.h"
#include "Aspect.h"

WiFiServer telnet_srv(23);

ESP8266TelnetServer::ESP8266TelnetServer() {
}
void ESP8266TelnetServer::began() {
	telnet_srv.begin();
	telnet_srv.setNoDelay(true);
	DEBUG_INIT_PRINTLN("Please connect Telnet Client.");
}

void ESP8266TelnetServer::handle() {

	// look for Client connect trial
	if (telnet_srv.hasClient()) {
		if (!serverClient || !serverClient.connected()) {
			disconnect();
			serverClient = telnet_srv.available();
			DEBUG_PRINT("New Telnet client\n");
			serverClient.flush(); // clear input buffer, else you get strange characters
		}
	}

	while (serverClient.available()) { // get data from Client
		char charRX = serverClient.read();
		if (charRX == 'q') {
			disconnect();
		}
		DEBUG_PRINT(charRX);
	}
}
void ESP8266TelnetServer::send(String msg) {
	if (serverClient && serverClient.connected()) {
		serverClient.print(msg);
	}
	delay(10);  // to avoid strange characters left in buffer
}
void ESP8266TelnetServer::disconnect() {
	if (serverClient) {
		serverClient.stop();
		DEBUG_PRINT("Telnet Client Stop\n");
	}
}
