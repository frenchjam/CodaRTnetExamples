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


class CodaLegacyPolledTracker : public Tracker {

protected:
		
	Timer	acquisitionTimer;
	bool	acquiring;
	bool	overrun;

	// Real-time marker data
	// Defines storage for one Coda frame.
	CODA_FRAME_DATA_STRUCT coda_data_frame;
	codaBYTE bInView[ CODA_MAX_MARKERS ];
	codaFLOAT fPosition[ CODA_MAX_MARKERS * 3 ];

public:


	CodaLegacyPolledTracker( void ) : acquiring(false) {}
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

	virtual void GetUnitTransform( int unit, Vector3 &offset, Matrix3x3 &rotation );
	virtual void SetUnitTransform( int unit, Vector3 &offset, Matrix3x3 &rotation );

};

};
