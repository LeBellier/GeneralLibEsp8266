/*
 * ServerManager.cpp
 *
 *  Created on: 8 mars 2017
 *      Author: Bruno
 */

#include "ServerManager.h"

ServerManager::ServerManager() {

}
ServerManager::~ServerManager() {
}

void ServerManager::setServersON(uint8_t status) {
	serversON = status;

	if (!SPIFFS.begin()) { //init spiffs (spi file system)
		DEBUG_SVR_M("Can't open de File system");
	}

	if (serversON & SERVER_TELNET) { //init Telnet Server
		telnetSvr.reset(new ESP8266TelnetServer());
		telnetSvr->begin();
	}
	if (serversON & SERVER_DNS) { //init DNS
		dnsSvr.reset(new MDNSResponder());
		//init DNS
		if (dnsSvr->begin(dnsName, WiFi.localIP())) {
			DEBUG_SVR_M("MDNS responder started");
			String msg = F("You can use the name: http://{1}.local/");
			msg.replace("{1}", dnsName);
			DEBUG_SVR_M(msg);
		}
	}
	if (serversON & SERVER_HTTP) { //init Http Server
		httpSvr.reset(new ESP8266WebServer());
		// in the setup of the .ino
		//httpSvr->onNotFound(handleRequestFile);
		//serverManager.getHttpServer().on("/pixel", HTTP_GET, pixelRequest); // and all other adresse page you want out of file
		httpSvr->begin();
		DEBUG_SVR_M("HTTP server started");
	}

	if (serversON & SERVER_FTP) { //init Ftp Server
		ftpSvr.reset(new FtpServer());
		ftpSvr->begin(userName, passeword); //username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
		String msg = F("FTP server started; MdP:{1}, User:{2}");
		msg.replace("{2}", userName);
		msg.replace("{1}", passeword);
		DEBUG_SVR_M(msg);
	}

	if (serversON & SERVER_OTA) { //init Ota Server
		//ArduinoOTA.setPort(8266);// Port defaults to 8266
		ArduinoOTA.setHostname(userName);		// Hostname defaults to esp8266-[ChipID]
		ArduinoOTA.setPassword(passeword); // No authentication by default
		ArduinoOTA.begin();
		String msg = F("OTA server started; MdP:{1}, User:{2}");
		msg.replace("{2}", userName);
		msg.replace("{1}", passeword);
		DEBUG_SVR_M(msg);
	}

	if (_debug) {
		delay(20);
	}
}

void ServerManager::update() {
	ArduinoOTA.handle();
	if (serversON & SERVER_TELNET) {
		telnetSvr->handleClient();
	}
	if (serversON & SERVER_DNS) {
		dnsSvr->update();
	}
	if (serversON & SERVER_HTTP) {
		httpSvr->handleClient();
	}
	if (serversON & SERVER_FTP) {
		ftpSvr->handleFTP();
	}
}
String ServerManager::printRequest() {
	String message = "URI: ";
	message += httpSvr->uri();
	message += "\r\n  Method: ";
	message += (httpSvr->method() == HTTP_GET) ? "GET" : "POST";
	message += "\r\n  Arguments: ";
	message += httpSvr->args();
	for (uint8_t i = 0; i < httpSvr->args(); i++) {
		message += "\r\n NAME:" + httpSvr->argName(i) + " VALUE:" + httpSvr->arg(i);
	}
	return message;
}
void ServerManager::handleRequestFile() {
	String uriAsked = httpSvr->uri();
	DEBUG_SVR_M(uriAsked);

	//check the request
	if (!loadFromSpiffs(uriAsked)) { // no file at the uri found
		String message = "File Not Detected  ";
		message += printRequest();
		httpSvr->send(404, "text/plain", message);
		DEBUG_SVR_M(message);
	}
}

bool ServerManager::loadFromSpiffs(String path) {

	String dataType = "text/plain";
	if (path.endsWith("/"))
		path += "index.html";
	if (path.endsWith(".src"))
		path = path.substring(0, path.lastIndexOf("."));
	else if (path.endsWith(".html"))
		dataType = "text/html";
	else if (path.endsWith(".css"))
		dataType = "text/css";
	else if (path.endsWith(".js"))
		dataType = "application/javascript";
	else if (path.endsWith(".png"))
		dataType = "image/png";
	else if (path.endsWith(".gif"))
		dataType = "image/gif";
	else if (path.endsWith(".jpg"))
		dataType = "image/jpeg";
	else if (path.endsWith(".ico"))
		dataType = "image/x-icon";
	else if (path.endsWith(".xml"))
		dataType = "text/xml";
	else if (path.endsWith(".pdf"))
		dataType = "application/pdf";
	else if (path.endsWith(".zip"))
		dataType = "application/zip";

	File dataFile = SPIFFS.open(path.c_str(), "r");
	if (!dataFile) {
		return false;
	}

	if (httpSvr->hasArg("download"))
		dataType = "application/octet-stream";
	if (httpSvr->streamFile(dataFile, dataType) != dataFile.size()) {
		DEBUG_SVR_M("Sent less data than expected!");
		return false;
	}
	dataFile.close();
	return true;
}

uint8_t ServerManager::isServersON() {
	return serversON;
}

void ServerManager::setDnsName(char*param) {
	dnsName = param;
}
void ServerManager::setUserName(char*param) {
	userName = param;
}
void ServerManager::setPasseword(char*param) {
	passeword = param;
}
void ServerManager::setDebug(bool param) {
	_debug = param;
}

void ServerManager::printDebug(String text) {
	DEBUG_SVR_M(text);
}
void ServerManager::setReadTelnetCallback(void (*func)(char*, uint8_t)) {
	telnetSvr->setReadCallback(func);
}

template<typename Generic>
void ServerManager::DEBUG_SVR_M(Generic text) {
	if (_debug) {
		if (telnetSvr->hasConnectedClient()) {
			telnetSvr->print("*SM*: ");
			telnetSvr->println(text);
		} else {
			Serial.print("*SM*: ");
			Serial.println(text);
		}
	}
}

/*Dont work i dont know why
 ESP8266WebServer* ServerManager::getHttpServer() {
 return &httpServer;
 }*/
