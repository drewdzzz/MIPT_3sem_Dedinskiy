#ifndef SORT_INCLUDE
#define SORT_INCLUDE

template<class T, class Cmp>
void BubbleSort(T* begin, T* end, Cmp cmp) {
	for (T* curr1 = begin; curr1 != end; ++curr1)
		for (T* curr2 = curr1; curr2 != end; ++curr2) 
			if (cmp(curr2, curr1)) {
				mySwap(curr1, curr2);
			}
}




template<typename T, typename Cmp>
T* partition(T* begin, T* end, Cmp cmp) {

    T* move_place = begin - 1;

    T* it = begin;

    while (it < end) {
        if(cmp(it, end)) {
            ++move_place;
            mySwap(move_place, it);
        }
        ++it;
    }

    mySwap(move_place + 1, end); 

    return ++move_place;
}

template<typename T, typename Cmp>
void QuickSort(T* begin, T* end, Cmp cmp) {
    if(begin != end && begin + 1 != end){
        T* mid = partition (begin, end - 1, cmp);
        QuickSort (begin, mid, cmp);
        QuickSort (mid + 1, end, cmp);
    }
}

/*
void mergeSort(IntCount* begin, IntCount* end, Cmp cmp){

}*/


#endif