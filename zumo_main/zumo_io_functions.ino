
// Atmega328p datasheet:
// https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
 
inline void Setup()
{
  // Enable interrupts
  sei();
  
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

  // ADC
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
  ADCSRA |= 1 << ADEN;

  { // Set up timer 0 for the micros() function
    // Disable PWM on pin 5 and 6
    TCCR0A |= (1 << WGM01) | (1 << WGM00);

    // Set timer 0 prescale factor to 64
    TCCR0B |= (1 << CS01) | (1 << CS00);
  
    // Enable timer 0 overflow interrupt
    TIMSK0 |= (1 << TOIE0);
  }
}

inline void SetDriveDirectionToForward()
{
  // Set pin 7 (right belt) to LOW (forward)
  PORTD &= ~(1 << 7);
  // Set pin 8 (left belt) to LOW (forward)
  PORTB &= ~(1 << 0);
}

inline void SetDriveDirectionToBackward()
{
  // Set pin 7 (right belt) to HIGH (backward)
  PORTD |= (1 << 7);
  // Set pin 8 (left belt) to HIGH (backward)
  PORTB |= (1 << 0);
}

inline void SetDriveDirectionToSpinLeft()
{
  // Set pin 7 (right belt) to LOW (forward)
  PORTD &= ~(1 << 7);
  // Set pin 8 (left belt) to HIGH (backward)
  PORTB |= (1 << 0);
}

inline void SetDriveDirectionToSpinRight()
{
  // Set pin 7 (right belt) to HIGH (backward)
  PORTD |= (1 << 7);
  // Set pin 8 (left belt) to LOW (forward)
  PORTB &= ~(1 << 0);
}

inline void SetDriveSpeed(const uint8_t t_speed)
{
  if (t_speed == 0)
  {
    // Set pin  9 (right belt speed),
    //         10 (left belt speed) to not use PWM
    TCCR1A &= ~((1 << COM1A1) | (1 << COM1B1));
    
    // Set pin  9 (right belt speed), 
    //         10 (left belt speed) to LOW (no speed)
    PORTB &= ~((1 << 1) | (1 << 2));
  }
  else if (t_speed == 255)
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
    OCR1A = t_speed;
   
    // Set pin 10 (left belt speed) PWM duty
    OCR1B = t_speed;
  }
}

inline void SetDriveSpeedLeftBelt(const uint8_t t_speed)
{
  if (t_speed == 0)
  {
    // Set pin 10 (left belt speed) to not use PWM
    TCCR1A &= ~(1 << COM1B1);
    
    // Set pin 10 (left belt speed) to LOW (no speed)
    PORTB &= ~(1 << 2);
  }
  else if (t_speed == 255)
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
    OCR1B = t_speed;
  }
}

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

inline bool GetStartButtonIsPressed()
{
  return ((PINB >> 4) & 0b1) == 0b0;
}

// bit0 = true: border detected on left, bit1 = true: border detected on right
inline uint8_t GetDetectedBorders()
{
  if (kUseWhiteAsBorderColor)
  {
    return ~(PIND >> 4) & 0b11;
  }
  else
  {
    return (PIND >> 4) & 0b11;
  }
}

inline bool GetTargetCenterLeft()
{
  return AI2_value >= kRangeCenterInverse;
}

inline bool GetTargetCenterRight()
{
  return AI3_value >= kRangeCenterInverse;
}

inline bool GetTargetSideLeft()
{
  return UDI0_value_cm <= kRangeSide;
}

inline bool GetTargetSideRight()
{
  return UDI1_value_cm <= kRangeSide;
}
