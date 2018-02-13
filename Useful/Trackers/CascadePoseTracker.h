/*********************************************************************************/
/*                                                                               */
/*                              CascadePoseTrackers.h                            */
/*                                                                               */
/*********************************************************************************/

// A PoseTracker that combines Oculus and Coda.

#pragma once

#include "../Trackers/PoseTrackers.h"

#define MAX_CASCADED_TRACKERS	8

namespace PsyPhy {

class CascadePoseTracker : public PoseTracker {

	private:

		PoseTracker	*tracker[MAX_CASCADED_TRACKERS];
		int nTrackers;

	protected:

	public:

		CascadePoseTracker( PoseTracker *tracker = nullptr );
		int AddTracker( PoseTracker *tracker );
		~CascadePoseTracker();

		bool Initialize( void );
		bool  Update( void );
		bool Release( void );
		bool GetCurrentPoseIntrinsic( PsyPhy::TrackerPose &pose );

};

};


