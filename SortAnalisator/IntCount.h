#ifndef STRUCT_INCLUDE
#define STRUCT_INCLUDE

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

class IntCount;

void mySwap(IntCount& a, IntCount& b);
struct MyIntLess;

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

	IntCount(const IntCount& other): value(other.value) {}

	IntCount& operator=(const IntCount& other) {
		value = other.value;
	}

	IntCount& operator=(int value) {
		this->value = value;
	}

	bool operator<(const IntCount& other) {
		return value < other.value;
	}

	friend void mySwap(IntCount* a, IntCount* b);
	friend MyIntLess;
};

void mySwap(IntCount* a, IntCount* b) {
    IntCount temp = *a;
    *a = *b;
    *b = temp;
	++IntCount::CopyCounter;
}

struct MyIntLess {
	bool operator()(IntCount* a, IntCount* b) { 
		++IntCount::CompCounter;
		return *a < *b; 
	}
};

unsigned int IntCount::CopyCounter = 0;
unsigned int IntCount::CompCounter = 0;

#endif