#include <fstream>
#include <random>
#include <functional>
#include <sstream>
#include <string>
#include <iostream>
#include <chrono>

int main(int argc, char** argv)
{
  if (argc < 2)
    return 0;
  std::mt19937 generator(std::random_device{}());
  std::uniform_int_distribution<int> distribution(-100, 100);
  auto random = std::bind(distribution, generator);

  std::ofstream outputFile;
  std::stringstream ss(argv[1]);
  std::string fileName;
  int arraySize;

  if (!(ss >> arraySize)){
    std::cout << "Invalid Input for array size. Please give an integer " << std::endl;
    return -1;
  }
  fileName = std::string("test_") + argv[1] + ".txt";
  outputFile.open(fileName.c_str());
  
  for (int i = 0; i < 10; i++) {
    outputFile << "[";
    for (int j = 0; j < arraySize; j++) {
      if (j + 1 == arraySize)
        outputFile << random();
      else
        outputFile << random() << ", ";
    }
    outputFile << "]\n\n";
  }

  return 0;
}
