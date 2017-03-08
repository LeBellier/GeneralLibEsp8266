/*
 * ServerManager.cpp
 *
 *  Created on: 8 mars 2017
 *      Author: Bruno
 */

#include <ServerManager.h>

void handleRequestOnFile() {

}

ServerManager::ServerManager(Logger* logger) :
		logger(logger) {
	// TODO Auto-generated constructor stub

}

ServerManager::~ServerManager() {
	// TODO Auto-generated destructor stub
}

void ServerManager::initDnsHttpFtpOtaTelnetServers(char* dnsName, char* ftpUser,
		char* ftpPasseWord, char*otaHostName, char*otaPasseWord) {
	//init spiffs (spi file system)
	if (!SPIFFS.begin()) {
		//logger.println("Can't open de File system");
	}
	//init DNS
	if (mdns.begin(dnsName, WiFi.localIP())) {
		//logger.println("MDNS responder started");
		//logger.print("You can use the name: http://");
		//logger.print(dnsName);
		//logger.println(".local/");
	}
	//init Http Server
	httpServer.onNotFound(handleRequestOnFile);
	httpServer.begin();
	//logger.println("HTTP server started");

	//init Ftp Server
	ftpSrv.begin(ftpUser, ftpPasseWord); //username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)
	//logger.print("FTP server started; MdP:");
	//logger.print(ftpUser);
	//logger.print(", User:");
	//logger.println(ftpPasseWord);

	//init Ota Server
	//ArduinoOTA.setPort(8266);// Port defaults to 8266
	ArduinoOTA.setHostname(otaHostName);	// Hostname defaults to esp8266-[ChipID]
	ArduinoOTA.setPassword(otaPasseWord); // No authentication by default
	ArduinoOTA.begin();
	//logger.print("OTA server started");

	//init Telnet Server
	telnetServeur.began(logger);

}
void ServerManager::updateServers() {
	mdns.update();
	ftpSrv.handleFTP();
	httpServer.handleClient();
	ArduinoOTA.handle();
	telnetServeur.handle(logger);
}

void ServerManager::handleRequestFile() {
	String uriAsked = httpServer.uri();
	//logger.println(uriAsked);

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
		//logger.println(message);
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
		logger->sendln("Sent less data than expected!");
		return false;
	}
	dataFile.close();
	return true;
}

