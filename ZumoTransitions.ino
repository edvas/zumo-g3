// Single variable used to store the starting time for the transitions that are time based
static uint64_t g_timer_start = 0uL;

inline bool TestTransitionToStarting()
{
  if (((PINB >> 4) & 0b1) == 0b0)
  {
    g_timer_start = millis();
    return true;
  }
  else
  {
    return false;
  }
}

inline bool TestTransitionToSeekingForward(const uint64_t duration)
{
  if(millis() - g_timer_start >= duration)
  {
    SetDriveDirectionToForward();
    SetDriveSpeed(255);
    return true;
  }
  else
  {
    return false; 
  }
}

inline bool TestTransitionToSeekingSideLeft()
{
  if(GetTargetSideLeft())
  {
    SetDriveDirectionToSpinRight();
    SetDriveSpeed(127);
    return true;
  }
  else
  {
    return false;
  }
}

inline bool TestTransitionToSeekingSideRight()
{
  if(GetTargetSideRight())
  {
    SetDriveDirectionToSpinLeft();
    SetDriveSpeed(127);
    return true;
  }
  else
  {
    return false;
  }
}

inline bool TestTransitionToSeekingCenterLeft()
{
  if(GetTargetCenterLeft())
  {
    SetDriveDirectionToForward();
    SetDriveSpeedLeftBelt(127);
    SetDriveSpeedRightBelt(255);
    return true;
  }
  else
  {
    return false;
  }
}

inline bool TestTransitionToSeekingCenterRight()
{
  if(GetTargetCenterRight())
  {
    SetDriveDirectionToForward();
    SetDriveSpeedLeftBelt(255);
    SetDriveSpeedRightBelt(127);
    return true;
  }
  else
  {
    return false;
  }
}

inline bool TestTransitionToAvoidingBorderOnLeftMovingBackwards(const uint8_t detected_borders)
{
  if(((detected_borders >> 0) & 0b1) == 0b1)
  {
    g_timer_start = millis();
    SetDriveDirectionToBackward();
    SetDriveSpeed(255);
    return true;
  }
  else
  {
    return false;
  }
}

inline bool TestTransitionToAvoidingBorderOnLeftTurning(const uint64_t duration)
{
  if(millis() - g_timer_start >= duration)
  {
    SetDriveDirectionToSpinRight();
    SetDriveSpeed(255);
    return true;
  }
  else
  {
    return false;
  }
}

inline bool TestTransitionToAvoidingBorderOnRightMovingBackwards(const uint8_t detected_borders)
{
  if(((detected_borders >> 1) & 0b1) == 0b1)
  {
    g_timer_start = millis();
    SetDriveDirectionToBackward();
    SetDriveSpeed(255);
    return true;
  }
  else
  {
    return false;
  }
}

inline bool TestTransitionToAvoidingBorderOnRightTurning(const uint64_t duration)
{
  if(millis() - g_timer_start >= duration)
  {
    SetDriveDirectionToSpinLeft();
    SetDriveSpeed(255);
    return true;
  }
  else
  {
    return false;
  }
}

inline bool TestTransitionToAvoidingBorderInFrontMovingBackwards(const uint8_t detected_borders)
{
  if(((detected_borders >> 0) & 0b11) == 0b11)
  {
    g_timer_start = millis();
    SetDriveDirectionToBackward();
    SetDriveSpeed(255);
    return true;
  }
  else
  {
    return false;
  }
}

inline bool TestTransitionToAvoidingBorderInFrontTurning(const uint64_t duration)
{
  if(millis() - g_timer_start >= duration)
  {
    g_timer_start = millis();
    SetDriveDirectionToSpinRight();
    SetDriveSpeed(255);
    return true;
  }
  else
  {
    return false;
  }
}

inline State TestAllTransitionsToAvoidingBorders()
{
  const uint8_t detected_borders = GetDetectedBorders();
          
  // Event border detector on both sides
  if (TestTransitionToAvoidingBorderInFrontMovingBackwards(detected_borders))
  {
    return State::AvoidingBorderInFrontMovingBackwards;
  }
  
  // Event border detector on right side
  if (TestTransitionToAvoidingBorderOnRightMovingBackwards(detected_borders))
  {
    return State::AvoidingBorderOnRightMovingBackwards;
  }

  // Event border detector on left side
  if (TestTransitionToAvoidingBorderOnLeftMovingBackwards(detected_borders))
  {
    return State::AvoidingBorderOnLeftMovingBackwards;
  }
          
  return State::None;
}
