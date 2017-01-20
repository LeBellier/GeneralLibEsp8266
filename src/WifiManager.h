#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H
// Prototypage

#include <ESP8266WiFi.h>
#include <ESP8266WiFiScan.h>
#include <Aspect.h>
//#include "PersonnalData.h" //have to define nbSSID+SSIDs+passewords
void initWIFI(); //Connection if it's possible
ESP8266WiFiClass WiFi;

// Déclaration
void initWIFI() {
	char *ssid;
	char *password;

	boolean wifiFounded = false;
	// Set WiFi to station mode and disconnect from an AP if it was previously connected
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	delay(100);
	// WiFi.scanNetworks will return the number of networks found
	int n = WiFi.scanNetworks();
	if (n != 0) {
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < nbSSID; ++j) {
				if (((ESP8266WiFiScanClass) WiFi).SSID(i) == SSIDs[j]) {
					ssid = SSIDs[j];
					password = passewords[j];
					wifiFounded = true;
					break;
				}
			}
			if (wifiFounded) {
				WiFi.begin(ssid, password);
				DEBUG_INIT_PRINTLN("");
				// Wait for connection
				while (WiFi.status() != WL_CONNECTED) {
					delay(500);
					DEBUG_INIT_PRINT(".");
				}
				String sIP = WiFi.localIP().toString();
				DEBUG_INIT_PRINTLN("");
				DEBUG_INIT_PRINT("Connected to ");
				DEBUG_INIT_PRINTLN(ssid);
				DEBUG_INIT_PRINT("IP address: ");
				DEBUG_INIT_PRINTLN(sIP);
				break;
			}
		}
	}
}
#endif
