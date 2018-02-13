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

namespace PsyPhy {

typedef struct {
	Pose		pose;
	bool		visible;
	double		time;
} TrackerPose;

extern TrackerPose NullTrackerPose;
void CopyTrackerPose( TrackerPose &destination, TrackerPose &source ) ;

class PoseTracker : public PsyPhy::VectorsMixin {

	private:

	protected:

		Pose		poseOffset;
		bool		initialized;

	public:

		PoseTracker() : initialized(false) {
			CopyQuaternion( poseOffset.orientation, nullQuaternion );
			CopyVector( poseOffset.position, zeroVector );
		}
		~PoseTracker (){};

		virtual bool Initialize( void ){ 
			initialized = true;
			return true; 
		};			
		virtual bool  Update( void ){ return true; };
		virtual bool Release( void ){ 
			initialized = false;
			return true; 
		};

		// Transform such that the null pose in intrinsic coordinates gives the specified pose.
		virtual void OffsetTo( const Pose &pose );
		// Boresight so that the specified pose in intrinsic coordinates becomes the null pose.
		virtual void BoresightAt( const Pose &pose );
		// Boresight so that the current pose becomes the specified pose.
		virtual bool BoresightTo( const Pose &pose );
		// Boresight so that the current pose becomes the null pose.
		virtual bool Boresight( int retry_count = 5 );
		virtual void Unboresight( void );
		 
		virtual bool GetCurrentPosition( PsyPhy::Vector3 position );
		virtual bool GetCurrentOrientation( PsyPhy::Quaternion orientation );
		virtual bool GetCurrentPose( TrackerPose &pose );
		virtual bool GetCurrentPoseIntrinsic( TrackerPose &pose );

};


};

