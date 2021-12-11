#pragma GCC optimize ("Os")

//#define Z3_DEBUG
//#define Z3_DEBUG_STATE

// Variables for speed
const uint8_t kForwardSpeed = 255;
const uint8_t kBackwardSpeed = 255;
const uint8_t kSpinSpeedAvoidingSide = 255;
const uint8_t kSpinSpeedAvoidingFront = 255;
const uint8_t kFastTurnSpeedFastTrack = 200;
const uint8_t kFastTurnSpeedSlowTrack = 63;
const uint8_t kSlowTurnSpeedFastTrack = 255;
const uint8_t kSlowTurnSpeedSlowTrack = 127;

// Variables for duration
const uint64_t kMilliseconds = 1000; // Unit: ms
const uint64_t kStartDelayDuration = 5000 * kMilliseconds; // Unit: ms * ms = us
const uint64_t kNoUpdateStartDuration = 1000 * kMilliseconds; // Unit: ms * ms = us
const uint64_t kAvoidingSideBorderBackingDuration = 200 * kMilliseconds; // Unit: ms * ms = us
const uint64_t kAvoidingFrontBorderBackingDuration = 200 * kMilliseconds; // Unit: ms * ms = us
const uint64_t kAvoidingBorderTurnDuration = 400 * kMilliseconds; // Unit: ms * ms = us

// Variables for sensors
const bool kUseWhiteAsBorderColor = true;
const uint8_t kRangeCenterInverse = 65;
const uint8_t kRangeSide = 50; // Unit: cm

constexpr float kUltrasonicRangeSearchMultiplier = 1.5f; // Ex. value of 1.5f equals +50% search range
constexpr float kSpeedOfSound = 0.034f; // Unit: cm/us
constexpr float kUltrasonicRoundTripDurationCorrection = 2.0f; // Unit: none, math constant
constexpr uint64_t kUltrasonicTimeToDistanceDivisor = kUltrasonicRoundTripDurationCorrection / kSpeedOfSound; // Unit: us/cm
constexpr uint64_t kUltrasonicTimeout = kUltrasonicRangeSearchMultiplier * kRangeSide * kUltrasonicTimeToDistanceDivisor; // Unit: us

// States for the state machine implementation
enum class State
{
  kNone,
  
  kIdle,
  kStarting,
  kMovingBlind,

  kSeeking,
  
  kAvoidingBorderOnLeftMovingBackwards,
  kAvoidingBorderOnRightMovingBackwards,
  kAvoidingBorderInFrontMovingBackwards,
  kAvoidingBorderTurning,
};


int main()
{
  #ifdef Z3_DEBUG
    // NOTE: Using baudrates other than 9600 may need initialization of the USB controller.
    //       Replaceing 'main()' with 'setup()' and 'loop()' does this in the background
    Serial.begin(9600);
  #endif

  Setup();
  
  State state = State::kIdle;

  // Loop
  for(;;)
  {
    #ifdef Z3_DEBUG_STATE
      Serial.print("State: ");
      Serial.println(static_cast<int>(state));
    #endif
    
    ReadAnalogInputs();

    ReadUltrasonicDistanceInputs();

    // State machine
    switch(state)
    {
      case State::kIdle:
        // Event start button pressed
        if (TryTransitionToStarting())
        {
          state = State::kStarting;
        }
        break;
        
      case State::kStarting:
        // Event timeout
        if (TryTransitionToMovingBlind(kStartDelayDuration))
        {
          state = State::kMovingBlind;
        }

        break;

      case State::kMovingBlind:
        // Event timeout
        if (TryTransitionToSeekingForward(kNoUpdateStartDuration))
        {
          state = State::kSeeking;
        }
        break;
      
      case State::kSeeking:
        // Event border detector on both sides
        const auto return_state = TryTransitionToAvoidingBorder();
        if (return_state != State::kNone)
        {
          state = return_state;
          break;
        }

        // Event target detector on left side active
        if (TryTransitionToSeekingSideLeft())
        {
          state = State::kSeeking;
          break;
        }

        // Event target detector on right side active
        if (TryTransitionToSeekingSideRight())
        {
          state = State::kSeeking;
          break;
        }
        
        // Event target detector on left center active
        if (TryTransitionToSeekingCenterLeft())
        {
          state = State::kSeeking;
          break;
        }

        // Event target detector on right center active
        if (TryTransitionToSeekingCenterRight())
        {
          state = State::kSeeking;
          break;
        }
        break;

      case State::kAvoidingBorderOnLeftMovingBackwards:
        // Event timeout
        if (TryTransitionToAvoidingBorderOnLeftTurning(kAvoidingSideBorderBackingDuration))
        {
          state = State::kAvoidingBorderTurning;
        }
        break;
      
      case State::kAvoidingBorderOnRightMovingBackwards:
        // Event timeout
        if (TryTransitionToAvoidingBorderOnRightTurning(kAvoidingSideBorderBackingDuration))
        {
          state = State::kAvoidingBorderTurning;
        }
        break;

      case State::kAvoidingBorderInFrontMovingBackwards:
        // Event timeout
        if (TryTransitionToAvoidingBorderInFrontTurning(kAvoidingFrontBorderBackingDuration))
        {
          state = State::kAvoidingBorderTurning;
        }
        break;

      case State::kAvoidingBorderTurning:
        // Event timeout
        if (TryTransitionToSeekingForward(kAvoidingBorderTurnDuration))
        {
          state = State::kSeeking;
        }
        break;
        
      default:
        break;
    };
  }
}
