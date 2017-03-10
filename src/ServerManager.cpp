/*
 * ServerManager.cpp
 *
 *  Created on: 8 mars 2017
 *      Author: Bruno
 */

#include <ServerManager.h>

ServerManager::ServerManager() {
}
ServerManager::~ServerManager() {
}

void ServerManager::initDnsHttpFtpOtaTelnetServers(char* dnsName, char* ftpUser,
		char* ftpPasseWord, char*otaHostName, char*otaPasseWord) {

	//init spiffs (spi file system)
	if (!SPIFFS.begin()) {
		DEBUG_INIT_PRINTLN("Can't open de File system");
	}

	//init DNS
	if (mdns.begin(dnsName, WiFi.localIP())) {
		DEBUG_INIT_PRINTLN("MDNS responder started");
		DEBUG_INIT_PRINT("You can use the name: http://");
		DEBUG_INIT_PRINT(dnsName);
		DEBUG_INIT_PRINTLN(".local/");
	}

	//init Http Server
	// in the setup of the .ino
	//serverManager.getHttpServer().onNotFound(handleRequestOnFile);
	//serverManager.getHttpServer().on("/pixel", HTTP_GET, pixelRequest); // and all other adresse page you want out of file
	httpServer.begin();
	DEBUG_INIT_PRINTLN("HTTP server started");

	//init Ftp Server
	ftpSrv.begin(ftpUser, ftpPasseWord); //username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
	DEBUG_INIT_PRINT("FTP server started; MdP:");
	DEBUG_INIT_PRINT(ftpPasseWord);
	DEBUG_INIT_PRINT(", User:");
	DEBUG_INIT_PRINTLN(ftpUser);

	//init Ota Server
	//ArduinoOTA.setPort(8266);// Port defaults to 8266
	ArduinoOTA.setHostname(otaHostName);	// Hostname defaults to esp8266-[ChipID]
	ArduinoOTA.setPassword(otaPasseWord); // No authentication by default
	ArduinoOTA.begin();
	DEBUG_INIT_PRINTLN("OTA server started");

	//init Telnet Server
	telnetServeur.begin();

}

void ServerManager::updateServers() {
	mdns.update();
	ftpSrv.handleFTP();
	httpServer.handleClient();
	ArduinoOTA.handle();
	telnetServeur.handleClient();
}

void ServerManager::handleRequestFile() {
	String uriAsked = httpServer.uri();
	printlnDebug(uriAsked);

	//check the request
	if (!loadFromSpiffs(uriAsked)) { // no file at the uri found
		String message = "File Not Detected  ";
		message += "URI: ";
		message += uriAsked;
		message += "\n  Method: ";
		message += (httpServer.method() == HTTP_GET) ? "GET" : "POST";
		message += "\n  Arguments: ";
		message += httpServer.args();
		for (uint8_t i = 0; i < httpServer.args(); i++) {
			message += "\n NAME:" + httpServer.argName(i) + " VALUE:"
					+ httpServer.arg(i);
		}
		httpServer.send(404, "text/plain", message);
		printlnDebug(message);
	}
}

bool ServerManager::loadFromSpiffs(String path) {

	String dataType = "text/plain";
	if (path.endsWith("/"))
		path += "index.html";
	if (path.endsWith(".src"))
		path = path.substring(0, path.lastIndexOf("."));

	if (path.endsWith(".html"))
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

	if (httpServer.hasArg("download"))
		dataType = "application/octet-stream";
	if (httpServer.streamFile(dataFile, dataType) != dataFile.size()) {
		printlnDebug("Sent less data than expected!");
		return false;
	}
	dataFile.close();
	return true;
}

void ServerManager::printDebug(String s) {
	if (telnetServeur.hasConnectedClient()) {
		telnetServeur.print(s);
	} else {
		DEBUG_PRINT(s);
	}
}

void ServerManager::printlnDebug(String s) {
	if (telnetServeur.hasConnectedClient()) {
		telnetServeur.println(s);
	} else {
		DEBUG_PRINTLN(s);
	}
}

/*Dont work i dont know why
 ESP8266WebServer* ServerManager::getHttpServer() {
 return &httpServer;
 }*/
