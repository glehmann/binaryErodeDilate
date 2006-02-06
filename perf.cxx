#include "itkImageFileReader.h"

#include "itkBinaryDilateImageFilter.h"
/*#include "itkBinaryErodeImageFilter.h"*/
#include "itkBinaryBallStructuringElement.h"

#include "itkTimeProbe.h"
#include <vector>
#include "itkMultiThreader.h"

int main(int, char * argv[])
{
  itk::MultiThreader::SetGlobalMaximumNumberOfThreads(1);

  const int dim = 3;
  typedef unsigned char PType;
  typedef itk::Image< PType, dim >    IType;
  
  // read the input image
  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  
  typedef itk::BinaryBallStructuringElement< PType, dim> KernelType;
  KernelType kernel;
  KernelType::SizeType kernelSize;
  kernelSize.Fill( 20 );
  kernelSize[2] = 7;
  kernel.SetRadius(kernelSize);
  kernel.CreateStructuringElement();
   
  typedef itk::BinaryDilateImageFilter< IType, IType, KernelType > DilateType;
  DilateType::Pointer dilate = DilateType::New();
  dilate->SetInput( reader->GetOutput() );
  dilate->SetKernel( kernel );
  
/*  typedef itk::BinaryErodeImageFilter< IType, IType, KernelType > ErodeType;
  ErodeType::Pointer erode = ErodeType::New();
  erode->SetInput( reader->GetOutput() );
  erode->SetKernel( kernel );*/
  
  reader->Update();
  
  
  std::cout << "#b" << "\t" 
            << "fg" << "\t" 
            << "erode" << "\t" 
            << "dilate" << "\t" 
            << std::endl;

  for( int b=0; b<2; b++ )
    {
    dilate->SetBoundaryIsForeground( b );
//     erode->SetBoundaryIsForeground( b );

    for( int v=100; v<=200; v+=100 )
      {
      dilate->SetForegroundValue( v );
/*      erode->SetForegroundValue( v );*/
  
      itk::TimeProbe etime;
      itk::TimeProbe dtime;
  
      for( int i=0; i<1; i++ )
        {
        etime.Start();
/*        erode->Update();*/
        etime.Stop();
/*        erode->Modified();*/
  
        dtime.Start();
        dilate->Update();
        dtime.Stop();
        dilate->Modified();
  
        }
        
      std::cout << b << "\t" 
                << v << "\t" 
                << etime.GetMeanTime() << "\t" 
                << dtime.GetMeanTime() << "\t" 
                << std::endl;
      }
    }
  return 0;
}

