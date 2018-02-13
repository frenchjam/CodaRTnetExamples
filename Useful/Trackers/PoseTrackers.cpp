/*********************************************************************************/
/*                                                                               */
/*                                PoseTrackers.cpp                               */
/*                                                                               */
/*********************************************************************************/

// A 'PoseTracker' is a device that tracks the 6D position and orientation of a object.
// For the moment it is used only for polled real-time tracking.
// They could later be augmented to include continuous recording at a fixed frequency.

#pragma once

#include "stdafx.h"
#include "PoseTrackers.h"
#include "../Useful/Useful.h"
#include "../Useful/fMessageBox.h"
#include "../Useful/fOutputDebugString.h"

using namespace PsyPhy;

TrackerPose PsyPhy::NullTrackerPose = {{{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 1.0}}, true, 0.0};

// A global function to facilitate copying tracker poses even if you are not a tracker.

void PsyPhy::CopyTrackerPose( TrackerPose &destination, TrackerPose &source ) {
	static VectorsMixin vm;
	destination.time = source.time;
	destination.visible = source.visible;
	vm.CopyVector( destination.pose.position, source.pose.position );
	vm.CopyQuaternion( destination.pose.orientation, source.pose.orientation );
};

// Transform such that the null pose in intrinsic coordinates gives the specified pose.
void PoseTracker::OffsetTo( const Pose &pose ) {
	CopyPose( poseOffset, pose );
}

// Boresight so that the specified pose in intrinsic coordinates becomes the null pose.
void PoseTracker::BoresightAt( const Pose &pose ) {
	CopyPose( poseOffset, pose );
	poseOffset.orientation[X] *= -1.0;
	poseOffset.orientation[Y] *= -1.0;
	poseOffset.orientation[Z] *= -1.0;
	ScaleVector( poseOffset.position, poseOffset.position, -1.0 );
}

// Boresight so that the current position and orientation is the null position and orientation.
bool PoseTracker::Boresight( int retry_count ) {
	TrackerPose tpose;
	bool success;
	for ( int i = 0; i < retry_count; i++ ) {
		success = GetCurrentPoseIntrinsic( tpose );
		if ( success ) {
			BoresightAt( tpose.pose );
			return( success );
		}
		// If we did not get a valid position, sleep a little and try again.
		Sleep( 20 );
		// Need to allow an update before trying to get the latest position.
		// Not sure how to handle errors in Update() so just abort for now if it happens.
		fAbortMessageOnCondition( !Update(), "PoseTrackers", "Error executing Update() on retry count %d", i );
	}
	fOutputDebugString( "Boresight() failed after %d retry attempts.\n", retry_count );
	return( success );
}

// Boresight so that the current pose is now the specified pose.
bool PoseTracker::BoresightTo( const Pose &pose ) {
	bool success;
	if ( success = Boresight() ) {
		// Combine the current offset that brings us to zero position
		// and orientation with the specified position and orientation
		// such that new readings at this position will correspond to 
		// the specified pose.
		Pose new_offset;
		MultiplyQuaternions( new_offset.orientation, poseOffset.orientation, pose.orientation );
		AddVectors( new_offset.position, poseOffset.position, pose.position );
		CopyPose( poseOffset, new_offset );
	}
	return success;
}

// Remove boresight correction and return to intinsic reference frame.
void PoseTracker::Unboresight( void ) {
	CopyVector( poseOffset.position, zeroVector );
	CopyQuaternion( poseOffset.orientation, nullQuaternion );
}

bool PoseTracker::GetCurrentPosition( Vector3 position ) {
	TrackerPose pose;
	GetCurrentPose( pose );
	CopyVector( position, pose.pose.position );
	return( pose.visible );
}
bool PoseTracker::GetCurrentOrientation( Quaternion orientation ) {
	TrackerPose tpose;
	GetCurrentPose( tpose );
	CopyVector( orientation, tpose.pose.orientation );
	return( tpose.visible );
}
bool PoseTracker::GetCurrentPose( TrackerPose &pose ) {
	TrackerPose intrinsic_pose;
	GetCurrentPoseIntrinsic( intrinsic_pose );
	MultiplyQuaternions( pose.pose.orientation,  poseOffset.orientation, intrinsic_pose.pose.orientation );
	AddVectors( pose.pose.position, intrinsic_pose.pose.position, poseOffset.position );
	pose.visible = intrinsic_pose.visible;
	pose.time = intrinsic_pose.time;
	return( pose.visible );
}

// If GetCurrentPoseIntrinsic() does not get redefined by an inheriting class, just return a nonvisible pose.
bool PoseTracker::GetCurrentPoseIntrinsic( TrackerPose &pose ) {
	pose.visible = false;
	return( pose.visible );
}

