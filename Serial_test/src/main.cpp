#include "mbed.h"
#include "MODSERIAL.h"
MODSERIAL pc(USBTX, USBRX); // tx, rx

int main() {
  pc.printf("Hello World!");
  while(1) {
    pc.putc(pc.getc() + 1);
  }
 }
