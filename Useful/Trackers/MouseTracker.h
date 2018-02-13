/*********************************************************************************/
/*                                                                               */
/*                                  MouseTracker.h                               */
/*                                                                               */
/*********************************************************************************/

// A tracker to follow mouse movements. This is primarily used for simulating
//  a real tracker during testing.

#pragma once

#include "Trackers.h"

class MouseTracker : public Tracker {

private:

	bool		acquisitionOn;
	bool		overrun;
	Timer		acquisitionTimer;
	double		duration;
	int			nPolled;

	HWND		dlg;

	FILE		*fp;

	MarkerFrame	polledMarkerFrames[MAX_FRAMES];

	
protected:

public:

	MouseTracker( HWND dlg = NULL ) : acquisitionOn(false), overrun(false) {
		this->dlg = dlg;
	}

	void Initialize( void );
	void Quit( void );

	int  Update( void );
	void StartAcquisition( float max_duration );
	void StopAcquisition( void );
	bool GetAcquisitionState( void );
	bool CheckOverrun( void );

	int	 RetrieveMarkerFrames( MarkerFrame frames[], int max_frames );
	int	 RetrieveMarkerFramesUnit( MarkerFrame frames[], int max_frames, int unit );
	bool GetCurrentMarkerFrame( MarkerFrame &frame );
	bool GetCurrentMarkerFrameUnit( MarkerFrame &frame, int unit );

};

