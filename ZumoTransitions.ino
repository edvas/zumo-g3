
// Single variable used to store the starting time for the transitions that are time based
static uint64_t g_timer_start = 0uL;

inline bool CanTransitionToStarting()
{
  if (((PINB >> 4) & 0b1) == 0b0)
  {
    g_timer_start = micros();
    return true;
  }
  else
  {
    return false;
  }
}

inline bool CanTransitionToSeekingForward(const uint64_t duration)
{
  if(micros() - g_timer_start >= duration)
  {
    SetDriveDirectionToForward();
    SetDriveSpeed(kForwardSpeed);
    return true;
  }
  else
  {
    return false; 
  }
}

inline bool CanTransitionToSeekingSideLeft()
{
  if(GetTargetSideLeft())
  {
    SetDriveDirectionToForward();
    SetDriveSpeedLeftBelt(kFastTurnSpeedSlowTrack);
    SetDriveSpeedRightBelt(kFastTurnSpeedFastTrack);
    return true;
  }
  else
  {
    return false;
  }
}

inline bool CanTransitionToSeekingSideRight()
{
  if(GetTargetSideRight())
  {
    SetDriveDirectionToForward();
    SetDriveSpeedLeftBelt(kFastTurnSpeedFastTrack);
    SetDriveSpeedRightBelt(kFastTurnSpeedSlowTrack);
    return true;
  }
  else
  {
    return false;
  }
}

inline bool CanTransitionToSeekingCenterLeft()
{
  if(GetTargetCenterLeft())
  {
    SetDriveDirectionToForward();
    SetDriveSpeedLeftBelt(kSlowTurnSpeedSlowTrack);
    SetDriveSpeedRightBelt(kSlowTurnSpeedFastTrack);
    return true;
  }
  else
  {
    return false;
  }
}

inline bool CanTransitionToSeekingCenterRight()
{
  if(GetTargetCenterRight())
  {
    SetDriveDirectionToForward();
    SetDriveSpeedLeftBelt(kSlowTurnSpeedFastTrack);
    SetDriveSpeedRightBelt(kSlowTurnSpeedSlowTrack);
    return true;
  }
  else
  {
    return false;
  }
}

inline bool CanTransitionToAvoidingBorderOnLeftMovingBackwards(const uint8_t detected_borders)
{
  if(((detected_borders >> 0) & 0b1) == 0b1)
  {
    g_timer_start = micros();
    SetDriveDirectionToBackward();
    SetDriveSpeed(kBackwardSpeed);
    return true;
  }
  else
  {
    return false;
  }
}

inline bool CanTransitionToAvoidingBorderOnLeftTurning(const uint64_t duration)
{
  if(micros() - g_timer_start >= duration)
  {
    SetDriveDirectionToSpinRight();
    SetDriveSpeed(kSpinSpeedAvoidingSide);
    return true;
  }
  else
  {
    return false;
  }
}

inline bool CanTransitionToAvoidingBorderOnRightMovingBackwards(const uint8_t detected_borders)
{
  if(((detected_borders >> 1) & 0b1) == 0b1)
  {
    g_timer_start = micros();
    SetDriveDirectionToBackward();
    SetDriveSpeed(kBackwardSpeed);
    return true;
  }
  else
  {
    return false;
  }
}

inline bool CanTransitionToAvoidingBorderOnRightTurning(const uint64_t duration)
{
  if(micros() - g_timer_start >= duration)
  {
    SetDriveDirectionToSpinLeft();
    SetDriveSpeed(kSpinSpeedAvoidingSide);
    return true;
  }
  else
  {
    return false;
  }
}

inline bool CanTransitionToAvoidingBorderInFrontMovingBackwards(const uint8_t detected_borders)
{
  if(((detected_borders >> 0) & 0b11) == 0b11)
  {
    g_timer_start = micros();
    SetDriveDirectionToBackward();
    SetDriveSpeed(kBackwardSpeed);
    return true;
  }
  else
  {
    return false;
  }
}

inline bool CanTransitionToAvoidingBorderInFrontTurning(const uint64_t duration)
{
  if(micros() - g_timer_start >= duration)
  {
    SetDriveDirectionToSpinRight();
    SetDriveSpeed(kSpinSpeedAvoidingFront);
    return true;
  }
  else
  {
    return false;
  }
}

inline State CanTransitionToAvoidingBorder()
{
  const uint8_t detected_borders = GetDetectedBorders();
          
  // Event border detector on both sides
  if (CanTransitionToAvoidingBorderInFrontMovingBackwards(detected_borders))
  {
    return State::kAvoidingBorderInFrontMovingBackwards;
  }
  
  // Event border detector on right side
  if (CanTransitionToAvoidingBorderOnRightMovingBackwards(detected_borders))
  {
    return State::kAvoidingBorderOnRightMovingBackwards;
  }

  // Event border detector on left side
  if (CanTransitionToAvoidingBorderOnLeftMovingBackwards(detected_borders))
  {
    return State::kAvoidingBorderOnLeftMovingBackwards;
  }
          
  return State::kNone;
}
