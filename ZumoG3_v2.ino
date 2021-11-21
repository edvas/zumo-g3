#pragma GCC optimize ("Os")

const bool USE_WHITE_AS_BORDER = true;
const uint8_t MAX_SPEED = 255;
const uint8_t NO_SPEED = 0;

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
 
  { // Pin and timer configuration
    sei();
    
    SetPinModes();
    
    { // Set up timer 0 for millis()
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
        if (TestTransitionToSeekingForward(5000))
        {
          state = State::SeekingForward;
        }
        break;
        
      case State::SeekingForward:
        { // Scope for detecting borders
          // Event border detector on both sides
          const auto return_state = TestAllTransitionsToAvoidingBorders();
          if (return_state != State::None)
          {
            state = return_state;
            break;
          }
        }

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
        
      case State::SeekingSideLeft:
        { // Scope for detecting borders
          // Event border detector on both sides
          const auto return_state = TestAllTransitionsToAvoidingBorders();
          if (return_state != State::None)
          {
            state = return_state;
            break;
          }
        }

        // Event target detector on right side active
        if (TestTransitionToSeekingSideRight())
        {
          state = State::SeekingSideRight;
          break;
        }

        // Event target detector on right center active
        if (TestTransitionToSeekingCenterRight())
        {
          state = State::SeekingCenterRight;
          break;
        }
        break;
      case State::SeekingSideRight:
        { // Scope for detecting borders
          // Event border detector on both sides
          const auto return_state = TestAllTransitionsToAvoidingBorders();
          if (return_state != State::None)
          {
            state = return_state;
            break;
          }
        }

        // Event target detector on left side active
        if (TestTransitionToSeekingSideLeft())
        {
          state = State::SeekingSideLeft;
          break;
        }

        // Event target detector on left center active
        if (TestTransitionToSeekingCenterLeft())
        {
          state = State::SeekingCenterLeft;
          break;
        }
        break;
        
      case State::SeekingCenterLeft:
        { // Scope for detecting borders
          // Event border detector on both sides
          const auto return_state = TestAllTransitionsToAvoidingBorders();
          if (return_state != State::None)
          {
            state = return_state;
            break;
          }
        }

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

        // Event target detector on right center active
        if (TestTransitionToSeekingCenterRight())
        {
          state = State::SeekingCenterRight;
          break;
        }
      
        break;
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

        // Event target detector on left center active
        if (TestTransitionToSeekingCenterLeft())
        {
          state = State::SeekingCenterLeft;
          break;
        }
        break;

      case State::AvoidingBorderOnLeftTurning:
      case State::AvoidingBorderOnRightTurning:
      case State::AvoidingBorderInFrontTurning:
        // Event timeout
        if (TestTransitionToSeekingForward(400))
        {
          state = State::SeekingForward;
        }
        break;

      case State::AvoidingBorderOnLeftMovingBackwards:
        // Event timeout
        if (TestTransitionToAvoidingBorderOnLeftTurning(200))
        {
          state = State::AvoidingBorderOnLeftTurning;
        }
        break;
      
      case State::AvoidingBorderOnRightMovingBackwards:
        // Event timeout
        if (TestTransitionToAvoidingBorderOnRightTurning(200))
        {
          state = State::AvoidingBorderOnRightTurning;
        }
        break;

      case State::AvoidingBorderInFrontMovingBackwards:
        // Event timeout
        if (TestTransitionToAvoidingBorderInFrontTurning(200))
        {
          state = State::AvoidingBorderInFrontTurning;
        }
        break;
        
      default:
        break;
    };
  }
}
