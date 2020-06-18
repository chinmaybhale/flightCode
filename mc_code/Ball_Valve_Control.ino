//  Helios Rocketry- Avionics- Static Fire flight computer- Electric Ball Valve control

// The purpose of this code is to control a ball valve locally by accepting a high signal from the main R-Pi
// Current version is only subject to the Electrically Actuated Ball valves
// The main computer needs to give a high signal to actuate the open valve. The valve shall stay open until 
// high signal is given.

int solenoidPin = 4;                                                  // Local pin connection to the solenoid
int inputRead = 7;                                                    // Pin connected to R-Pi
int valveState = LOW;                                                 // State of the valve
int CurrentSignal;                                                    // Current signal from R-Pi
int PreviousSignal = LOW;                                             // Signal from previous iteration
long time = 0;                                                        // Initialize timer variable
long debounce = 200;                                                  // IGNORE this- temporary variable for testing purposes


void setup() {

  pinMode(inputRead, INPUT);                                          // Setting input pin
  pinMode(solenoidPin, OUTPUT);                                       // Setting output pin

}

void loop() {
// IGNORE the debounce variable- only for testing purposes with a switch
  CurrentSignal = digitalRead(inputRead);                             
  if (CurrentSignal == HIGH && PreviousSignal == LOW) {
      valveState = HIGH;
      delay(2000);
      valveState = LOW;
  }
  else if (CurrentSignal == HIGH && PreviousSignal == HIGH) {
      valveState = LOW;
    }
    else if (CurrentSignal == LOW && PreviousSignal ==HIGH){
      valveState = HIGH;
      delay(2000);
      valveState = LOW;
    }
    else{
      valveState = LOW;
    }

  digitalWrite(solenoidPin, valveState);
  PreviousSignal = CurrentSignal;
}
