/*
Andy Matthew Guinto
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
using namespace std;

int main(int argc, char *argv[])
{
	// keep track of length of longest word AND counts for longest words
	int longestWord = 0;
	map<string,int> longWordCounts;

	// keep track of length of longest line AND count
	int longestLine = 0;
	int longestLineCnt = 0;

	string inLine;

	// remember which flags were specified by the user
	bool mFlag = false;
	bool cFlag = false;
	bool bFlag = false;

	// use these flags to enforce the behavior of arguments
	bool checkingForArgs = true;
	bool foundOneFile = false;

	for( int i = 1; i < argc; i++ ) {
		string arg(argv[i]);

		if( checkingForArgs && arg[0] == '-' ) {
			if( arg == "-m" ) mFlag = true;
			else if( arg == "-c" ) cFlag = true;
			else if( arg == "-b" ) bFlag = true;
			else {
				cout << arg << " UNRECOGNIZED FLAG" << endl;
				return -1;
			}
			continue;
		}
		else {
			checkingForArgs = false; // NOTE: as soon as I see something that is NOT an argument,
									// all subsequent args are treated as though they are filenames
		}

		foundOneFile = true;

		// initialize all of my counters
		longestWord = 0;
		longestLine = 0;
		longestLineCnt = 0;
		longWordCounts.clear();

		ifstream inFile(arg);

		if( !inFile.is_open() ) {
			cout << arg << " FILE NOT FOUND" << endl;
			continue;
		}

		// loop through every line in the file
		while( getline(inFile, inLine) ) {

			int bLineLen = 0;
			istringstream wstream(inLine);
			string word;

			// this loop breaks the line into words using a stringstream

			// when -b is specified, the length of the line is calculated differently
			// than when it is not. this code calculates a "bLineLen" ... a length of
			// the line when -b is specified. Its the sum of the length of the words,
			// plus a space between words

			while( wstream >> word ) {
				int wlen = word.length();
				if( bLineLen == 0 )
					bLineLen = wlen; /* first word */
				else
					bLineLen += wlen + 1; /* all other words */

				// this word is a candidate for new longest word! so
				// throw away all previous longest-word stuff
				if( wlen > longestWord ) {
					longestWord = wlen;
					longWordCounts.clear();
				}
				if( wlen == longestWord ) {
					longWordCounts[word]++; // count the word using the map
				}
			}

			// this is the longest line processing; the length
			// is calculated one of two ways, but the logic is the same
			// for remembering the length of the longest and for counting it
			int len = bFlag ? bLineLen : inLine.length();
			if( len > longestLine ) {
				longestLine = len;
				longestLineCnt = 0;
			}
			if( len == longestLine ) {
				longestLineCnt++;
			}
		}

		// in the case of -m, I have to figure out which words to print... I have to figure
		// out which of the longest words appears most often
		int maxWordCount = 0;
		if( mFlag ) {
			for( auto it = longWordCounts.begin(); it != longWordCounts.end(); it++ )
				if( it->second > maxWordCount )
					maxWordCount = it->second;
		}

		cout << arg << ":" << endl;
		if( longWordCounts.size() > 0 ) { /* only print words if there are any words :) */
			bool printed = false; // i use this to remember if i need comma space before the word or not
			for( auto sIt = longWordCounts.begin(); sIt != longWordCounts.end(); sIt++ ) {
				if( mFlag && sIt->second != maxWordCount ) // this is how i handle -m: if -m and not max count, skip it
					continue;
				if( printed )
					cout << ", ";
				printed = true;
				cout << sIt->first;
				if( cFlag )
					cout << "(" << sIt->second << ")";
			}
			cout << endl;
		}

		cout << longestLine;
		if( cFlag )
			cout << "(" << longestLineCnt << ")";
		cout << endl;

		inFile.close();
	}

	// if I went through all the arguments and never saw a filename, print this message
	if( !foundOneFile )
		cout << "NO FILES" << endl;

	return 0;
}
