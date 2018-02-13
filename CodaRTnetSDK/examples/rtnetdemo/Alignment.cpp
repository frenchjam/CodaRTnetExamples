#include <conio.h>
#include <iostream>
#include <strstream>
#include <iomanip>
#include "codaRTNetProtocolCPP/RTNetClient.h"
#include "codaRTNetProtocolCPP/DeviceOptionsAlignment.h"
#include "codaRTNetProtocolCPP/DeviceInfoAlignment.h"
#include "Alignment.h"

Alignment::Alignment()
{
	RegisterParameter("origin", origin);
	RegisterParameter("x0", x0);
	RegisterParameter("x1", x1);
	RegisterParameter("xy0", xy0);
	RegisterParameter("xy1", xy1);
	RegisterParameter("expected_result", expected_result);
	RegisterParameter("allow_retry", allow_retry);
	RegisterParameter("allow_cancel", allow_cancel);
}

void Alignment::RunRTNet() throw(TracedException, codaRTNet::NetworkException, codaRTNet::DeviceStatusArray)
{
	// loop to allow multiple tries when retry is enabled
	while (true)
	{
		// prompt user
		Comment("Prompting user to perform alignment");		
		std::cerr << "Press Y to perform alignment, any other key to cancel..." << std::endl;
		int ch = _getch();
		if(tolower(ch) != 'y')
		{
			if (allow_cancel.Value())
			{
				// merely log the fact that alignment was cancelled but it's not an error
				Comment("Alignment cancelled");
				break;
			}
			else
			{
				// allow_cancel is disabled by the parameters so we must report this as an error and quit
				COMMAND_STOP("Alignment cancelled");
			}
		}

		// make alignment structure with specified marker options
		// these will have been filled in from the command file
		codaRTNet::DeviceOptionsAlignment align(origin.Value(), x0.Value(), x1.Value(), xy0.Value(), xy1.Value());

		// perform alignment
		// this will cause system to look for alignment markers
		// - they must be in position by this time
		Client().setDeviceOptions(align);

		// retrieve information
		codaRTNet::DeviceInfoAlignment info;
		Client().getDeviceInfo(info);

		// print alignment diagnostics		
		LogStatus(align, info);

		// look to see if got the result we expected
		if ((long)info.dev.dwStatus == expected_result.Value())
		{
			// expected value found
			Comment("Result as expected");
			break;
		}
		else
		{
			// unexpected value found
			std::ostrstream formattedmessage;
			formattedmessage << "Expected result " << expected_result.Value() << " but got " << info.dev.dwStatus;
			Comment(formattedmessage);

			if (allow_retry.Value())
			{
				// if retry enabled, just log the retry and loop around for another go
				Comment("Retry enabled");
			}
			else
			{
				// if retry disabled, call Fail() to produce an exception and quit this method
				COMMAND_STOP("Unexpected alignment result and retry disabled");
			}
		}
	}
}

void Alignment::LogStatus(const codaRTNet::DeviceOptionsAlignment& align, const codaRTNet::DeviceInfoAlignment& info)
{
	unsigned long marker_id_array[5] = 
	{ 
		align.opt.dwMarkerOrigin, 
		align.opt.dwMarkerX0, 
		align.opt.dwMarkerX1, 
		align.opt.dwMarkerXY0, 
		align.opt.dwMarkerXY1 
	};

	const char* marker_label_array[5] = 
	{
		"origin",
		"x0    ",
		"x1    ",
		"xy0   ",
		"xy1   "
	};

	const char* camera_label_array[3] = 
	{
		"A",
		"B",
		"C"
	};

	// print alignment status value
	Comment1("Alignment result", info.dev.dwStatus);
	switch (info.dev.dwStatus)
	{
	case 0:
		Comment("success");
		break;
	case CODANET_ALIGNMENTERROR_SYSTEM:
		Comment("system error");
		break;
	case CODANET_ALIGNMENTERROR_ALREADY_ACQUIRING:
		Comment("already acquiring (is another program running?");
		break;
	case CODANET_ALIGNMENTERROR_OCCLUSIONS:
		Comment("occlusions");
		break;
	case CODANET_ALIGNMENTERROR_XTOOCLOSE:
		Comment("x-axis markers too close");
		break;
	case CODANET_ALIGNMENTERROR_XYTOOCLOSE:
		Comment("xy-plane markers too close");
		break;
	case CODANET_ALIGNMENTERROR_NOTPERP:
		Comment("marked axes not sufficiently perpendicular");
		break;
	default:
		Comment1("unknown alignment status error code", info.dev.dwStatus);
		break;
	}

	// number of CX1 units
	DWORD nunits = info.dev.dwNumUnits;

	// frame count
	DWORD nframes = info.dev.dwNumFrames;

	// print visibility information
	for (DWORD icoda = 0; icoda < nunits; icoda++)
	{
		// index of Codamotion CX1 unit
		Comment1("CODA", icoda+1);

		// data from each marker
		for (DWORD imarker = 0; imarker < 5; imarker++)
		{
			// actual marker identity
			DWORD marker_identity = marker_id_array[imarker];

			for (DWORD icam = 0; icam < 3; icam++)
			{
				std::ostrstream formattedmessage;
				
				// build label for this marker and camera combination
				formattedmessage 
					<< marker_label_array[imarker] 
				  << " (Marker " << std::setw(2) << marker_identity << ") " 
					<< camera_label_array[icam] 
					<< ": ";

				// print visibility graph for frames of data
				// show a 1 for visible and _ for occluded
				// (show alternate frames only to save screen space)
				for (DWORD iframe = 0; iframe < nframes; iframe+=2)
				{
					BYTE flag = info.dev.camera_flag[3*nframes*5*icoda + 3*nframes*imarker + 3*iframe + icam];
					if (flag <= 10)
						formattedmessage.put('_');
					else
						formattedmessage.put('1');
				}

				// show
				Comment(formattedmessage);
			}
		}
	}
}
