/*********************************************************************************/
/*                                                                               */
/*                            OclulusRemotePoseTracker.cpp                       */
/*                                                                               */
/*********************************************************************************/

// A pose tracker driven by the Oculus remote control.

#include "stdafx.h"

// The following includes comes from the Oculus OVR source files.
// The path is set via the user macro $(OVRSDKROOT) and via the property pages
//  in the VS2010 project files. I was able to modify $(OVRSDKROOT) by editing 
//  OVRRootPath.props. I could not figure out how to do it within VS2010.
#include "GL/CAPI_GLE.h"
#include "Extras/OVR_Math.h"
#include "OVR_CAPI_GL.h"
#include <assert.h>

#include "OculusRemotePoseTracker.h"
#include "../Useful/Useful.h"
#include "../Useful/fOutputDebugString.h"
#include "../Useful/fMessageBox.h"



using namespace PsyPhy;

OculusRemotePoseTracker::OculusRemotePoseTracker( double gain, ovrSession session ) {
	this->session = session;
	this->gain = gain;
	PoseTracker::PoseTracker();
}

bool OculusRemotePoseTracker::Initialize( void ) { 

	// Only initialize once.
	if ( initialized ) return true;
	initialized = true;

	ovrResult			result;
	ovrGraphicsLuid		luid;

	if ( session == nullptr ) {
		result = ovr_Initialize( nullptr );
		fAbortMessageOnCondition( OVR_FAILURE( result ), "OculusRemotePoseTracker", "Failed to initialize libOVR." );
		result = ovr_Create( &session, &luid );
		fAbortMessageOnCondition( !OVR_SUCCESS( result), "OculusRemotePoseTracker", "Failed to create OVR session." );
		fprintf( stderr, "Oculus OVR intialized.\n" );
	}

	TimerStart( timer );
	CopyVector( eulerAngles, zeroVector );
	CopyPose( current_pose.pose, nullPose );
	current_pose.time = 0.0;
	current_pose.visible = false;
	return( initialized );
}

bool OculusRemotePoseTracker::Update( void ) { 
			
	// Get the current state of the Oculus Remote buttons.
	ovrInputState state;
	ovr_GetInputState(	session,  ovrControllerType_Remote, &state );

	if ( state.Buttons & ovrButton_Left ) eulerAngles[ROLL] -= gain;
	else if ( state.Buttons & ovrButton_Right ) eulerAngles[ROLL] += gain;

	return( true );
}

bool OculusRemotePoseTracker::Release( void ) { 
	return( true );
}

bool OculusRemotePoseTracker::GetCurrentPoseIntrinsic( PsyPhy::TrackerPose &pose ) { 
	CopyVector( pose.pose.position, zeroVector );
	Quaternion rollQ, pitchQ, yawQ;
	Quaternion intermediateQ;

	SetQuaternion( rollQ, eulerAngles[ROLL], kVector );
	SetQuaternion( pitchQ, eulerAngles[PITCH], iVector );
	MultiplyQuaternions( intermediateQ, pitchQ, rollQ );
	SetQuaternion( yawQ, eulerAngles[YAW], jVector );
	MultiplyQuaternions( pose.pose.orientation, yawQ, intermediateQ );
	pose.time = TimerElapsedTime( timer );
	pose.visible = true;
	return pose.visible;
}


