#ifndef _DATA_READER_H_
#define _DATA_READER_H_

#include "TextReader.h"
#include <vector>
#include <map>

/** Data point (element of packet) as read from structured text */
class DataPoint
{
public:

	/** Construct empty */
	DataPoint();

	/** Copy constructor - calls copy operator=() */
	DataPoint(const DataPoint& src)
	{ *this = src; }

	/** Copy operator */
	const DataPoint& operator=(const DataPoint& src);

	/** Append coordinate value
			@param x Value
		*/
	void AddValue(float x)
	{ value.push_back(x); }

	/** Set occlusion flag
			@param o flag value
		*/
	void SetOccluded(bool o)
	{ occluded = o; }

	/** Append residual value
			@param r residual
		*/
	void AddResidual(long r)
	{ residual.push_back(r); }

	/** Append intensity value
			@param i intensity
		*/
	void AddIntensity(long i)
	{ intensity.push_back(i); }

	/** Get value array */
	const std::vector<float>& Value() const
	{ return value; }

	/** Get occlusion flag */
	bool Occluded() const
	{ return occluded; }

	/** Get residual array */
	const std::vector<long>& Residual() const
	{ return residual; }

	/** Get intensity array */
	const std::vector<long>& Intensity() const
	{ return intensity; }

private:
	std::vector<float> value;
	bool occluded;
	std::vector<long> residual;
	std::vector<long> intensity;
};

/** Data packet as read from structured text corresponding to RTNetworkPacket */
class DataPacket
{
public:

	/** Construct empty */
	DataPacket();

	/** Copy constructor - calls copy operator */
	DataPacket(const DataPacket& src)
	{ *this = src; }

	/** Copy operator */
	const DataPacket& operator=(const DataPacket& src);

	/** Append a point
			@param p Point to append
		*/
	void AddPoint(const DataPoint& p)
	{ points.push_back(p); }

	/** Get points array */
	const std::vector<DataPoint>& Points() const
	{ return points; }

public:

	/** Device ID from RTNetworkPacket */
	long device;

	/** Device tick from RTNetworkPacket */
	long tick;

	/** Device page from RTNetworkPacket */
	long page;

private:
	std::vector<DataPoint> points;
};

/** Data acquisition as read from structured text */
class DataAcquisition
{
public:

	/** Construct empty */
	DataAcquisition();

	/** Append a packet corresponding to RTNetworkPacket information 
			@param packet Packet to append */
	void AddPacket(DataPacket& packet)
	{ packets.push_back(packet); }

	/** Append device sample-rate settings 
			@param device Device to set
			@param rate Rate in Hz
		*/
	void SetDeviceSampleRate(long device, float rate)
	{ device_sample_rate[device] = rate; }

	/** Get array of packets */
	const std::vector<DataPacket>& Packets() const
	{ return packets; }

	/** Retrieve sample rate info
			@param device Device whose rate to look up
			@param rate Filled with rate if found
			@return true if found, false otherwise
		*/
	bool GetDeviceSampleRate(long device, float& rate);

private:
	DataAcquisition(const DataAcquisition& src);
	const DataAcquisition& operator=(const DataAcquisition& src);

private:
	std::map<long, float> device_sample_rate;
	std::vector<DataPacket> packets;
};

/** Read data from structured text as output by DataWriter */
class DataReader : protected TextReader
{
public:
	/** Construct for specified stream
			@param _stream Stream to read from
	*/
	DataReader(std::istream& _stream);

	/** Find start of object with specified type and title
			@param objtype type to look for
			@param title title to look for
			@return true if found, false if not
			@throws TracedException if there was a problem parsing type or title
	*/
	bool LocateObject(const std::string& objtype, const std::string& title) throw(TracedException);

	/** Load structed acquisition object as writted using DataWriter
			@param acquisition To fill with acquisition read
			@param title The title of the acquisition object to read
			@throws TracedException if problems parsing acquisition
	*/
	void LoadAcquisition(DataAcquisition& acquisition, const std::string& title) throw(TracedException);

	/** Helper to parse string to 32-bit integer 
			@param value String to parse
			@return parsed value
			@throws TracedException if could not parse
		*/
	static long ParseInt32(const std::string& value) throw(TracedException);

	/** Helper to parse string to 32-bit floating point
			@param value String to parse
			@return parsed value
			@throws TracedException if could not parse
		*/
	static float ParseFloat32(const std::string& value) throw(TracedException);
};

#endif
