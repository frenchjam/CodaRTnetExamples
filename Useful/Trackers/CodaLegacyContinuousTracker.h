/*********************************************************************************/
/*                                                                               */
/*                           CodaLegacyContinuousTracker.h                        */
/*                                                                               */
/*********************************************************************************/

///
/// A tracker that uses the legacy CODA SDK to perform unbuffered acquisition.
///
#pragma once

// Disable warnings about "unsafe" functions.
#define _CRT_SECURE_NO_WARNINGS

#include "../Useful/fOutputDebugString.h"
#include "../Useful/Timers.h"
#include "../Include/codasys.h"
#include "../CodaLegacySDK/CodaUtilities.h"
#include "Trackers.h"

#define C
namespace PsyPhy {


class CodaLegacyContinuousTracker : public Tracker {

protected:
		
	Timer	acquisitionTimer;
	bool	acquiring;
	bool	overrun;

	int nAvailableMarkers;

	// Real-time marker data
	// Defines storage for one Coda frame.
	CODA_FRAME_DATA_STRUCT coda_data_frame;
	codaBYTE bInView[ CODA_MAX_MARKERS ];
	codaFLOAT fPosition[ CODA_MAX_MARKERS * 3 ];

	// Recorded marker data
	// Defines storage for data stored on the CODA server between 
	//  two calls to empty the buffers.
	CODA_ACQ_DATA_MULTI_STRUCT coda_multi_acq_frame;
	codaBYTE bInViewMulti[ CODA_MAX_MARKERS * CODA_BUFFER_FRAMES];
	codaFLOAT fPositionMulti[ CODA_MAX_MARKERS * CODA_BUFFER_FRAMES * 3 ];

public:


	CodaLegacyContinuousTracker( void ) : acquiring(false), nAvailableMarkers(0) {}
	virtual void Initialize( const char *ini_filename = "CodaSDK.ini" );
	virtual void StartAcquisition( float duration );
	virtual bool GetAcquisitionState();
	virtual bool CheckAcquisitionOverrun( void );
	virtual bool GetCurrentMarkerFrameUnit( MarkerFrame &frame, int selected_unit );
	virtual void StopAcquisition( void );
	virtual int  Update( void );
	virtual void Quit( void );

	virtual void StartContinuousAcquisition( void );
	virtual void StopContinuousAcquisition( void );

};

};
