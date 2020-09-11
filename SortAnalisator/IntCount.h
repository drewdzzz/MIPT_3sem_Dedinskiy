#ifndef STRUCT_INCLUDE
#define STRUCT_INCLUDE

struct Results {
	unsigned int copies;
	unsigned int comps;

	Results(int copies, int comps) : copies(copies), comps(comps) {

	}
};

class IntCopyCompCount {
	static unsigned int CopyCounter;
	static unsigned int CompCounter;
public:

	static Results getCounters() {
		return Results(CopyCounter, CompCounter);
	}

	int value;
	static void NullCounters() {
		CopyCounter = 0;
		CompCounter = 0;
	}

	IntCopyCompCount() : value(0) {
	}

	IntCopyCompCount(const IntCopyCompCount& other): value(other.value) {
		++CopyCounter;
	}

	IntCopyCompCount& operator=(const IntCopyCompCount& other) {
		value = other.value;
	}

	bool operator<(const IntCopyCompCount& other) {
		++CompCounter;
		return value < other.value;
	}

};

unsigned int IntCopyCompCount::CopyCounter = 0;
unsigned int IntCopyCompCount::CompCounter = 0;

struct MyIntLess {
	bool operator()(IntCopyCompCount* a, IntCopyCompCount* b) { return *a < *b; }
};
#endif