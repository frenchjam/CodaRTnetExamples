#include <strstream>
#include "codaRTNetProtocol/codartclient.h"
#include "../IO/DataReader.h"
#include "../IO/DataWriter.h"
#include "SelfTestMacros.h"
#include "SelfTestDataWriter.h"

void WriteTestPacket3DExt(DataWriter& writer, unsigned long device, unsigned long tick, unsigned long page, const std::vector<float>& xyz, const std::vector<unsigned char>& valid, const std::vector<unsigned char>& intensity, const std::vector<unsigned short>& residual);

void SelfTestDataWriter() throw(TracedException)
{
	std::ostrstream testoutput;
	
	DataWriter w(testoutput);

	w.BeginFile();

	// add an example version
	CODANET_VERSION myversion = { 2, 5, 9, 12 };
	w.AddVersion("my version", (const codaRTNet::Version&)myversion);

	// acquisition object header
	std::map<WORD, float> device_sample_rate;
	device_sample_rate[29] = 1000.22f;
	device_sample_rate[23] = 52.43f;
	w.BeginAcquisition("a test", device_sample_rate);

	// make sample packet with 2 markers, 6 intensity values per marker, 2 residuals per marker
	std::vector<float> xyz;
	std::vector<unsigned char> valid;
	std::vector<unsigned char> intensity;
	std::vector<unsigned short> residual;
	xyz.push_back(45.1f);
	xyz.push_back(-8.2f);
	xyz.push_back(99.9f);
	xyz.push_back(2.2f);
	xyz.push_back(3.3f);
	xyz.push_back(-4.4f);
	valid.push_back(1);
	valid.push_back(0);
	intensity.push_back(11);
	intensity.push_back(22);
	intensity.push_back(33);
	intensity.push_back(44);
	intensity.push_back(55);
	intensity.push_back(66);
	intensity.push_back(98);
	intensity.push_back(87);
	intensity.push_back(76);
	intensity.push_back(65);
	intensity.push_back(54);
	intensity.push_back(43);
	residual.push_back(20578);
	residual.push_back(31999);
	residual.push_back(12345);
	residual.push_back(541);

	// write same data 3 times just changing tick number
	WriteTestPacket3DExt(w, 101, 888, 3, xyz, valid, intensity, residual);
	WriteTestPacket3DExt(w, 101, 999, 3, xyz, valid, intensity, residual);
	WriteTestPacket3DExt(w, 101, 10001, 3, xyz, valid, intensity, residual);

	// acquisition footer
	w.EndAcquisition();

	// try another single-packet acquisition
	w.BeginAcquisition("another one", device_sample_rate);
	WriteTestPacket3DExt(w, 101, 10001, 3, xyz, valid, intensity, residual);
	w.EndAcquisition();

	w.EndFile();

	testoutput << std::ends;

	// now use reader to read back
	std::string testbuffer( testoutput.str() );
	std::istrstream testinput( testbuffer.c_str() );
	DataReader r(testinput);

	// check we can load last acquisition first if we want to
	DataAcquisition acq0;
	r.LoadAcquisition(acq0, "another one");
	SELF_TEST_ASSERT(acq0.Packets().size() == 1);

	// load first acquisition
	DataAcquisition acq;
	r.LoadAcquisition(acq, "a test");

	// check loaded values
	float rate(0.0f);
	SELF_TEST_ASSERT(acq.GetDeviceSampleRate(23, rate) && rate == 52.43f);
	SELF_TEST_ASSERT(acq.GetDeviceSampleRate(29, rate ) && rate == 1000.22f);
	SELF_TEST_ASSERT(acq.Packets().size() == 3);
	const DataPacket& packet = acq.Packets()[2];
	SELF_TEST_ASSERT(packet.Points().size() == 2);
	SELF_TEST_ASSERT(packet.Points()[0].Value().size() == 3);
	SELF_TEST_ASSERT(packet.Points()[1].Value().size() == 3);
	SELF_TEST_ASSERT(packet.Points()[0].Residual().size() == 2);
	SELF_TEST_ASSERT(packet.Points()[1].Residual().size() == 2);
	SELF_TEST_ASSERT(packet.Points()[0].Intensity().size() == 6);
	SELF_TEST_ASSERT(packet.Points()[1].Intensity().size() == 6);
	SELF_TEST_ASSERT(packet.Points()[0].Value()[0] == 45.1f);
	SELF_TEST_ASSERT(packet.Points()[0].Value()[1] == -8.2f);
	SELF_TEST_ASSERT(packet.Points()[0].Value()[2] == 99.9f);
	SELF_TEST_ASSERT(packet.Points()[1].Value()[0] == 2.2f);
	SELF_TEST_ASSERT(packet.Points()[1].Value()[1] == 3.3f);
	SELF_TEST_ASSERT(packet.Points()[1].Value()[2] == -4.4f);
	SELF_TEST_ASSERT(packet.Points()[0].Residual()[0] == 20578);
	SELF_TEST_ASSERT(packet.Points()[0].Residual()[1] == 31999);
	SELF_TEST_ASSERT(packet.Points()[1].Residual()[0] == 12345);
	SELF_TEST_ASSERT(packet.Points()[1].Residual()[1] == 541);
	SELF_TEST_ASSERT(packet.Points()[0].Intensity()[0] == 11);
	SELF_TEST_ASSERT(packet.Points()[0].Intensity()[1] == 22);
	SELF_TEST_ASSERT(packet.Points()[0].Intensity()[2] == 33);
	SELF_TEST_ASSERT(packet.Points()[0].Intensity()[3] == 44);
	SELF_TEST_ASSERT(packet.Points()[0].Intensity()[4] == 55);
	SELF_TEST_ASSERT(packet.Points()[0].Intensity()[5] == 66);
	SELF_TEST_ASSERT(packet.Points()[1].Intensity()[0] == 98);
	SELF_TEST_ASSERT(packet.Points()[1].Intensity()[1] == 87);
	SELF_TEST_ASSERT(packet.Points()[1].Intensity()[2] == 76);
	SELF_TEST_ASSERT(packet.Points()[1].Intensity()[3] == 65);
	SELF_TEST_ASSERT(packet.Points()[1].Intensity()[4] == 54);
	SELF_TEST_ASSERT(packet.Points()[1].Intensity()[5] == 43);
}

void WriteTestPacket3DExt(DataWriter& writer, unsigned long device, unsigned long tick, unsigned long page, const std::vector<float>& xyz, const std::vector<unsigned char>& valid, const std::vector<unsigned char>& intensity, const std::vector<unsigned short>& residual)
{
	// loop counters
	unsigned char* ptr(NULL);
	size_t imarker(0);
	size_t icamera(0);
	
	const size_t nmarkers = xyz.size() / 3;
	const size_t nintensity_trio_per_marker = intensity.size() / (3*nmarkers);
	const size_t ncameras = 3*nintensity_trio_per_marker;
	const size_t nresiduals = (nintensity_trio_per_marker > 1) ? (residual.size() / nmarkers) : 0;

	// layout of CODANET_PACKETID_3DRESULT_EXT is:
	// WORD num markers
	// BYTE num cameras
	// BYTE num residuals (num cx1 units sent in this packet, or zero)
	// for each marker
	//   float x
	//   float y
	//   float z
	//   BYTE valid (on/off)
	//   for each camera
	//     BYTE intensity
	//   end for
	//   for each residual
	//     WORD microns
	//   end for
	// end for
	codaRTNet::RTNetworkPacket np((unsigned short)device, CODANET_PACKETID_3DRESULT_EXT, tick, page, 4 + (3*sizeof(float)+1+ncameras+2*nresiduals)*nmarkers);

	// compose packet data		
	ptr = np.getData();

	// num markers word
	*(unsigned short*)ptr = codanet_htons( (unsigned short)nmarkers );
	ptr += 2;

	// num cameras byte
	*(ptr++) = (unsigned char)ncameras;

	// num residuals byte
	*(ptr++) = (unsigned char)nresiduals;

	// per-marker loop
	for (imarker = 0; imarker < nmarkers; imarker++)
	{
		// copy marker data (network byte order)
		((float*)ptr)[0] = codanet_htonf(xyz[3*imarker  ]);
		((float*)ptr)[1] = codanet_htonf(xyz[3*imarker+1]);
		((float*)ptr)[2] = codanet_htonf(xyz[3*imarker+2]);
		ptr += 3*sizeof(float);

		// copy single validity byte (on/off)
		*(ptr++) = valid[imarker];

		// copy ncameras * intensity values
		for (icamera = 0; icamera < ncameras; icamera++)
			*(ptr++) = intensity[ncameras*imarker + icamera];

		// copy residuals (network byte order)
		for (size_t iresid = 0; iresid < nresiduals; iresid++, ptr += 2)
			*(unsigned short*)ptr = codanet_htons( residual[imarker*nresiduals + iresid] );
	}

	writer.AddAcquisitionPacket(np);
}
