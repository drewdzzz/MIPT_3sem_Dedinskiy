#ifndef MEASURE_TOOLS_INCLUDE
#define MEASURE_TOOLS_INCLUDE

namespace TestRes {
	int BUBBLESORT_COPIES[AnConst::MEASURE_NUM];
	int BUBBLESORT_COMPS [AnConst::MEASURE_NUM];
	int QUICKSORT_COPIES [AnConst::MEASURE_NUM];
	int QUICKSORT_COMPS  [AnConst::MEASURE_NUM];
}

void sort_test(void (*sort_func)(IntCount*, IntCount*, MyIntLess), int* copy_result, int* comp_result) {
	for (int size = AnConst::MEASURE_FREQ, i = 0; i <= AnConst::MEASURE_NUM; size += AnConst::MEASURE_FREQ, ++i) {
		IntCount* arr = new IntCount[size];
		for (int i = 0; i < size; ++i)
			arr[i] = rand() % 100000;
		IntCount::NullCounters();
		sort_func(arr, arr + size, MyIntLess());
		
		copy_result[i] = IntCount::getCounters().copies;
		comp_result[i] = IntCount::getCounters().comps;
		delete[] arr;
	}

}

#endif