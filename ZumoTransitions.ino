
// Single variable used to store the starting time for the transitions that are time based
static uint64_t g_transition_timer_start = 0uL;

inline bool TryTransitionToStarting()
{
  if (GetStartButtonIsPressed())
  {
    g_transition_timer_start = micros();
    return true;
  }
  else
  {
    return false;
  }
}

inline bool TryTransitionToMovingBlind(const uint64_t par_duration)
{
  if(micros() - g_transition_timer_start >= par_duration)
  {
    g_transition_timer_start = micros();
    SetDriveDirectionToForward();
    SetDriveSpeed(kForwardSpeed);
    return true;
  }
  else
  {
    return false; 
  }
}

inline bool TryTransitionToSeekingForward(const uint64_t par_duration)
{
  if(micros() - g_transition_timer_start >= par_duration)
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

inline bool TryTransitionToSeekingSideLeft()
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

inline bool TryTransitionToSeekingSideRight()
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

inline bool TryTransitionToSeekingCenterLeft()
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

inline bool TryTransitionToSeekingCenterRight()
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

inline bool TryTransitionToAvoidingBorderOnLeftMovingBackwards(const uint8_t par_detected_borders)
{
  if(((par_detected_borders >> 0) & 0b1) == 0b1)
  {
    g_transition_timer_start = micros();
    SetDriveDirectionToBackward();
    SetDriveSpeed(kBackwardSpeed);
    return true;
  }
  else
  {
    return false;
  }
}

inline bool TryTransitionToAvoidingBorderOnLeftTurning(const uint64_t par_duration)
{
  if(micros() - g_transition_timer_start >= par_duration)
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

inline bool TryTransitionToAvoidingBorderOnRightMovingBackwards(const uint8_t par_detected_borders)
{
  if(((par_detected_borders >> 1) & 0b1) == 0b1)
  {
    g_transition_timer_start = micros();
    SetDriveDirectionToBackward();
    SetDriveSpeed(kBackwardSpeed);
    return true;
  }
  else
  {
    return false;
  }
}

inline bool TryTransitionToAvoidingBorderOnRightTurning(const uint64_t par_duration)
{
  if(micros() - g_transition_timer_start >= par_duration)
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

inline bool TryTransitionToAvoidingBorderInFrontMovingBackwards(const uint8_t par_detected_borders)
{
  if(((par_detected_borders >> 0) & 0b11) == 0b11)
  {
    g_transition_timer_start = micros();
    SetDriveDirectionToBackward();
    SetDriveSpeed(kBackwardSpeed);
    return true;
  }
  else
  {
    return false;
  }
}

inline bool TryTransitionToAvoidingBorderInFrontTurning(const uint64_t par_duration)
{
  if(micros() - g_transition_timer_start >= par_duration)
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

inline State TryTransitionToAvoidingBorder()
{
  const auto detected_borders = GetDetectedBorders();
          
  // Event border detector on both sides
  if (TryTransitionToAvoidingBorderInFrontMovingBackwards(detected_borders))
  {
    return State::kAvoidingBorderInFrontMovingBackwards;
  }
  
  // Event border detector on right side
  if (TryTransitionToAvoidingBorderOnRightMovingBackwards(detected_borders))
  {
    return State::kAvoidingBorderOnRightMovingBackwards;
  }

  // Event border detector on left side
  if (TryTransitionToAvoidingBorderOnLeftMovingBackwards(detected_borders))
  {
    return State::kAvoidingBorderOnLeftMovingBackwards;
  }
          
  return State::kNone;
}
