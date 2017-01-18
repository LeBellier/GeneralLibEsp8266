#ifndef ARTNET_H
#define ARTNET_H

#include <ArtnetWifi.h>
extern ArtnetWifi artnet;

void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data) {

#ifdef DEBUG
  Serial.print("DMX: Univ: ");
  Serial.print(universe, DEC);
  Serial.print(", Seq: ");
  Serial.print(sequence, DEC);
  Serial.print(", Data (");
  Serial.print(length, DEC);
  Serial.print("): ");

  // send out the buffer
  for (int i = 0; i < length; i++) {
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
#endif

  // read universe and put into the right part of the display buffer
  for (int i = 0; i < length / 3; i++) {
    int led = i;
    if (led < strip.numPixels()) {
      strip.setMatrixPixelColor(led, data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
    }
  }
  strip.show();
}

#endif
