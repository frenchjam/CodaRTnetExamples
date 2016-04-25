/***************************************************************************/
/*                                                                         */
/*                               grtest.c                                  */
/*                                                                         */
/***************************************************************************/

/*    (c) Copyright 1994, 1995 - PsyPhy Consulting. All Rights Reserved.    */

/*
*	Test the 2D Graphics Views and Displays systems.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <useful.h>
#include <windows.h>
#include <commdlg.h>

#include "../../GripMMI/GripMMI (Visual C++ 2010 Express)/PsyPhy2dGraphicsLib/OglDisplay.h"
#include "../../GripMMI/GripMMI (Visual C++ 2010 Express)/PsyPhy2dGraphicsLib/Graphics.h"
#include "../../GripMMI/GripMMI (Visual C++ 2010 Express)/PsyPhy2dGraphicsLib/Displays.h"
#include "../../GripMMI/GripMMI (Visual C++ 2010 Express)/PsyPhy2dGraphicsLib/Views.h"
#include "../../GripMMI/GripMMI (Visual C++ 2010 Express)/PsyPhy2dGraphicsLib/Layouts.h"

//#include <2dMatrix.h>
//#include <Regression.h>

//#include "ogldisplayinterface.h"

#define MAX_SAMPLES 100000
#define MAX_FRAMES 100000
#define MAX_MARKERS 56

#define NAN -999.99

int nChannels = 32;
int nMarkers = 28;

// Display was incorrect for first two flights. Changing the mapping to plots
//  here changes nothing in the data files.
// Erroneous mapping for flights 1 and 2
// int emg3Dchannel[32] = { 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 28, 29, 30 };
// Correct mapping.
//int emg3Dchannel[32] = { 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 27, 28, 29, 30 };
int emg3Dchannel[32] = { 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 27, 28, 29, 30 };
int emg3Dchannels = 14;

// Erroneous mapping for flights 1 and 2
// int emg1Dchannel[32] = {14, 15, 27, 31};
// Correct mapping.
int emg1Dchannel[32] = {14, 15, 26, 31};
int emg1Dchannels = 4;

int accPchannel = 7;
int tgt1Dchannel = 6;

//int plot3Dmarker[MAX_MARKERS] = { 8, 9, 12, 13, 14, 17, 18, 19, 20, 21, 22, 23 };
//int plot3Dmarkers = 12;
int plot3Dmarker[MAX_MARKERS] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55 };
int plot3Dmarkers = 56;

int plot1Dmarker[MAX_MARKERS] = { 0,1,2,3,4,5,6,7 };
int plot1Dmarkers = 8;

int atiChannel[2][6] = {{ 0, 1, 2, 3, 4, 5 }, {8, 9, 10, 11, 12, 13}};

int color3D = RED;
int color1D = GREEN;

Display display; 

// EMG for 3D experiment
Layout emg3Dlayout;

// EMG for 1D experiment
Layout emg1Dlayout;

// Manipulandum Data
Layout atiLayout;

// Accelerometer data and tangential velocities.

Layout misc_layout;
View	accPview;
View	vis3Dview;
View	vis1Dview;
View	tgt1Dview;

View	xy_view;
View	xZ_view;

short adcValue[MAX_SAMPLES][32];
unsigned long adcSample[MAX_SAMPLES];
unsigned long adcTick[MAX_SAMPLES];

float position[MAX_FRAMES][MAX_MARKERS][3];
short int visible[MAX_FRAMES][MAX_MARKERS];

unsigned long cx1Frame[MAX_FRAMES];
unsigned long cx1Tick[MAX_FRAMES];

#define X 0
#define Y 1
#define Z 2


int main ( int argc, char *argv[] ) {

	char *fileroot = "";

	if ( argc > 1 ) fileroot = argv[1];

	char filename[1024];

	int samples = 0;
	int chan;
	int value;

	int frames = 0;
	int mrk;

	int bytes;

	if ( strlen( fileroot ) == 0 ) {
		OPENFILENAME ofn;
		char dialogFilename[1024] = "";

		memset(&ofn,0,sizeof(ofn)); 
		ofn.lStructSize = sizeof(ofn);    
		ofn.hwndOwner = NULL;     
		ofn.hInstance = NULL;     
		ofn.lpstrFilter = "Coda Files\0*.mrk\0\0";         
		ofn.nMaxFile = sizeof( dialogFilename );     
		ofn.lpstrFile = dialogFilename;
		ofn.lpstrTitle = "Please Select A File To Open";     
		ofn.Flags = OFN_NONETWORKBUTTON | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY; 

		GetOpenFileName( &ofn );
		dialogFilename[ strlen( dialogFilename ) - 4 ] = 0;
		fileroot = dialogFilename;
	}

	FILE *fp;

	sprintf( filename, "%s.mrk", fileroot );

	fp = fopen( filename, "r" );
	if ( !fp ) {
		char message[2048];
		sprintf( message, "Error opening %s for read.", filename );
		//		MessageBox( NULL, message, "RTnetPlotData", MB_OK | MB_ICONEXCLAMATION );
	}
	else {
		for ( frames = 0; frames < MAX_FRAMES; frames++ ) {
			if ( 2 != fscanf( fp, "%d %d", &cx1Frame[frames], &cx1Tick[frames] ) ) break;
			for ( mrk = 0; mrk < nMarkers; mrk++ ) {
				int value;
				fscanf( fp, "%d %f %f %f", &value, 
					&position[frames][mrk][X], &position[frames][mrk][Y], &position[frames][mrk][Z] );
				visible[frames][mrk] = ( value ? mrk * 2 + 1 : mrk * 2 );
				if ( !value ) position[frames][mrk][X] = position[frames][mrk][Y] = position[frames][mrk][Z] = NaN;
			}
		}
		fclose( fp );
	}
	fprintf( stderr, "Frames: %d\n", frames );

	sprintf( filename, "%s.adc", fileroot );
	fp = fopen( filename, "r" );
	if ( !fp ) {
		char message[2048];
		sprintf( message, "Error opening %s for read.", filename );
		//		MessageBox( NULL, message, "RTnetPlotData", MB_OK | MB_ICONEXCLAMATION );
	}
	else {

		for ( samples = 0; samples < MAX_SAMPLES; samples++ ) {
			if ( 2 != fscanf( fp, "%d %d", &adcSample[samples], &adcTick[samples] ) ) break;
			for ( chan = 0; chan < nChannels; chan++ ) {
				fscanf( fp, "%d", &value );
				adcValue[samples][chan] = value;
			}
		}
		fclose( fp );
	}
	fprintf( stderr, "Samples: %d\n", samples );

	display = DefaultDisplay();
	DisplayInit( display );
	Erase( display );

	emg3Dlayout = CreateLayout( display, emg3Dchannels, 1 );
	LayoutSetDisplayEdgesRelative( emg3Dlayout, 0.51, 0.25, 1.0, 1.0 );
	Color( display, color3D );
	LayoutBox( emg3Dlayout );

	emg1Dlayout = CreateLayout( display, emg1Dchannels, 1 );
	LayoutSetDisplayEdgesRelative( emg1Dlayout, 0.01, 0.25, 0.5, 0.40 );
	Color( display, color1D );
	LayoutBox( emg1Dlayout );

	atiLayout = CreateLayout( display, 2, 1 );
	LayoutSetDisplayEdgesRelative( atiLayout, 0.01, 0.40, 0.5, 0.55 );
	Color( display, color1D );
	LayoutBox( atiLayout );

	misc_layout = CreateLayout( display, 2, 2 );
	LayoutSetDisplayEdgesRelative( misc_layout, 0.01, 0.0, 1.0, 0.25 );

	vis3Dview = LayoutView( misc_layout, 0, 1 );
	vis1Dview = LayoutView( misc_layout, 0, 0 );
	tgt1Dview = LayoutView( misc_layout, 1, 0 );
	accPview = LayoutView( misc_layout, 1, 1 );

	xy_view = CreateView( display );
	ViewSetDisplayEdgesRelative( xy_view, 0.01, 0.55, 0.5, 1.0 );
	ViewSetEdges( xy_view, -2500.0, -1500.0, 1000, 1000.0 );
	ViewMakeSquare( xy_view );
	ViewColor( xy_view, GREY4 );
	ViewBox( xy_view );

	ViewSetXLimits( vis3Dview, 0, frames );
	ViewColor( vis3Dview, color3D );
	ViewBox( vis3Dview );
	ViewSetYLimits( vis3Dview, -1, plot3Dmarkers * 2 );

	ViewSetXLimits( vis1Dview, 0, frames );
	ViewSetYLimits( vis1Dview, 16 * 2 - 1, 16 * 2 + plot1Dmarkers * 2 );
	ViewColor( vis1Dview, color1D );
	ViewBox( vis1Dview );

	if ( frames > 0 ) {
		ViewAutoScaleInit( vis3Dview );
		ViewSetXLimits( vis3Dview, 0, frames - 1 );
		for ( mrk = 0; mrk < plot3Dmarkers; mrk++ ) {
			int mm = plot3Dmarker[mrk];
			ViewSelectColor( xy_view, mrk );
			ViewXYPlotAvailableFloats( xy_view, &position[0][mm][X], &position[0][mm][Y], 0, frames - 1, sizeof( position[0] ), sizeof( position[0] ), NAN );
			ViewAutoScaleShorts( vis3Dview,  &visible[0][mm], 0, frames - 1, sizeof( visible[0] ));
		}
		vis3Dview->user_top = vis3Dview->user_top + 1;
		vis3Dview->user_bottom = vis3Dview->user_bottom - 1;
		for ( mrk = 0; mrk < plot3Dmarkers; mrk++ ) {
			int mm = plot3Dmarker[mrk];
			ViewSelectColor( vis3Dview, mrk );
			ViewPlotShorts( vis3Dview, &visible[0][mm], 0, frames - 1, bytes = sizeof( visible[0] ) );
		}

		ViewAutoScaleInit( vis1Dview );
		ViewSetXLimits( vis1Dview, 0, frames - 1 );
		for ( mrk = 0; mrk < plot1Dmarkers; mrk++ ) {
			int mm = plot1Dmarker[mrk];
			ViewSelectColor( xy_view, mrk );
			ViewXYPlotAvailableFloats( xy_view, &position[0][mm][X], &position[0][mm][Y], 0, frames - 1, sizeof( position[0] ), sizeof( position[0] ), NAN );
			ViewAutoScaleShorts( vis1Dview, &visible[0][mm], 0, frames - 1, sizeof( visible[0] ));
		}
		//		vis1Dview->user_top = vis1Dview->user_top + 1;
		//		vis1Dview->user_bottom = vis1Dview->user_bottom - 1;
		for ( mrk = 0; mrk < plot1Dmarkers; mrk++ ) {
			int mm = plot1Dmarker[mrk];
			ViewSelectColor( vis1Dview, mrk );
			ViewPlotShorts( vis1Dview, &visible[0][mm], 0, frames - 1, sizeof( visible[0] ) );
		}
	}

	if ( samples > 0 ){
		ViewSetXLimits( tgt1Dview, 0, samples );
		ViewSetYLimits( tgt1Dview, 0, 10000 );
		ViewColor( tgt1Dview, color1D );
		ViewBox( tgt1Dview );

		ViewSetColor( tgt1Dview, BLUE );
		ViewPlotShorts( tgt1Dview, &adcValue[0][tgt1Dchannel], 0, samples - 1, sizeof( adcValue[0] ));

		ViewSetXLimits( accPview, 0, samples );
		ViewAutoScaleInit( accPview );
		ViewSetXLimits( accPview, 0, samples );
		ViewAutoScaleShorts( accPview, &adcValue[0][accPchannel], 0, samples - 1, sizeof( adcValue[0] ));
		ViewSetColor( accPview, GREY4 );
		ViewBox( accPview );

		ViewSetColor( accPview, MAGENTA );
		ViewPlotShorts( accPview, &adcValue[0][accPchannel], 0, samples - 1, sizeof( adcValue[0] ));

		int bytes =  sizeof( adcValue[0] );
		for ( chan = 0; chan < emg1Dchannels; chan++ ) {
			View view;
			view = LayoutView( emg1Dlayout, chan, 0 );
			ViewSelectColor( view, chan );
			ViewAutoScaleInit( view );
			ViewSetXLimits( view, 0, samples );
			ViewAutoScaleShorts( view, &adcValue[0][emg1Dchannel[chan]], 0, samples - 1, sizeof( adcValue[0] ));
			ViewPlotShorts( view, &adcValue[0][emg1Dchannel[chan]], 0, samples - 1, sizeof( adcValue[0] ));
		}

		for ( chan = 0; chan < emg3Dchannels; chan++ ) {
			View view;
			view = LayoutView( emg3Dlayout, chan, 0 );
			ViewSelectColor( view, chan );
			ViewAutoScaleInit( view );
			ViewSetXLimits( view, 0, samples );
			ViewAutoScaleShorts( view, &adcValue[0][emg3Dchannel[chan]], 0, samples - 1, sizeof( adcValue[0] ));
			ViewPlotShorts( view, &adcValue[0][emg3Dchannel[chan]], 0, samples - 1, sizeof( adcValue[0] ));
		}

		for ( int ati = 0; ati < 2; ati++ ) {
			View view;
			view = LayoutView( atiLayout, atiChannel[ati][chan], 0 );
			ViewAutoScaleInit( view );
			ViewSetXLimits( view, 0, samples );
			for ( chan = 0; chan < 6; chan++ ) {
				ViewAutoScaleShorts( view, &adcValue[0] [atiChannel[ati][chan]], 0, samples - 1, sizeof( adcValue[0] ));
			}
			for ( chan = 0; chan < 6; chan++ ) {
				ViewSelectColor( view, chan );
				ViewPlotShorts( view, &adcValue[0][atiChannel[ati][chan]], 0, samples - 1, sizeof( adcValue[0] ));
			}
		}
	}
	ViewColor( xy_view, BLUE );
	ViewTitle(xy_view, fileroot, INSIDE_LEFT, INSIDE_TOP, 0.0 );

	RunOglWindow();

	Close( display );

	return( 0 );


}

