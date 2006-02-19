#Change PROJECT_NAME to the name of your project
PROJECT(binaryErodeDilate)

# set the name of the input image used to run the test
SET(INPUT_IMAGE ${CMAKE_SOURCE_DIR}/images/2th_cthead1.png)

#include some macros from another file...
INCLUDE(IJMacros.txt)

#The following lines are required to use Dart
ENABLE_TESTING()
INCLUDE(Dart)
FIND_PROGRAM(IMAGE_COMPARE ImageCompare)

#Declare any external dependencies that your project may have here.
#examples include: ITK, VTK, JPEG, PNG, OpenGL, ZLIB, Perl, Java
#If you're not sure what name to use, look in the Modules directory of your
#cmake install and check that a file named Find(Package).cmake exists
SET(Required_Packages
  ITK
)

#this foreach loads all of the packages that you specified as required.
#It shouldn't need to be modified.
FOREACH(Package ${Required_Packages})
  LOADPACKAGE(${Package})
ENDFOREACH(Package)

#Set any libraries that your project depends on.
#examples: ITKCommon, VTKRendering, etc
SET(Libraries
  ITKCommon
  ITKBasicFilters
  ITKIO
)


# install devel files
FILE(GLOB develFiles *.h *.txx) 
FOREACH(f ${develFiles})
  INSTALL_FILES(/include/InsightToolkit/BasicFilters FILES ${f})
ENDFOREACH(f)



# option for wrapping
OPTION(BUILD_WRAPPERS "Wrap library" OFF)
IF(BUILD_WRAPPERS)
   SUBDIRS(Wrapping)
ENDIF(BUILD_WRAPPERS)
   
   

#the following block of code is an example of how to build an executable in
#cmake.  Unmodified, it will add an executable called "MyExe" to the project.
#MyExe will be built using the files MyClass.h and MyClass.cxx, and it will
#be linked to all the libraries you specified above. 
#You can build more than one executable per project

SET(CurrentExe "erode_1pt")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "dilate_1pt")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "erode")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "dilate")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "perf")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "perf_old")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

#the following line is an example of how to add a test to your project.
#Testname is the title for this particular test.  ExecutableToRun is the
#program which will be running this test.  It can either be a part of this
#project or an external executable.  After that list any args that are needed
#for this test.  Include as many tests as you like.  If your project doesn't have
#any tests you can comment out or delete the following line.
# ADD_TEST(Testname ExecutableToRun arg1 arg2 arg3)

SET(PARAMS
  "200-0-0"
  "100-0-0"
  "200-0-1"
  "100-0-1"
  "200-150-0"
  "100-150-0"
  "200-150-1"
  "100-150-1"
)

FOREACH(param ${PARAMS})
  STRING(REGEX REPLACE "([0-9]*)-([0-9]*)-([0-9]*)" "\\1" fg "${param}")
  STRING(REGEX REPLACE "([0-9]*)-([0-9]*)-([0-9]*)" "\\2" bg "${param}")
  STRING(REGEX REPLACE "([0-9]*)-([0-9]*)-([0-9]*)" "\\3" border "${param}")

  ADD_TEST(Dilate-${param} dilate ${fg} ${bg} ${border} ${INPUT_IMAGE} dilate-${param}.png)
  ADD_TEST(CompareDilate-${param} ${IMAGE_COMPARE} dilate-${param}.png ${CMAKE_SOURCE_DIR}/images/dilate-${param}.png)

  ADD_TEST(Dilate1pt-${param} dilate_1pt ${fg} ${bg} ${border} ${INPUT_IMAGE} dilate_1pt-${param}.png)
  ADD_TEST(CompareDilate1pt-${param} ${IMAGE_COMPARE} dilate_1pt-${param}.png ${CMAKE_SOURCE_DIR}/images/dilate_1pt-${param}.png)

  ADD_TEST(Erode-${param} erode ${fg} ${bg} ${border} ${INPUT_IMAGE} erode-${param}.png)
  ADD_TEST(CompareErode-${param} ${IMAGE_COMPARE} erode-${param}.png ${CMAKE_SOURCE_DIR}/images/erode-${param}.png)

  ADD_TEST(Erode1pt-${param} erode_1pt ${fg} ${bg} ${border} ${INPUT_IMAGE} erode_1pt-${param}.png)
  ADD_TEST(CompareErode1pt-${param} ${IMAGE_COMPARE} erode_1pt-${param}.png ${CMAKE_SOURCE_DIR}/images/erode_1pt-${param}.png)
ENDFOREACH(param)
