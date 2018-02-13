#ifndef _DATA_WRITER_H_
#define _DATA_WRITER_H_

#include <ostream>
#include <map>
#include <string>
#include "codaRTNetProtocolCPP/PacketDecode3DResultExt.h"
#include "codaRTNetProtocolCPP/PacketDecodeADC16.h"
#include "codaRTNetProtocolCPP/Version.h"

/** Summary
		Write data to a structured text file

		Description
		Writes data in structured text format. This is intended to be a subset of the JSON standard
		JSON format ( http://www.json.org/ ) though some aspects such as escaped character sequences
		are not supported.
	*/
class DataWriter
{
public:
	/** Construct to write to specified stream
			@param _stream Stream to write to
		*/
	DataWriter(std::ostream& _stream);

	/** Write version information
			@param title The title to associate with this version
			@param v Version using RTNet SDK version class
		*/
	void AddVersion(const std::string& title, const codaRTNet::Version& v);

	/** Write file opening section, just an open bracket [ */
	void BeginFile();

	/** Summary
			Begin an acquisition object.

			Description
			Will write a type and title, followed by sample rates for each device, followed by start of packet list.

			@param title The title to write
			@param Sample rate list to write
		*/
	void BeginAcquisition(const std::string& title, const std::map<WORD, float>& device_sample_rate);

	/** Write acquisition packet in structured format - supports 3DResultExt and ADC16 packet layouts
			@param packet Packet in know format
			@parma sync_stop Useful for external sync diagnosics - sets an external sync flag in packet
		*/
	void AddAcquisitionPacket(const codaRTNet::RTNetworkPacket& packet, bool sync_stop = false);

	/** Write end-of-acquisition characters */
	void EndAcquisition();

	/** Complete file to form JSON we can parse */
	void EndFile();

private:
	DataWriter(const DataWriter& src);
	const DataWriter& operator=(const DataWriter& src);

protected:

	/** Summary
			Begin an object.

			Description
			Will write a type and title like { "type" : "acquisition", "title": "mytitle"

			@param objtype The object type to write
			@param title The title to write

			*/
	void BeginObject(const std::string& objtype, const std::string& title);

	/** Write object end character } */
	void EndObject();

protected:
	std::ostream& stream;
	codaRTNet::PacketDecode3DResultExt decode3D;
	codaRTNet::PacketDecodeADC16 decodeADC;
	DWORD packet_count;
	DWORD object_count;
};


#endif