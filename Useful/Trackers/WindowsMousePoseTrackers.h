/*********************************************************************************/
/*                                                                               */
/*                            WindowsMousePoseTrackers.h                         */
/*                                                                               */
/*********************************************************************************/

// Use the mouse to drive a pose in various ways.

#pragma once

#include "../Trackers/PoseTrackers.h"
#include "../Useful/Timers.h"

namespace PsyPhy {

// A base class on which to build all sorts of mouse-based trackers.
class WindowsMousePoseTracker : public PoseTracker {

	private:

	protected:
		Timer timer;
		OpenGLWindow *window;
		double gain;

	public:

		WindowsMousePoseTracker( OpenGLWindow *ptr = nullptr, double gain = 1.0 );
		~WindowsMousePoseTracker() {}

		virtual bool GetCurrentPoseIntrinsic( TrackerPose *pose ) { return true; }
		virtual double GetTime( void );

};

// A mouse tracker that controls roll movements only.
// Moving the mouse toward the right or upwards will cause a counter-clockwise rotation around k.
// Moving along the diagonal will change the orientation most rapidly.
// You can change the gain and change the direction by setting the gain to be negative.
class WindowsMouseRollPoseTracker : public WindowsMousePoseTracker {
public:
		Vector3 eulerAngles;
		WindowsMouseRollPoseTracker( OpenGLWindow *ptr = nullptr, double gain = 1.0 );
		~WindowsMouseRollPoseTracker();
		bool Update( void );
		bool GetCurrentPoseIntrinsic( TrackerPose &pose );
};

// A mouse tracker that controls roll and pitch with arrow keys.
class WindowsArrowPoseTracker : public WindowsMousePoseTracker {
public:
		Vector3 eulerAngles;
		WindowsArrowPoseTracker( OpenGLWindow *ptr = nullptr, double gain = 1.0 );
		~WindowsArrowPoseTracker();
		bool Update( void );
		bool GetCurrentPoseIntrinsic( TrackerPose &pose );
};
}
