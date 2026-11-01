#include <SPI.h>

const byte numTiles = 2;
bool framebuf[8][numTiles*8];
bool nextbuf[8][numTiles*8];
unsigned long last = 0;
int genNo = 0;
#define MOSI_PIN 19
#define SCK_PIN 18
#define LATCH_PIN 17
#define OE_PIN 20

void setLed(int row, int col, bool on){
  if (col >= (numTiles*8) || row >= 8 || row < 0 || col < 0){
    return;
  }
  framebuf[row][col] = on;
}

void randStart(){
  for(byte i = 0; i<8; i++){
    for (byte j = 0; j < (8*numTiles); j++){
      framebuf[i][j] = random(2);
    }
  }
  genNo = 0;
}

int countAdj(int row, int col){
  byte counter = 0;
  for (int offsetr = -1; offsetr <= 1; offsetr++){
    for (int offsetc = -1; offsetc <= 1; offsetc++){
      int nrow = (row + offsetr + 8) % 8 ;
      int ncol = (col + offsetc + numTiles*8) % (numTiles*8);
      if (offsetr == 0 && offsetc ==0){
        continue;}
      if (framebuf[nrow][ncol]){
        counter++;
      }
    }
  }
  return counter;
}

void nextStep(){
  for (byte r = 0; r < 8; r++){
    for (byte c = 0; c < 8*numTiles; c++){
      int n = countAdj(r, c);
      if (framebuf[r][c] && (n == 2 || n == 3)){
        nextbuf[r][c] = true;
      } else if (!framebuf[r][c] && n == 3){
        nextbuf[r][c] = true;
      } else{
        nextbuf[r][c] = false;
      }
    }
  }
  for (byte i = 0; i < 8; i++){
    for (byte j = 0; j < 8*numTiles; j++){
      framebuf[i][j] = nextbuf[i][j];
    }
  }
}

void clearMatrix(){
  for (byte i = 0; i < 8; i++){
    for (byte j = 0; j < (8*numTiles); j++){
      framebuf[i][j] = false;
    }
  }
}

void dispR(int row){
  digitalWrite(OE_PIN, HIGH);
  for (int tile = numTiles -1; tile >= 0; tile--){
    byte colbyte = 0;
    for (int c = 0; c < 8; c++){
      if (framebuf[row][c + (tile*8)]){
        colbyte |= (1 << c);
      }
    }
    SPI.transfer(1 << row);
    SPI.transfer(colbyte);
  }
  digitalWrite(LATCH_PIN, HIGH);
  digitalWrite(LATCH_PIN, LOW);
  digitalWrite(OE_PIN, LOW);
}

void setup() {
  SPI.setTX(MOSI_PIN);
  SPI.setSCK(SCK_PIN);
  SPI.begin();
  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(OE_PIN, OUTPUT);
  digitalWrite(OE_PIN, HIGH);
  digitalWrite(LATCH_PIN, LOW);
  clearMatrix();
  randomSeed(analogRead(A0));
  randStart();
}

void loop() {
  for (int r = 0; r < 8; r++){
    dispR(r);
    delayMicroseconds(1000);
  }
  if (millis() - last > 150){
    last = millis();
    nextStep();
    genNo++;
    if (genNo > 300){
      randStart();
    }
  }

}
