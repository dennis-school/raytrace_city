#ifndef PAIR_H_
#define PAIR_H_

class Pair;

class Pair;
typedef Pair Point2;
typedef Pair Vector2;

class Pair {
    public:
// --- data members ------------------------------------------------------------

        // union to acces the same elements by
        // x, y or data[index]
        union {
            double data[2];
            struct {
                double x;
                double y;
            };
        };

// --- Constructors ------------------------------------------------------------

        explicit Pair(double X = 0, double Y = 0);

};

#endif
