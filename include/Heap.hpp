//
// Created by User on 2025/9/19.
//

#ifndef PLINC_HEAP_HPP
#define PLINC_HEAP_HPP

namespace OplHeap {
    class Heap {
    public:
        Heap(int);
    private:
        int* mem;
        int size;
    };

    Heap::Heap(int size) {

    }
}

#endif //PLINC_HEAP_HPP
