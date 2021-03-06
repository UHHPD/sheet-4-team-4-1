#ifndef DATA_HH
#define DATA_HH

#include <vector>
#include <string>
#include<cmath>
#include<iostream>

using namespace std;

class Data {
 public:
  std::string m_dataID;
  Data(const std::string& filename);
  Data(Data&, Data&);
  unsigned int size() const { return m_data.size(); }
  double measurement(int i) const { return m_data[i]; }
  double binCenter(int i) const { return (m_bins[i] +m_bins[i+1])/2; }
  double binLow(int i) const { return m_bins[i]; }
  double binHigh(int i) const { return m_bins[i+1]; }
  double error(int i) const { return m_errors[i]; }

  std::vector<double> giveBins(){return m_bins;}
  std::vector<double> giveData(){return m_data;}
  std::vector<double> giveErrors(){return m_errors;}
  Data operator+ (Data&);
  
  
  vector<double> returnOutliers(const Data& in, int n) 
  {
    double delta_y = 0;
    double sigma_delta_y =0;
    vector <double> differences ; 
    vector <double> indices;
    
    // m_data.size(): size of data points (ifstream in data.cc)
    // m_data is the object on which we call the entire function checkCompatibility
    for(int i = 0; i < m_data.size(); i += 1)
    {
      delta_y = abs(m_data[i] - in.measurement(i) );   
      // error propagation
      sigma_delta_y = sqrt( pow(m_errors[i], 2) +  pow(in.error(i), 2) ); 
      if( delta_y / sigma_delta_y > n  )
      {
        differences.push_back(m_data[i]); 
        indices.push_back(binCenter(i));
      }
    }
    std::cout << std::endl << "Experiments "<< m_dataID << " and " << in.m_dataID << " show " << differences.size() << " Outlier(s) at " << n << " standard deviations: " ;
    for(int i = 0; i < indices.size(); i++)
    {
        std::cout << "(" << indices[i] << ", " << differences[i]<< ")" << std::endl;
    }
    return differences, indices; 
  }
  
  
  int checkCompatibility(const Data& in, int n) 
  {
    double delta_y = 0;
    double sigma_delta_y =0;
    int differences = 0; 
    

    // m_data.size(): size of data points (ifstream in data.cc)
    // m_data is the object on which we call the entire function checkCompatibility
    for(int i = 0; i < m_data.size(); i += 1)
    {
      delta_y = abs(m_data[i] - in.measurement(i) );   
      // error propagation
      sigma_delta_y = sqrt( pow(m_errors[i], 2) +  pow(in.error(i), 2) ); 
      if( delta_y / sigma_delta_y > n  )
      {
        differences += 1; 
      }
    }
    return differences; 
  }
  
  // function to calculate chi^2/ndf
  double chi_square_test( )
  {
    const int ndf = 52; 
    double chi_square = 0; 

    for(int i = 0; i < m_data.size(); i++ )
    {
      // chi_square += pow( (m_data[i] - this->f(this->binCenter( i ))) / m_errors[i], 2 )/ndf; 
      chi_square += pow( (m_data[i] - f(binCenter( i ))) / m_errors[i], 2 )/ndf; 
    }

    return chi_square; 
  }


 private:
  Data() {}  // disallow empty data set
  void assertSizes();
  std::vector<double> m_data;
  std::vector<double> m_bins;
  std::vector<double> m_errors;
  // Prediction function f:  
  double f(double x)
  {
    return (0.005 - pow(10, -5) * x + 0.08 * exp(-0.015 * x));  
  }
};

#endif
