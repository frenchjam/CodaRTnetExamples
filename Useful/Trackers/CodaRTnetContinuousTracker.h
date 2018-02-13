/*********************************************************************************/
/*                                                                               */
/*                           CodaRTnetContinuousTracker.h                        */
/*                                                                               */
/*********************************************************************************/

///
/// Another version of the CodaRTnetTracker that uses unbuffered acquisition.
///
#pragma once

// Disable warnings about "unsafe" functions.
#define _CRT_SECURE_NO_WARNINGS

#include "../Useful/fOutputDebugString.h"
#include "../Useful/Timers.h"
#include "CodaRTnetTracker.h"

// The CODA RTnet server has a bug such that it eats up memory in continuous mode.
// Here we set a maximum duration for continuous acquisitions. If the acquisition
// goes on longer, the continuous acquisition is stopped and restarted. 
#define MAX_CONTINUOUS_DURATION 600.0

namespace PsyPhy {


class CodaRTnetContinuousTracker : public CodaRTnetTracker {

protected:
		
	// The CODA continuously sends separate packets for each CODA unit. It is conceivable that
	// the two packets from a given time slice could be read on separate calls to Update().
	// So we keep track of the number of acquired frames for each CODA unit separately.
	// The common parameter nFrames is nevertheless maintained and set to the smaller of the
	// individual frame counters.
	unsigned int nFramesPerUnit[MAX_UNITS];

	Timer	acquisitionTimer;
	bool	acquiring;
	Timer	runningTimer;
	double	maxContinuous;

public:

	CodaRTnetContinuousTracker( void ) : acquiring(false), maxContinuous( MAX_CONTINUOUS_DURATION ) {}
	virtual void Initialize( const char *ini_filename = "CodaRTnet.ini" );
	virtual void StartAcquisition( float duration );
	virtual bool GetAcquisitionState();
	virtual bool GetCurrentMarkerFrameUnit( MarkerFrame &frame, int selected_unit );
	virtual void StopAcquisition( void );
	virtual int  Update( void );
	virtual void  Quit( void );

	virtual void StartContinuousAcquisition( void );
	virtual void RestartContinuousAcquisition( void );
	virtual void StopContinuousAcquisition( void );

};

};
