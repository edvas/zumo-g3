static constexpr bool USE_WHITE_AS_BORDER = true;

// Atmega328p datasheet:
// https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
 
inline void SetPinModes()
{
  // Set pin 3 (buzzer),
  //         7 (right belt direction) to OUTPUT
  DDRD |= (1 << 3) | (1 << 7);
  
  // Set pin  8 (left belt direction), 
  //          9 (right belt speed), 
  //         10 (left belt speed),
  //         13 (LED) to OUTPUT
  DDRB |= (1 << 0) | (1 << 1) | (1 << 2); 

  // Set pin 4 (left border detector), 
  //         5 (right border detector) to INPUT_PULLUP
  PORTD |= (1 << 4) | (1 << 5);
  DDRD &= ~((1 << 4) | (1 << 5));

  // Set pin 12 (start button) to INPUT_PULLUP 
  PORTB |= (1 << 4);
  DDRB &= ~(1 << 4);

  // Use phase correct 8-bit PWM for pin  9 (right belt speed), 
  //                                     10 (left belt speed)
  TCCR1A =  (1 <<WGM10);
  TCCR1B = (1 << CS11) | (1 << CS10);
}

inline void SetDriveDirectionToForward()
{
  // Set pin 7 (Right belt) to LOW (Forward)
  PORTD &= ~(1 << 7);
  // Set pin 8 (Left belt) to LOW (Forward)
  PORTB &= ~(1 << 0);
}

inline void SetDriveDirectionToBackward()
{
  // Set pin 7 (Right belt) to HIGH (Backward)
  PORTD |= (1 << 7);
  // Set pin 8 (Left belt) to HIGH (Backward)
  PORTB |= (1 << 0);
}

inline void SetDriveDirectionToSpinLeft()
{
  // Set pin 7 (Right belt) to LOW (Forward)
  PORTD &= ~(1 << 7);
  // Set pin 8 (Left belt) to HIGH (Backward)
  PORTB |= (1 << 0);
}

inline void SetDriveDirectionToSpinRight()
{
  // Set pin 7 (Right belt) to HIGH (Backward)
  PORTD |= (1 << 7);
  // Set pin 8 (Left belt) to LOW (Forward)
  PORTB &= ~(1 << 0);
}

inline void SetDriveSpeed(const uint8_t a_speed)
{
  if (a_speed == 0)
  {
    // Set pin  9 (right belt speed),
    //         10 (left belt speed) to not use PWM
    TCCR1A &= ~((1 << COM1A1) | (1 << COM1B1));
    
    // Set pin  9 (right belt speed), 
    //         10 (left belt speed) to LOW (no speed)
    PORTB &= ~((1 << 1) | (1 << 2));
  }
  else if (a_speed == 255)
  {
    // Set pin  9 (right belt speed),
    //         10 (left belt speed) to not use PWM
    TCCR1A &= ~((1 << COM1A1) | (1 << COM1B1));
    
    // Set pin  9 (right belt speed),
    //         10 (left belt speed) to HIGH (max speed)
    PORTB |= (1 << 1) | (1 << 2);
  }
  else
  {
    // Set pin  9 (right belt speed),
    //         10 (left belt speed) to use PWM
    TCCR1A |= (1 << COM1A1) | (1 << COM1B1);
    
    // Set pin 9 (right belt speed) PWM duty
    OCR1A = a_speed;
   
    // Set pin 10 (left belt speed) PWM duty
    OCR1B = a_speed;
  }
}

inline void SetDriveSpeedLeftBelt(const uint8_t a_speed)
{
  if (a_speed == 0)
  {
    // Set pin 10 (left belt speed) to not use PWM
    TCCR1A &= ~(1 << COM1B1);
    
    // Set pin 10 (left belt speed) to LOW (no speed)
    PORTB &= ~(1 << 2);
  }
  else if (a_speed == 255)
  {
    // Set pin 10 (left belt speed) to not use PWM
    TCCR1A &= ~(1 << COM1B1);
    
    // Set pin 10 (left belt speed) to HIGH (max speed)
    PORTB |= (1 << 2);
  }
  else
  {
    // Set pin 10 (left belt speed) to use PWM
    TCCR1A |= 1 << COM1B1;
    
    // Set pin 10 (left belt speed) PWM duty
    OCR1B = a_speed;
  }
}

// Changes io pin 9 and uses TIMER1A
inline void SetDriveSpeedRightBelt(const uint8_t a_speed)
{
  if (a_speed == 0)
  {
    // Set pin 9 (right belt speed) to not use PWM
    TCCR1A &= ~(1 << COM1A1);
    
    // Set pin 9 (right belt speed) to LOW (no speed)
    PORTB &= ~(1 << 1);
  }
  else if (a_speed == 255)
  {
    // Set pin 9 (right belt speed) to not use PWM
    TCCR1A &= ~(1 << COM1A1);
    
    // Set pin 9 (right belt speed) to HIGH (max speed)
    PORTB |= (1 << 1);
  }
  else
  {
    // Set pin 9 (right belt speed) to use PWM
    TCCR1A |= 1 << COM1A1;
    
    // Set pin 9 (right belt speed) PWM duty
    OCR1A = a_speed;
  }
}

// bit0 = border detected on left, bit1 = border detected on right
inline uint8_t GetDetectedBorders()
{
  if (USE_WHITE_AS_BORDER)
  {
    return ~(PIND >> 4) & 0b11;
  }
  else
  {
    return (PIND >> 4) & 0b11;
  }
}
