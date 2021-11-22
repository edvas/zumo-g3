#pragma GCC optimize ("Os")

//#define Z3_DEBUG
//#define Z3_DEBUG_ONE_LINE
//#define Z3_DEBUG_PRINT_AI
//#define Z3_DEBUG_PRINT_UDI
//#define Z3_DEBUG_STATE

#define Z3_FEATURE_SEEK_SIDE

const bool USE_WHITE_AS_BORDER = true;
const uint8_t MAX_SPEED = 255;
const uint8_t NO_SPEED = 0;

const uint8_t RANGE_SIDE_CM = 50;
const uint8_t RANGE_CENTER = 65;

constexpr uint64_t UDI_TIMEOUT = RANGE_SIDE_CM * 90;
constexpr uint64_t MS = 1000;


extern uint8_t AI2_value;
extern uint8_t AI3_value;

extern uint8_t UDI0_value_cm;
extern uint8_t UDI1_value_cm;

enum class State
{
  None,
  
  Idle,
  Starting,

  SeekingForward,
  SeekingSideLeft,
  SeekingSideRight,
  SeekingCenterLeft,
  SeekingCenterRight,
  
  AvoidingBorderOnLeftMovingBackwards,
  AvoidingBorderOnLeftTurning,
  AvoidingBorderOnRightMovingBackwards,
  AvoidingBorderOnRightTurning,
  AvoidingBorderInFrontMovingBackwards,
  AvoidingBorderInFrontTurning,
};

int main()
{
  #ifdef Z3_DEBUG
    // NOTE: Using baudrates other than 9600 may need initialization of the USB controller.
    //       Replaceing 'main()' with 'setup()' and 'loop()' does this in the background
    Serial.begin(9600);
  #endif

  // Enable interrupts
  sei();

  { // Pin and timer configuration

    
    SetPinModes();
    
    { // Set up timer 0 for micros()
      // Disable PWM
      TCCR0A |= (1 << WGM01) | (1 << WGM00);
  
      // Set timer 0 prescale factor to 64
      TCCR0B |= (1 << CS01) | (1 << CS00);
    
      // Enable timer 0 overflow interrupt
      TIMSK0 |= (1 << TOIE0);
    }
  }

  State state = State::Idle;

  for(;;)
  {
    #ifdef Z3_DEBUG_PRINT_AI
      Serial.print("AI: ");
      Serial.print(AI2_value);
      Serial.print(" ");
      Serial.println(AI3_value);
    #endif

    #ifdef Z3_DEBUG_PRINT_UDI
      Serial.print("UDI: ");
      Serial.print(UDI0_value_cm);
      Serial.print(" ");
      Serial.println(UDI1_value_cm);
    #endif

    #ifdef Z3_DEBUG_STATE
      Serial.print("State: ");
      Serial.println(static_cast<int>(state));
    #endif

    #ifdef Z3_DEBUG_ONE_LINE
      Serial.print(AI2_value);
      Serial.print(">");
      Serial.print(AI3_value);
      Serial.print(">");
      Serial.print(UDI0_value_cm);
      Serial.print(">");
      Serial.print(UDI1_value_cm);
      Serial.print(">");
      Serial.println(static_cast<int>(state));
    #endif
    
    ReadAnalogInputs();

    #ifdef Z3_FEATURE_SEEK_SIDE
      ReadUltrasonicDistanceInputs();
    #endif

    // State machine
    switch(state)
    {
      case State::Idle:
        // Event start button pressed
        if (TestTransitionToStarting())
        {
          state = State::Starting;
        }
        break;
        
      case State::Starting:
        // Event timeout
        if (TestTransitionToSeekingForward(5000 * MS))
        {
          state = State::SeekingForward;
          _delay_ms(1000);
        }
        break;
        
      case State::SeekingForward:
      case State::SeekingSideLeft:
      case State::SeekingSideRight:   
      case State::SeekingCenterLeft:
      case State::SeekingCenterRight:
        { // Scope for detecting borders
          // Event border detector on both sides
          const auto return_state = TestAllTransitionsToAvoidingBorders();
          if (return_state != State::None)
          {
            state = return_state;
            break;
          }
        }

        #ifdef Z3_FEATURE_SEEK_SIDE
          // Event target detector on left side active
          if (TestTransitionToSeekingSideLeft())
          {
            state = State::SeekingSideLeft;
            break;
          }
  
          // Event target detector on right side active
          if (TestTransitionToSeekingSideRight())
          {
            state = State::SeekingSideRight;
            break;
          }
        #endif
        
        // Event target detector on left center active
        if (TestTransitionToSeekingCenterLeft())
        {
          state = State::SeekingCenterLeft;
          break;
        }

        // Event target detector on right center active
        if (TestTransitionToSeekingCenterRight())
        {
          state = State::SeekingCenterRight;
          break;
        }
        break;

      case State::AvoidingBorderOnLeftMovingBackwards:
        // Event timeout
        if (TestTransitionToAvoidingBorderOnLeftTurning(200 * MS))
        {
          state = State::AvoidingBorderOnLeftTurning;
        }
        break;
      
      case State::AvoidingBorderOnRightMovingBackwards:
        // Event timeout
        if (TestTransitionToAvoidingBorderOnRightTurning(200 * MS))
        {
          state = State::AvoidingBorderOnRightTurning;
        }
        break;

      case State::AvoidingBorderInFrontMovingBackwards:
        // Event timeout
        if (TestTransitionToAvoidingBorderInFrontTurning(200 * MS))
        {
          state = State::AvoidingBorderInFrontTurning;
        }
        break;

      case State::AvoidingBorderOnLeftTurning:
      case State::AvoidingBorderOnRightTurning:
      case State::AvoidingBorderInFrontTurning:
        // Event timeout
        if (TestTransitionToSeekingForward(400 * MS))
        {
          state = State::SeekingForward;
        }
        break;
      
      default:
        break;
    };
  }
}
