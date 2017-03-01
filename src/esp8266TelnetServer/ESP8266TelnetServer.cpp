/*
 * ESP8266TelnetServer.cpp
 *
 *  Created on: 1 mars 2017
 *      Author: Bruno
 */

#include <esp8266TelnetServer/ESP8266TelnetServer.h>

ESP8266TelnetServer::ESP8266TelnetServer(uint16_t port) :
		WiFiServer(port) {
}
void ESP8266TelnetServer::begin() {
	begin();
	setNoDelay(true);
	Serial.println("Please connect Telnet Client.");
}

void ESP8266TelnetServer::handle() {

	// look for Client connect trial
	if (hasClient()) {
		if (!serverClient || !serverClient.connected()) {
			if (serverClient) {
				serverClient.stop();
				Serial.println("Telnet Client Stop");
			}
			serverClient = available();
			Serial.println("New Telnet client");
			serverClient.flush(); // clear input buffer, else you get strange characters
		}
	}

	while (serverClient.available()) { // get data from Client
		Serial.write(serverClient.read());
	}
}
void ESP8266TelnetServer::send(String msg) {
	if (serverClient && serverClient.connected()) {
		serverClient.print(msg);
	}
	delay(10);  // to avoid strange characters left in buffer
}
