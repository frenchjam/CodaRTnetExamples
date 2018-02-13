#include <strstream>
#include "../IO/DataReader.h"
#include "SelfTestMacros.h"
#include "SelfTestDataReader.h"

void SelfTestDataReader() throw(TracedException)
{
	{
		const char* testdata = 
			"[\n" \
			" { \"nothing\": \"novalue\" }, \n" \
			" { \"type\": \"person\", \"title\": \"Bob\" }, \n" \
			" { \"type\": \"fruit\", \"title\": \"durian\"} \n" \
			"]\n";

		std::istrstream teststream(testdata);

		DataReader r(teststream);
		SELF_TEST_ASSERT(!r.LocateObject("nothere", "sometitle"));
		SELF_TEST_ASSERT(!r.LocateObject("fruit", "apple"));
		SELF_TEST_ASSERT(r.LocateObject("fruit", "durian"));
		SELF_TEST_ASSERT(r.LocateObject("person", "Bob"));	

	}

	{
		const char* testacq =
			"[\n" \
			" { \"type\": \"acquisition\", \"title\": \"buffer monitor\", \"devices\": [ { \"device\": 100, \"sample_rate\": 52.2}, { \"device\": 201, \"sample_rate\": 22.222} ], \"packets\": [\n" \
			"   { \"device\": 100, \"tick\": 4, \"page\": 23, \"data\": [ { \"value\": [ -98.2, 33.4, 22.2 ], \"occluded\": 1, \"residual\": [ 18, 23 ], \"intensity\": [ 53, 57, 49, 28, 63, 108 ] }, { \"value\": [ -2.2, -3.3, 4.4 ], \"occluded\": 0, \"residual\": [ 18, 23], \"intensity\": [ 1, 2, 3, 4, 5, 6 ] } ] },\n" \
			"   { \"device\": 201, \"tick\": 7, \"page\": 24, \"data\": [ { \"value\": [ -98.2, 33.4, 22.2 ], \"occluded\": 1, \"residual\": [ 18, 23 ], \"intensity\": [ 53, 57, 49, 28, 63, 108 ] }, { \"value\": [ -2.2, -3.3, 4.4 ], \"occluded\": 0, \"residual\": [ 18, 23], \"intensity\": [ 1, 2, 3, 4, 5, 6 ] } ] }\n" \
			"]";

		std::istrstream acqstream(testacq);

		DataReader r(acqstream);
		DataAcquisition acq;
		r.LoadAcquisition(acq, "buffer monitor");

		SELF_TEST_ASSERT(acq.Packets().size() == 2);
		const DataPacket& p0 = acq.Packets()[0];
		const DataPacket& p1 = acq.Packets()[1];
		SELF_TEST_ASSERT(p0.Points().size() == 2);
		SELF_TEST_ASSERT(p1.Points().size() == 2);
		SELF_TEST_ASSERT(p0.device == 100);
		SELF_TEST_ASSERT(p0.tick == 4);
		SELF_TEST_ASSERT(p0.page == 23);
		SELF_TEST_ASSERT(p1.device == 201);
		SELF_TEST_ASSERT(p1.tick == 7);
		SELF_TEST_ASSERT(p1.page == 24);
		const DataPoint& d0 = p0.Points()[0];
		const DataPoint& d1 = p1.Points()[1];
		SELF_TEST_ASSERT(d0.Value().size() == 3);
		SELF_TEST_ASSERT(d0.Intensity().size() == 6);
		SELF_TEST_ASSERT(d0.Residual().size() == 2);
		SELF_TEST_ASSERT(d1.Value().size() == 3);
		SELF_TEST_ASSERT(d1.Intensity().size() == 6);
		SELF_TEST_ASSERT(d1.Residual().size() == 2);
		SELF_TEST_ASSERT(d0.Value()[0] == -98.2f);
		SELF_TEST_ASSERT(d0.Value()[1] ==  33.4f);
		SELF_TEST_ASSERT(d0.Value()[2] ==  22.2f);
		SELF_TEST_ASSERT(d0.Occluded());
		SELF_TEST_ASSERT(d0.Residual()[0] ==  18L);
		SELF_TEST_ASSERT(d0.Residual()[1] ==  23L);
		SELF_TEST_ASSERT(d0.Intensity()[0] ==  53L);
		SELF_TEST_ASSERT(d0.Intensity()[1] ==  57L);
		SELF_TEST_ASSERT(d0.Intensity()[2] ==  49L);
		SELF_TEST_ASSERT(d0.Intensity()[3] ==  28L);
		SELF_TEST_ASSERT(d0.Intensity()[4] ==  63L);
		SELF_TEST_ASSERT(d0.Intensity()[5] == 108L);

		float testrate(0.0f);	
		SELF_TEST_ASSERT(!acq.GetDeviceSampleRate(99, testrate));
		SELF_TEST_ASSERT(acq.GetDeviceSampleRate(100, testrate));
		SELF_TEST_ASSERT(testrate == 52.2f);
		SELF_TEST_ASSERT(acq.GetDeviceSampleRate(201, testrate));
		SELF_TEST_ASSERT(testrate == 22.222f);
	}
}
