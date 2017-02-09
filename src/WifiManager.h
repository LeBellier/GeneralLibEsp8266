#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H
// Prototypage

#include <ESP8266WiFi.h>
#include <ESP8266WiFiScan.h>
#include <Aspect.h>
//#include "PersonnalData.h" //have to define nbSSID+SSIDs+passewords
void initWIFI(); //Connection if it's possible
//ESP8266WiFiClass WiFi;// comment this if you compile it on arduino IDE

// Déclaration
void initWIFI() {
	char *ssid;
	char *password;

	boolean wifiFounded = false;
	// Set WiFi to station mode and disconnect from an AP if it was previously connected
	WiFi.enableSTA(true);
	WiFi.disconnect();
	delay(100);
	// WiFi.scanNetworks will return the number of networks found
	int n = WiFi.scanNetworks();
	if (n != 0) {
		int i = 0, j = 0;
		while (i < n && !wifiFounded) {
			while (j < nbSSID && !wifiFounded) {
				if (((ESP8266WiFiScanClass) WiFi).SSID(i) == SSIDs[j]) {
					ssid = SSIDs[j];
					password = passewords[j];
					wifiFounded = true;
				}
				j++;
			}
			i++;
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
		} else {
			const char *ssid = "ESP-MPU";
			//const char *password = "thereisnospoon";

			/* You can remove/add the password parameter if you want the AP to be open/secure. */
			//	WiFi.softAP(ssid, password);
			WiFi.enableAP(true);
			WiFi.softAP(ssid);
			IPAddress ip(192, 168, 4, 1); // this 3 lines for a fix IP-address
			IPAddress gateway(192, 168, 4, 1);
			IPAddress subnet(255, 255, 255, 0);

			WiFi.softAPConfig(ip, gateway, subnet);

		}
	}
}
#endif
