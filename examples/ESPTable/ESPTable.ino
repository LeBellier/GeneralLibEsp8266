

/*
   Programme pour table à led
   Bruno Bellier
*/

#define FTP_DEBUG 
#define DEBUG
#define DEBUG_INIT
String fileName = "Progam: ESPTable.ino";

#include "FS.h"
#include "PersonnalData.h"
#include <GlobalVariables.h>
#include <MatrixStrip.h>
#include <PagesWeb.h>
#include <WifiManager.h>
#include <Animations.h>
#include <ArtNet.h>
#include <Aspect.h>





void setup(void) {

  pinMode( led, OUTPUT);
  digitalWrite( led, 0);
  strip.rgbBlink();

  hasDebug(initWIFILite);
  hasDebugDelay(15);
  artnet.setArtDmxCallback(onDmxFrame);
  artnet.begin();
  hasDebug(initDNS);
  hasDebugDelay(10);
  hasDebug(initWebServer);
  hasDebugDelay(10);
  hasDebug(initFtpServer);

  digitalWrite( led, 1);
}
void loop() {
  updateServers();
  artnet.read();
  if (animate == true) {
    startShow(showType);
    animate = false;
  }
}

