/**********************************************************************************************
** Project 1 - Algorithm 3, Divide and Conquer implementation
** Project Group: Team Dragon
** Authors: Mel Drews,  Jordan Grant
** Description: Implementation of a divide and conquer algorithm to compute the Maximum Subarray
**				Algorithm Pseudocode:
**
**				MaximumSub(array)
**					
**					return max, start, end
**				
**				Overview of Test program			
** 				1) An array of the form "[a1, a2, a3,..., an]" is read from a file
** 				2) The information is parsed into a STL vector using the stringstream class
** 				3) The vector is passed to MaximumSub which returns the sum, start index, and stop
**					index of the maximum subarray in an integer array 
**				4) The original array, subarray, and maximum sum are written to an output file in 
**					the following form:
**					[a1, a2, a3, ..., an]
**					[aj, aj+1, ..., ai-1, ai]
**					max
**				5) The process is completed for all arrays in test file
***********************************************************************************************/
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

/*********************************************************************************************
** Function: 	MaximumSub
** Paramaters:	reference to vector<int>
** Return: 		Array of 3 integers a[0] = max sum, a[1] = sub start index, a[2] = sub end index
** Description:	Brute Force Maximum Subarray algorithm. Iterates over array and, for every index,
**					calculates sum from index to 0. 
**				If a new maximum sum is found, it is saved, along with the start/stop indices in
**					larger array
**				The final max sum, start/stop indices are returned in an array.
**********************************************************************************************/
int* MaximumSub(std::vector<int>& arr, int first, int last);

/*********************************************************************************************
** Function: 	writeToOutput
** Paramaters:	reference to ofstream, reference to vector<int>, pointer to MaximumSub return values
** Return: 		void
** Description:	Outputs the original array, maximum subarray, and maximum sum as follows:
**				[a1, a2, a3, ..., an]
**				[aj, aj+1, ..., ai-1, ai] 
**				max				
**
**				where 0 <= j <= i <= n
**********************************************************************************************/
void writeToOutput(std::ofstream& out, std::vector<int>& arr, int maxSub[]);

/*********************************************************************************************
** Function: 	parseArray
** Paramaters:	value of array string, reference to empty vector<int>
** Return: 		void
** Description:	string is read into a stream, which is parsed by comma delimited getline calls.
**				Each comma delimited string is converted to an int by stringstream, then pushed
**				onto vector. 
**				Loop exits when getline receives an eof signal.
**********************************************************************************************/
void parseArray(std::string str, std::vector<int>&  arr);


/// Main Function
int main(int argc, char** argv)
{
	int *maxSub;				// to hold values for maximum subarray
	std::vector<int> array;		//STL container for array
	
	std::ifstream inputFile;	// file IO
	std::ofstream outputFile;
	
	std::string fileLine;		// holds string representation of array
	
	
	// open input file
	inputFile.open("MSS_TestProblems.txt" );
	if (inputFile.fail())
    {
		// Report error and return if file failed to open
        std::cout << "Could not open file" << std::endl; 
        return -1;
    }
	
	// open output file, creates file if none exists
	outputFile.open("MSS_TestResults.txt");
	
	while(std::getline(inputFile, fileLine)) { 				//read entire line from file into fileLine, until eof
		
		if (fileLine.size() > 2) {							//check if array is not empty ([])		
			parseArray(fileLine, array);
			
			maxSub = MaximumSub(array, 0, array.size() - 1);
		  
			writeToOutput(outputFile, array, maxSub); 
      
			delete[] maxSub;								// free memory for maximum subarray
			std::vector<int>().swap(array);					// free memory for vector, initialize new vector
		}
	}
	inputFile.close();										//close files
	outputFile.close();
}

/*********************************************************************************************
** Function: 	MaximumSub
** Paramaters:	reference to vector<int>
** Return: 		Array of 3 integers a[0] = max sum, a[1] = sub start index, a[2] = sub end index
** Description:	Brute Force Maximum Subarray algorithm. Iterates over array and, for every index,
**					calculates sum from index to 0. 
**				If a new maximum sum is found, it is saved, along with the start/stop indices in
**					larger array
**				The final max sum, start/stop indices are returned in an array.
**********************************************************************************************/
int* MaximumSub(std::vector<int>& arr, int first, int last) {
	if (first == last) {
		int* base = new int[3];
		base[0] = arr[first];
		base[1] = first;
		base[2] = first;
		return base;
	}
	int middle = (first + last) / 2;
	int *left = MaximumSub(arr, 0, middle);
	int *right = MaximumSub(arr, middle + 1, last);
	
	int *combo = new int[3];
	int leftMax = arr[middle];
    int rightMax = arr[middle+1];
	
    int temp = 0;
    for(int i = middle; i >= first; i--) {
        temp += arr[i];
        if(temp > leftMax) {
			leftMax = temp;
			combo[1] = i;
		}
    }
	
    temp = 0;
    for(int i = middle + 1; i <= last; i++) {
        temp += arr[i];
        if(temp > rightMax) {
			rightMax = temp;
			combo[2] = i;
		}
    }
	
	combo[0] = leftMax + rightMax;
	
	if (left[0] > right[0]){
		delete[] right;
		if (left[0] > combo[0]) {
			delete[] combo;
			return left;
		}
		else {
			delete[] left;
			return combo;
		}
		
	}
	else {
		delete[] left;
		if (right[0] > combo[0])  {
			delete[] combo;
			return right;
		}
		else {
			delete[] right;
			return combo;
		}
	}
	
}


/*********************************************************************************************
** Function: 	writeToOutput
** Paramaters:	reference to ofstream, reference to vector<int>, pointer to MaximumSub return values
** Return: 		void
** Description:	Outputs the original array, maximum subarray, and maximum sum as follows:
**				[a1, a2, a3, ..., an]
**				[aj, aj+1, ..., ai-1, ai] 
**				max				
**
**				where 0 <= j <= i <= n
**********************************************************************************************/
void writeToOutput(std::ofstream& out, std::vector<int>& arr, int maxSub[]) {
  int size = arr.size();
  
  out << "[";										//output open bracket
  
  for(int i = 0; i < size; i++) {					// output original array comma delimited
    if (i + 1 == size)
      out << arr[i];
    else
      out << arr[i] << ", ";
  }
  
  out << "]\n[";									// output closing bracket and opening for subarray
  
  for (int i = maxSub[1]; i <= maxSub[2]; i++) {	// output comma delimited subarray
    if (i == maxSub[2])
      out << arr[i];
    else
      out << arr[i] << ", ";
  }
  
  out << "]\n" << maxSub[0] << "\n\n";				// output closing bracket, sum, and padding newlines
}

/*********************************************************************************************
** Function: 	parseArray
** Paramaters:	value of array string, reference to empty vector<int>
** Return: 		void
** Description:	string is read into a stream, which is parsed by comma delimited getline calls.
**				Each comma delimited string is converted to an int by stringstream, then pushed
**				onto vector. 
**				Loop exits when getline receives an eof signal.
**********************************************************************************************/
void parseArray(std::string str, std::vector<int>& arr) {
	// remove opening and closing bracket
	str.erase(0, 1);
	str.erase(str.size() - 1);
	
	// initialize stringstreams for whole string and each integer
	std::stringstream ss(str),
					  ss_temp;
					  
	// temperary storage for each value in array
	std::string s_temp;
	int i_temp;
	
	while(std::getline(ss, s_temp, ',')) {			// get each comma delimited value
		ss_temp << s_temp;							// read into string
		ss_temp >> i_temp;							// write out to int
		
		arr.push_back(i_temp);						// push onto vector				
		
		// reset ss_temp for next value
		ss_temp.str("");
		ss_temp.clear();
	}
	
}

