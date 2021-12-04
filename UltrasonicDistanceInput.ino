#include <util/delay.h>

uint8_t UDI0_value_cm = 0;
uint8_t UDI1_value_cm = 0;

void ReadUltrasonicDistanceInputs()
{ 
  // Clears the trigPin
  DDRC |= (1 << 0);
  PORTC &= ~(1 << 0);
  _delay_us(2);
  PORTC |= (1 << 0);
  _delay_us(10);
  PORTC &= ~(1 << 0);
  DDRC &= ~(1 << 0);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  uint16_t duration = pulseIn(A0, HIGH, kUltrasonicTimeout);
  // Calculating the distance
  UDI0_value_cm = (duration / kUltrasonicTimeToDistanceDivisor) - 1;


  // Clears the trigPin
  DDRC |= (1 << 1);
  PORTC &= ~(1 << 1);
  _delay_us(2);
  PORTC |= (1 << 1);
  _delay_us(10);
  PORTC &= ~(1 << 1);
  DDRC &= ~(1 << 1);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(A1, HIGH, kUltrasonicTimeout);
  // Calculating the distance
  UDI1_value_cm = (duration / kUltrasonicTimeToDistanceDivisor) - 1;
}
