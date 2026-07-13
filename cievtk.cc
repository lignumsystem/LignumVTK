#include <cstdlib>
#include <iostream>
#include <string>
#include <ParseCommandLine.h>
#include <CIESky.h>
/// \file cievtk.cc
/// \brief Main program for `vietk`

int Usage()
{
  cout << "Usage:" <<endl;
  cout << "./cievtk -azimuth|-a <number> -inclination|-i <number> -radiation|-r <number> -cie|-c 0|1 -output|-o <file.vtp>" <<endl;
  cout << "-cie 0: CIE standard ovecast sky with equal step size for sectors" << endl;
  cout << "-cie 1: CIE standard ovecast sky with equal area size sectors" <<endl;
  return  EXIT_SUCCESS;
}

int main(int argc,char* argv[])
{
  string argument;
  int nazim = 0;
  int nincl = 0;
  double rad_total = 0.0;
  int cie_type = -1;
  string file_name;
  const string empty_string("");

  if (argc < 2||CheckCommandLine(argc,argv,"-help")||CheckCommandLine(argc,argv,"-h")){
    return Usage();
  }
  if (ParseCommandLine(argc,argv,"-azimuth",argument) || ParseCommandLine(argc,argv,"-a",argument)){
    nazim =std::stoi(argument.c_str());
    std::cout << "Using " << nazim << " azimuths" << std::endl;  
  }
  if (ParseCommandLine(argc,argv,"-inclination",argument) || ParseCommandLine(argc,argv,"-i",argument)){
    nincl =std::stoi(argument.c_str());
    std::cout << "Using " << nincl  << " inclinations" << std::endl;  
  }
  if (ParseCommandLine(argc,argv,"-radiation",argument) || ParseCommandLine(argc,argv,"-r",argument)){
    rad_total =std::stod(argument.c_str());
    std::cout << "Using " << rad_total << " total radiation" << std::endl;  
  }
  if (ParseCommandLine(argc,argv,"-cie",argument) || ParseCommandLine(argc,argv,"-c",argument)){
    cie_type =std::stoi(argument.c_str());
    std::cout << "Using type " << cie_type << " CIE sky breakdown to sectors" << std::endl;  
  }
  if (ParseCommandLine(argc,argv,"-outfile",argument) || ParseCommandLine(argc,argv,"-o",argument)){
    file_name = argument;
    std::cout << "Using " << file_name << " output file name" << std::endl;  
  }
  if ((nazim == 0) || (nincl == 0)|| (rad_total == 0) || (file_name ==  empty_string) || (cie_type == -1)){
    std::cerr << "Missing command line argument" << std::endl;
    return EXIT_FAILURE;
  }
  
  if (cie_type == 0){
    lignumvtk::CIESOC soc(nazim,nincl,rad_total);
    soc.writeHemisphere(file_name);
  }
  else if (cie_type == 1){
    lignumvtk::CIESOCEqualArea soc(nazim,nincl,rad_total);
    int ret_val=soc.writeHemisphere("SOC.vtp");
  }
  else{
    std:: cerr << "Unknown CIE type (-cie) " << cie_type << std::endl;
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
