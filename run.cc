#include <iostream>
#include <vector>
#include <functional>
#include <string>
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

// Making average without overload: 
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
  
  //read bin 27 from all four data sets
  for(int i; i < 4; i++)
  {
    cout << "measured cross section of experiment " << exp[i] << " in bin 27: " << meas.at(i).measurement(27) << endl;
  }
  
  //calculating delta_y and sigma_delta_y for bin 27 and data set A and B
  double delta_y;
  double sigma_delta_y;
  double n;
  delta_y = abs(meas.at(1).measurement(27) -  meas.at(0).measurement(27));
  sigma_delta_y = sqrt(pow(meas.at(1).error(27),2)+pow(meas.at(0).error(27),2)); //error propagation
  n = delta_y/sigma_delta_y;
  cout << "delta_y AB: " << delta_y << endl;
  cout << "sigma_delta_y AB: " << sigma_delta_y << endl;
  cout << "n = delta_y/sigma_delta_y: " << n << endl; 
  
  // here is the data from experiment A
  cout << "bin 27: from " << datA.binLow(27) << " to " << datA.binHigh(27)
       << endl;
  cout << "measurement of experiment A in bin 27: " << datA.measurement(27)
       << endl;

  // Average two datasets (which is only done if they are compatible within 3 standard-deviations): 
  Data avgAB = datA + datB;
  Data avgAC = datA + datC;
  Data avgAD = datA + datD;
  Data avgBC = datB + datC;
  Data avgBD = datB + datD;
  Data avgCD = datC + datD;

  // FOR EXERCISE 2: 
  cout << meas[0].chi_square_test() << endl; 
  cout << meas[1].chi_square_test() << endl; 
  cout << meas[2].chi_square_test() << endl; 
  cout << meas[3].chi_square_test() << endl; 

  // Check which experiments are compatible with each other within 3 standard-dev.: 
  cout << "Number of differences between expA and expB with 3 standard-dev. " << meas[0].checkCompatibility(meas[1], 3) << endl ; 
  cout << "Number of differences between expA and expC with 3 standard-dev. " << meas[0].checkCompatibility(meas[2], 3) << endl; 
  cout << "Number of differences between expA and expD with 3 standard-dev. " << meas[0].checkCompatibility(meas[3], 3) << endl; 
  cout << "Number of differences between expB and expC with 3 standard-dev. "  << meas[1].checkCompatibility(meas[2], 3) << endl; 
  cout << "Number of differences between expB and expD with 3 standard-dev. "  <<meas[1].checkCompatibility(meas[3], 3) << endl; 
  cout << "Number of differences between expC and expD with 3 standard-dev. "  << meas[2].checkCompatibility(meas[3], 3) << endl; 

  cout << endl;

  // Check which experiments are compatible with each other within 2 standard-dev.: 
  cout << "Number of differences between expA and expB with 2 standard-dev. " << meas[0].checkCompatibility(meas[1], 2) << endl ; 
  cout << "Number of differences between expA and expC with 2 standard-dev. " << meas[0].checkCompatibility(meas[2], 2) << endl; 
  cout << "Number of differences between expA and expD with 2 standard-dev. " << meas[0].checkCompatibility(meas[3], 2) << endl; 
  cout << "Number of differences between expB and expC with 2 standard-dev. "  << meas[1].checkCompatibility(meas[2], 2) << endl; 
  cout << "Number of differences between expB and expD with 2 standard-dev. "  <<meas[1].checkCompatibility(meas[3], 2) << endl; 
  cout << "Number of differences between expC and expD with 2 standard-dev. "  << meas[2].checkCompatibility(meas[3], 2) << endl;

  return 0;
}