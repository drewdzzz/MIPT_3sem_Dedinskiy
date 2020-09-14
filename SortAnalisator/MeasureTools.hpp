#ifndef MEASURE_TOOLS_INCLUDE
#define MEASURE_TOOLS_INCLUDE

namespace TestRes {
Results BUBBLESORT[AnConst::MEASURE_NUM];
Results  QUICKSORT[AnConst::MEASURE_NUM];
}

void sort_test(void (*sort_func)(IntCount*, IntCount*), Results* result_array) {
	for (int size = AnConst::MEASURE_FREQ, i = 0; i <= AnConst::MEASURE_NUM; size += AnConst::MEASURE_FREQ, ++i) {
		IntCount* arr = new IntCount[size];
		for (int i = 0; i < size; ++i)
			arr[i] = rand() % 100000;
		IntCount::NullCounters();
		sort_func(arr, arr + size);
		
		
		result_array[i] = IntCount::getCounters();
		delete[] arr;
	}

}

#endif