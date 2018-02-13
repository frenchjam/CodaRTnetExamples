#include "DataReader.h"

const char* DataReaderModuleName = "DataReader";
#define DATA_READER_STOP(_message) STOP(DataReaderModuleName, _message)
#define DATA_READER_STOP2(_message, _value) STOP2(DataReaderModuleName, _message, _value)

DataPoint::DataPoint() :
	occluded(false)
{
}

const DataPoint& DataPoint::operator=(const DataPoint& src)
{
	value = src.value;
	occluded = src.occluded;
	residual = src.residual;
	intensity = src.intensity;
	return *this;
}

DataReader::DataReader(std::istream& _stream) :
TextReader(_stream)
{
}

DataPacket::DataPacket() :
	device(0UL),
	tick(0UL),
	page(0UL)
{
}

const DataPacket& DataPacket::operator=(const DataPacket& src)
{
	device = src.device;
	tick = src.tick;
	page = src.page;
	points = src.points;
	return *this;
}

DataAcquisition::DataAcquisition()
{
}

bool DataAcquisition::GetDeviceSampleRate(long device, float& rate)
{
	std::map<long, float>::const_iterator iter = device_sample_rate.find(device);
	if (iter != device_sample_rate.end())
	{
		rate = iter->second;
		return true;
	}
	else
	{
		return false;
	}
}

long DataReader::ParseInt32(const std::string& value) throw(TracedException)
{
	long x(0);
	if (sscanf(value.c_str(), " %d", &x) != 1)
		DATA_READER_STOP("Could not parse integer");
	return x;
}

float DataReader::ParseFloat32(const std::string& value) throw(TracedException)
{
	float x (0.0f);
	if (sscanf(value.c_str(), " %f", &x) != 1)
		DATA_READER_STOP("Could not parse number");
	return x;
}

bool DataReader::LocateObject(const std::string& objtype, const std::string& title) throw(TracedException)
{
	stream.clear();
	stream.seekg(0, std::istream::beg);
	long listdepth = 0;
	long objdepth = 0;
	bool inquote = false;

	while (stream.good() && !stream.eof())
	{
		// manage a stack of object/list hierarchy
		int c = stream.get();
		if (inquote)
		{
			if (c == '\"')
			{
				inquote = false;
			}
		}
		else
		{
			if (c == '\"')
			{
				inquote = true;
			}
			else
			{
				bool objbegin = false;
				if (c == '[')
				{
					listdepth++;
				}
				else if (c == ']')
				{
					listdepth--;
				}
				else if (c == '{')
				{
					objbegin = true;
					objdepth++;
				}
				else if (c == '}')
				{
					objdepth--;
				}

				// we are looking for top-level objects with type followed by title as specified
				if (objbegin && listdepth == 1 && objdepth == 1)
				{
					std::string typekey;
					std::string typevalue;
					ParseKeyValue(typekey, typevalue);
					if (typekey == "type" && typevalue == objtype)
					{
						if (NextNonSpace() == ',')
						{
							std::string titlekey;
							std::string titlevalue;
							ParseKeyValue(titlekey, titlevalue);
							if (titlekey == "title" && titlevalue == title)
							{
								return true;
							}
						}
					}
				}

			}
		}
	}

	return false;
}

void DataReader::LoadAcquisition(DataAcquisition& acquisition, const std::string& title)
{
	if (!LocateObject("acquisition", title))
		DATA_READER_STOP2("Could not find acquisition with title", title);
	
	while (NextNonSpace() == ',')
	{
		std::string sectionkey;
		ParseQuotedString(sectionkey);
		if (NextNonSpace() != ':')
			DATA_READER_STOP("Expected colon");

		if (sectionkey == "devices")
		{
			if (NextNonSpace() != '[')
				DATA_READER_STOP("Expected list of devices");

			while (true)
			{
				if (NextNonSpace() != '{')
					DATA_READER_STOP("Expected open bracket for device info");

				// scan for device and sample rate
				long device(0);
				float sample_rate(0.0f);
				while (true)
				{					
					// scan key and value
					std::string key;
					std::string value;
					ParseKeyValue(key, value);

					// store value as appropriate
					if (key == "device")
						device = ParseInt32(value);
					else if (key == "sample_rate")
						sample_rate = ParseFloat32(value);

					// either continue to next or end this list
					int c = NextNonSpace();
					if (c == '}')
						break;
					else if (c != ',')
						DATA_READER_STOP("Expected comma");
				}

				// place in list
				acquisition.SetDeviceSampleRate(device, sample_rate);

				// either continue to next or end this list
				int c = NextNonSpace();
				if (c == ']')
					break;
				else if (c != ',')
					DATA_READER_STOP("Expected comma between device rate objects");
			}
		}
		else if (sectionkey == "packets")
		{
			if (NextNonSpace() != '[')
				DATA_READER_STOP("Expected packet list");

			while (true)
			{
				if (NextNonSpace() != '{')
					DATA_READER_STOP("Expected start of packet object");

				DataPacket packet;

				while (true)
				{
					// scan key and colon
					std::string key;
					ParseQuotedString(key);
					if (NextNonSpace() != ':')
						DATA_READER_STOP("Expected colon");

					// data is a list of points
					if (key == "data")
					{
						if (NextNonSpace() != '[')
							DATA_READER_STOP("Expected start of data list");

						// point is a set of key-value pairs for fields, some of which are lists
						while (true)
						{
							if (NextNonSpace() != '{')
								DATA_READER_STOP("Expected start of data point object");

							// point to fill with data
							DataPoint point;

							while (true)
							{
								// key to field
								std::string field;
								ParseQuotedString(field);
								if (NextNonSpace() != ':')
									DATA_READER_STOP("Expected colon");

								// can have lists or single values
								SkipSpace();
								bool islist(false);
								if (stream.peek() == '[')
								{
									islist = true;
									stream.get();
								}
								long valuecount = 0;
								while (true)
								{
									if (islist)
									{
										// skip space between items
										SkipSpace();

										// end list
										if (stream.peek() == ']')
										{
											stream.get();
											break;
										}

										// expect comma separator
										if (valuecount++ > 0)
										{
											if (stream.get() != ',')
												DATA_READER_STOP("Expected comma between field elements");
										}
									}

									// get value string
									std::string numberstring;
									ParseValue(numberstring);

									// parse appropriately
									if (field == "value")
									{
										point.AddValue( ParseFloat32(numberstring) );
									}
									else if (field == "occluded")
									{
										long occluded = ParseInt32(numberstring);
										point.SetOccluded(occluded ? true : false);
									}
									else if (field == "intensity")
									{
										point.AddIntensity( ParseInt32(numberstring) );
									}
									else if (field == "residual")
									{
										point.AddResidual( ParseInt32(numberstring) );
									}

									// we're done if it was just a single value anyway
									if (!islist)
										break;
								}

								// either continue to next or end this point
								int c = NextNonSpace();
								if (c == '}')
									break;
								else if (c != ',')
									DATA_READER_STOP("Expected comma between fields within point");
							}

							// add point
							packet.AddPoint(point);

							// either continue to next or end this object
							int c = NextNonSpace();
							if (c == ']')
								break;
							else if (c != ',')
								DATA_READER_STOP("Expected comma between data points");
						}
					}
					else
					{
						// assume a single value
						std::string value;
						ParseValue(value);

						// store in required place
						if (key == "device")
							packet.device = DataReader::ParseInt32(value);
						else if (key == "tick")
							packet.tick = DataReader::ParseInt32(value);
						else if (key == "page")
							packet.page = DataReader::ParseInt32(value);
					}

					// either continue to next or end this object
					int c = NextNonSpace();
					if (c == '}')
						break;
					else if (c != ',')
						DATA_READER_STOP("Expected comma between packet items");
				}

				// add packet to acquisition
				acquisition.AddPacket(packet);

				// either continue to next or end this list
				int c = NextNonSpace();
				if (c == ']')
					break;
				else if (c != ',')
					DATA_READER_STOP("Expected comma between packets");
			}
		}
	}
}