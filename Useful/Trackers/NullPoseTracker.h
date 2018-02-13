/*********************************************************************************/
/*                                                                               */
/*                                  PoseTrackers.h                               */
/*                                                                               */
/*********************************************************************************/

// A 'PoseTracker' is a device that tracks the 6D position and orientation of a object.
// For the moment it is used only for polled real-time tracking.
// They could later be augmented to include continuous recording at a fixed frequency.

#pragma once

#include "../VectorsMixin/VectorsMixin.h"
#include "../Useful/Timers.h"
#include "PoseTrackers.h"


namespace PsyPhy {

class NullPoseTracker : public PoseTracker {

private:
	Timer	timer;
public:
	bool Initialize( void );
	bool GetCurrentPoseIntrinsic( TrackerPose &pose );
};

};

