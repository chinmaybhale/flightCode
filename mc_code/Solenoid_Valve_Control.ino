//  Helios Rocketry- Avionics- Static Fire flight computer- Solenoid Valve control

// The purpose of this code is to control a solenoid valve locally by accepting a high signal from the main R-Pi
// Current version is only subject to the Solenoid valves
// The main computer needs to give an constant high signal to actuate the solenoid valve. The valve shall stay open until 
// high signal is given.

int solenoidPin = 4;                                                  // Local pin connection to the solenoid
int inputRead = 7;                                                    // Pin connected to R-Pi
int valveState = LOW;                                                 // State of the valve
int CurrentSignal;                                                    // Current signal from R-Pi
long time = 0;                                                        // Initialize timer variable

void setup() {

  pinMode(inputRead, INPUT);                                          // Setting input pin
  pinMode(solenoidPin, OUTPUT);                                       // Setting output pin

}

void loop() {
// IGNORE the debounce variable- only for testing purposes with a switch
  CurrentSignal = digitalRead(inputRead);                             
  if (CurrentSignal == HIGH ) {
      valveState = HIGH;
      }
  else{
            valveState = LOW;
      }
          digitalWrite(solenoidPin, valveState);
}
