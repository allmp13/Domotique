#include <Stepper.h>

// change this to the number of steps on your motor
#define STEPS  2048
// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to
Stepper stepper(STEPS, 8, 10, 9, 11);

// the position of the motor
int pos = 0;

// end of the brownian movement 
boolean finish = false;

void setup()
{
  // set the speed of the motor to 7 RPMs
  // values higher than this leads to lose of steps
  stepper.setSpeed(10);
}

void loop()
{
stepper.step(STEPS);
delay (50);
}

