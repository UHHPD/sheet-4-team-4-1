#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include<cmath>
#include <fstream>


#include "Data.hh"

// generic function comparing two values of some type T used later for int and
// double
template <class T>
bool testEqual(const std::string& name, T expected, T real) {
  if (expected != real) {
    std::cout << "(" << name << ": <" << expected << ">"
              << ", " << real << ") ";
    return false;
  }
  return true;
}

bool testReadingSize() {
  std::cout << "testReadingSize...";
  Data datA("testA");
  return testEqual<int>("size", 1, datA.size());
}

bool testReadingMeasurement() {
  std::cout << "testReadingMeasurement...";
  Data datA("testA");
  return testEqual("measurement", 10., datA.measurement(0));
}

bool testReadingBinEdges() {
  std::cout << "testReadingBinEdges...";
  Data datA("testA");
  return testEqual("bin low", 0., datA.binLow(0)) &&
         testEqual("bin high", 1., datA.binHigh(0));
}

bool testReadingErrors() {
  std::cout << "testReadingErrors...";
  Data datA("testA");
  return testEqual("error", 2., datA.error(0));
}

bool testCopyConstructor() {
  std::cout << "testCopyConstructor...";
  Data a("testA");
  Data b(a);
  Data c = a;  // equivalent to Data c(a)
  return testEqual("measurement", 10., b.measurement(0)) &&
         testEqual("measurement", 10., c.measurement(0));
}


bool testCompatibility()
{
  std::cout << "testCompatibility...";
  Data a("testA");
  Data b("testB");

  int difference = a.checkCompatibility(b, 1); 
  
  // std::cout << difference << endl;

  return testEqual( "compatibilty", 1, difference );
}


void runTests() {
  std::cout << "running tests...\n";
  std::vector<std::function<bool()> > tests(
      {testReadingSize, testReadingMeasurement, testReadingBinEdges,
       testReadingErrors, testCopyConstructor, testCompatibility});
  for (auto test : tests)
    std::cout << (test() ? " ok" : " FAILED!") << std::endl;
}


Data average(Data datX, Data datY, string X, string Y)
{
  ofstream fout("average" + X +Y +".txt");

  fout << datX.size() << endl;
  // Write bins in new file "average.txt": 
  for(int i = 0; i <= datX.size(); i += 1)
  {
    fout << datX.binLow(i) << " " ; 
  }
  fout << endl; 

  if(datX.checkCompatibility(datY, 3) == 0)
  {
    double y1 = 0;
    double y2 = 0;
    double w1 = 0;
    double w2 = 0;

    for( int i = 0; i < datX.size(); i++)
    {
      y1 = datX.measurement(i);
      y2 = datY.measurement(i);
      w1 = pow( datX.error(i), -2);
      w2 = pow( datY.error(i), -2);
      fout << (y1*w1+y2*w2)/(w1+w2) << " ";
    }
    fout << endl; 
    for( int i = 0; i < datX.size(); i++)
    {
      w1 = pow( datX.error(i), -2);
      w2 = pow( datY.error(i), -2);
      fout << sqrt(1/(w1+w2)) << " ";
    }
    fout.close(); 
  }
  else
  {
    std::cout << "NON-COMPATIBLE!!!"; 
  }

  // Create instance of class Data: 
  Data average("average" + X +Y + ".txt");
  return average; 
}





int main() 
{
  using namespace std;

  cout << "******************************************************" << endl;
  runTests();
  cout << "******************************************************" << endl;
  // create an object which holds data of experiment A
  Data datA("exp_A");
  Data datB("exp_B");
  Data datC("exp_C");
  Data datD("exp_D");

  // vector<Data> meas(4) only works if keyword "private" is commented out
  // in header. "meas" = measurements 
  vector<Data> meas;
  vector<string> exp{"A","B", "C", "D"};
  meas.push_back(datA); 
  meas.push_back(datB); 
  meas.push_back(datC);
  meas.push_back(datD);  
  
  
  Data sum = datA + datB;

  for(int i = 0; i < sum.size(); i++){
    std::cout << sum.measurement(i) << std::endl;
  }
}