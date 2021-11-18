#pragma GCC optimize ("O3")

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
  SetPinModes();

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

      case State::AvoidingBorderOnLeftMovingBackwards:
        // Event timeout
        if (TestTransitionToAvoidingBorderOnLeftTurning(500))
        {
          state = State::AvoidingBorderOnLeftTurning;
        }
        break;

      case State::AvoidingBorderOnLeftTurning:
      case State::AvoidingBorderOnRightTurning:
      case State::AvoidingBorderInFrontTurning:
        // Event timeout
        if (TestTransitionToSeekingForward(1000))
        {
          state = State::SeekingForward;
        }
        break;
      
      case State::AvoidingBorderOnRightMovingBackwards:
        // Event timeout
        if (TestTransitionToAvoidingBorderOnRightTurning(500))
        {
          state = State::AvoidingBorderOnRightTurning;
        }
        break;

      case State::AvoidingBorderInFrontMovingBackwards:
        // Event timeout
        if (TestTransitionToAvoidingBorderInFrontTurning(500))
        {
          state = State::AvoidingBorderInFrontTurning;
        }
        break;
        
      default:
        break;
    };
  }
}
