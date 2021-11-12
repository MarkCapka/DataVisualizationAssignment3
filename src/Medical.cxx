

//Mark Capka
    //VR & Data Visualization - assignment 3: 3D Scalar Data Visualizaiton



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
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkOutlineFilter.h>
#include <vtkCamera.h>
#include <vtkProperty.h>
#include <vtkPolyDataNormals.h>
#include <vtkContourFilter.h>
#include <vtkSmartPointer.h>
#include <vtkStructuredPointsReader.h>


int main (int argc, char *argv[])
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


  // An isosurface, or contour value of 500 is known to correspond to the
  // skin of the patient. Once generated, a vtkPolyDataNormals filter is
  // is used to create normals for smooth surface shading during rendering.
  vtkSmartPointer<vtkContourFilter> skinExtractor =
    vtkSmartPointer<vtkContourFilter>::New();
  skinExtractor->SetInputConnection(v16->GetOutputPort());
    

  //TODO will only display this portion in this color? lower values will be redish, higher values will be bluish
  skinExtractor->SetValue(0, 0.5); //week 7 lab 1 slide 11 //changing the value seems to change the color of the object. 
  



  //TODO generate iso-surfaces for various iso-values (0.25, 0.5, and so on)


  //TODO assignment 3 step 2: in depth analysis: 
    //a) Iso-surface extraction
    //b) probing method: 


    //a) 
        //extract iso-surfaces for different iso values by using the marching cube algorithm. 
        //triangulate the data using vtkPolyDataMapper
    


    //b) probe the temperature data at regular intervals along x,y,z. As data dimension is 18x18x10, analyze the data by taking cross sections at regular intervals from x,y,z. see pdf for example. 
       


  //TODO step 3: Use Scale Bars and labels
   // use a scale bar to visualize the range of data shown in the figures, label the output
    


  //TODO step 4: visual analysis: 
    //analyze and explore temperature distrubtion inside the region. Write down your own visual analysis -> export to pdf named: 3DScalarVisualization.pdf

  







  vtkSmartPointer<vtkPolyDataNormals> skinNormals =
    vtkSmartPointer<vtkPolyDataNormals>::New();
  skinNormals->SetInputConnection(skinExtractor->GetOutputPort());
  skinNormals->SetFeatureAngle(60.0);

  vtkSmartPointer<vtkPolyDataMapper> skinMapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  skinMapper->SetInputConnection(skinNormals->GetOutputPort());
  
  //skinMapper->ScalarVisibilityOff(); //Mark - commented out per week 7 lab 1 slide 12     //should allow us to see color now

  vtkSmartPointer<vtkActor> skin =
    vtkSmartPointer<vtkActor>::New();
  skin->SetMapper(skinMapper);

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
  outline->GetProperty()->SetColor(0,0,0);

  // It is convenient to create an initial view of the data. The FocalPoint
  // and Position form a vector direction. Later on (ResetCamera() method)
  // this vector is used to position the camera to look at the data in
  // this direction.
  vtkSmartPointer<vtkCamera> aCamera =
    vtkSmartPointer<vtkCamera>::New();
  aCamera->SetViewUp (0, 1, 0); //changed per week 7 lab 1 slide 15
  aCamera->SetPosition (0, 0, 1); //changed per week 7 lab 1 slide 15
  aCamera->SetFocalPoint (0, 0, 0);
  aCamera->ComputeViewPlaneNormal();
  aCamera->Azimuth(0.0); //changed per week 7 lab 1 slide 15
  aCamera->Elevation(0.0);//changed per week 7 lab 1 slide 15

  // Actors are added to the renderer. An initial camera view is created.
  // The Dolly() method moves the camera towards the FocalPoint,
  // thereby enlarging the image.
  aRenderer->AddActor(outline);
  aRenderer->AddActor(skin);
  aRenderer->SetActiveCamera(aCamera);
  aRenderer->ResetCamera ();
  aCamera->Dolly(1.0); //changed per week 7 lab 1 slide 15

  // Set a background color for the renderer and set the size of the
  // render window (expressed in pixels).
  aRenderer->SetBackground(.2, .3, .4);
  renWin->SetSize(640, 480);

  // Note that when camera movement occurs (as it does in the Dolly()
  // method), the clipping planes often need adjusting. Clipping planes
  // consist of two planes: near and far along the view direction. The
  // near plane clips out objects in front of the plane; the far plane
  // clips out objects behind the plane. This way only what is drawn
  // between the planes is actually rendered.
  aRenderer->ResetCameraClippingRange ();

  // Initialize the event loop and then start it.
  iren->Initialize();
  iren->Start();

  return EXIT_SUCCESS;
}
