
int memIndex = 0;
const int nMem = 90;
int currentMean[4];
int sensorValue[4]; 
int sampleMem[4][nMem];
int sDevMem[4][nMem];

const int freqValWSize = 8;
int freqVal[4];
int fVal3[3];
int fValAvg[4];
int freqValMem[4][freqValWSize];
int freqValWinIndex = 0;
int freqValMapped[3];

const int analogInPin0 = A0; //Low Pass
const int analogInPin1 = A1; //Band Pass Low
const int analogInPin2 = A2; //Band Pass High
const int analogInPin3 = A3; //High Pass

void setup() {
  Serial.begin(9600);
  
}

void loop() {

  for(int i = 0; i < 4; i++){
    switch(i){
      case(0):sensorValue[i] = analogRead(analogInPin0); break;
      case(1):sensorValue[i] = analogRead(analogInPin1); break;
      case(2):sensorValue[i] = analogRead(analogInPin2); break;
      case(3):sensorValue[i] = analogRead(analogInPin3); break;
    }
    unsigned long sMemSum = 0;
    unsigned long sDevSum = 0;
    sampleMem[i][memIndex] = sensorValue[i];
    sDevMem[i][memIndex] = sq(sensorValue[i] - currentMean[i]);
    int standardDeviation = 0;
    for(int n = 0; n < nMem; n++){
      sMemSum += sampleMem[i][n];
      sDevSum += sDevMem[i][n];
    }
    currentMean[i] = (int)(sMemSum/nMem);
    standardDeviation = (int)sqrt(sDevSum/nMem);
    
    if(abs(currentMean[i] - sensorValue[i]) > (standardDeviation/1.5)){
    freqVal[i] = abs((currentMean[i] - sensorValue[i]) - (standardDeviation/1.5));
    }
   
    else{
      freqVal[i] = fValAvg[i];
      
    }
    
  }
 

///////////////V2 CODE////////////////////////////////
  for(int i = 0; i < 4; i++){
    int fAvgDiv = freqValWSize;
    freqValMem[i][freqValWinIndex] = freqVal[i];
    freqValWinIndex++;
    if(freqValWinIndex == freqValWSize)freqValWinIndex = 0;
    int fValSum = 0;
    for(int j = 0; j < freqValWSize; j++){
      if(freqValMem[i][j] == 0)fAvgDiv--;
      fValSum += freqValMem[i][j];
    }
    //if(abs(fValAvg[i] - (int)(fValSum / fAvgDiv)) > )
    fValAvg[i] = (int)(fValSum / fAvgDiv);
   // fValAvg[i] = (fValAvg[i] + (int)(fValSum / fAvgDiv))/2;
  }
////////////////////////////////////////////////////////////////


  fVal3[0] = fValAvg[0];
  if(fValAvg[1] < fValAvg[2] && fValAvg[1]!=0){fVal3[1] = fValAvg[1];}
  else{fVal3[1] = fValAvg[2];}
  fVal3[2] = fValAvg[3];
  
  int val = max(map(fVal3[0], 20, 170, 3, 25),3);
  
    if(freqValMapped[0] > val){
      freqValMapped[0] = freqValMapped[0]-1;
    }
    else if(freqValMapped[0] < val){
      freqValMapped[0] = freqValMapped[0]+1;
    }
    else{
      freqValMapped[0] = val;
    }
  
  
  val = max(map(fVal3[1], 20, 170, 3, 25),3);
 
    if(freqValMapped[1] > val){
      freqValMapped[1] = freqValMapped[1]-1;
    }
    else if(freqValMapped[1] < val){
      freqValMapped[1] = freqValMapped[1]+1;
    }
    else{
      freqValMapped[1] = val;
    }
  

  val = max(map(fVal3[2], 20, 170, 3, 25),3);

    if(freqValMapped[2] > val){
      freqValMapped[2] = freqValMapped[2]-1;
    }
    else if(freqValMapped[2] < val){
      freqValMapped[2] = freqValMapped[2]+1;
    }
    else{
      freqValMapped[2] = val;
    }
  
    Serial.print(char(freqValMapped[0]));
    Serial.print(char(freqValMapped[1]));
    Serial.print(char(freqValMapped[2]));
    Serial.print(char(255));
    Serial.flush();
  memIndex++;
  if(memIndex == nMem) memIndex = 0;
  delay(12);
}
