
#define CW  1
#define CCW 0

const byte pinA = 2;
const byte pinB = 3;
const byte pinButton = 4;

volatile byte stateA = HIGH;
volatile byte stateB = HIGH;
byte stateButton = HIGH;

unsigned long buttonTimeOut = millis();



void setup()
{
  Serial.begin(19200);

  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(pinA), HandleRotaryA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinB), HandleRotaryB, CHANGE);
}



enum EncoderState
{
  WAIT_ALL,

  //CLOCKWISE STATES
  CW_WAIT_B_LOW,
  CW_WAIT_A_HIGH,
  CW_WAIT_B_HIGH,

  //COUNTERCLOCKWISE STATES
  CCW_WAIT_A_LOW,
  CCW_WAIT_B_HIGH,
  CCW_WAIT_A_HIGH

};

volatile EncoderState eState = WAIT_ALL;




void loop()
{
  ReadButton();
}



void ReadButton()
{
  if (millis() - buttonTimeOut > 80)
  {
    byte currentButtonState = digitalRead(pinButton);
    if (stateButton == LOW && currentButtonState == HIGH)
      ButtonPushed();

    stateButton = currentButtonState;

    buttonTimeOut = millis();
  }
}




void ButtonPushed()
{
  Serial.println("BUTTON");
}




void Rotate(int dir)
{
  eState = WAIT_ALL;

  if (dir == CW)
  {
    Serial.println("CLOCKWISE");
  }
  else if (dir == CCW)
  {
    Serial.println("COUNTER-CLOCKWISE");
  }
}




void HandleRotary(int pin, byte state)
{
  if (pin == pinA)
  {
    switch (eState)
    {
      case WAIT_ALL:
        if (state == LOW)
          eState = CW_WAIT_B_LOW;
        break;

      case CW_WAIT_B_LOW:
        if (state == HIGH)
          eState = WAIT_ALL;
        break;

      case CW_WAIT_A_HIGH:
        if (state == HIGH)
          eState = CW_WAIT_B_HIGH;
        break;

      case CW_WAIT_B_HIGH:
        if (state == LOW)
          eState = CW_WAIT_A_HIGH;
        break;

      case CCW_WAIT_A_LOW:
        if (state == LOW)
          eState = CCW_WAIT_B_HIGH;
        break;

      case CCW_WAIT_B_HIGH:
        if (state == HIGH)
          CCW_WAIT_A_LOW;
        break;

      case CCW_WAIT_A_HIGH:
        if (state == HIGH)
          Rotate(CCW);
        break;
    }
  }

  else if (pin == pinB)
  {
    switch (eState)
    {
      case WAIT_ALL:
        if (state == LOW)
          eState = CCW_WAIT_A_LOW;
        break;

      case CW_WAIT_B_LOW:
        if (state == LOW)
          eState = CW_WAIT_A_HIGH;
        break;

      case CW_WAIT_A_HIGH:
        if (state == HIGH)
          eState = CW_WAIT_B_LOW;
        break;

      case CW_WAIT_B_HIGH:
        if (state == HIGH)
          Rotate(CW);
        break;

      case CCW_WAIT_A_LOW:
        if (state == HIGH)
          eState = WAIT_ALL;
        break;

      case CCW_WAIT_B_HIGH:
        if (state == HIGH)
          eState = CCW_WAIT_A_HIGH;
        break;

      case CCW_WAIT_A_HIGH:
        if (state == LOW)
          eState = CCW_WAIT_B_HIGH;
        break;
    }

  }

  if (stateA == HIGH && stateB == HIGH)
    eState = WAIT_ALL;
}




void HandleRotaryA()
{
  byte newState = digitalRead(pinA);
  if (stateA != newState)
  {
    stateA = newState;
    HandleRotary(pinA, stateA);
  }
}




void HandleRotaryB()
{
  byte newState = digitalRead(pinB);
  if (stateB != newState)
  {
    stateB = newState;
    HandleRotary(pinB, stateB);
  }
}

