// Helios Rocketry AFCP/mc_code
// Ball_Valve_Control.ino- Arduino code to program Atmel ATTiny45 using an Arduino UNO as an ISP.

// The purpose of this code is to control a ball valve locally by accepting a high signal from the main R-Pi
// Current version is only subject to the Electrically Actuated Ball valves
// The main computer needs to give a high signal to actuate the open valve. The valve shall stay open until
// high signal is given. The program also gives feedback to the Rpi on the position of the valve.

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
int valvePin = 4;         // Local pin connection to the solenoid
int inputRead = 7;        // Pin connected to R-Pi
int feedback = 3          // Pin for sending valve state feedback to the Pi
int valveState = LOW;     // State of the valve
int CurrentSignal;        // Current signal from R-Pi
int PreviousSignal = LOW; // Signal from previous iteration
long time = 0;            // Initialize timer variable
long debounce = 200;      // IGNORE this- temporary variable for testing purposes

void setup()
{
  pinMode(inputRead, INPUT); // Setting input pin
  pinMode(valvePin, OUTPUT); // Setting output pin
  pinMode(feedback, OUTPUT); 
}
//---------------------------------------------------------------------------------------------------------------------------

void loop()
{
  // IGNORE the debounce variable- only for testing purposes with a switch
  CurrentSignal = digitalRead(inputRead);
  if (CurrentSignal == HIGH && PreviousSignal == LOW)
  {
    valveState = HIGH;
    digitalWrite(valvePin, valveState);
    delay(2000);
    valveState = LOW;
    digitalWrite(valvePin, valveState);
    digitalWrite(feedback, HIGH);
  }
  else if (CurrentSignal == HIGH && PreviousSignal == HIGH)
  {
    valveState = LOW;
    digitalWrite(valvePin, valveState);
    digitalWrite(feedback, HIGH);
  }
  else if (CurrentSignal == LOW && PreviousSignal == HIGH)
  {
    valveState = HIGH;
    digitalWrite(valvePin, valveState);
    delay(2000);
    valveState = LOW;
    digitalWrite(valvePin, valveState);
    digitalWrite(feedback, LOW);
  }
  else
  {
    valveState = LOW;
    digitalWrite(valvePin, valveState);
    digitalWrite(feedback, LOW);
  }

  PreviousSignal = CurrentSignal;
}

//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
