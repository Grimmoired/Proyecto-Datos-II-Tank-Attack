//
// Created by j1p2p3a4 on 5/18/2026.
//

#ifndef TANKATTACK_PRIORITY_QUEUE_H
#define TANKATTACK_PRIORITY_QUEUE_H
#include "pair.h"
#include "ArregloDinamico.h"

using wto  = pair<int,pair<int,int>>;

struct priority_queue {
    ArregloDinamico<wto> heap;

     void swap(wto& a, wto& b) {
        wto temp = a;
        a = b;
        b = temp;

    }

    void push(wto val) {
        heap.agregar(val);
        int idx = heap.size() - 1;
        while (idx > 0) {
            int p = (idx - 1)/2;
            if (heap[p].first > heap[idx].first) {
                wto temp = heap[p];
                heap[p] = heap[idx];
                heap[idx] = temp;

                idx = p;
            } else {
                //esta en el orden correcto
                break;
            }
        }
    }

    void pop() {
        wto temp = heap[0];
        heap[0] = heap[heap.size() - 1];
        heap[heap.size() - 1] = temp;

        heap.pop_back();

        int i = 0, n = heap.size();

        while (true) {
            int l = 2*i + 1, r = 2*i + 2;
            int smallest = i;
            if (l < n && heap[l].first < heap[smallest].first) {
                smallest = l;
                swap(heap[smallest], heap[l]);
            }
            if (r < n && heap[r].first < heap[smallest].first) {
                smallest = r;
                swap(heap[smallest],heap[r]);
            }

            if (smallest == i) break;

            i = smallest;
        }
    }

    wto top() {
        return heap[0];
    }

    bool empty() {
        return heap.vacio();
    }

    bool erase() {
         heap.limpiar(); 
     }
};
#endif //TANKATTACK_PRIORITY_QUEUE_H
