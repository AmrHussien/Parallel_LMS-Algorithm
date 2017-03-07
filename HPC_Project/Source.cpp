#include<iostream>
#include<math.h>
#include<string>
#include<vector>
#include<ctime>
#include <fstream>
#include <sstream>
#include<omp.h>
#include <tchar.h>
#include<algorithm>
#include<math.h>
#include<utility>
#include<tuple>
using namespace std;

#define N 10000
int LCS[N][N];
int LCS_Parallel[N][N]; 

/**
* Reading from file.
*
*@return : 2 strings x and y.
*
*/
pair<string,string> ReadFromFile(string fileName)
{ 
	string x = "";
	string y = "";
	ifstream myfile(fileName + ".txt");
	if (myfile.is_open())
	{
		string line;
		while (getline(myfile, line))
		{
			stringstream linestream(line);
			string data;
			getline(linestream, data,'@');
			linestream >> x >> y;
		}
 		myfile.close();
	}
	return make_pair(x, y);
}

int LCS_Calculation(string X, string Y, int xSize, int ySize)
{
	//vector< vector <int> > LCS(X.size() + 1, vector<int>(Y.size() + 1));
	for (int i = 0; i <= xSize; i++){
		for (int j = 0; j <= ySize; j++){
			if (i == 0 || j == 0) 
				LCS[i][j] = 0;
			else if (X[i - 1] == Y[j - 1])
				LCS[i][j] = LCS[i - 1][j - 1] + 1;
			else
				LCS[i][j] = max(LCS[i][j - 1], LCS[i - 1][j]);
		}
	}
	return LCS[xSize][ySize];
}
int LCS_Parallel_1(string X, string Y, int xSize, int ySize, int nThreads)
{

	 
	int i, j;
	omp_set_num_threads(nThreads);
#pragma omp parallel private(i,j)  
	{
		for (i = 0; i <= xSize; i++){

			for (j = 0; j <= ySize; j++){
				if (i == 0 || j == 0)
					LCS[i][j] = 0;
				else if (X[i - 1] == Y[j - 1])

					LCS[i][j] = LCS[i - 1][j - 1] + 1;
				else
 
					LCS[i][j] = max(LCS[i][j - 1], LCS[i - 1][j]);
			}

		}
	}
	return LCS[xSize][ySize];
}
 
int LCS_Parallel_2(string X, string Y,int xSize,int ySize, int numThreads)
{
	
	for (int i = 0, j = 0; j <= ySize && i <= xSize; j++)
	{

		int AntiDiagonal_Size = min(j, xSize - i);						
#pragma omp parallel for num_threads(numThreads)
		for (int k = 0; k <= AntiDiagonal_Size; ++k)//for each anti-diagonal element
		{
			int xPosition = i + k, yPosition = j - k;									  
			if (xPosition == 0 || yPosition == 0)
				LCS_Parallel[xPosition][yPosition] = 0;
			else if (X[xPosition - 1] == Y[yPosition - 1])
				LCS_Parallel[xPosition][yPosition] = LCS_Parallel[xPosition - 1][yPosition - 1] + 1;
			else
				LCS_Parallel[xPosition][yPosition] = max(LCS_Parallel[xPosition - 1][yPosition], LCS_Parallel[xPosition][yPosition - 1]);
		}
		if (j >= ySize)								//Increment the right side ,If the upper side is finished  
			j = ySize - 1, i++;
	}
	return LCS_Parallel[xSize][ySize];
}

 
int main()
{
	string X, Y;
	 int nThreads;
	 tie(X, Y) = ReadFromFile("data");
	 int xSize = X.size();
	 int ySize = Y.size();

	cout << "String 1 : "<<X<<'\n';
	cout << "String 2 : "<<Y<<'\n';
	cout << "No. Threads : ";
	cin >> nThreads;

	// Serial code 
	const clock_t begin_time = clock();
	printf("\nLength of LCS is %d\n", LCS_Calculation(X, Y, xSize, ySize));
	float time = float(clock() - begin_time) / CLOCKS_PER_SEC;
	cout << "Time : " << time << endl;

	// Parallel code
	const clock_t begin_time2 = clock();
	printf("\nLength of LCS parallel is %d\n", LCS_Parallel_2(X, Y, xSize, ySize, nThreads));
	float time2 = float(clock() - begin_time2) / CLOCKS_PER_SEC;
	cout << "Time : " << time2 << endl;



	int rand;
	cin >> rand;
	return 0;
}