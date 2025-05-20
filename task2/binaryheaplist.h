#ifndef BINARYHEAPLIST_H
#define BINARYHEAPLIST_H
#include <queue>
#include "node.h"
class BinaryHeapList {
    std::shared_ptr<node> root;
    std::shared_ptr<node> get_free_node();
    void shiftUp(std::shared_ptr<node>);
    void shiftDown(std::shared_ptr<node>);
    void cl(std::shared_ptr<node>& v);

   public:
    void insert(int key);
    std::shared_ptr<node> get_last();
    std::pair<int, bool> get_max();
    void clear();
    bool empty();
};

#endif	// BINARYHEAPLIST_H
