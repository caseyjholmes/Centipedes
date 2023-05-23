//CASEY HOLMES - HOLMES MODULAR - "CENTIPEDES" REV3 - 5/21/23

//constants
const int buttonPin = 2;                       //button pin
const int output = 3;                          //output transistor on pin 3
const int outputLed = 6;                       //red output LED
const int clockLed =  5;                       //green clock led
const int clockPin = 4;                        //clock input transistor pin
long debounce = 20;                            //debounce time no flicker

// Variables
int buttonVal = 0;                             //variable to store current state of the button, usable in interrupt 
int buttonLast = 0;                            //variable to store last state of button
int clockState = 0;                            //variable to store current clock status
int buttonState = 0;                           //varable to store current state of button
bool isOn = true;                              //variable to store state of master duty clock cycle
long btnDnTime;                                //time button was pressed down
long btnUpTime;                                //time button was released
boolean ignoreUp = false;                      //weather ignore button release because press + hold was triggered
unsigned long loopspot = 0;                    //loops pot read variable
unsigned long speedpot =0;                     //speed pot read variable

void setup() {
pinMode(buttonPin, INPUT_PULLUP);              //initialize the pushbutton pin as an input with a internal pullup resistor
pinMode(output, OUTPUT);                       //initialize output transistor
pinMode(outputLed, OUTPUT);                    //initialize output LED as output
digitalWrite(outputLed, 0);                    //initialize output LED as off
pinMode(clockLed, OUTPUT);                     //initialize the LED pin as an output:
digitalWrite(clockLed, 0);                     //initialize cock LED as off
pinMode(clockPin, INPUT);                      //initialize clock pin as input
}

//function for button pressed (triggering a single centipede)
void shortpress() {                            //button was momentarily pressed
digitalWrite(outputLed, 1);                    //turn on LED green indicating button press.
digitalWrite(clockLed, 1);                     //turn on the other color LED
loopspot = map(analogRead(A1), 0, 1023, 100, 1);//read and map pot on A1 into loopspot variable
for (int b = 0; b < loopspot; b++) {           //run for loop for loopspot amount of times
    speedpot = map(analogRead(A0), 0, 1023, 5, 1000);//read and scale Analog 0 into speedpot
    digitalWrite(outputLed, 0);                //turn on output led
    digitalWrite(output, 1);                   //turn on output
    delay(speedpot);                           //wait here for length of speed pot variable
    digitalWrite(outputLed, 1);                //turn off the output LED
    digitalWrite(output, 0);                   //turn off the output
    delay(speedpot);                           //wait here for length of speed pot variable
    }
digitalWrite(outputLed, 0);                    //keep red output led off
digitalWrite(output, 0);                       //turn off the output
digitalWrite(clockLed, 0);                     //keep the red clock output led off
}

void loop() {
clockState = digitalRead(clockPin);                        //read the clock input pin    
while(clockState){                                         //while the clock input that is held low is low..  
  buttonVal = digitalRead(buttonPin);                      //read the pushbutton input pin:
  clockState = digitalRead(clockPin);                      //read the clock input pin
  
//button handler-----------------------------------------------------------------------------------------------------------------------------------------------------------------
if (buttonVal == LOW && buttonLast == HIGH && (millis() - btnUpTime) > long(debounce)) { //test for button press and store down time
   btnDnTime = millis();
   shortpress();         //this was enabled. it triggers cetipede when button is pressed 
}  
if (buttonVal == HIGH && buttonLast == LOW && (millis() - btnDnTime) > long(debounce)) {  //test for button release and store up time
   if (ignoreUp == false) {
      //shortpress();     //this was disabled. it triggers centipede when button is released
      } else {
      ignoreUp = false;
      btnDnTime = millis();
      }
}  
//if (buttonVal == LOW && (millis() - btnDnTime) > long(holdTime)) {//test for button held longer than hold time                           ....disabled for now (easter egg removed)
  // buttonState = !buttonState;                                    //invert state of button variable, trigger master clock routine
   //ignoreUp = true;
   //btnDnTime = millis();
//}  
buttonLast = buttonVal;                                    //update last button state memory for button handler routine

//clock input handler routine----------------------------------------------------------------------------------------------------------------------------------------------------
if (!clockState) {                                         //if clock input pin goes HIGH (clock has gone HIGH again)      
    digitalWrite(outputLed, 1);                            //turn on green led for duration of cycle indicating trigger press
    digitalWrite(clockLed, 1);                             //Turn on red LED
    loopspot = map(analogRead(A1), 0, 1023, 100, 1);       //read and map pot on A1 into loopspot variable
    for (int i = 0; i < loopspot; i++) {                   //run for loop for loopspot amount of times
         speedpot = map(analogRead(A0), 0, 1023, 5, 1000); //read and map pot on A into speed pot variable    
         digitalWrite(clockLed, 0);                        //turn on red clock output LED
         digitalWrite(output, 1);                          //turn on output
         delay(speedpot);                                  //wait here for length of speed pot variable
         digitalWrite(clockLed, 1);                        //turn off red clock output led
         digitalWrite(output, 0);                          //turn off output
         delay(speedpot);                                  //wait here for length of speed pot variable
         }
    }
}
digitalWrite(clockLed, 0);                                 //keep the red clock output led off when not in use
digitalWrite(output, 0);                                   //keep the output off when not in use
digitalWrite(outputLed, 0);                                //keep green output led off when not in use (placing these here turns everything off when not being used)
}
