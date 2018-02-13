/*********************************************************************************/
/*                                                                               */
/*                            WindowsMousePoseTrackers.cpp                       */
/*                                                                               */
/*********************************************************************************/

#include "stdafx.h"

#include "../Useful/Useful.h"
#include "../Useful/fMessageBox.h"
#include "../Useful/fOutputDebugString.h"

#include "../OpenGLObjects/OpenGLWindows.h"
#include "WindowsMousePoseTrackers.h"

#include <math.h>

using namespace PsyPhy;

WindowsMousePoseTracker::WindowsMousePoseTracker( OpenGLWindow *ptr, double gain ) {
	window = ptr;
	this->gain = gain;
	TimerStart( timer );
	PoseTracker();
}

// Define a common method to determine the timestamp for all MousePoseTrackers.
double WindowsMousePoseTracker::GetTime( void ) { 
	// Time is the elapsed time since this tracker instance was initialized.
	return( TimerElapsedTime( timer ) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

WindowsMouseRollPoseTracker::WindowsMouseRollPoseTracker( OpenGLWindow *window, double gain ) {
	this->window = window;
	this->gain = gain;
	CopyVector( eulerAngles, zeroVector );
	WindowsMousePoseTracker::WindowsMousePoseTracker( window, gain );
}

bool WindowsMouseRollPoseTracker::Update ( void ) {
	// Here we check the state of the key and move one gain step per cycle.
	// If framerate is constant, velocity should be constant.
	// An alternative approach would be to move according to the number of key down
	// events since the last cycle. That would allow for finer control and benefit from the
	// accelerating nature of events when you hold down a key.
	if ( window->Key[VK_SHIFT] ) {
		if ( window->Key[VK_LEFT] ) eulerAngles[YAW] -= gain;
		if ( window->Key[VK_RIGHT] ) eulerAngles[YAW] += gain;
		if ( window->Key[VK_UP] ) eulerAngles[PITCH] -= gain;
		if ( window->Key[VK_DOWN] ) eulerAngles[PITCH] += gain;
	}
	return true;
}

bool WindowsMouseRollPoseTracker::GetCurrentPoseIntrinsic( TrackerPose &pose ) {

	// Place the position of this simulated tracker.
	CopyVector( pose.pose.position, zeroVector );
	// When using this tracker in simulation, it is useful to have the ability 
	// to raise and lower the hand.
	static double simulated_hand_height;
	if ( window->Key['W'] ) simulated_hand_height = 0.0;
	if ( window->Key['X'] ) simulated_hand_height = -250.0;
	pose.pose.position[Y] = simulated_hand_height;

	// Compute the orientation output based on the mouse position and the results
	// of the various key presses handled in the Update() method.
	Quaternion rollQ, pitchQ, yawQ;
	Quaternion intermediateQ;
	// A feature of the Oculus window on the laptop screen is that you can move the mouse infinitely in any direction.
	// When the cursor gets to the edge of the screen, it wraps around to the other side.
	// Here we use this feature to allow the roll angle to move without a limit.
	SetQuaternion( rollQ, ( window->mouseCumulativeX - window->mouseCumulativeY ) * gain, kVector );
	// Add in the pitch and yaw components, computed from eulerAngles[] that were modulated with key presses in Update().
	SetQuaternion( pitchQ, eulerAngles[PITCH], iVector );
	MultiplyQuaternions( intermediateQ, pitchQ, rollQ );
	SetQuaternion( yawQ, eulerAngles[YAW], jVector );
	MultiplyQuaternions( pose.pose.orientation, yawQ, intermediateQ );
	pose.time = GetTime();
	pose.visible = true;
	return pose.visible;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

WindowsArrowPoseTracker::WindowsArrowPoseTracker( OpenGLWindow *window, double gain ) {
	this->window = window;
	this->gain = gain;
	CopyVector( eulerAngles, zeroVector );
	WindowsMousePoseTracker::WindowsMousePoseTracker( window, gain );
}

bool WindowsArrowPoseTracker::Update ( void ) {
	// Here we check the state of the key and move one gain step per cycle.
	// If framerate is constant, velocity should be constant.
	// An alternative approach would be to move according to the number of key down
	// events since the last cycle. That would allow for finer control and benefit from the
	// accelerating nature of events when you hold down a key.
	if ( !window->Key[VK_SHIFT] ) {
		if ( window->Key[VK_CONTROL] && window->Key[VK_LEFT] ) eulerAngles[YAW] -= gain;
		else if ( window->Key[VK_LEFT] ) eulerAngles[ROLL] -= gain;

		if ( window->Key[VK_CONTROL] && window->Key[VK_RIGHT] ) eulerAngles[YAW] += gain;
		else if ( window->Key[VK_RIGHT] ) eulerAngles[ROLL] += gain;

		if ( window->Key[VK_UP] ) eulerAngles[PITCH] -= gain;
		if ( window->Key[VK_DOWN] ) eulerAngles[PITCH] += gain;
	}
	return true;
}

bool WindowsArrowPoseTracker::GetCurrentPoseIntrinsic( TrackerPose &pose ) {

	// Place the position of this simulated tracker.
	CopyVector( pose.pose.position, zeroVector );

	// Compute the orientation output based on the mouse position and the results
	// of the various key presses handled in the Update() method.
	Quaternion rollQ, pitchQ, yawQ;
	Quaternion intermediateQ;
	// Chain roll, pitch and yaw components, computed from eulerAngles[] that were modulated with key presses in Update().
	SetQuaternion( rollQ, eulerAngles[ROLL], kVector );
	SetQuaternion( pitchQ, eulerAngles[PITCH], iVector );
	SetQuaternion( yawQ, eulerAngles[YAW], jVector );
	MultiplyQuaternions( intermediateQ, pitchQ, rollQ );
	MultiplyQuaternions( pose.pose.orientation, yawQ, intermediateQ );
	pose.time = GetTime();
	pose.visible = true;
	return pose.visible;
}
