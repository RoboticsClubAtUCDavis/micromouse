#include "Path.h"
#include <assert.h>

std::vector<DirectionVector>::const_reverse_iterator Path::begin() const {
    return directions.rbegin();
}
std::vector<DirectionVector>::const_reverse_iterator Path::end() const {
    return directions.rend();
}

std::vector<DirectionVector>::size_type Path::size() const {
    return directions.size();
}

void Path::push_back(DirectionVector vec) {
    directions.push_back(vec);
}

void Path::clear() {
    directions.clear();
}
