/*********************************************************************************/
/*                                                                               */
/*                                PoseTrackerFilter.h                            */
/*                                                                               */
/*********************************************************************************/

#pragma once

#include "../Trackers/PoseTrackers.h"

namespace PsyPhy {

class PoseTrackerFilter : public PoseTracker {

	private:

		PoseTracker	*unfiltered_tracker;
		TrackerPose current_pose;


	protected:

	public:

		double filterConstant;
		PoseTrackerFilter( PoseTracker *tracker = nullptr, double filter_constant = 0.0 );
		~PoseTrackerFilter();

		bool Initialize( void );
		bool  Update( void );
		bool Release( void );
		bool GetCurrentPoseIntrinsic( PsyPhy::TrackerPose &pose );

};

};


