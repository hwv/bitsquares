#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[]) {
	if (argc != 3) {
		cout << "Usage: " <<  argv[0] << " [string length] [number of batches]" << endl;
		cout << "Computes the combined answer for output files in [string length]" << endl;
		return 1;
	}

	int N = atoi(argv[1]);
	int batches = atoi(argv[2]);

	vector< vector<string> > outputfiles;

	// Verify output files and find max squares
	int max = 0;
	for	(int i=1; i<=batches; i++) {
		stringstream filename;
		filename << N << "/output/" << i << "-" << batches << ".output";
		ifstream file(filename.str());
		vector<string> lines;

		if (file) {
			string line;
			while(getline(file,line)) {
				lines.push_back(line);
			}

			if (lines.size() != 5) {
				cout << "Failed. " << filename.str() << " has " << lines.size() << " lines. It should have 5!" << endl;
				return 1;
			}

			int maxsquares = atoi(lines[3].c_str());
			if (maxsquares > max) {
				max = maxsquares;
			}

			// job completion time
			int deltaseconds = atoi(lines[2].c_str())-atoi(lines[1].c_str());
			int days = deltaseconds / (24*3600);
			int hours = (deltaseconds-(days*24*3600))/3600;
			int minutes = (deltaseconds-(days*24*3600+hours*3600))/60;
			int seconds = deltaseconds - (days*24*3600+hours*3600+minutes*60);

			cout << filename.str() << "\t COMPLETED in ";

			if (days > 0)
				cout << days << "d";
			if (days > 0 || hours > 0)
				cout << hours << "h";
			if (days > 0 || hours > 0 || minutes > 0)
				cout << minutes << "m";
			cout << seconds << "s";

			cout << "\t MAX=" << maxsquares << endl;


			outputfiles.push_back(lines);

		} else {
			cout << "Failed. " << filename.str() << " does not exist!" << endl;
			return 1;
		}
	}

	// Merge output from batches attaining max squares
	cout << "maxsquares[" << N << "]=[" << max << ",[";
	bool first = true;
	for	(int i=0; i<batches; i++) {
		if(atoi(outputfiles[i][3].c_str()) == max) {
			if (!first) {
				cout << ",";
			}
			cout << outputfiles[i][4];
			first = false;
		}
	}
	cout << "]]" << endl;

	return 0;
}
