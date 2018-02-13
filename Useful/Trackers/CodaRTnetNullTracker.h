/*********************************************************************************/
/*                                                                               */
/*                              CodaRTnetNullTracker.h                           */
/*                                                                               */
/*********************************************************************************/

///
/// Fake a RTnet tracker.
///
#pragma once

// Disable warnings about "unsafe" functions.
#define _CRT_SECURE_NO_WARNINGS

#include "../Useful/fOutputDebugString.h"
#include "../Useful/Timers.h"
#include "CodaRTnetTracker.h"

namespace PsyPhy {


class CodaRTnetNullTracker : public CodaRTnetTracker {

protected:
		
	Timer	acquisitionTimer;
	bool	acquiring;
	void FakeMovementData( int unit, int index );

public:

	bool	fakeMovements;

	CodaRTnetNullTracker( void ) : acquiring(false), fakeMovements(false)  {}
	virtual void Initialize( const char *ini_filename = "CodaRTnet.ini" );
	virtual void StartContinuousAcquisition( void );
	virtual void StartAcquisition( double duration );
	virtual bool GetAcquisitionState();
	virtual bool GetCurrentMarkerFrameUnit( MarkerFrame &frame, int selected_unit );
	virtual void SetUnitTransform( int unit, Vector3 &offset, Matrix3x3 &rotation );
	virtual void GetUnitTransform( int unit, Vector3 &offset, Matrix3x3 &rotation );
	virtual void GetAlignmentTransforms( Vector3 offset[MAX_UNITS], Matrix3x3 rotation[MAX_UNITS] );
	virtual void SetAlignmentTransforms( Vector3 offset[MAX_UNITS], Matrix3x3 rotation[MAX_UNITS] );
	virtual void StopAcquisition( void );
	virtual void AbortAcquisition( void );
	virtual void Quit( void );
	virtual int  Update( void );

	virtual void RestartContinuousAcquisition( void );

};

};
