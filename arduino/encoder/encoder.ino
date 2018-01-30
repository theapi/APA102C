
#define PIN_ENCODER_A D3
#define PIN_ENCODER_B D2

volatile uint8_t brightness = 3;
volatile uint8_t brightness_changed = 0;
volatile uint8_t encoder_ab = 0; // The previous & current reading


/**
 * returns change in encoder state (-1,0,1) 
 */
int8_t encoder_read()
{
  // enc_states[] array is a look-up table; 
  // it is pre-filled with encoder states, 
  // with “-1″ or “1″ being valid states and “0″ being invalid. 
  // We know that there can be only two valid combination of previous and current readings of the encoder 
  // – one for the step in a clockwise direction, 
  // another one for counterclockwise. 
  // Anything else, whether it's encoder that didn't move between reads 
  // or an incorrect combination due to bouncing, is reported as zero.
  static int8_t enc_states[] = {
    0,-1,1,0, 1,0,0,-1, -1,0,0,1, 0,1,-1,0
  };
  
  /*
   The lookup table of the binary values represented by enc_states 
     ___     ___     __
   A    |   |   |   |
        |___|   |___|
      1 0 0 1 1 0 0 1 1
      1 1 0 0 1 1 0 0 1
     _____     ___     __
   B      |   |   |   |
          |___|   |___|
   
   A is represented by bit 0 and bit 2
   B is represented by bit 1 and bit 3
   With previous and current values stored in 4 bit data there can be
   16 possible combinations.
   The enc_states lookup table represents each one and what it means:
   
   [0] = 0000; A & B both low as before: no change : 0
   [1] = 0001; A just became high while B is low: reverse : -1
   [2] = 0010; B just became high while A is low: forward : +1
   [3] = 0011; B & A are both high after both low: invalid : 0
   [4] = 0100; A just became low while B is low: forward : +1
   [5] = 0101; A just became high after already being high: invalid : 0
   [6] = 0110; B just became high while A became low: invalid : 0
   [7] = 0111; A just became high while B was already high: reverse : -1
   [8] = 1000; B just became low while A was already low: reverse : -1
   etc...
   
   Forward: 1101 (13) - 0100 (4) - 0010 (2) - 1011 (11)
   Reverse: 1110 (14) - 1000 (8) - 0001 (1) - 0111 (7)
   
  */

  // ab gets shifted left two times 
  // saving previous reading and setting two lower bits to “0″ 
  // so the current reading can be correctly ORed.
  encoder_ab <<= 2;
  
  // digital read is fast enough on esp8266
  uint8_t pinA = digitalRead(PIN_ENCODER_A);
  uint8_t pinB = digitalRead(PIN_ENCODER_B);
  // Set the two least significant bits.
  bitWrite(encoder_ab, 0, pinA);
  bitWrite(encoder_ab, 1, pinB);

  //encoder_ab = (encoder_ab & 0x0f);
  
  // At this point, we have previous reading of encoder pins in bits 2,3 of ab, 
  // current readings in bits 0,1, and together they form index of (AKA pointer to) enc_states[]  
  // array element containing current state.
  // The index being the the lowest nibble of ab (ab & 0x0f)
  return ( enc_states[( encoder_ab & 0x0f )]);
}

// Pin interrupt
void encoder_ISR() {
  int8_t direction;
  direction = encoder_read();
  if (direction == 1 && brightness < 255) {
    brightness++;
    brightness_changed = 1;
  }
  else if (direction == -1 && brightness > 0) {
    brightness--;
    brightness_changed = 1;
  }
}

void setup() {
  pinMode(PIN_ENCODER_A, INPUT_PULLUP); 
  pinMode(PIN_ENCODER_B, INPUT_PULLUP);

  Serial.begin(115200);
  Serial.println();

  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_A), encoder_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_B), encoder_ISR, CHANGE);
}

void loop() {
  if (brightness_changed) {
    brightness_changed = 0;
//    Serial.print("ab: ");
//    Serial.print(encoder_ab, BIN);
//    Serial.print(" brightness: ");
    Serial.println(brightness);
  }
}

