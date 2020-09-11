#ifndef STRUCT_INCLUDE
#define STRUCT_INCLUDE

struct Point{
	double x;
	double y;

	Point() = default;
	Point(double x, double y): x(x), y(y) {
	}	
};

struct Results {
	unsigned int copies;
	unsigned int comps;

	Results() = default;

	Results(int copies, int comps) : copies(copies), comps(comps) {

	}
	Results& operator=(const Results& other) {
		copies = other.copies;
		comps = other.comps;
	}
};

class IntCount {
	static unsigned int CopyCounter;
	static unsigned int CompCounter;
public:
	int value;

	static Results getCounters() {
		return Results(CopyCounter, CompCounter);
	}

	static void NullCounters() {
		CopyCounter = 0;
		CompCounter = 0;
	}

	IntCount() : value(0) {
	}

	IntCount(const IntCount& other): value(other.value) {
		++CopyCounter;
	}

	IntCount& operator=(const IntCount& other) {
		value = other.value;
	}

	IntCount& operator=(int value) {
		this->value = value;
	}

	bool operator<(const IntCount& other) {
		++CompCounter;
		return value < other.value;
	}

	bool operator<=(const IntCount& other) {
		++CompCounter;
		return value <= other.value;
	}

};

unsigned int IntCount::CopyCounter = 0;
unsigned int IntCount::CompCounter = 0;

struct MyIntLess {
	bool operator()(IntCount* a, IntCount* b) { return *a < *b; }
};
#endif