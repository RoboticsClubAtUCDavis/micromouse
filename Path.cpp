#include "Path.h"
#include <assert.h>

std::vector<DirectionVector>::const_iterator Path::begin() const {
    return directions.begin();
}
std::vector<DirectionVector>::const_iterator Path::end() const {
    return directions.end();
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
