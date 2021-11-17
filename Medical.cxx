

//Mark Capka
	//VR & Data Visualization - Assignment 3: 3D Scalar Data Visualizaiton
		//medical.cxx is the base file 

	//isosurface extraction


/*=========================================================================

  Program:   Visualization Toolkit
  Module:    Medical1.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

	 This software is distributed WITHOUT ANY WARRANTY; without even
	 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
	 PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
//
// This example reads a volume dataset, extracts an isosurface that
// represents the skin and displays it.
//

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVolume16Reader.h>
#include <vtkStructuredPointsReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkTextActor.h> 
#include <vtkTextProperty.h>
#include <vtkActor.h>
#include <vtkOutlineFilter.h>
#include <vtkCamera.h>
#include <vtkStripper.h>
#include <vtkLookupTable.h>
#include <vtkImageDataGeometryFilter.h>
#include <vtkProperty.h>
#include <vtkPolyDataNormals.h>
#include <vtkContourFilter.h>
#include <vtkImageData.h>
#include <vtkImageMapToColors.h>
#include <vtkImageActor.h>
#include <vtkSmartPointer.h>
#include <vtkImageMapper3D.h>
#include <vtkAxesActor.h>
#include <vtkCaptionActor2D.h>
#include <vtkScalarBarActor.h>



int main(int argc, char* argv[])
{

	// Create the renderer, the render window, and the interactor. The renderer
	// draws into the render window, the interactor enables mouse- and
	// keyboard-based interaction with the data within the render window.
	//
	vtkSmartPointer<vtkRenderer> aRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renWin =
		vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(aRenderer);

	// Set a background color for the renderer and set the size of the
// render window (expressed in pixels).
	aRenderer->SetBackground(.2, .3, .4);
	renWin->SetSize(640, 480);


	vtkSmartPointer<vtkRenderWindowInteractor> iren =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);



	// The following reader is used to read a series of 2D slices (images)
	// that compose the volume. The slice dimensions are set, and the
	// pixel spacing. The data Endianness must also be specified. The reader
	// uses the FilePrefix in combination with the slice number to construct
	// filenames using the format FilePrefix.%d. (In this case the FilePrefix
	// is the root name of the file: quarter.)

  //mark - removed per assignment instructions -> replaced with lines below
	//vtkSmartPointer<vtkVolume16Reader> v16 =
	//  vtkSmartPointer<vtkVolume16Reader>::New();
	//v16->SetDataDimensions (64,64);
	//v16->SetImageRange (1,93);
	//v16->SetDataByteOrderToLittleEndian();
	//v16->SetFilePrefix ("headsq/quarter");
	//v16->SetDataSpacing (3.2, 3.2, 2.5);


		//assignment 3 step 1: import the temperature data from temperature.dat file. Use vtkStructuredPointsReader to import the data. 
	//Mark - adding back from instructions: //week 7 lab 1 slide  8
	vtkSmartPointer<vtkStructuredPointsReader> v16 = vtkSmartPointer<vtkStructuredPointsReader>::New();
	v16->SetFileName("temperature.dat");
	v16->Update();


	// An outline provides context around the data.
	//
	vtkSmartPointer<vtkOutlineFilter> outlineData =
		vtkSmartPointer<vtkOutlineFilter>::New();
	outlineData->SetInputConnection(v16->GetOutputPort());

	vtkSmartPointer<vtkPolyDataMapper> mapOutline =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapOutline->SetInputConnection(outlineData->GetOutputPort());

	vtkSmartPointer<vtkActor> outline =
		vtkSmartPointer<vtkActor>::New();
	outline->SetMapper(mapOutline);
	outline->GetProperty()->SetColor(0, 0, 0);

	// Now create a lookup table that consists of the full hue circle
// (from HSV).
	vtkSmartPointer<vtkLookupTable> hueLut =
		vtkSmartPointer<vtkLookupTable>::New();
	hueLut->SetTableRange(0, 1);
	hueLut->SetHueRange(0.0, 0.667);	
	hueLut->SetSaturationRange(1.0, 1.0);
	hueLut->SetValueRange(1, 1);
	hueLut->Build(); //effective built

	// An isosurface, or contour value of 500 is known to correspond to the
	// skin of the patient. Once generated, a vtkPolyDataNormals filter is
	// is used to create normals for smooth surface shading during rendering.
	vtkSmartPointer<vtkContourFilter> skinExtractor =
		vtkSmartPointer<vtkContourFilter>::New();
	skinExtractor->SetInputConnection(v16->GetOutputPort());		
	skinExtractor->Update();

	//TODO Changing the below value changes which colors are displayed 1 at a time.

	//skinExtractor->SetValue(0, 0.1);
	//skinExtractor->SetValue(0, 0.2);
	//skinExtractor->SetValue(0, 0.3);
	//skinExtractor->SetValue(0, 0.5);
	skinExtractor->SetValue(0, 0.75); //week 7 lab 1 slide 11 //changing the value seems to change the color of the object. 







	vtkSmartPointer<vtkPolyDataNormals> skinNormals =
		vtkSmartPointer<vtkPolyDataNormals>::New();
	skinNormals->SetInputConnection(skinExtractor->GetOutputPort());
	skinNormals->SetFeatureAngle(60.0);

	vtkSmartPointer<vtkStripper> skinStripper =
		vtkSmartPointer<vtkStripper>::New();
	skinStripper->SetInputConnection(skinNormals->GetOutputPort());
	skinStripper->Update();



	vtkSmartPointer<vtkPolyDataMapper> skinMapper =				//triangulates the data
		vtkSmartPointer<vtkPolyDataMapper>::New();
	skinMapper->SetInputConnection(skinNormals->GetOutputPort());

	//skinMapper->ScalarVisibilityOff(); //Mark - commented out per week 7 lab 1 slide 12     //should allow us to see color now


	skinMapper->SetInputConnection(skinStripper->GetOutputPort());
	skinMapper->SetLookupTable(hueLut);
	skinMapper->UseLookupTableScalarRangeOn();
	skinMapper->Update();


	vtkSmartPointer<vtkActor> skin =
		vtkSmartPointer<vtkActor>::New();
	skin->SetMapper(skinMapper);


	
	//END ISOSURFACE


	//2b) probing method is below. 


	
	// Create the first  plane. The filter vtkImageMapToColors
	// maps the data through the corresponding lookup table created above.  The
	// vtkImageActor is a type of vtkProp and conveniently displays an image on
	// a single quadrilateral plane. It does this using texture mapping and as
	// a result is quite fast. (Note: the input image has to be unsigned char
	// values, which the vtkImageMapToColors produces.) Note also that by
	// specifying the DisplayExtent, the pipeline requests data of this extent
	// and the vtkImageMapToColors only processes a slice of data.


	//PROBING: for any of the probing methods, uncomment the 4 methods below relating to colors, sagittal, axial, and coronal methods.



	vtkSmartPointer<vtkImageMapToColors> colors = vtkSmartPointer<vtkImageMapToColors>::New();
	colors->SetInputConnection(v16->GetOutputPort());
	colors->SetLookupTable(hueLut);
	colors->Update();

	//18x18x10: 
	//FOR EACH OF THE BELOW, UNCOMMENT THE SEGMENTS YOU WOULD LIKE DISPLAYED.


	vtkSmartPointer<vtkImageActor> sagittal = vtkSmartPointer<vtkImageActor>::New();
	sagittal->GetMapper()->SetInputConnection(colors->GetOutputPort());
	//	sagittal->SetDisplayExtent(0,0, 0,17, 0,9); //only 9, 0-17, 0-9
		sagittal->SetDisplayExtent(4,4, 0,17, 0,9); //only 9, 0-17, 0-9
	//sagittal->SetDisplayExtent(9, 9, 0, 17, 0, 9); //only 9, 0-17, 0-9
	//	sagittal->SetDisplayExtent(13,13, 0,17, 0,9); //only 9, 0-17, 0-9
	//sagittal->SetDisplayExtent(17, 17, 0, 17, 0, 9); //only 9, 0-17, 0-9

	vtkSmartPointer<vtkImageActor> axial = vtkSmartPointer<vtkImageActor>::New();
	axial->GetMapper()->SetInputConnection(colors->GetOutputPort());
	//axial->SetDisplayExtent(0,17, 0,0, 0,9);//0-18, only 9 , 0-9 
	axial->SetDisplayExtent(0,17, 4,4, 0,9);//0-18, only 9 , 0-9 
	//axial->SetDisplayExtent(0,17, 9,9, 0,9);//0-18, only 9 , 0-9 
	//axial->SetDisplayExtent(0,17, 13,13, 0,9);//0-18, only 9 , 0-9 
	//axial->SetDisplayExtent(0,17, 17,17, 0,9);//0-18, only 9 , 0-9 



	vtkSmartPointer<vtkImageActor> coronal = vtkSmartPointer<vtkImageActor>::New();
	coronal->GetMapper()->SetInputConnection(colors->GetOutputPort());
	//coronal->SetDisplayExtent(0,17, 0,17, 0,0);	 
	coronal->SetDisplayExtent(0,17, 0,17, 3,3);	
	//coronal->SetDisplayExtent(0,17, 0,17, 6,6);	
	//coronal->SetDisplayExtent(0,17, 0,17, 9,9);	




	//END PROBING

	// Setup the text and add it to the renderer	//added per assignment 3 step 3: scale bars and labels
	vtkSmartPointer<vtkTextActor> textActor = vtkSmartPointer<vtkTextActor>::New();
	textActor->SetInput("Iso-Surfaces and Probing for temperature variation:");	//title
	textActor->SetPosition(200, 40);						//location in the program from the bottom left corner
	textActor->GetTextProperty()->SetFontSize(24);			//font size
	textActor->GetTextProperty()->SetColor(1.0, 1.0, 1.0);	//color of text

	//
	////adding axes - assignment 3 part 3 
	vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();

	//x axis
	axes->GetXAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
	axes->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(1, 0, 0);	
	axes->SetXAxisLabelText("X");
	axes->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(15);

	//y axis
	axes->GetYAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
	axes->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(1, 0, 0);
	axes->SetYAxisLabelText("Y");
	axes->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(15);

	//z axis
	axes->GetZAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
	axes->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(1, 0, 0);
	axes->SetZAxisLabelText("Z");
	axes->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(15);
	
	
	axes->SetTotalLength(5.0, 5.0, 5.0);	//setting the length of the x, y, z axes


	//scalar bar added to scene: 
	vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<vtkScalarBarActor>::New();

	scalarBar->UnconstrainedFontSizeOn();
	scalarBar->SetTitle("Temperature"); //scalar bar label
	scalarBar->SetNumberOfLabels(5);	//number of labels displayed on the scalar bar

	scalarBar->SetLookupTable(hueLut);
	scalarBar->GetPositionCoordinate()->SetValue(.125, .125);

	scalarBar->SetWidth(0.125);	//bar width
	scalarBar->SetHeight(0.75);	//bar heigth




	

	// Now we are creating three orthogonal planes passing through the
	// volume. Each plane uses a different texture map and therefore has
	// different coloration.




	// It is convenient to create an initial view of the data. The FocalPoint
	// and Position form a vector direction. Later on (ResetCamera() method)
	// this vector is used to position the camera to look at the data in
	// this direction.
	vtkSmartPointer<vtkCamera> aCamera =
		vtkSmartPointer<vtkCamera>::New();
	aCamera->SetViewUp(0, 1, 0); //changed per week 7 lab 1 slide 15
	aCamera->SetPosition(0, 0, 1); //changed per week 7 lab 1 slide 15
	aCamera->SetFocalPoint(0, 0, 0);
	aCamera->ComputeViewPlaneNormal();
	aCamera->Azimuth(0.0); //changed per week 7 lab 1 slide 15
	aCamera->Elevation(0.0);//changed per week 7 lab 1 slide 15

	// Actors are added to the renderer. An initial camera view is created.
	// The Dolly() method moves the camera towards the FocalPoint,
	// thereby enlarging the image.
	
	aRenderer->AddActor(outline);
	
	
	//ISO-SURFACE uncomment the below line along with any of the skin methods
	aRenderer->AddActor(skin); //uncomment for isosurface display


	//PROBING: have the below 3 lines uncommented, along with the colors and sagittal, axial, and coronal methods
	aRenderer->AddActor(sagittal);
	aRenderer->AddActor(axial);
	aRenderer->AddActor(coronal);


	//labels and titles and annotations
	aRenderer->AddActor(textActor);
	aRenderer->AddActor(axes);
	aRenderer->AddActor(scalarBar);

	
	aRenderer->SetActiveCamera(aCamera);

	aRenderer->ResetCamera();
	aCamera->Dolly(1.0); //changed per week 7 lab 1 slide 15


	// Note that when camera movement occurs (as it does in the Dolly()
	// method), the clipping planes often need adjusting. Clipping planes
	// consist of two planes: near and far along the view direction. The
	// near plane clips out objects in front of the plane; the far plane
	// clips out objects behind the plane. This way only what is drawn
	// between the planes is actually rendered.
	aRenderer->ResetCameraClippingRange();

	// Initialize the event loop and then start it.
	iren->Initialize();
	iren->Start();

	return EXIT_SUCCESS;
}
