/*********************************************************************************/
/*                                                                               */
/*                                  Trackers.h                                   */
/*                                                                               */
/*********************************************************************************/

// A 'Tracker' is a device that tracks in 3D the movements of a set of markers.
// The interface is designed around the CodaRTnet interface.
// The main functions allow to:
//	1) start and stop continuous acquisitions at a fixed frequency
//	2) retrieve the time series of marker positions and their visibility
//	3) query the tracker in real time to retrieve the latest observed marker positions.


#pragma once
#include <stdio.h>
#include <stdlib.h>

#include "../Useful/Timers.h"
#include "../Useful/Useful.h"
#include "../VectorsMixin/VectorsMixin.h"


/********************************************************************************/

// Constants used to allocate buffers. They should be small to save space but
//  large enough to handle most options. Here I have set them for the GRASP project.

// On GRASP with DEX, the max number of markers is 24, but the CODA will acquire 28
//  when in 200Hz mode. So we allocate enough space for all to keep the retrieval 
//  routines happy.
#define MAX_MARKERS				24
// On DEX (GRASP) we have 2 units. In the lab we could have more, but I set it to
//  2 in order to save buffer space.
#define MAX_UNITS				2
// The longest duration of acquisition allowed. (samples/sec * sec/min * min)
#define MAX_MINUTES	10
#define MAX_FRAMES				(200 * 60 * MAX_MINUTES)
#define DEFAULT_SAMPLE_PERIOD	0.005
#define INVISIBLE				-99999.99999

// Structures that hold the collected data.
typedef struct {
	PsyPhy::Vector3	position;
	bool	visibility;
} MarkerState;

typedef struct {
	MarkerState	marker[MAX_MARKERS];
	double		time;
} MarkerFrame;

typedef struct {
	MarkerFrame	combinedFrame;
	MarkerFrame unitFrame[MAX_UNITS];
	bool	hasCombinedFrame;
	int		unitFrames;
} MarkerFrameSet;


namespace PsyPhy {

extern void	CopyMarkerFrame( MarkerFrame &destination, MarkerFrame &source );

class Tracker : public PsyPhy::VectorsMixin {

	private:

	protected:

	public:

		// Number of markers to be acquired.
		int nMarkers;
		int nUnits;
		unsigned int nFrames;

		// Buffers to hold the data retrieved from the CODA units.
		// I am making them public so that the calling program can access them directly,
		// rather than going through RetrieveMarkerFramesUnit();
		MarkerFrame recordedMarkerFrames[MAX_UNITS][MAX_FRAMES];

		double samplePeriod;

		Tracker() : nUnits( 0 ), nMarkers( MAX_MARKERS ), samplePeriod( DEFAULT_SAMPLE_PERIOD ) {} ;

		// These are the core functions that each tracker must provide.
		virtual void Initialize( const char *ini_filename = "" ) = 0;
		virtual void StartAcquisition( float max_duration ) = 0;
		virtual void StopAcquisition( void ) = 0;
		virtual bool GetAcquisitionState( void ) = 0;
		virtual bool CheckAcquisitionOverrun( void ) = 0;
		virtual int  Update( void ) = 0;
		virtual void Quit( void ) = 0;

		// This comes from the need of the CODA RTnet trackers to start and stop continuous
		//  acquisitions so as to not overrun the memory on the server.
		// Most trackers will not need these.
		virtual void StartContinuousAcquisition( void ){}
		virtual void RestartContinuousAcquisition( void ){}
		virtual void StopContinuousAcquisition( void ){}


		// This is a useful function that depends on GetAcquisitionState().
		virtual void	WaitAcquisitionCompleted( void );

		// AbortAcquisition() may employ a more brute force method than StopAcquisition(),
		// but by default they are the same.
		virtual void	AbortAcquisition( void ) {
			StopAcquisition();
		}

		// Retieve all the marker data 
		virtual int		RetrieveMarkerFrames( MarkerFrame frames[], int max_frames );
		virtual int		RetrieveMarkerFramesUnit( MarkerFrame frames[], int max_frames, int unit );

		virtual bool	GetCurrentMarkerFrame( MarkerFrame &frame );
		virtual bool	GetCurrentMarkerFrameUnit( MarkerFrame &frame, int unit );
		virtual bool	GetCurrentMarkerFrameIntrinsic( MarkerFrame &frame, int unit );
		virtual void	ComputeIntrinsicMarkerFrame( MarkerFrame &iframe, int unit, MarkerFrame &frame );

		virtual double	GetSamplePeriod( void );
		virtual int		GetNumberOfUnits( void );
		virtual int		GetNumberOfMarkers( void );
		virtual void	GetUnitTransform( int unit, PsyPhy::Vector3 &offset, PsyPhy::Matrix3x3 &rotation );
		virtual void	SetUnitTransform( int unit, PsyPhy::Vector3 &offset, PsyPhy::Matrix3x3 &rotation );
		virtual void	GetAlignmentTransforms( Vector3 offsets[MAX_UNITS], Matrix3x3 rotations[MAX_UNITS] );
		virtual void	SetAlignmentTransforms( Vector3 offsets[MAX_UNITS], Matrix3x3 rotations[MAX_UNITS] );

		void	ComputeAverageMarkerFrame( MarkerFrame &frame, MarkerFrame frames[], int n_frames );
		void	WriteColumnHeadings( FILE *fp, int unit ) ;
		void	WriteMarkerData( FILE *fp, MarkerFrame &frame ) ;
		void	WriteMarkerFile( char *filename );

};

}

