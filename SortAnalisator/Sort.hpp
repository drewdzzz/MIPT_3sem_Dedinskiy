#ifndef SORT_INCLUDE
#define SORT_INCLUDE

template<class T>
void BubbleSort(T* begin, T* end) {
	for (T* curr1 = begin; curr1 != end; ++curr1)
		for (T* curr2 = curr1; curr2 != end; ++curr2) 
			if (*(curr2) < *(curr1)) {
				T temp = *curr2;
				*curr2 = *(curr1);
				*(curr1) = temp;
			}
}




template<typename T>
T* partition(T* begin, T* end) {

    T* move_place = begin - 1;

    T* it = begin;

    while (it < end) {
        if(*it <= *end) {
            ++move_place;
            std::swap(*move_place, *it);
        }
        ++it;
    }

    std::swap(*(move_place + 1), *end); 

    return ++move_place;
}

template<typename T>
void QuickSort(T* begin, T* end) {
    if(begin != end && begin + 1 != end){
        T* mid = partition (begin, end - 1);
        QuickSort (begin, mid);
        QuickSort (mid + 1, end);
    }
}

/*
void mergeSort(IntCount* begin, IntCount* end, Cmp cmp){

}*/


#endif