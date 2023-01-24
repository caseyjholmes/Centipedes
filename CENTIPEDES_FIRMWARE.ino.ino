//CASEY HOLMES
//HOLMES MODULAR
//CENTIPEDES CODE REV2
//1/20/23

//constants
const int button = 2;                         //button pin
const int output = 3;                         //output transistor on pin 3
const int outputLed = 6;                      //red output LED
const int clockLed =  5;                      //green clock led
const int clockPin = 4;                       //clock input transistor pin
unsigned long holdTime = 1000;                //interval = a 3 second wait
long btnDnTime;                               //time button was pressed down
long btnUpTime;                               //time button was released
boolean ignoreUp = false;                     //weather ignore button release because press + hold was triggered
long debounce = 20;                           //debounce time no flicker

// Variables
int buttonVal = 0;                            //variable to store current state of the button
int buttonLast = 0;                           //variable to store last state of button
int clockState = 0;                           //variable to store current clock status
int buttonState = 0;

void setup() {
  pinMode(button, INPUT_PULLUP);               //initialize the pushbutton pin as an input with a internal pullup resistor so buttons can pull it down and trigger it without extra resistors on the board
  pinMode(output, OUTPUT);                     //initialize output transistor
  pinMode(outputLed, OUTPUT);                  //initialize output LED as output
  digitalWrite(outputLed, 0);                  //initialize output LED as off
  pinMode(clockLed, OUTPUT);                   //initialize the LED pin as an output:
  digitalWrite(clockLed, 0);                   //initialize cock LED as off
  pinMode(clockPin, INPUT);                    //initialize clock pin as input
}

void shortpress() {                            //button was briefly momentarily pressed
  digitalWrite(outputLed, 1);                  //turn on LED green indicating button press.. for duration of cycle
  digitalWrite(clockLed, 1);                   //turn on the other color LED
  int loopspot = analogRead(A1);               //create variable int named loopspot and read into it from analog input A1
  loopspot = map(loopspot, 0, 1023, 100, 1);   //scale it to use it with the loopspot (value between 1 and 100)
  for (int b = 0; b < loopspot; b++) {         //run for loop for loopspot amount of times
    Serial.println(loopspot);                  //debug loopspot variable to serial monitor
    int speedpot = analogRead(A0);             //create variable int named speedpot and read analog pot on A0 into it
    speedpot = map(speedpot, 0, 1023, 5, 1000);// scale it to use it with the loopspot (value between 5 and 1000)
    digitalWrite(outputLed, 0);                //turn on output led
    digitalWrite(output, 1);                   //turn on output
    delay(speedpot);                           //wait for length of speed pot variable
    digitalWrite(outputLed, 1);                //turn off the output LED
    digitalWrite(output, 0);                   //turn off the output
    delay(speedpot);                           //wait for length of speed pot variable
  }
  digitalWrite(outputLed, 0);                  //keep red output led off
  digitalWrite(output, 0);                     //turn off the output
  digitalWrite(clockLed, 0);                   //keep the red clock output led off
}

void loop() {
 buttonVal = digitalRead(button);             //read the pushbutton input pin:
 clockState = digitalRead(clockPin);          //read the clock input pin

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
  
  if (buttonVal == LOW && (millis() - btnDnTime) > long(holdTime)) {                       //test for button held longer than hold time
     buttonState = !buttonState;                                                           //invert state of button variable, trigger master clock routine
     ignoreUp = true;
     btnDnTime = millis();
     }
  
  if (!clockState) {                             //if clock input pin goes low (it's held high)
    digitalWrite(outputLed, 1);                 //turn on green led for duration of cycle indicating trigger press
    digitalWrite(clockLed, 1);                  //Turn on red LED
    int loopspot = analogRead(A1);              //create variable int named loopspot and read analog pot on A1 into it
    loopspot = map(loopspot, 0, 1023, 100, 1);  // scale it to use it with the loopspot (value between 1 and 100)
    for (int i = 0; i < loopspot; i++) {        //run for loop for loopspot amount of times
      int speedpot = analogRead(A0);             //create variable int named speedpot and read analog pot on A0 into it
      speedpot = map(speedpot, 0, 1023, 5, 1000);// scale it to use it with the loopspot (value between 15 and 100
      digitalWrite(clockLed, 0);                 //turn on red clock output LED
      digitalWrite(output, 1);                   //turn on output
      delay(speedpot);                           //wait for length of speed pot variable
      digitalWrite(clockLed, 1);                 //turn off red clock output led
      digitalWrite(output, 0);                   //turn off output
      delay(speedpot);                           //wait for length of speed pot variable
    }
  } else {                                     //if the button is high
    digitalWrite(clockLed, 0);                 //keep the red clock output led off
    digitalWrite(output, 0);                   //keep the output off
    digitalWrite(outputLed, 0);                //keep green output led off
  }

  if (buttonState == 1){
     int speedpot = analogRead(A0);             //create variable int named speedpot and read analog pot on A0 into it
     speedpot = map(speedpot, 0, 1023, 5, 1000);// scale it to use it with the loopspot (value between 15 and 100
     digitalWrite (outputLed, 1);
     digitalWrite (output, 1);
     delay(speedpot);                           //wait for length of speed pot variable
     digitalWrite(outputLed, 0);                 //turn off output led
     digitalWrite(output, 0);                   //turn off output
     delay(speedpot);
   }

 buttonLast = buttonVal;
   
}
