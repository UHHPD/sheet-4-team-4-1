#include "Data.hh"

#include <iostream>
#include <fstream>
#include <cassert>
#include <stdexcept>
#include <cmath>
#include <string>

using namespace std;

Data::Data(const std::string& filename) 
{
  m_dataID = filename;

  ifstream file(filename);
  

  // check if file opened correctly
  if (!file.is_open()) 
  {
    throw std::runtime_error("Error could not open file with name: " +
                             filename);
  }

  int size = -1;
  file >> size;

  // read in data from file: m_siz+1 bin edges
  for (int i = 0; i < size + 1; ++i) {
    double edge;
    file >> edge;
    m_bins.push_back(edge);
  }
  // read in data from file: m_siz bin contents
  for (int i = 0; i < size; ++i) {
    double entries;
    file >> entries;
    m_data.push_back(entries);
  }
  
  for (int i = 0; i < size; ++i) {
    double errors;
    file >> errors;
    m_errors.push_back(errors);
  }
  // done! close the file
  file.close();

  void assertSizes();
}; 

/*Second constructor to creata data object from averaging two existing data objects*/
Data::Data(Data& inA, Data& inB){
    m_dataID = "avg of " + inA.m_dataID + " and " + inB.m_dataID;
    m_bins = inA.giveBins();

    for(int i = 0; i < inA.size(); i++){
      
      m_data.push_back(

        (pow(inA.error(i), -2) * inA.measurement(i) + pow(inB.error(i), -2) * inB.measurement(i))/(pow(inA.error(i), -2) + pow(inB.error(i), -2))
        
      );
    }

    for(int i = 0; i < inA.size(); i++){
      m_errors.push_back(
        sqrt(1/(pow(inA.error(i), -2) + 1/pow(inB.error(i), -2)))
      );
    }

    void assertSizes();
  
}

void Data::assertSizes() 
{ 
  assert(m_data.size() + 1 == m_bins.size()  && m_errors.size() + 1 == m_bins.size() );
}

Data Data::operator+ (Data& input){
  if(this->checkCompatibility(input, 3) == 0){
    Data out(*this, input);
  return out;
  }
  // else{throw std::runtime_error("NON-COMPATIBLE");}  
  else
  {
    Data out(*this, input);
    std:cout  << out.m_dataID << " NON-COMPATIBLE" << endl;;
    return out; 
  }  
}
/*
int checkCompatibility(const Data& in, int n)
{
  vector <double> delta_y;
  vector <double> sigma_delta_y;
  int differences; 

  // We have 56 data points in total. 
  for(int i = 0; i < 56; i += 1)
  {
    delta_y[i] = abs( this->measurement(i) - in.measurement(i) );   

    // error propagation
    sigma_delta_y[i] = sqrt( pow(this->error(i), 2) +  pow(in.error(i), 2) ); 
    
    if( delta_y[i] / sigma_delta_y[i] >= n  )
    {
      differences += 1; 
    }
  }

  return differences; 
}
*/