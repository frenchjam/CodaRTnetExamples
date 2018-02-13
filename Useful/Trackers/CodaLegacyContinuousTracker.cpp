/***************************************************************************/
/*                                                                         */
/*                         CodaRTnetContinuousTracker                      */
/*                                                                         */
/***************************************************************************/

#include "stdafx.h"

// A tracker that uses the CODA RTnet interface.
// In this version the RTnet server is set to send data continuously without
// any buffering on the server side. Capturing the data on the client side
// is handled by calling Update() frequently enough so as not to miss any packets.


#include <windows.h>
#include <mmsystem.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../Include/codasys.h"
#include "../CodaLegacySDK/CodaUtilities.h"

#include "../Useful/fMessageBox.h"
#include "../Useful/fOutputDebugString.h"
#include "../VectorsMixin/VectorsMixin.h"
#include "../Useful/ini.h"

#include "Trackers.h"
#include "CodaLegacyContinuousTracker.h"

using namespace PsyPhy;

void CodaLegacyContinuousTracker::Initialize( const char *ini_filename ) {

	// Initialize the CODA hardware.
	CodaConnectStartAndPrepare( NULL ); // Hard coded for the moment.
	samplePeriod = 0.010;	
	nAvailableMarkers = CodaGetNumMarkers();

	// Prepare for acquiring single frames of marker data.
	coda_data_frame.dwChannelStart = 0;
	coda_data_frame.dwNumChannels = nAvailableMarkers;
	coda_data_frame.pData = fPosition;
	coda_data_frame.pValid = bInView;

	// Prepare for acquiring a continuous stream of marker data.
	coda_multi_acq_frame.bAutoBufferUpdate = 0;
	coda_multi_acq_frame.dwChannelStart = 0;
	coda_multi_acq_frame.dwNumChannels = nAvailableMarkers;
	coda_multi_acq_frame.dwBufferFrames = CODA_BUFFER_FRAMES;
	coda_multi_acq_frame.dwFrameStart = 0;
	coda_multi_acq_frame.dwNumFrames = 0;
	coda_multi_acq_frame.dwUnit = 0;
	coda_multi_acq_frame.pData = fPositionMulti;
	coda_multi_acq_frame.pValid = bInViewMulti;

	// How many CODA units are there?
	// It would be nice if we could read this value from the CODA system.
	// Alternatively, we should read it from the ini file.
	// Here we get it from a static variable in CodaUtilies.
	nUnits = codaUnits;

	acquiring = false;
	// Fill the first frame with a record in which all the markers are invisible.
	// This will be sent as the current frame until such time that a real frame has been read.
	unsigned int index = 0;
	for ( int unit = 0; unit < nUnits; unit++ ) {
		recordedMarkerFrames[unit][index].time = 0.0;
		for ( int mrk = 0; mrk < nMarkers; mrk++ ) recordedMarkerFrames[unit][index].marker[mrk].visibility = false;
	}
	nFrames = 0;
	StartContinuousAcquisition();
}

// StartContinuousAcquisition() is specific to the continuous tracker. It puts the CODA server
// into continuous acquisition mode. It is called once byt Initialize() and may well never be needed elsewhere.
void CodaLegacyContinuousTracker::StartContinuousAcquisition( void ) {
	OutputDebugString( "CodaAcqStart() ... " );
	// Tell the CODA system to save only single buffers full of data.
	// We will retreive the data as it comes in.
	CodaAcqSetSaveMode( CODA_ACQ_SAVE_BUFFER_ONLY );
	// Initiate continuous acquisition of marker information by the CODAs.
	// The -1 means that acquisitions will continue until CodaAcqStop is called.
	// Because there is no buffering happening on the CODA side, it should go on forever.
	CodaAcqStart( -1 );	
	OutputDebugString( "OK.\n" );

}

// Attempt to halt an ongoing continuous aquisition. 
// Does not care if it was actually acquiring or not.
// Does not retrieve the data.
void CodaLegacyContinuousTracker::StopContinuousAcquisition( void ) {

	int status;
	char msg[1024];

	OutputDebugString( "Stopping acquisition ..." );
	// Stop any acquisitions that may be in progress. 
	status = CodaAcqStop();
	if ( status != CODA_OK ) {
		CodaGetErrorMessage( msg );
		MessageBox( NULL, msg, "CodaAcqStop() failed!", MB_OK );
	}
	OutputDebugString( "OK.\n" );
}

void CodaLegacyContinuousTracker::Quit( void ) {
		
	int status;
	char msg[1024];

	// Stop any acquisitions that may be in progress. 
	StopContinuousAcquisition();
	// Shutdown the CODA units.
 	status = CodaShutDown();
	if ( status != CODA_OK ) {
		CodaGetErrorMessage( msg );
		MessageBox( NULL, msg, "CodaShutDown() failed!", MB_OK );
	}
	/* Disconnect from server. */
	status = CodaDisconnect();
}

// Start and stop reading marker frames into a buffer.
// We assume that continuous acquisitions are already occuring, so all we do 
// is say to start inserting the data from the beginning of the buffer.
void CodaLegacyContinuousTracker::StartAcquisition( float duration ) {
	nFrames = 0;
	acquiring = true;
	TimerSet( acquisitionTimer, duration );
}

// To stop acquiring we simply set the acquisition flag to false.
// The frame counters will stay where they are and any new incoming data
// will be placed in the buffer just after the most recent frame of the acquired series.
void CodaLegacyContinuousTracker::StopAcquisition( void ) {
	acquiring = false;
	for ( int unit = 0; unit < nUnits; unit++ ) {
		unsigned int next = nFrames % MAX_FRAMES;
		unsigned int previous = ( next - 1 ) % MAX_FRAMES;
		// New frames will get written to the place in the buffer pointed to by nFrames.
		// If not acquiring, GetCurrentMarkerFrame will return the data from that location as well.
		// If GetCurrentMarkerFrame() is called too soon, there will not be any new available data.
		// So we prefill the location with the last sample from the recorded time series, this being
		// the latest valid data until a new frame comes in.
		CopyMarkerFrame( recordedMarkerFrames[unit][next],  recordedMarkerFrames[unit][previous] );
	}
}

bool CodaLegacyContinuousTracker::GetAcquisitionState( void ) {
	Update();
	return( acquiring );
}

bool CodaLegacyContinuousTracker::CheckAcquisitionOverrun( void ) { 
	Update();
	return overrun; 
}

int CodaLegacyContinuousTracker::Update( void ) {

	if ( TimerTimeout( acquisitionTimer ) ) {
		// If we are still acquiring when the timer runs out, this is considered
		// to be an overrun. The caller may want to know.
		if ( acquiring ) overrun = true;
		// If we are in a fixed duration acquisition and the timer runs out, 
		// then stop acquiring.
		acquiring = false;
	}

	//
	// Copy the acquired time series of manipulandum positions into an array.
	//

	// Determine how many frames were actually acquired, if any.
	int status = CodaAcqBufferUpdate();
	unsigned int nAcqFrames = CodaAcqGetNumFramesMarker();
	static int cumulative = 0;
	fOutputDebugString( "Status: %d  Frames: %4d  %8d\n", status, nAcqFrames, cumulative );
	cumulative += nAcqFrames;

	if ( nAcqFrames <= 0 ) return( false );

	// Make sure that we don't try to go past the end of the buffer.
	if ( nFrames + nAcqFrames > MAX_FRAMES ) nAcqFrames = MAX_FRAMES - nFrames - 1;
	for ( int unit = 0; unit < nUnits; unit++ ) {
		// One should select the CODA unit here.
		CodaGetOnly( unit );
		// Now get the buffered marker data for that unit.
		CodaAcqGetMultiMarker( &coda_multi_acq_frame );
		// Copy the newly acquired marker data to the recorded arrays.
		for ( unsigned int frm = 0; frm < nAcqFrames; frm++ ) {
			int index = (nFrames + frm) % MAX_FRAMES;
			MarkerFrame *frame = &recordedMarkerFrames[unit][index];
			frame->time = index * samplePeriod;
			for ( int mrk = 0; mrk < nMarkers; mrk++ ) {
				int offset = frm * nAvailableMarkers + mrk;
				if ( bInViewMulti[offset] ) {
					for ( int k = 0; k < 3; k++ ) frame->marker[mrk].position[k] = fPositionMulti[ offset * 3 + k];
					frame->marker[mrk].visibility = true;
				}
				else {
					// Missing values, i.e. when the marker was not visible, are indicated by the INVISIBLE flag.
					for ( int k = 0; k < 3; k++ ) frame->marker[mrk].position[k] = INVISIBLE;
					frame->marker[mrk].visibility = false;
				}
			}
		}
	}
	// If we are acquiring a buffer full of data, then advance to the next frame,
	// taking care not to overrun the buffers.
	if ( acquiring ) nFrames += nAcqFrames;
	return( true );
}

// Copy the most recent packet for the unit in question.
bool CodaLegacyContinuousTracker::GetCurrentMarkerFrameUnit( MarkerFrame &frame, int selected_unit ) {
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

