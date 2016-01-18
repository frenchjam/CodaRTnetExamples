#include "DataWriter.h"

DataWriter::DataWriter(std::ostream& _stream) :
	stream(_stream),
	packet_count(0UL),
	object_count(0UL)
{
}

void DataWriter::BeginFile()
{
	stream << "[" << std::endl;
}

void DataWriter::EndFile()
{
	stream << "]" << std::endl;
}

void DataWriter::BeginObject(const std::string& objtype, const std::string& title)
{
	// comma between multiple objects
	if (object_count++ > 0)
		stream << "," << std::endl;

	// begin and append title
	stream << "{ \"type\": \"" << objtype << "\", \"title\": \"" << title << "\"";
}

void DataWriter::EndObject()
{
	stream << "}" << std::endl;
}

void DataWriter::AddVersion(const std::string& title, const codaRTNet::Version& v)
{
	BeginObject("version", title);
	stream 
		<< ", \"major\": " << (int)v.wMajor
		<< ", \"minor\": " << (int)v.wMinor
		<< ", \"build\": " << (int)v.wBuild
		<< ", \"revision\": " << (int)v.wRevision
		<< " ";
	EndObject();
}

void DataWriter::BeginAcquisition(const std::string& title, const std::map<WORD, float>& device_sample_rate)
{
	// reset packet count
	packet_count = 0UL;

	// begin acquisition object
	BeginObject("acquisition", title);

	// append info about all devices
	stream << ", \"devices\": [";
	unsigned long count(0);
	for (std::map<WORD, float>::const_iterator iter( device_sample_rate.begin() ); iter != device_sample_rate.end(); iter++, count++)
	{
		if (count > 0)
			stream << ",";
		stream << " { \"device\": " << iter->first << ", \"sample_rate\": " << iter->second << "}";
	}
	stream << " ]";

	// begin packets list section
	stream << ", \"packets\": [" << std::endl;
}

void DataWriter::EndAcquisition()
{
	// end packets list section
	stream << "]" << std::endl;

	// end acquisition object
	EndObject();
}

void DataWriter::AddAcquisitionPacket(const codaRTNet::RTNetworkPacket& packet, bool sync_stop /*= false*/)
{
	// comma separator if doing multiple packets
	if (packet_count++ > 0UL)
	{
		stream << ",";
	}

	// begin packet object
	stream << " {";

	// log packet header
	CODANET_PACKET_HEADER header;
	packet.readHeader(&header);
	stream 
		<< " \"device\": " << (int)header.wDeviceID
		<< ", \"tick\": " << (int)header.dwTick
		<< ", \"page\": " << (int)header.dwPage;

	// optional sync-stop flag for CODA external-sync diagnostics
	if (sync_stop)
		stream << ", \"sync_stop\": 1";

	// begin data section
	stream	<< ", \"data\": ";

	if (decode3D.canDecodePacket(packet))
	{
		if (decode3D.decode(packet))
		{
			DWORD nummarkers = decode3D.getNumMarkers();
			DWORD numresiduals = decode3D.getNumResidualsPerMarker();
			BYTE numcameras = decode3D.getNumCamerasPerMarker();

			// begin list of markers
			stream << "[";

			for (DWORD imarker = 0; imarker < nummarkers; imarker++)
			{
				const float* position = decode3D.getPosition(imarker);
				const WORD* residual = decode3D.getResiduals(imarker);
				const BYTE* intensity = decode3D.getIntensity(imarker);
				
				// separator between multiple markers
				if (imarker > 0)
					stream << ",";

				// begin marker object
				stream << " {";

				// position list
				stream << " \"value\": [ " << position[0] << ", " << position[1] << ", " << position[2] << " ]";

				// validty flag
				stream << ", \"occluded\": " << (decode3D.getValid(imarker) ? "0" : "1");

				// residuals list
				stream << ", \"residual\": [ ";
				for (DWORD iresidual = 0; iresidual < numresiduals; iresidual++)
				{
					if (iresidual > 0)
						stream << ", ";
					stream << (int)residual[iresidual];
				}
				stream << " ]";

				// intensity list
				stream << ", \"intensity\": [ ";
				for (DWORD icamera = 0; icamera < numcameras; icamera++)
				{
					if (icamera > 0)
						stream << ", ";
					stream << (int)intensity[icamera];
				}
				stream << " ]";

				// end marker object
				stream << " }";
			}

			// end list of markers
			stream << " ]";
		}
	}
	else if (decodeADC.canDecodePacket(packet))
	{

		if (decodeADC.decode(packet))
		{
			// begin list of analog values
			stream << "[";

			// append value objects for each channel
			DWORD numvalues = decodeADC.getNumValues();
			short* values = decodeADC.getValues();
			for (DWORD ivalue = 0; ivalue < numvalues; ivalue++)
			{
				if (ivalue > 0)
					stream << ",";
				stream << " { \"value\": " << values[ivalue] << " }";
			}

			// end list of analog values
			stream << " ]";
		}
	}
	else
	{
		stream << " { \"error\": \"unrecognised packet type\" }";
	}

	// finish packet object
	stream << "}" << std::endl;
}
