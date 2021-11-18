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
  return millis() - g_timer_start >= duration;
}

inline bool TestTransitionToSeekingSideLeft()
{
  return false;
}

inline bool TestTransitionToSeekingSideRight()
{
  return false;
}

inline bool TestTransitionToSeekingCenterLeft()
{
  return false;
}

inline bool TestTransitionToSeekingCenterRight()
{
  return false;
}

inline bool TestTransitionToAvoidingBorderOnLeftMovingBackwards(const uint8_t detected_borders)
{
  return ((detected_borders >> 4) & 0b1) == 0b1;
}

inline bool TestTransitionToAvoidingBorderOnLeftTurning(const uint64_t duration)
{
  return millis() - g_timer_start >= duration;
}

inline bool TestTransitionToAvoidingBorderOnRightMovingBackwards(const uint8_t detected_borders)
{
  return ((detected_borders >> 5) & 0b1) == 0b1;
}

inline bool TestTransitionToAvoidingBorderOnRightTurning(const uint64_t duration)
{
  return millis() - g_timer_start >= duration;
}

inline bool TestTransitionToAvoidingBorderInFrontMovingBackwards(const uint8_t detected_borders)
{
  return ((detected_borders >> 4) & 0b11) == 0b11;
}

inline bool TestTransitionToAvoidingBorderInFrontTurning(const uint64_t duration)
{
  return millis() - g_timer_start >= duration;
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
