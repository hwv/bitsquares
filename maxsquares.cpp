/**
 FILE OUTPUT FORMAT:

 HEADLINE
 TIMESTART
 TIMEEND
 MAXSQUARECOUNT
 MAXSTRINGS
**/

#include <iostream>
#include <cstdint>
//#include <boost/dynamic_bitset.hpp>
#include <vector>
#include <unordered_set>
#include <cstdlib>

using namespace std;

int N;
//uint64_t MASK;

// reverse lower N bits of v
uint64_t rev(uint64_t v) {
  uint64_t r = 0;
  for (int i=0; i<N; i++) {
    r <<= 1;
    r |= v & 1;
    v >>= 1;
  }
  return r;
}

// returns number of squares lower N bits of v
int count_squares(uint64_t v) {
	int ds = 0; // distinct squares
	unordered_set<uint64_t> squares; // distinct squares of length 2l

	for(int l=1; l<=N/2; l++) {
		uint64_t r = (((uint64_t)1 << l)-1); // root of square mask
		for(int i=0; i+2*l<N+1; i++) {
			uint64_t right = (v & r) >> i; // right half of substring
			if ((right == (v & (r << l)) >> (i+l)) && squares.insert(right).second) { // is substring of length 2l ending on bit i a new square?
					++ds; // new square of length 2l!
			}
			r <<= 1;
		}
		squares.clear(); // clear squares of length 2l
	}
	return ds;
}

int main(int argc, char *argv[]) {
    if (argc < 2 || argc == 3 || argc > 4) {
		cout << "Usage: " << argv[0] << " #N [#BatchNumber #BatchesTotal]" << endl;
		return 0;
    }

	N = atoi(argv[1]);
	uint64_t MASK = ((uint64_t)1<<N)-1;
	//MAX_I = ((uint64_t)1<<(N-1))-1;

	int batch_no = 1;
	int batch_total = 1;

	if (argc == 4) {
		batch_no = atoi(argv[2]);
		batch_total = atoi(argv[3]);
	}

	uint64_t batch_size = ((uint64_t)1<<(N-1))/batch_total;
	uint64_t start = (batch_no-1)*batch_size;
	uint64_t end = batch_no == batch_total ? ((uint64_t)1<<(N-1))-1 : start+batch_size-1;

	cout << "N=" << N << " (batch " << batch_no << "/" << batch_total << " = [" << start << "-" << end << "])" << endl; // LINE 1 (HEADLINE)


	cout << time(0) << endl; // LINE 2 (TIMESTART)

	int max=0,s;

	std::vector<uint64_t> max_strings;
	for (uint64_t i = start; i<=end; i++) {
		//debug
		//cout << i << "\t" << boost::dynamic_bitset<>(N,i) << endl;

		uint64_t r = rev(i);
		if (((i & 1) && i < ((~r) & MASK)) || (!(i & 1) && i > r)) {
			continue; // consider only one element from each equiv. class
		}

		s = count_squares(i);

		if (s > max) {
			max = s;
			max_strings.clear();
		}
		if (s == max) {
			max_strings.push_back(i);
		}
	}

	cout << time(0) << endl; // LINE 3 (TIMEEND)

	cout << max << endl; // LINE 4 (MAXSQUARECOUNT)

	// LINE 5 (MAXSTRINGS)
	for (uint64_t i = 0; i<max_strings.size(); ++i) {
		cout << max_strings[i];
		if ( i<max_strings.size()-1 )
			cout << ",";
	}
  return 0;
}
