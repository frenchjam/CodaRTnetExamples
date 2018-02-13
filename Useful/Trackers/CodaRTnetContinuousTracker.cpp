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
	// Fill the first frame with a record in which all the markers are invisible.
	// This will be sent as the current frame until such time that a real frame has been read.
	unsigned int index = 0;
	for ( int unit = 0; unit < nUnits; unit++ ) {
		nFramesPerUnit[unit] = 0;
		recordedMarkerFrames[unit][index].time = 0.0;
		for ( int mrk = 0; mrk < nMarkers; mrk++ ) recordedMarkerFrames[unit][index].marker[mrk].visibility = false;
	}
	nFrames = 0;
	StartContinuousAcquisition();
}

// StartContinuousAcquisition() is specific to the continuous tracker. It puts the CODA RTnet server
// into continuous acquisition mode. It is called once byt Initialize() and may well never be needed elsewhere.
void CodaRTnetContinuousTracker::StartContinuousAcquisition( void ) {
	cl.setAcqMaxTicks( DEVICEID_CX1, CODANET_ACQ_UNLIMITED );
	OutputDebugString( "cl.startAcqContinuous()\n" );
	cl.startAcqContinuous();
	TimerSet( runningTimer, maxContinuous );
}

// Attempt to halt an ongoing aquisition. 
// Does not care if it was actually acquiring or not.
// Does not retrieve the data.
void CodaRTnetContinuousTracker::RestartContinuousAcquisition( void ) {
	Timer timer;
	TimerStart( timer );
	cl.stopAcq();
	cl.prepareForAcq();
	cl.startAcqContinuous();
	double elapsed = TimerElapsedTime( timer );
	fOutputDebugString( "CodaRTnetContinuousTracker:Restarting acquisition took %f s.\n", elapsed );
	TimerSet( runningTimer, maxContinuous );
}



// Attempt to halt an ongoing aquisition. 
// Does not care if it was actually acquiring or not.
// Does not retrieve the data.
void CodaRTnetContinuousTracker::StopContinuousAcquisition( void ) {
	OutputDebugString( "Stopping acquisition ..." );
	cl.stopAcq();
	OutputDebugString( "OK.\n" );
}

void CodaRTnetContinuousTracker::Quit( void ) {
	StopContinuousAcquisition();
	CodaRTnetTracker::Quit();
}

// Start and stop reading marker frames into a buffer.
// We assume that continuous acquisitions are already occuring, so all we do 
// is say to start inserting the data from the beginning of the buffer.
void CodaRTnetContinuousTracker::StartAcquisition( float duration ) {
	for ( int unit = 0; unit < nUnits; unit++ ) nFramesPerUnit[unit] = 0;
	nFrames = 0;
	overrun = false;
	acquiring = true;
	TimerSet( acquisitionTimer, duration );
}

// To stop acquiring we simply set the acquisition flag to false.
// The frame counters will stay where they are and any new incoming data
// will be placed in the buffer just after the most recent frame of the acquired series.
void CodaRTnetContinuousTracker::StopAcquisition( void ) {
	acquiring = false;
	for ( int unit = 0; unit < nUnits; unit++ ) {
		unsigned int next = nFramesPerUnit[unit] % MAX_FRAMES;
		unsigned int previous = ( next - 1 ) % MAX_FRAMES;
		// New frames will get written to the place in the buffer pointed to by nFramesPerUnit[].
		// If not acquiring, GetCurrentMarkerFrame will return the data from that location as well.
		// If GetCurrentMarkerFrame() is called to soon, there will not be any new available data.
		// So we prefill the lcoation with the last sample from the recorded time series, this being
		// the latest valid data until a new frame comes in.
		CopyMarkerFrame( recordedMarkerFrames[unit][next],  recordedMarkerFrames[unit][previous] );
	}
}

bool CodaRTnetContinuousTracker::GetAcquisitionState( void ) {
	Update();
	return( acquiring );
}

int CodaRTnetContinuousTracker::Update( void ) {
	
	int mrk;
	int tick;
	
	int nChecksumErrors = 0;
	int nTimeouts = 0;
	int nUnexpectedPackets = 0;
	int nFailedFrames = 0;
	int nRepeatedPackets[MAX_UNITS];
	int previous_tick[MAX_UNITS];
	int nSuccessfullPackets = 0;
	static int cumulativeSuccessfullPackets = 0;

	bool status = false;


	if ( TimerTimeout( acquisitionTimer ) ) {
		// If we are still acquiring when the timer runs out, this is considered
		// to be an overrun. The caller may want to know.
		if ( acquiring ) overrun = true;
		// If we are in a fixed duration acquisition and the timer runs out, 
		// then stop acquiring.
		acquiring = false;
	}

	// Retrieve any UDP packets that have been sent by the CodaRTnet server since the last call.
	// Test to see if the same time slice comes in more than once.
	for ( int unit = 0; unit < MAX_UNITS; unit++ ) {
		nRepeatedPackets[unit] = 0;
		previous_tick[unit] = -1;
	}
	while ( true ) {

		// CodaRTnet packet-handling objects cannot be reused for more than one packet.
		// Otherwise, a bug in the CodaRTnet SDK causes a memory leak.
		// So these objects are instantiated here within a block so that 
		// the destructors get called on each cycle.

		// Generic data packet
		codaRTNet::RTNetworkPacket			local_packet;
		// decoder objects
		codaRTNet::PacketDecode3DResultExt	local_decode3D;		// 3D measurements (CX1)

		// We set an extremely short timeout to effectively make a non-blocking call.
		// Timeout means there are no new packets available.
		if ( stream.receivePacket( local_packet, 100) == CODANET_STREAMTIMEOUT) break; 

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

			// Stay within bounds.
			if ( n_markers > MAX_MARKERS ) n_markers = MAX_MARKERS;
			
			// The 'page' number is used to say which CODA unit the packet belongs to.
			int   unit = local_decode3D.getPage();
			if ( unit >= nUnits ) {
				// I don't believe that we should ever get here, but who knows?
				MessageBox( NULL, "Which unit?!?!", "Dexterous", MB_OK );
				exit( RTNET_RETRIEVEERROR );
			}

			// See if this is a repeat of the previous packet.
			if ( ( tick = local_decode3D.getTick() ) == previous_tick[unit] ) ++nRepeatedPackets[unit];
			previous_tick[unit] = tick;
			// fOutputDebugString( "Unit: %d  Tick: %8d\n", unit, tick );
	
			// Compute the time from the tick counter in the packet and the tick duration.
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
			// If we are acquiring a buffer full of data, then advance to the next frame,
			// taking care not to overrun the buffers.
			if ( acquiring ) {
				if ( nFramesPerUnit[unit] < MAX_FRAMES ) nFramesPerUnit[unit]++;
				else {
					fOutputDebugString( "MAX_FRAMES exceeded. Unit: %d %d \n", unit,  nFramesPerUnit[unit] );
				}
				
			}
			
			// Signal that we got some data in this loop.
			status = true;
		}
	}
	cumulativeSuccessfullPackets += nSuccessfullPackets;
	// fOutputDebugString( "nSuccessfulPackets: %8d Repeats: %8d %8d Cumulative: %8d\n", nSuccessfullPackets, nRepeatedPackets[0], nRepeatedPackets[1], cumulativeSuccessfullPackets );

	// Our model of a tracker assumes an equal number of frames for each unit. But in this
	// tracker it is conceivable that a packet for one unit might have already arrived, while
	// the next one is not yet here. So we set the number of frames for all units to the lowest 
	// number of frames for each unit.
	nFrames = nFramesPerUnit[0];
	for ( int unit = 1; unit < nUnits; unit++ ) {
		if ( nFramesPerUnit[unit] < nFrames ) nFrames =  nFramesPerUnit[unit];
	}

	// If we are not in the middle of an acquisition, check if the continuous acquisition
	// has been running for a long time and if so, stop and restart.
	if ( TimerTimeout( runningTimer ) ) RestartContinuousAcquisition();

	return( status );
	
}

// Copy the most recent packet for the unit in question.
bool CodaRTnetContinuousTracker::GetCurrentMarkerFrameUnit( MarkerFrame &frame, int selected_unit ) {
	// Make sure that any packets that were sent were read.
	Update();
	unsigned int index;
	// If we are acquiring a time series, nFramesPerUnit points is one more than the index of the last acquired sample.
	// So we back up one sample and send that frame.
	if ( acquiring ) index = ( nFramesPerUnit[selected_unit] - 1 ) % MAX_FRAMES;
	// If we are not acquiring a time series, the most recent data is copied into the location pointed to by nFramesPerUnit,
	//  which does not advance.
	else index = nFramesPerUnit[selected_unit] % MAX_FRAMES;
	CopyMarkerFrame( frame, recordedMarkerFrames[selected_unit][index] );
	return true;
}

