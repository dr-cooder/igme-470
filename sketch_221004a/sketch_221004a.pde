/* WM Harris 470.8-1  2.28.22
 Demo serial communication with arduino
 and  vizualize xyz accelerometer & gyro data
 Arduino Nano code:
 https://create.arduino.cc/editor/wmharris/e6c72fda-e64b-4d47-82fb-f931ce4a8536
*/
// adapted from Serial Event Example by Tom Igoe 

import processing.serial.*; 

/* Global Variables */
Serial myPort;    // The serial port
PFont myFont;     // The display font
String latestData ="";  // Input string from serial port
int lf = 10;      // ASCII linefeed 

//wmh additions
int canvasX = 0; //current graph X

void setup() { 
  size(300, 500); 
  noStroke();
  background(255, 250, 200);
  // List all the available serial ports: 
  printArray(Serial.list()); 

  // Open whatever port is the one you're using. 
  myPort = new Serial(this, Serial.list()[2], 9600); 
  myPort.bufferUntil(lf);
} 

void draw() { 
  fill(255); //white rect
  rect(0, 0, width, 20); //bg for serial text
  fill(0); //black serial text
  text(latestData, 10, 15);

  //paired with data from Nano
  /* Arduino serial code:
      Serial.print(1); //for accel, 0 for gyro
      Serial.print("\t");
      Serial.print(x);
      Serial.print("\t");
      Serial.print(y);
      Serial.print("\t");
      Serial.print(z);
      Serial.print("\t");
      Serial.println(maxA);
   */

  //very rough viz of x,y,z as 3 graphs
  String[] arrofVals = latestData.split("\t");
  //x : 30 - 170, orange
  //y : 180 - 320, purple
  //z : 330 - 470, teal
  if (arrofVals.length > 4) {
    int accORgyro = int(arrofVals[0]);
    float x = float(arrofVals[1]);
    float y = float(arrofVals[2]);
    float z = float(arrofVals[3]);
    float maxVal = float(arrofVals[4]);
    
    x = map(x, -maxVal, maxVal, 0, 140);
    y = map(y, -maxVal, maxVal, 0, 140);
    z = map(z, -maxVal, maxVal, 0, 140);
    //draw them
    if (accORgyro == 1) { //bg color
      fill(200); //light accel mode
    } else {
      fill(100); //dark gyro mode
    }
    rect(canvasX, 20, 5, height); //bg line
    fill(255, 100, 0); //orange x
    rect(canvasX, 100, 4, x - 70);
    fill(200, 0, 200); //purple y
    rect(canvasX, 250, 4, y - 70);
    fill(0, 200, 200); //teal z
    rect(canvasX, 400, 4, z - 70);

    //move to next line, with wrapping
    canvasX = (canvasX + 1) % (width - 4);
  }
} 

void serialEvent(Serial p) { 
  latestData = p.readString();
  trim(latestData);
}
