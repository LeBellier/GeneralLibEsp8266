/*
   MatrixStrip.h

    Created on: 21 nov. 2016
        Author: bruno
*/

#ifndef MATRIXSTRIP_H
#define MATRIXSTRIP_H

#include <Adafruit_NeoPixel.h>
#include "ConstCharColor.h"

class MatrixStrip: public Adafruit_NeoPixel {
  public:
    MatrixStrip(uint8_t pinStrip, uint8_t nbRows, uint8_t nbColumns);
    virtual ~MatrixStrip() {};
    void setMatrixPixelColor(uint16_t n, uint32_t color);
    void setMatrixPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
    void setColor(uint32_t color);
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void rgbBlink();
    void colorBlink(uint32_t color);
    void fillChar(char letter, uint32_t color);
    void fillNumber(byte positionScreen, char letter, uint32_t color);
    void printNumber(int i, String number);
    // numpixels(); pour get le nombre de pixels
  private:
    uint8_t nbRows, nbColumns;
};

#endif /* MATRIXSTRIP_H */
