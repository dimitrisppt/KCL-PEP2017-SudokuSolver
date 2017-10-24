#ifndef BEST_FIRST_SEARCH_H
#define BEST_FIRST_SEARCH_H

#include "Searchable.h"

#include <memory>
using std::unique_ptr;

class BestFirstSearch {

protected:

    /// Make sure you increment this every time you 'expand' a node, by getting it successors and putting them on the queue        
    int nodes = 0;
    
    
public:

    BestFirstSearch(std::unique_ptr<Searchable> && startFrom) {
        // TODO Put startFrom onto the queue:
        
    }
    
    int getNodesExpanded() const {
        return nodes;
    }
    
    Searchable * solve() {
            
        // TODO Implement search, returning a pointer to the solution (if one is found)
    
        return nullptr;
    }
};


// Do not edit below this line

#endif
