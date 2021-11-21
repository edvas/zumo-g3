constexpr uint8_t AI_ADMUX_FLAGS = (1 << REFS0) | (1 << ADLAR);

enum class AnalogInput
{
  AI2,
  AI3,
};

static AnalogInput analog_pin = AnalogInput::AI2;

uint8_t AI2_value = 0;
uint8_t AI3_value = 0;

void ReadAnalogInputs()
{ 
  // Test if analog-to-digital conversion is finished
  if(!((ADCSRA >> ADSC) & 0b1))
  {
    // NOTE: the 'analog_pin' variable can be replaced by reading the first 3 bits in ADMUX
    switch(analog_pin)
    {
    case AnalogInput::AI2:
      // Read the 8-bit value
      AI2_value = ADCH;
      
      // Set next conversion to AI3
      ADMUX = AI_ADMUX_FLAGS | (3 & 0b111);
      analog_pin = AnalogInput::AI3;
      
      // Start next conversion
      ADCSRA |= 1 << ADSC;
      
      break;
    case AnalogInput::AI3:
      // Read the 8-bit value
      AI3_value = ADCH;
      
      // Set next conversion to AI3
      ADMUX = AI_ADMUX_FLAGS | (2 & 0b111);
      analog_pin = AnalogInput::AI2;
      
      // Start next conversion
      ADCSRA |= 1 << ADSC;
      
      break;
    default:
      break;
    }
  }
}
