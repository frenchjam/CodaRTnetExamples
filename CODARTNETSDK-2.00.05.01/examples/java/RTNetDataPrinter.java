
// Import Codamotion RTNet library (requires codamotionrtnet.jar on the classpath) 
import com.codamotion.rtnet.*;

// Import address class from standard Java network library
import java.net.InetSocketAddress;

/**
 * Codamotion RTNet Java Examples: RTNetDataPrinter
 *
 * RTNetDataPrinter is a Java program to print marker and analog data to 
 * standard output. 
 * 
 * Connects to first available server and, if system is not 
 * already running, starts system using first available hardware 
 * configuration. Uses buffered acquisition mode and monitor function for 
 * low-latency realtime monitoring. Prints realtime 3D and analog data to the
 * screen at 20 x 100ms intervals, then stops acquisition.  After acquisition
 * has been stoppped, marker and analog buffered data is downloaded and 
 * printed to standard output as tab-delimited tables. 
 * 
 * Copyright (C) Charnwood Dynamics Ltd.
 * All rights reserved.
 *
 */
public class RTNetDataPrinter {

	/**
	 * Main program entry point.
	 * 
	 * @param args command-line arguments (not used)
	 */
	public static void main(String[] args) {

		// attempt to find server
		InetSocketAddress[] serverAddress = RTNetClient.doAutoDiscoverServer();
		if (serverAddress.length == 0) {
			System.err.println("No servers found");
			System.exit(1);
		}

		try
		{
			// Connect to first server
			RTNetClient client = new RTNetClient(serverAddress[0]);
						
			// Enumerate the different hardware configurations defined on the server
			HWConfig[] config = client.enumerateHWConfig();
			
			// This program requires at least one hardware configuration to be defined
			if (config.length == 0) {
				System.err.println("ERROR: no hardware configurations are available");
				System.exit(1);
			}
			
			// Print a list of all hardware configurations
			System.err.format("Found %d hardware configurations:\n", config.length);
			for (int iconfig = 0; iconfig < config.length; iconfig++)
				System.err.format("HWCONFIG[%d][address: %08X]: %s\n", 
						iconfig, config[iconfig].getAddressHandle(), config[iconfig].getName());

			// Get the address handle for the first available configuration
			int firstConfigAddress = config[0].getAddressHandle();
			
			// See if it has CX1 marker data and/or GS16AIO analog data
			boolean haveCX1 = false;
			boolean haveGS16AIO = false;
			HWConfigDeviceEnable[] deviceEnable = client.getHWConfigDeviceEnable(firstConfigAddress);
			for (int idevice = 0; idevice < deviceEnable.length; idevice++) {
				if (deviceEnable[idevice].isEnabled()) {
					if (deviceEnable[idevice].getDevice() == RTNetClientExtensionCX1.DEVICEID_CX1)
						haveCX1 = true;
					else if (deviceEnable[idevice].getDevice() == RTNetClientExtensionGS16AIO.DEVICEID_GS16AIO)
						haveGS16AIO = true;
				}
			}
			
			// Attemp to start the first available configuration
			client.startSystem(firstConfigAddress);
			
			// Codamotion cx1 functions
			RTNetClientExtensionCX1 clientCX1 = new RTNetClientExtensionCX1(client);
			
			// GS16AIO ADC functions
			RTNetClientExtensionGS16AIO clientGS16AIO = new RTNetClientExtensionGS16AIO(client);
			
			// Create stream to receive incoming data (UDP packets) 
			// The parameters mean this is on port 8050 and with 10ms timeout
			RTDataStream stream = client.createDataStream(8050, 10);

			// Begin separate thread for monitoring incoming data (see PacketMonitor class below)
			PacketMonitor monitorRunner = new PacketMonitor(stream);
			Thread monitorThread = new Thread(monitorRunner);
			monitorThread.start();

			// Limit to 2000 cx1 samples or GS16AIO samples
			if (haveCX1)
				client.setAcqMaxTicks(RTNetClientExtensionCX1.DEVICEID_CX1, 2000);
			else if (haveGS16AIO)
				client.setAcqMaxTicks(RTNetClientExtensionGS16AIO.DEVICEID_GS16AIO, 2000);
				
			// Prepare for acquisition
			client.prepareForAcq();
		
			// Start acquisition in buffered mode
			client.startAcqContinuousBuffered();

			// Trigger the buffer monitor function every 100ms.
			// This will cause data packets to be received by the PacketMonitor class 
			for (int t = 0; t < 20; t++) {
				Thread.sleep(100);
				client.monitorAcqBuffer();
			}
			
			// Stop acquisition
			client.stopAcq();

			// Kill the monitor class
			monitorRunner.requestStop();

			// Sleep to allow stop flag to be processed by monitor thread
			// - this must be >= the timeout set for the RTDataStream 
			Thread.sleep(10);			
			
			// Download marker data & print to standard output
			if (haveCX1) {
				// Find number of frames of 3D data and print to output
				int numFrames = client.getAcqBufferNumPackets(RTNetClientExtensionCX1.DEVICEID_CX1);
				System.out.println("\nMARKER FRAMES:\t" + numFrames);

				// New decoder class for 3D results
				PacketDecode3dResultExt decoder3D = new PacketDecode3dResultExt();
				
				// Request each frame in turn and print value to screen
				for (int i = 0; i < numFrames; i++) {
					client.requestAcqBufferPacket(RTNetClientExtensionCX1.DEVICEID_CX1, i);
					RTNetworkPacket pkt = stream.receivePacket();
					if (decoder3D.decode(pkt)) {
						int[] valid = decoder3D.getValid();
						float[] position = decoder3D.getPosition();
						if (valid[0] != 0)
							System.out.println(i + "\t" + position[0] + "\t" + position[1] + "\t" + position[2]);
						else
							System.out.println(i + "\t\t\t");
							
					}
					else {
						System.out.println(i + "\tDECODE ERROR");
					}
				}
				
			}

			// Download analog data & print to standard output
			if (haveGS16AIO) {				
				// Find number of frames of analog and print to output
				int numFrames = client.getAcqBufferNumPackets(RTNetClientExtensionGS16AIO.DEVICEID_GS16AIO);
				System.out.println("\nANALOG FRAMES:\t" + numFrames);

				// New decoder class for analog results
				PacketDecodeADC16 decoderADC16 = new PacketDecodeADC16();
				
				// Request each frame in turn and print value to screen
				for (int i = 0; i < numFrames; i++) {
					client.requestAcqBufferPacket(RTNetClientExtensionGS16AIO.DEVICEID_GS16AIO, i);
					RTNetworkPacket pkt = stream.receivePacket();
					if (decoderADC16.decode(pkt)) {
						short[] value = decoderADC16.getValue();
						System.out.println(i + "\t" + value[0]);
					}
					else {
						System.out.println(i + "\tDECODE ERROR");
					}
				}
			}
			
		}
		catch(RTException ex) {
			// RTNet exception occured - print details
			ex.printStackTrace();
		}
		catch (InterruptedException e) {
			// this is extremely unlikely - caused in the event that Thread.sleep fails
			e.printStackTrace();
		}

	}

	/**
	 * PacketMonitor class defines a thread to monitor incoming data packets from 
	 * the Codamotion RTNet server.
	 * 
	 */
	static class PacketMonitor implements Runnable {
		
		RTDataStream stream;
		boolean requestStop = false;

		/**
		 * PacketMonitor
		 * Construct monitor thread on the specified stream
		 * @param stream An input stream made using createDataStream
		 */
		public PacketMonitor(RTDataStream stream) {
			this.stream = stream;
		}
		
		/**
		 * requestStop
		 * Sets a flag which causes this monitoring thread to quit
		 */
		public void requestStop() {
			requestStop = true;
		}

		/**
		 * run
		 * Entry-point for the main monitoring thread
		 */
		public void run() {


			// decoders for different packet types:
			
			// 3D results decoder - for marker data
			PacketDecode3dResultExt decoder3D = new PacketDecode3dResultExt();
			
			// ADC16 decoder - for 16-bit digitized analog data
			PacketDecodeADC16 decoderADC16 = new PacketDecodeADC16();

			// Loop until a stop request is received
			while (!requestStop) {
				try {
					// receive the next available packet - may time out
					RTNetworkPacket pkt = stream.receivePacket();
					
					// packet receieved - retrieve the packet header
					RTNetworkPacket.Header h = pkt.getHeader();

					// display packet information for known packet types
					if (decoder3D.decode(pkt)) {
						float[] pos = decoder3D.getPosition();
						System.err.println(h.tick + ": " + pos[0] + " " + pos[1] + " " + pos[2]);
					}
					else if (decoderADC16.decode(pkt)) {
						short[] value = decoderADC16.getValue();
						System.err.println(h.tick + ": " + value[0]);
					}
				}
				catch (DataStreamException e) {
					// one bad packet or timeout - just continue
				}
				catch (ConnectionException e) {
					// network connection broken - must finish now
					e.printStackTrace();
					break;
				}
			}
		}
	}
}
