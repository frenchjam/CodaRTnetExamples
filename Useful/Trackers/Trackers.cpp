/*********************************************************************************/
/*                                                                               */
/*                                    Trackers.cpp                               */
/*                                                                               */
/*********************************************************************************/

// Base class for a 3D/6D Tracker.

#include "stdafx.h"
#include "Trackers.h"

using namespace PsyPhy;

/**************************************************************************************/

// Provide some generic methods for copying marker frames.

void PsyPhy::CopyMarkerFrame( MarkerFrame &destination, MarkerFrame &source ) {
	static VectorsMixin vm;
	int mrk;
	destination.time = source.time;
	for ( mrk = 0; mrk < MAX_MARKERS; mrk++ ) {
		destination.marker[mrk].visibility = source.marker[mrk].visibility;
		vm.CopyVector( destination.marker[mrk].position, source.marker[mrk].position );
	}
}

/***************************************************************************/

// Accesssor methods for some key parameters.

double Tracker::GetSamplePeriod( void ) {
	return( samplePeriod );
}
int Tracker::GetNumberOfUnits( void ) {
	return( nUnits );
}
int Tracker::GetNumberOfMarkers( void ) {
	return( nMarkers );
}

// Wait until an acquisition with a set number of frames has been completed.
void Tracker::WaitAcquisitionCompleted( void ) {
	while ( GetAcquisitionState() ) {
		Update();
		Sleep( 1 );
	}
}


// Should return the most recent frame of data. 
// If your tracker does not provide this functionality, then simply
//  return false to signal that no frame is available.
bool  Tracker::GetCurrentMarkerFrameUnit( MarkerFrame &frame, int unit ) { 
	return( false );
}

// Get the most recent frame of marker data from a default unit, or 
//  data that has been combined on line from multiple units.
// By default, simply take the data from unit 0.
bool  Tracker::GetCurrentMarkerFrame( MarkerFrame &frame ) { 
	return( GetCurrentMarkerFrameUnit( frame, 0 ) );
}

// After an acquisiton of a fixed time or number of frames, retrieve the full buffer of frames. 

// Note that these definitions are circular. A derived class must either provide a method to retrieve
//  the frames from a default unit, in which case specifying any unit will get you the default unit
//  (i.e. unit number is ignored) 
//           ** OR **
// provide the means to get the frames from a specified unit and allow unit 0 to be considered as 
// the default unit. The derived class can, of course, also specify both.

// Get the most recent frame of marker data from a default unit, or 
//  data that has been combined on line from multiple units.
int  Tracker::RetrieveMarkerFrames( MarkerFrame frames[], int max_frames ) { 
	// By default, simply take the data from unit 0.
	return( RetrieveMarkerFramesUnit( frames, max_frames, 0 ) );
}

int  Tracker::RetrieveMarkerFramesUnit( MarkerFrame frames[], int max_frames, int unit ) { 
	// If the tracker has no concept of separate units, just get the data from the default unit.
	return( RetrieveMarkerFrames( frames, max_frames ) );
}

void	Tracker::ComputeAverageMarkerFrame( MarkerFrame &frame, MarkerFrame frames[], int n_frames ) {
	int mrk, frm;
	for ( mrk = 0; mrk < MAX_MARKERS; mrk++ ) {
		CopyVector( frame.marker[mrk].position, zeroVector );
		frame.marker[mrk].visibility = false;
	}
	frame.time = 0.0;

	for ( mrk = 0; mrk < MAX_MARKERS; mrk++ ) {
		int count = 0;
		for ( frm = 0; frm < n_frames; frm++ ) {
			if ( frames[frm].marker[mrk].visibility ) {
				AddVectors( frame.marker[mrk].position, frame.marker[mrk].position, frames[frm].marker[mrk].position );
				count++;
			}
		}
		if ( count > 0 ) {
			frame.marker[mrk].visibility = true;
			ScaleVector( frame.marker[mrk].position, frame.marker[mrk].position, 1.0 / (double) count );
		}
	}
}

void Tracker::WriteColumnHeadings( FILE *fp, int unit ) {
	fprintf( fp, "Time.%1d;", unit );
	for ( int mrk = 0; mrk <nMarkers; mrk++ ) {
		fprintf( fp, " M%02d.%1d.V; M%02d.%1d.X; M%02d.%1d.Y; M%02d.%1d.Z;", mrk, unit, mrk, unit, mrk, unit, mrk, unit  );
	}
}

void Tracker::WriteMarkerData( FILE *fp, MarkerFrame &frame ) {
	fprintf( fp, "%9.3f;", frame.time );
	for ( int mrk = 0; mrk < nMarkers; mrk++ ) {
		fprintf( fp, " %1d;",  frame.marker[mrk].visibility );
		for ( int i = 0; i < 3; i++ ) fprintf( fp, "%9.3f;",  frame.marker[mrk].position[i] );
	}
}

void Tracker::WriteMarkerFile( char *filename ) {
	FILE *fp = fopen( filename, "w" );
	fAbortMessageOnCondition( !fp, "Error opening %s for writing.", filename );
	fprintf( fp, "%s\n", filename );
	fprintf( fp, "Tracker Units: %d\n", nUnits );
	fprintf( fp, "Markers: %d\n", nMarkers );
	fprintf( fp, "Frames: %d\n", nFrames );
	fprintf( fp, "Frame;" );
	for ( int unit = 0; unit < nUnits; unit++ ) WriteColumnHeadings( fp, unit );
	fprintf( fp, "\n" );
	for ( unsigned int frm = 0; frm < nFrames; frm++ ) {
		fprintf( fp, "%8u;", frm );
		for ( int unit = 0; unit < nUnits; unit++ ) WriteMarkerData( fp, recordedMarkerFrames[unit][frm] );
		fprintf( fp, "\n" );
	}
	fclose( fp );
}

/**************************************************************************************/

// Get the latest frame of marker data from the specified unit.
// Marker positions are expressed in the intrinsic reference frame of the unit.

// Provide methods to set and retrieve the alignment transformation for a given unit from 
//  the intrinsic to aligned reference frames. The base class assumes that no alignment is necessary.
void Tracker::GetUnitTransform( int unit, Vector3 &offset, Matrix3x3 &rotation ) {
	CopyVector( offset, zeroVector );
	CopyMatrix( rotation, identityMatrix );
}
void Tracker::SetUnitTransform( int unit, Vector3 &offset, Matrix3x3 &rotation ) {
	static bool first_call = true;
	if ( first_call ) {
		int response = fMessageBox( MB_YESNO, "Tracker", "Warning - SetUnitTransform() not handled by this tracker.\nDo you want to continue?" );
		if ( response == IDNO) exit( -1 );
		first_call = false;
	}
}

// Get the entire set of transforms.
void Tracker::GetAlignmentTransforms( Vector3 offsets[MAX_UNITS], Matrix3x3 rotations[MAX_UNITS] ) {
	for ( int unit = 0; unit < MAX_UNITS; unit++ ) GetUnitTransform( unit, offsets[unit], rotations[unit] );
}
void Tracker::SetAlignmentTransforms( Vector3 offsets[MAX_UNITS], Matrix3x3 rotations[MAX_UNITS] ) {
	for ( int unit = 0; unit < MAX_UNITS; unit++ ) SetUnitTransform( unit, offsets[unit], rotations[unit] );
}


// Now provide the means to retrieve the frames and express the data in the intinsic frame.
// Note that there is not a "combined" version of this because it makes no sense
//  to combine the marker data in the intrinsic reference frames.
void Tracker::ComputeIntrinsicMarkerFrame( MarkerFrame &iframe, int unit, MarkerFrame &frame ) {

	Vector3		offset;
	Matrix3x3	rotation;
	Matrix3x3	inverse;
	Vector3		delta;

	// Retrieve the offset and rotation matrix for this unit.
	GetUnitTransform( unit, offset, rotation );

	// Inverse of a rotation matrix is just its transpose.
	TransposeMatrix( inverse, rotation );

	// Compute the position of each maker in intrinsic coordinates.
	for ( int mrk = 0; mrk < nMarkers; mrk++ ) {
		iframe.marker[mrk].visibility = frame.marker[mrk].visibility;
		if ( frame.marker[mrk].visibility ) {
			SubtractVectors( delta, frame.marker[mrk].position, offset );
			MultiplyVector( iframe.marker[mrk].position, delta, inverse );
		}
	}

}

bool Tracker::GetCurrentMarkerFrameIntrinsic( MarkerFrame &iframe, int unit ) {
	MarkerFrame	frame;
	int			status;
	// Get the current frame in aligned coordinates.
	status = GetCurrentMarkerFrameUnit( frame, unit );
	// I'm not sure what could go wrong, but signal if it does.
	if ( !status ) return( false );
	ComputeIntrinsicMarkerFrame( iframe, unit, frame );
	return( true );
}
