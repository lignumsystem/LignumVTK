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
  cout << "./cievtk [-help|-h] -azimuth|-a <number> -inclination|-i <number> -totrad |-r <number> -cie|-c <1-15,16> -params|-p <DK2002|ISO2004> -sunpolar|-sp <0-90 degrees>\
 -sunazimuth|-sa <0-360 degrees> -output|-o <file.vtp> " <<endl;
  return  EXIT_SUCCESS;
}

int main(int argc,char* argv[])
{ 
  string argument;
  int nazim = 0;
  int nincl = 0;
  double rad_total = 0.0;
  int cie_type = -1;
  double sun_polar=0.0;
  double sun_azimuth=0.0;
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
  if (ParseCommandLine(argc,argv,"-totrad",argument) || ParseCommandLine(argc,argv,"-r",argument)){
    rad_total =std::stod(argument.c_str());
    std::cout << "Using " << rad_total << " total incoming radiant flux" << std::endl;  
  }
  if (ParseCommandLine(argc,argv,"-cie",argument) || ParseCommandLine(argc,argv,"-c",argument)){
    cie_type =std::stoi(argument.c_str());
    std::cout << "Using type " << cie_type << " CIE sky" << std::endl;  
  }
  if (ParseCommandLine(argc,argv,"-outfile",argument) || ParseCommandLine(argc,argv,"-o",argument)){
    file_name = argument;
    std::cout << "Using " << file_name << " output file name" << std::endl;  
  }
  if ((nazim == 0) || (nincl == 0)|| (rad_total == 0) || (file_name ==  empty_string) || (cie_type == -1)){
    std::cerr << "Missing command line argument" << std::endl;
    return EXIT_FAILURE;
  }
  if ((cie_type == 0) || (cie_type > 16)){
    std::cerr << "CIE sky type must be 1-16" << endl;
      return EXIT_FAILURE;
  }
  
  if (cie_type == 16){
    lignumvtk::CIESOCEqualArea soc(nazim,nincl,rad_total);
    int ret_val = soc.writeHemisphere(file_name);
    return ret_val;
  }
  if (ParseCommandLine(argc,argv,"-sunpolar",argument) || ParseCommandLine(argc,argv,"-sp",argument)){
    sun_polar = std::stod(argument);
  }
  else{
    std::cerr << "Sun polar angle (degrees) missing " << endl;
    return EXIT_FAILURE;
  }
  
  if (ParseCommandLine(argc,argv,"-sunazimuth",argument) || ParseCommandLine(argc,argv,"-sa",argument)){
    sun_azimuth = std::stod(argument);
  }
  else{
    std::cerr << "Sun azimuth angle (degrees) missing " << endl;
    return EXIT_FAILURE;
  }
  vector<double> p;
  string default_parameters = "ISO2004";
  if (ParseCommandLine(argc,argv,"-params",argument) || ParseCommandLine(argc,argv,"-p",argument)){
    default_parameters = argument;
  }
  std::cout << "Using " << default_parameters << " CIE sky parameter set" << endl;
  if (default_parameters == "ISO2004"){
    p = lignumvtk::CIE_PARAMETERS[lignumvtk::ISO2004].at(cie_type);
  }
  else{
    p = lignumvtk::CIE_PARAMETERS[lignumvtk::DK2002].at(cie_type);
  }
  std::cout << "Using CIE Sky parameters " << p[0] << " " << p[1] << " " << p[2] << " " << p[3] << p[5] << endl; 
  lignumvtk::CIESGS sgs(nazim,nincl,p[0],p[1],p[2],p[3],p[4],sun_polar*(PI_VALUE/180.0),sun_azimuth*(PI_VALUE)/180.0,rad_total);
  int ret_val = sgs.writeHemisphere(file_name);
  return ret_val;
}
