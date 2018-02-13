/*********************************************************************************/
/*                                                                               */
/*                                PoseTrackerFilter.cpp                          */
/*                                                                               */
/*********************************************************************************/

// A pose tracker simply filters the output of another one.

#include "stdafx.h"

#include "PoseTrackerFilter.h"
#include "../Useful/Useful.h"
#include "../Useful/fOutputDebugString.h"
#include "../Useful/fMessageBox.h"

using namespace PsyPhy;

PoseTrackerFilter::PoseTrackerFilter( PoseTracker *tracker, double filter_constant ) {
	filterConstant = filter_constant;
	unfiltered_tracker = tracker;
	PoseTracker::PoseTracker();
}

bool PoseTrackerFilter::Initialize( void ) { 
	// Only initialize once.
	if ( initialized ) return true;
	// Initialize the components one-by-one. 
	// Overall success requires that all be successful.
	initialized = true;
	unfiltered_tracker->Initialize();
	CopyPose( current_pose.pose, nullPose );
	current_pose.time = 0.0;
	current_pose.visible = false;
	return( initialized );
}

bool PoseTrackerFilter::Update( void ) { 
	// Here we just update each of the individual trackers.
	// What we should do is perhaps align each tracker to the previous one
	//  at each time step, whenever both members of the pair are available.
	bool success = unfiltered_tracker->Update();
	return( success );
}

bool PoseTrackerFilter::Release( void ) { 
	return( unfiltered_tracker->Release() );
}

bool PoseTrackerFilter::GetCurrentPoseIntrinsic( PsyPhy::TrackerPose &pose ) { 
	TrackerPose component_pose;
	unfiltered_tracker->GetCurrentPose( component_pose );
	if ( component_pose.visible ) {

		ScaleVector( current_pose.pose.position, current_pose.pose.position, filterConstant );
		AddVectors( current_pose.pose.position, current_pose.pose.position, component_pose.pose.position );
		ScaleVector( current_pose.pose.position, current_pose.pose.position, ( 1.0 / ( 1.0 + filterConstant ) ) );

		for ( int i = 0; i < 4; i++ ) {
			current_pose.pose.orientation[i] = current_pose.pose.orientation[i] * filterConstant + component_pose.pose.orientation[i];
		}
		NormalizeQuaternion( current_pose.pose.orientation );
		CopyPose( pose.pose, current_pose.pose );
		pose.time = component_pose.time;
		pose.visible = component_pose.visible;
	}
	else pose.visible = false;
	return pose.visible; 
}


