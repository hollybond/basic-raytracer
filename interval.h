#ifndef INTERVAL_H
#define INTERVAL_H

class interval {
public:
    double min, max;

    interval() : min(+infinity), max(-infinity) {};

    interval(const double min, const double max) : min(min), max(max) {};

    const double size() const {
        return max - min;
    }

    const bool contains(const double x) const {
        return x >= min && x <= max;
    }

    const bool surrounds(const double x) const {
        return min < x && x < max;
    }

    static const interval empty, universe;
};

const interval interval::empty  = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, infinity);

#endif //INTERVAL_H
