#include <vector>
#include <set>
#include <iostream>
#include "../Headers/DuplicateRemover.h"

void DuplicateRemover::removeDuplicates(std::vector<int> &vect) {
    std::set<int> uniqueElements(vect.begin(), vect.end());
    vect.assign(uniqueElements.begin(), uniqueElements.end());
}
