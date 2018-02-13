/*********************************************************************************/
/*                                                                               */
/*                                NullPoseTracker.cpp                            */
/*                                                                               */
/*********************************************************************************/

// A 'PoseTracker' is a device that tracks the 6D position and orientation of a object.
// For the moment it is used only for polled real-time tracking.
// They could later be augmented to include continuous recording at a fixed frequency.

#include "stdafx.h"
#include "NullPoseTracker.h"
#include "../Useful/Useful.h"
#include "../Useful/fMessageBox.h"
#include "../Useful/fOutputDebugString.h"

using namespace PsyPhy;

bool NullPoseTracker::Initialize( void ) {
	TimerStart( timer );
	PoseTracker::Initialize();
	return true;
}

bool NullPoseTracker::GetCurrentPoseIntrinsic( TrackerPose &pose ) {
	CopyPose( pose.pose, nullPose );
	pose.visible = true;
	pose.time = TimerElapsedTime( timer );
	return( pose.visible );
}


