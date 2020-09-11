/*class IntCopyCompCount {
	static unsigned int CopyCounter;
	static unsigned int CompCounter;
public:

	int value;
	void NullCounters() {
		CopyCounter = 0;
		CompCounter = 0;
	}

	IntCopyCompCount(const IntCopyCompCount& other) {
		value = other.value;
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

template<class T>
class Less {
	bool operator(T* a, T* b) { return *a < *b; }
};*/