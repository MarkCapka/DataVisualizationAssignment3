cmake_minimum_required(VERSION 3.3)


PROJECT (Medical)

find_package(VTK COMPONENTS
  vtkFiltersGeometry
  vtkFiltersModeling
  vtkIOImage
  vtkIOXML
  vtkIOLegacy					#adding so that we can probe 
  vtkRenderingAnnotation		#adding so that we can add annotations to our visualized data
  vtkRenderingLabel				#adding so that we can add labels to our visualized data
  vtkImagingStatistics
  vtkInteractionStyle
  vtkRenderingVolumeOpenGL2
  vtkTestingRendering
  vtksys
)
include(${VTK_USE_FILE})

add_executable(Medical Medical.cxx)
target_link_libraries(Medical ${VTK_LIBRARIES})

