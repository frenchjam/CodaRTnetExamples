// The CODA RTnet server maintains a list of machine configurations.
// Each configuration defines which devices are to be activated, 
// such as which CODA cx1 units and which analog interfaces.
// The configurations are defined using CodaMotionConfigManager.jar on the CODA hub.
// The following #defines need to be set to correspond to the configurations 
// that are defined on the server.

// Any configuration that uses CODA cx1 units only.
// Here it doesn't matter how many cx1 units there are in the configuration.
// But see the parameter 'packetsPerFrame' below, which has to be set 
//  to be coherent with the number of units.
#define CX1only		0	

// A configuration that only does analog acquisions.
#define ADConly		1

// A configuration that does markers and analog acquisitions.
// See notes above for CX1only about the number of cx1 units.
#define CX1andADC	2

// Default is coda marker acquisition only.
static unsigned int codaConfig = CX1only;

#define DEVICEID_ADC DEVICEID_GS16AIO

#define X 0
#define Y 1
#define Z 2

// Freqencies in Hertz of each acqusition type.

// Hardwire the server IP address and port.
// There is a way to search for a server, but it takes time.
// This allows for a very quick connection.

// DEX via EPM LAN
char serverAddress[] = "192.168.200.133";

// DEX via ETD LAN
//char serverAddress[] = "10.80.12.103";

// Some other possibilities.
//char serverAddress[] = "192.168.1.1";
//char serverAddress[] = "172.20.164.7";
//char serverAddress[] = "192.168.200.150";

unsigned int serverPort = 10111;

// Determine the acquisition modes and corresponding freqencies.
DeviceOptionsCodaMode			coda_mode( CODANET_CODA_MODE_200, 1, false );
// Marker rate depends on the mode selected just above.
float cx1Rate = 200.0;
// For analog samples, pick the frequency you want between 50, 100, 200, 500, 1000.
float adcRate = 1000.0;
DeviceOptionsGS16AIOInputs		analog_mode( adcRate );

// Pick one of the following three options to determine if data from each
// coda is acquired and stored separately or combined.

// Option 1: Acquire data from multiple codas as combined by the coda system.
DeviceOptionsCodaPacketMode		packet_mode = CODANET_CODAPACKETMODE_COMBINED_COORD;
int packetsPerFrame = 1;

// Option 2: Acquire coordinates from each coda separately.
//DeviceOptionsCodaPacketMode	packet_mode = CODANET_CODAPACKETMODE_SEPARATE_COORD;
//int packetsPerFrame = 2; // Should be equal to the number of cx1 units in the configuration.

// Option 3: Acquire both the coordates from each coda and the combined coordinates.
// I don't think that I have ever used this.
//DeviceOptionsCodaPacketMode	packet_mode = CODANET_CODAPACKETMODE_SEPARATE_AND_COMBINED_COORD;
//int packetsPerFrame = 3; // Should be equal to the number of cx1 units in the configuration plus 1.

// This determines how .'s are displayed to show progress.
#define FRAMES_PER_DOT	1

// How many tries to get a data packet before giving up.
int maxRetries = 1;
