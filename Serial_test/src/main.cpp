#include "mbed.h"

Serial pc(USBTX, USBRX); // tx, rx

int main() {
  pc.printf("Hello World!");
  while(1) {
    if(pc.readable()){
      pc.putc(pc.getc() );
    }
  }
 }
