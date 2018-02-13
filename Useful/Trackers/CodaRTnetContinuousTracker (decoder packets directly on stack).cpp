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

#include "CodaRTnetContinuousTracker.h"

using namespace PsyPhy;

void CodaRTnetContinuousTracker::Initialize( const char *ini_filename ) {
	CodaRTnetTracker::Initialize( ini_filename );
	acquiring = false;
	overrun = false;
	// Fill the last frame with a record in which all the markers are invisible.
	// This will be sent as the current frame until such time that a real frame has been read.
	unsigned int index = ( (unsigned int) 0 - 1 ) % MAX_FRAMES;
	for ( int unit = 0; unit < nUnits; unit++ ) {
		nFramesPerUnit[unit] = 0;
		recordedMarkerFrames[unit][index].time = 0.0;
		for ( int mrk = 0; mrk < nMarkers; mrk++ ) recordedMarkerFrames[unit][index].marker[mrk].visibility = false;
	}
	nFrames = 0;
	StartContinuousAcquisition();
}

void CodaRTnetContinuousTracker::StartContinuousAcquisition( void ) {
	cl.setAcqMaxTicks( DEVICEID_CX1, CODANET_ACQ_UNLIMITED );
	OutputDebugString( "cl.startAcqContinuous()\n" );
	cl.startAcqContinuous();
}

void CodaRTnetContinuousTracker::StartAcquisition( double duration ) {
	for ( int unit = 0; unit < nUnits; unit++ ) nFramesPerUnit[unit] = 0;
	nFrames = 0;
	overrun = false;
	acquiring = true;
	TimerSet( acquisitionTimer, duration );
	StartContinuousAcquisition();
}

void CodaRTnetContinuousTracker::StopAcquisition( void ) {
	// Attempt to halt an ongoing aquisition. 
	// Does not care if it was actually acquiring or not.
	// Does not retrieve the data.
	OutputDebugString( "Stopping acquisition ..." );
	cl.stopAcq();
	OutputDebugString( "OK.\n" );
}

bool CodaRTnetContinuousTracker::GetAcquisitionState( void ) {
	Update();
	return( acquiring );
}

int CodaRTnetContinuousTracker::Update( void ) {
	
	int mrk;
	
	int nChecksumErrors = 0;
	int nTimeouts = 0;
	int nUnexpectedPackets = 0;
	int nFailedFrames = 0;
	int nSuccessfullPackets = 0;

	bool status = false;

	//* Generic data packet
	codaRTNet::RTNetworkPacket			local_packet;
	// decoder objects
	codaRTNet::PacketDecode3DResultExt	local_decode3D;		// 3D measurements (CX1)

	// If we are in a fixed duration acquisition and the timer runs out, 
	// stop acquiring. 
	if ( TimerTimeout( acquisitionTimer ) ) {
		if ( acquiring ) overrun = true;
		acquiring = false;
	}

	// Time out means there are no new packets available.
	while ( stream.receivePacket( local_packet, 100) != CODANET_STREAMTIMEOUT ) {
	
		// Check if the packet is corrupted.
		if ( !local_packet.verifyCheckSum() ) nChecksumErrors++;

		// Check if it is a 3D marker packet. It could conceivably  be a packet
		// from the ADC device, although we don't plan to use the CODA ADC at the moment.
		else if ( !local_decode3D.decode( local_packet ) ) nUnexpectedPackets++;

		// If we get this far, it is a valid marker packet.
		else {
			// Count the total number of valid packets..
			nSuccessfullPackets++;
			// find number of markers included in the packet.
			int n_markers = local_decode3D.getNumMarkers();

			// Single shots can return 56 marker positions, even if we are using
			// 200 Hz / 28 markers for continuous acquisition. Stay within bounds.
			if ( n_markers > MAX_MARKERS ) n_markers = MAX_MARKERS;
			
			// The 'page' number is used to say which CODA unit the packet belongs to.
			int   unit = local_decode3D.getPage();
			if ( unit >= nUnits ) {
				// I don't believe that we should ever get here, but who knows?
				MessageBox( NULL, "Which unit?!?!", "Dexterous", MB_OK );
				exit( RTNET_RETRIEVEERROR );
			}
			
			// Compute the time from the tick counter in the packet and the tick duration.
			// Actually, I am not sure if the tick is defined on a single shot acquistion.
			int index = nFramesPerUnit[unit] % MAX_FRAMES;
			MarkerFrame *frame = &recordedMarkerFrames[unit][index];
			frame->time = local_decode3D.getTick() * cl.getDeviceTickSeconds( DEVICEID_CX1 );

			// Get the marker data from the CODA packet.
			for ( mrk = 0; mrk < n_markers; mrk++ ) {
				float *pos = local_decode3D.getPosition( mrk );
				for ( int i = 0; i < 3; i++ ) frame->marker[mrk].position[i] = pos[i];
				frame->marker[mrk].visibility = ( local_decode3D.getValid( mrk ) != 0 );
			}

			// If the packet contains fewer markers than the nominal number for
			//  the apparatus, set the other markers to be out of sight..
			for ( mrk = mrk; mrk < nMarkers; mrk++ ) {
				float *pos = local_decode3D.getPosition( mrk );
				for ( int i = 0; i < 3; i++ ) frame->marker[mrk].position[i] =INVISIBLE;
				frame->marker[mrk].visibility = false;
			}
			if ( acquiring ) nFramesPerUnit[unit]++;
			
			// Signal that we got the data that we were seeking.
			status = true;
		}
	}
	// Our model of a tracker assumes an equal number of frames for each unit. But in this
	// tracker it is conceivable that a packet for one unit might have already arrived, while
	// the next one is not yet here. So we set the number of frames for all units to the lowest 
	// number of frames for each unit.
	nFrames = nFramesPerUnit[0];
	for ( int unit = 1; unit < nUnits; unit++ ) {
		if ( nFramesPerUnit[unit] < nFrames ) nFrames =  nFramesPerUnit[unit];
	}

	return( status );
	
}

bool CodaRTnetContinuousTracker::GetCurrentMarkerFrameUnit( MarkerFrame &frame, int selected_unit ) {
	// Make sure that any packets that were sent were read.
	Update();
	// Copy the most recent packet for the unit in question.
	// If we are acquiring a time series, nFramesPerUnit points is one more than the index of the last acquired sample.
	// If we are not acquiring a time series, the most recent data is copied into the location pointed to by nFramesPerUnit,
	//  which does not advance.
	unsigned int index;
	if ( acquiring ) index = ( nFramesPerUnit[selected_unit] - 1 ) % MAX_FRAMES;
	else index = nFramesPerUnit[selected_unit] % MAX_FRAMES;
	CopyMarkerFrame( frame, recordedMarkerFrames[selected_unit][index] );
	return true;
}

