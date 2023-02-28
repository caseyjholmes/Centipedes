//CASEY HOLMES
//HOLMES MODULAR
//CENTIPEDES CODE REV2.1 with updated EASTER EGGS!
//1/20/23

//constants
const int butt = 2;                           //button pin
const int output = 3;                         //output transistor on pin 3
const int outputLed = 6;                      //red output LED
const int clockLed =  5;                      //green clock led
const int clockPin = 4;                       //clock input transistor pin
unsigned long holdTime = 2000;                //interval = a 3 second wait
long debounce = 20;                           //debounce time no flicker

// Variables
int buttonVal = 0;                            //variable to store current state of the button
int buttonLast = 0;                           //variable to store last state of button
int clockState = 0;                           //variable to store current clock status
int buttonState = 0;                          //varable to store current state of button
bool isOn = true;                             //variable to store state of master duty clock cycle
long btnDnTime;                               //time button was pressed down
long btnUpTime;                               //time button was released
boolean ignoreUp = false;                     //weather ignore button release because press + hold was triggered
unsigned long loopspot = 0;                   //loops pot read variable
unsigned long speedpot =0;                    //speed pot read variable
unsigned long dutypot = 0;                    //duty pot read variable
unsigned long frequency = 0;                  //frequncy read variable for master clock
unsigned long offDuration = 0;                //variable to calculate duty cycle off time length
unsigned long previousTime = 0;               //stores previous time stamps from millis routines

void setup() {
  pinMode(butt, INPUT_PULLUP);                 //initialize the pushbutton pin as an input with a internal pullup resistor so buttons can pull it down and trigger it without extra resistors on the board
  pinMode(output, OUTPUT);                     //initialize output transistor
  pinMode(outputLed, OUTPUT);                  //initialize output LED as output
  digitalWrite(outputLed, 0);                  //initialize output LED as off
  pinMode(clockLed, OUTPUT);                   //initialize the LED pin as an output:
  digitalWrite(clockLed, 0);                   //initialize cock LED as off
  pinMode(clockPin, INPUT);                    //initialize clock pin as input
}

//function for handling short presses of the button (triggering a single centipede)
void shortpress() {                            //button was momentarily pressed
 digitalWrite(outputLed, 1);                   //turn on LED green indicating button press.
 digitalWrite(clockLed, 1);                    //turn on the other color LED
 loopspot = map(analogRead(A1), 0, 1023, 100, 1);//read and map pot on A1 into loopspot variable
 for (int b = 0; b < loopspot; b++) {          //run for loop for loopspot amount of times
     speedpot = map(analogRead(A0), 0, 1023, 5, 1000);//read and scale Analog 0 into speedpot
     digitalWrite(outputLed, 0);               //turn on output led
     digitalWrite(output, 1);                  //turn on output
     delay(speedpot);                          //wait for length of speed pot variable
     digitalWrite(outputLed, 1);               //turn off the output LED
     digitalWrite(output, 0);                  //turn off the output
     delay(speedpot);                          //wait for length of speed pot variable
     }
 digitalWrite(outputLed, 0);                  //keep red output led off
 digitalWrite(output, 0);                     //turn off the output
 digitalWrite(clockLed, 0);                   //keep the red clock output led off
}

void loop() {
 buttonVal = digitalRead(butt);               //read the pushbutton input pin:
 clockState = digitalRead(clockPin);          //read the clock input pin

 
//button handler routine
if (buttonVal == LOW && buttonLast == HIGH && (millis() - btnUpTime) > long(debounce)) { //test for button press and store down time
   btnDnTime = millis();
}  
if (buttonVal == HIGH && buttonLast == LOW && (millis() - btnDnTime) > long(debounce)) {  //test for button release and store up time
   if (ignoreUp == false) {
      shortpress();
   } else {
   ignoreUp = false;
   btnDnTime = millis();
   }
}  
if (buttonVal == LOW && (millis() - btnDnTime) > long(holdTime)) {//test for button held longer than hold time
   buttonState = !buttonState;                                 //invert state of button variable, trigger master clock routine
   ignoreUp = true;
   btnDnTime = millis();
}

//clock input signal handler routine     
while (!clockState){                                           // while the clock input that is held high goes low..  (trigger input has received a signal)
      clockState = digitalRead(clockPin);                      //read the clock input pin
      if (clockState) {                                        //if clock input pin goes HIGH (clock has gone HIGH again)
         digitalWrite(outputLed, 1);                           //turn on green led for duration of cycle indicating trigger press
         digitalWrite(clockLed, 1);                            //Turn on red LED
         loopspot = map(analogRead(A1), 0, 1023, 100, 1);      //read and map pot on A1 into loopspot variable
         for (int i = 0; i < loopspot; i++) {                  //run for loop for loopspot amount of times
             speedpot = map(analogRead(A0), 0, 1023, 5, 1000); //read and map pot on A into speed pot variable    
             digitalWrite(clockLed, 0);                        //turn on red clock output LED
             digitalWrite(output, 1);                          //turn on output
             delay(speedpot);                                  //wait for length of speed pot variable
             digitalWrite(clockLed, 1);                        //turn off red clock output led
             digitalWrite(output, 0);                          //turn off output
             delay(speedpot);                                  //wait for length of speed pot variable
             }
      } else {                                                 //if the button is high
      digitalWrite(clockLed, 0);                               //keep the red clock output led off
      digitalWrite(output, 0);                                 //keep the output off
      digitalWrite(outputLed, 0);                              //keep green output led off
      }
}
  
if (buttonState == 1){                                         //this is the master clock loop routine
   frequency = map(analogRead(A0), 0, 1023, 30, 3000);         //read and map pot on A into speed pot variable 1000 is max delay time(slow) 20 is minimum (fast)
   dutypot = map(analogRead(A1), 0, 1023,10, frequency);       //read and map pot on A into speed pot variable
   offDuration = frequency - dutypot;                          //calculation to get off time from duty cycle on time              

unsigned long currentTime = millis();                          // calculate current time since program started (in milliseconds)

if (isOn && (currentTime - previousTime >= dutypot)) {         //check if it's time to change the wave and LED signals by comparing isON variable with current and prvious time stamps and duty pot value
   digitalWrite (outputLed, 1);                                //turn on red output led
   digitalWrite (output, 1);                                   //turn on output
   isOn = false;                                               //update boolean is on with false value so next time through the loop it turns off instead of on
   previousTime = currentTime;                                 //set previous time to equal current time
   }
else if (!isOn && (currentTime - previousTime >= offDuration)){//check if it's time to change the wave and LED signals by comparing isON variable with current and prvious time stamps and off duration value
        digitalWrite(outputLed, 0);                            //turn off red output led
        digitalWrite(output, 0);                               //turn off output
        isOn = true;                                           //update boolean is on with false value so next time through the loop it turns on instead of off
        previousTime = currentTime;                            //set previous time to equal current time
        }
   }else{                                                      //this "else" condition was needed to turn off LEDs after getting out of master clock state and seems to have worked. 
   digitalWrite(clockLed, 0);                                  //keep the red clock output led off
   digitalWrite(output, 0);                                    //keep the output off
   digitalWrite(outputLed, 0);                                 //keep green output led off 
   }
 buttonLast = buttonVal;                                       //update last button state memory 
}
