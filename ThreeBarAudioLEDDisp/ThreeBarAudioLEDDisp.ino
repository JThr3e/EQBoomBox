#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

#define CLK 8 
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2

RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

int index = 0;
int lVal[3][3] = {{5, 0, 0},{0, 0, 5},{0,5 , 0}}; //array holding RGB light values
int n = 5;

int l = 1; //low band counter
int m = 3; //medium band counter
int h = 5; //high band counter

int tick = 0;
int fVal[3]; //holds incoming values from the arduino nano
int nVal[3] = {1,3,5};
void setup() {
        Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
        matrix.begin();
        matrix.fillScreen(0);
        fVal[0] = 5;
        fVal[1] = 5;
        fVal[2] = 5;
   

}

void loop() {
        
        if (Serial.available() > 0) {
                // read the incoming byte:
                int incomingByte = int(Serial.read());
                if(incomingByte != 255){
                  fVal[index] = incomingByte;
                  index++;
                }
                else{
                  index = 0;

                  //Weird workaround thing (not enough RAM? when all three set at once)
                  int r = random(0,3);
                  if(fVal[0] != fVal[r]){
                    matrix.fillRect(fVal[0], 0, 32-fVal[0], 4,matrix.Color333(0, 0, 0));
                  }
                  if(fVal[1] != fVal[r]){
                    matrix.fillRect(fVal[1], 6, 32-fVal[1], 4,matrix.Color333(0, 0, 0));
                  }
                  if(fVal[2] != fVal[r]){
                    matrix.fillRect(fVal[2], 12, 32-fVal[2], 4,matrix.Color333(0, 0, 0));
                  }
                  
                  ////RAINBOW///////////////
                  //Algorithm to scroll through rainbow colors
                  if(tick == 0){
                    for(int i = 0; i < 3; i++){
                    if(nVal[i] < 0) nVal[i] = 5;
                    if(nVal[i]%2 == 1){
                      if(lVal[i][nVal[i]%3] < 5){
                        lVal[i][nVal[i]%3] = lVal[i][nVal[i]%3]+1;
                      }
                      else{
                        lVal[i][nVal[i]%3] = 5;
                        nVal[i]--;
                      }
                    }
                    else if(nVal[i]%2 == 0){
                      if(lVal[i][nVal[i]%3] > 0){
                        lVal[i][nVal[i]%3] = lVal[i][nVal[i]%3]-1;
                      }
                      else{
                        lVal[i][nVal[i]%3] = 0;
                        nVal[i]--;
                      }
                    }
                  }
                  }
                  tick++;
                  if(tick > 2) tick = 0;
                  ///////////////////
                  
                  matrix.fillRect(0, 0, fVal[0], 4,matrix.Color333(lVal[0][0], lVal[0][1], lVal[0][2]));
                  matrix.fillRect(0, 6, fVal[1], 4,matrix.Color333(lVal[1][0], lVal[1][1], lVal[1][2]));
                  matrix.fillRect(0, 12,fVal[2],4,matrix.Color333(lVal[2][0], lVal[2][1], lVal[2][2]));
                  matrix.swapBuffers(false);
                }

        }

}

