#include <OneWire.h>
#include <DallasTemperature.h>
#include <Servo.h> 

 
// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2
#define MAX_RECV_LINE_LENGTH 200
#define MAX_COMMANDS 10
 
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

String strIncompleteLine = ""; // a string to hold incoming data
String strInputLine = ""; // a string to the recieved line
boolean bStrComplete = false; // whether the string is complete

String strCommands[MAX_COMMANDS];
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
Servo myservoV;
Servo myservoH;
 
int pos = 0;
int val;
int camPosH = 3;
int camPosV = 3;
int camPosHOld = 3;
int camPosVOld = 3;

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  strIncompleteLine.reserve(MAX_RECV_LINE_LENGTH);
  strInputLine.reserve(MAX_RECV_LINE_LENGTH);
  myservoV.attach(9);
  myservoH.attach(10);
  // Start up the library
  sensors.begin();
}
 
int SplitCommands(String strCommands, String strTab[],int maxItems)
{
  //Start position looking for ','
  int fisrtCommaPosition=0;
  int lastCommaPosition=0;
  int iItems=0;
  do
    { //Get the position of the next item
        lastCommaPosition = strCommands.indexOf(',',fisrtCommaPosition);
        if(lastCommaPosition != -1)
        { strTab[iItems]= strCommands.substring(fisrtCommaPosition,lastCommaPosition);
            fisrtCommaPosition=lastCommaPosition+1;
        }
        else
            //Prints the last element
            strTab[iItems]= strCommands.substring(fisrtCommaPosition,strInputLine.length()-1);
      
       iItems++;

     } while((lastCommaPosition >=0) & (iItems<maxItems));

  return iItems;
}

void loop(void)
{
  int i;
  sensors.requestTemperatures(); // Send the command to get temperatures
   Serial.print(sensors.getTempCByIndex(0)); // Why "byIndex"? 
    if (bStrComplete)
  { 
    bStrComplete=false;
    // Splits the incoming string into strCommands[0] .. [MAX_COMMANDS]
    int nCommands=SplitCommands(strInputLine,strCommands,MAX_COMMANDS); // Parses the CVS string
    if(strCommands[0]=="$CAM")
    {
      camPosH += strCommands[1].toInt();
      camPosV += strCommands[2].toInt();
      if (camPosH < 3) camPosH = 3;
      if (camPosV < 3) camPosV = 3;
      if (camPosH > 177) camPosH = 177;
      if (camPosV > 177) camPosV = 177;

      // SMOOTH VERTICAL MOVEMENT
      if (camPosVOld < camPosV)
      for (i=camPosVOld; i<= camPosV; i++)
      {
        myservoV.write(i);
        delay(10);
      }
      if (camPosVOld > camPosV)
      for (i=camPosVOld; i>= camPosV; i--)
      {
        myservoV.write(i);
        delay(10);
      }// SMOOTH VERTICAL MOVEMENT END
      
      // SMOOTH HORIZONTAL MOVEMENT
      if (camPosHOld < camPosH)
      for (i=camPosHOld; i<= camPosH; i++)
      {
        myservoH.write(i);
        delay(10);
      }
      if (camPosHOld > camPosH)
      for (i=camPosHOld; i>= camPosH; i--)
      {
        myservoH.write(i);
        delay(10);
      }// SMOOTH HORIZONTAL MOVEMENT END
      
      camPosHOld = camPosH;
      camPosVOld = camPosV; 
    }
  }
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    strIncompleteLine += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      strInputLine=strIncompleteLine;
      bStrComplete = true;
      strIncompleteLine="";
    }
  }
}
