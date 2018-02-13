/***************************************************************************/
/*                                                                         */
/*                           AutoRot & Perspectives                        */
/*                                                                         */
/***************************************************************************/

/*    (c) Copyright 1994, 1995 - PsyPhy Consulting. All Rights Reserved.    */

/*
*	Test the 2D Graphics Views and Displays systems.
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <useful.h>
#include <windows.h>
#include <commdlg.h>

#include "../PsyPhy2DGraphics/PsyPhy2dGraphicsLib/OglDisplay.h"
#include "../PsyPhy2DGraphics/PsyPhy2dGraphicsLib/Graphics.h"
#include "../PsyPhy2DGraphics/PsyPhy2dGraphicsLib/Displays.h"
#include "../PsyPhy2DGraphics/PsyPhy2dGraphicsLib/Views.h"
#include "../PsyPhy2DGraphics/PsyPhy2dGraphicsLib/Layouts.h"

//#include <2dMatrix.h>
//#include <Regression.h>

//#include "ogldisplayinterface.h"

#define MAX_SAMPLES 100000
#define MAX_FRAMES 100000
#define MAX_MARKERS 56
#define MAX_CHANNELS 32

#define NAN -999.99

int nChannels = 32;
int nMarkers = 56;

int emgChannel[MAX_CHANNELS] = {0, 1, 2, 3, 4, 5, 12, 13, 14, 15};
int emgChannels = 10;
int accChannel[MAX_CHANNELS] = {6, 7, 8, 9, 10, 11};
int accChannels = 6;

int autoRotMarker[MAX_MARKERS] = { 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55 };
int autoRotMarkers = 48;
int perspectivesMarker[MAX_MARKERS] = { 0, 1, 2, 3, 4, 5, 6, 7 };
int perspectivesMarkers = 8;

double min_x = -1000.0;
double max_x = 1500;
double min_y = -2000.0;
double max_y = 2000.0;
double min_z = 0.0;
double max_z = 2000;


Display display; 

// EMG plots
Layout emgLayout;

// Acc plots
Layout accLayout;

// Phase plots of markers
View	autoRotView;
View	perspectivesView;

// Identify which graph goes with which experiment by a colored border.
int autoRotColor = MAGENTA;
int perspectivesColor = CYAN;

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

	autoRotView = CreateView( display );
	ViewSetDisplayEdgesRelative( autoRotView, 0.01, 0.01, 0.5, 1.0 );
	ViewSetEdges( autoRotView, min_x, min_y, max_x, max_y );
	ViewMakeSquare( autoRotView );
	ViewColor( autoRotView, autoRotColor );
	ViewBox( autoRotView );

	perspectivesView = CreateView( display );
	ViewSetDisplayEdgesRelative( perspectivesView, 0.51, 0.66, 1.0, 1.0 );
	ViewSetEdges( perspectivesView, min_x, min_z, max_x, max_z );
	ViewMakeSquare( perspectivesView );
	ViewColor( perspectivesView, perspectivesColor );
	ViewBox( perspectivesView );

	emgLayout = CreateLayout( display, emgChannels, 1 );
	LayoutSetDisplayEdgesRelative( emgLayout, 0.51, 0.25, 1.0, 0.65 );
	Color( display, autoRotColor );
	LayoutBox( emgLayout );

	accLayout = CreateLayout( display, accChannels, 1 );
	LayoutSetDisplayEdgesRelative( accLayout, 0.51, 0.0, 1.0, 0.24 );
	Color( display, autoRotColor );
	LayoutBox( accLayout );

	if ( frames > 0 ) {
		for ( mrk = 0; mrk < perspectivesMarkers; mrk++ ) {
			int mm = perspectivesMarker[mrk];
			ViewSelectColor( perspectivesView, mrk );
			ViewScatterPlotAvailableFloats( perspectivesView, SYMBOL_FILLED_SQUARE, &position[0][mm][X], &position[0][mm][Z], 0, frames - 1, sizeof( position[0] ), sizeof( position[0] ), NAN );
		}
		for ( mrk = 0; mrk < autoRotMarkers; mrk++ ) {
			int mm = autoRotMarker[mrk];
			ViewSelectColor( autoRotView, mrk );
			ViewScatterPlotAvailableFloats( autoRotView, SYMBOL_FILLED_SQUARE, &position[0][mm][X], &position[0][mm][Y], 0, frames - 1, sizeof( position[0] ), sizeof( position[0] ), NAN );
		}
	}
	if ( samples > 0 ){

		int bytes =  sizeof( adcValue[0] );
		for ( chan = 0; chan < emgChannels; chan++ ) {
			View view;
			view = LayoutView( emgLayout, chan, 0 );
			ViewSelectColor( view, chan );
			ViewAutoScaleInit( view );
			ViewSetXLimits( view, 0, samples );
			ViewAutoScaleShorts( view, &adcValue[0][emgChannel[chan]], 0, samples - 1, sizeof( adcValue[0] ));
			ViewPlotShorts( view, &adcValue[0][emgChannel[chan]], 0, samples - 1, sizeof( adcValue[0] ));
		}

		for ( chan = 0; chan < accChannels; chan++ ) {
			View view;
			view = LayoutView( accLayout, chan, 0 );
			ViewSelectColor( view, chan );
			ViewAutoScaleInit( view );
			ViewSetXLimits( view, 0, samples );
			ViewAutoScaleShorts( view, &adcValue[0][accChannel[chan]], 0, samples - 1, sizeof( adcValue[0] ));
			ViewPlotShorts( view, &adcValue[0][accChannel[chan]], 0, samples - 1, sizeof( adcValue[0] ));
		}
	}
#if 0
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

		//		vis1Dview->user_top = vis1Dview->user_top + 1;
		//		vis1Dview->user_bottom = vis1Dview->user_bottom - 1;
		for ( mrk = 0; mrk < perspectivesMarkers; mrk++ ) {
			int mm = perspectivesMarker[mrk];
			ViewSelectColor( vis1Dview, mrk );
			ViewPlotShorts( vis1Dview, &visible[0][mm], 0, frames - 1, sizeof( visible[0] ) );
		}
	}


	}
#endif 

	ViewColor( autoRotView, autoRotColor );
//	ViewTitle(autoRotView, fileroot, INSIDE_LEFT, INSIDE_TOP, 0.0 );
	ViewTitle(autoRotView, "AutoRotation", INSIDE_LEFT, INSIDE_TOP, 0.0 );

	ViewColor( perspectivesView, perspectivesColor );
	ViewTitle(perspectivesView, "Perspectives", INSIDE_LEFT, INSIDE_TOP, 0.0 );

	Swap();
	RunOglWindow();

	Close( display );

	return( 0 );


}

