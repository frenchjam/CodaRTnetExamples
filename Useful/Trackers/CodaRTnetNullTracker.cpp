/***************************************************************************/
/*                                                                         */
/*                               CodaRTnetTracker                          */
/*                                                                         */
/***************************************************************************/

#include "stdafx.h"

// A tracker that uses the CODA RTnet interface.

#include <windows.h>
#include <mmsystem.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../Useful/fMessageBox.h"
#include "../Useful/fOutputDebugString.h"
#include "../VectorsMixin/VectorsMixin.h"
#include "../Useful/ini.h"

#include "CodaRTnetNullTracker.h"

using namespace PsyPhy;

void CodaRTnetNullTracker::Initialize( const char *ini_filename ) {
	acquiring = false;
	overrun = false;
	nUnits = 2;
	nMarkers = 24;
	nFrames = 0;
	// Fill the last frame with a record in which all the markers are invisible.
	// This will be sent as the current frame until such time that a real frame has been read.
	unsigned int index = nFrames % MAX_FRAMES;
	for ( int unit = 0; unit < nUnits; unit++ ) {
		recordedMarkerFrames[unit][index].time = 0.0;
		for ( int mrk = 0; mrk < nMarkers; mrk++ ) {
			recordedMarkerFrames[unit][index].marker[mrk].visibility = false;
			CopyVector( recordedMarkerFrames[unit][index].marker[mrk].position, zeroVector );
		}
	}
	Sleep( 3000 );
	TimerStart( acquisitionTimer );
}

void CodaRTnetNullTracker::StartContinuousAcquisition( void ) {}
void CodaRTnetNullTracker::RestartContinuousAcquisition( void ) {
	nFrames = 0;
	TimerStart( acquisitionTimer );
}

void CodaRTnetNullTracker::StartAcquisition( double duration ) {
	TimerSet( acquisitionTimer, duration );
	nFrames = 0;
	overrun = false;
	acquiring = true;
	StartContinuousAcquisition();
}

void CodaRTnetNullTracker::StopAcquisition( void ) {
	acquiring = false;
}
void CodaRTnetNullTracker::AbortAcquisition( void ) {
	acquiring = false;
	nFrames = 1;
}
void CodaRTnetNullTracker::Quit( void ) {}

bool CodaRTnetNullTracker::GetAcquisitionState( void ) {
	Update();
	return( acquiring );
}

int CodaRTnetNullTracker::Update( void ) {

	for ( int unit = 0; unit < nUnits; unit++ ) {
		int index = nFrames % MAX_FRAMES;
		recordedMarkerFrames[unit][index].time = TimerElapsedTime( acquisitionTimer );
		if ( fakeMovements ) FakeMovementData( unit, index );
		else {
			for ( int mrk = 0; mrk < nMarkers; mrk++ ) {
				recordedMarkerFrames[unit][index].marker[mrk].visibility = false;
				CopyVector( recordedMarkerFrames[unit][index].marker[mrk].position, zeroVector );
			}
		}
		// If we are acquiring a buffer full of data, then advance to the next frame,
		// taking care not to overrun the buffers.
	}
	if ( acquiring ) {
		if ( nFrames < MAX_FRAMES ) nFrames++;
		else fOutputDebugString( "MAX_FRAMES exceeded: %d \n", nFrames );
	}
	return( true );
}

bool CodaRTnetNullTracker::GetCurrentMarkerFrameUnit( MarkerFrame &frame, int selected_unit ) {
	// Make sure that any packets that were sent were read.
	Update();
	unsigned int index;
	// If we are acquiring a time series, nFramesPerUnit points is one more than the index of the last acquired sample.
	// So we back up one sample and send that frame.
	if ( acquiring ) index = ( nFrames - 1 ) % MAX_FRAMES;
	// If we are not acquiring a time series, the most recent data is copied into the location pointed to by nFramesPerUnit,
	//  which does not advance.
	else index = nFrames % MAX_FRAMES;
	CopyMarkerFrame( frame, recordedMarkerFrames[selected_unit][index] );
	return true;
}

// Create fake unit transforms. 
void CodaRTnetNullTracker::GetUnitTransform( int unit, Vector3 &offset, Matrix3x3 &rotation ) {
	offset[X] = 0.0;
	offset[Y] = 0.0;
	offset[Z] = -2000.0;
	SetRotationMatrix( rotation, 45.0 * (double)( unit + 1), 0.0, 30.0 );
}
void CodaRTnetNullTracker::SetUnitTransform( int unit, Vector3 &offset, Matrix3x3 &rotation ) {}

// Because CodaRTnetNullTracker is build on CodaRTnetTracker, we have to undo what CodaRTnetTracker does to these routines.
void CodaRTnetNullTracker::SetAlignmentTransforms( Vector3 offsets[MAX_UNITS], Matrix3x3 rotations[MAX_UNITS] ) {
	Tracker::SetAlignmentTransforms( offsets, rotations );
}
void CodaRTnetNullTracker::GetAlignmentTransforms( Vector3 offsets[MAX_UNITS], Matrix3x3 rotations[MAX_UNITS] ) {
	Tracker::GetAlignmentTransforms( offsets, rotations );
}

void CodaRTnetNullTracker::FakeMovementData( int unit, int index ) {

	static double angle = 0.0;
	int mrk, id;
	static int count = 0;

	for ( mrk = 0; mrk < 8; mrk++ ) {
		id = mrk;
		CopyVector( recordedMarkerFrames[unit][index].marker[id].position, zeroVector );
		recordedMarkerFrames[unit][index].marker[id].visibility = true;
	}
	for ( mrk = 8; mrk < 16; mrk++ ) {
		id = mrk;
		CopyVector( recordedMarkerFrames[unit][index].marker[id].position, zeroVector );
		recordedMarkerFrames[unit][index].marker[id].position[Z] -= 300.0;
		recordedMarkerFrames[unit][index].marker[id].position[Y] -= 250.0;
		recordedMarkerFrames[unit][index].marker[id].position[X] += 100.0 + 200.0 * sin( angle );
		recordedMarkerFrames[unit][index].marker[id].visibility = true;
	}
	for ( mrk = 16; mrk < 24; mrk++ ) {
		id = mrk;
		CopyVector( recordedMarkerFrames[unit][index].marker[id].position, zeroVector );
		recordedMarkerFrames[unit][index].marker[id].visibility = true;
	}
	// The following code causes intermittent occlusions of the simulated markers.
	for ( int unit = 0; unit < nUnits; unit++ ) {
		int mrk;
		for ( mrk = 0; mrk < MAX_MARKERS; mrk++ ) {
			if ( ( (mrk + count / 10 ) / 8 ) % 6 == unit ) recordedMarkerFrames[unit][index].marker[mrk].visibility = false;
			else recordedMarkerFrames[unit][index].marker[mrk].visibility = true;
		}
	}
	
	angle += 0.05;
	count++;
}