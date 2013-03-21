/*
 *
 *
 *
 *
 *
 *
 */
#include <iostream>
#include <string>

using namespace std;

int main( int argc, const char* argv[] )
{
	// Prints each argument on the command line.
	for( int i = 0; i < argc; i++ )
	{
	  cout << "arg" << i << ": " << argv[i] << endl;
	}

	//Get path to jpg passed in
	string path;
	path += argv[1];

	cout << "path = " << path << endl;
}
