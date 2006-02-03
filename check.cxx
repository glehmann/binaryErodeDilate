#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkSimpleFilterWatcher.h"

#include "itkBinaryDilateImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "itkNeighborhood.h"


int main(int, char * argv[])
{
  const int dim = 2;
  
  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

//   typedef itk::BinaryBallStructuringElement< PType, dim > SRType;
//   SRType kernel;
//   kernel.SetRadius( 1 );
//   kernel.CreateStructuringElement();

  typedef itk::Neighborhood<PType, dim> SRType;
  SRType kernel;
  kernel.SetRadius( 2 );
  for( SRType::Iterator kit=kernel.Begin(); kit!=kernel.End(); kit++ )
    {
    *kit = 0;
    }
  SRType::Iterator kit=kernel.Begin();
  *kit = 1;
/*  *(++kit) = 1;*/
  
  typedef itk::BinaryDilateImageFilter< IType, IType, SRType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );
  filter->SetKernel( kernel );
  filter->SetForegroundValue( 200 );
  filter->SetBackgroundValue( 100 );
  filter->SetBoundaryIsForeground( true );

/*  itk::SimpleFilterWatcher watcher(filter, "filter");*/
filter->Update();

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter->GetOutput() );

  filter->SetNumberOfThreads( 1 );
  writer->SetFileName( argv[2] );
  writer->Update();

  return 0;
}

