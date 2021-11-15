/*
 * minCircle.cpp
 *
 * Author: Noa Eitan 316222777, Coral Kuta 208649186
 */


#include "minCircle.h"

/**
 * the distance between 2 points
 * @param a
 * @param b
 * @return the distance
 */
float distance(Point a, Point b) {
    return sqrtf(((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)));
}

/**
 * Check if the point is inside the circle
 * @param c
 * @param p
 * @return ture if the point is inside the circle, false otherwise
 */
bool is_inside(const Circle& c, const Point& p) {
    return distance(c.center, p) <= c.radius;
}


/**
 * Helper function
 * @param bx
 * @param by
 * @param cx
 * @param cy
 * @return a point
 */
Point get_circle_center(float bx, float by, float cx, float cy) {
    float B = bx * bx + by * by;
    float C = cx * cx + cy * cy;
    float D = bx * cy - by * cx;
    return { (cy * B - by * C) / (2 * D),
             (bx * C - cx * B) / (2 * D) };
}


/**
 * Create the single circle that intersects 3 points
 * @param A
 * @param B
 * @param C
 * @return the circle
 */
Circle circle_from(const Point& A, const Point& B, const Point& C) {
    Point I = get_circle_center(B.x - A.x, B.y - A.y, C.x - A.x, C.y - A.y);

    I.x += A.x;
    I.y += A.y;
    return { I, distance(I, A) };
}


/**
 * Create the smallest possible circle that intersects 2 points
 * @param A
 * @param B
 * @return the circle
 */
Circle circle_from(const Point& A, const Point& B) {
    // set the center to be the midpoint of A and B
    Point C = Point( float ((A.x + B.x) / 2), float ((A.y + B.y) / 2) );

    // set the radius to be half the distance AB
    return { C, (distance(A, B) / 2) };
}


/**
 * Check if the circle is valid - if the points are within the circle's boundaries.
 * @param c
 * @param P
 * @return true if the circle is valid, false otherwise.
 */
bool is_valid_circle(const Circle& c, const vector<Point>& P) {
    // we iterate through all the points to check whether the points are inside the circle or not
    for (const Point& p : P)
        if (!is_inside(c, p))
            return false;
    return true;
}


/**
 * Create a circle for edge cases - given 0, 1, 2 or 3 points.
 * @param P
 * @return the trivial circle
 */
Circle min_circle_trivial(vector<Point>& P) {

    if (P.empty()) {
        // if we don't have points we return a circle which is a point - the center of the axes.
        return { Point(0, 0), 0 };
    }
    else if (P.size() == 1) {
        // if we have only one point we return a circle that's the point is the center and the radius is 0.
        return { P[0], 0 };
    }
    else if (P.size() == 2) {
        // if we have 2 points we return a circle from those 2 points.
        return circle_from(P[0], P[1]);
    }

    // we check if the circle can be determined by only 2 points which make the diameter
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {

            Circle c = circle_from(P[i], P[j]);
            if (is_valid_circle(c, P))
                return c;
        }
    }
    return circle_from(P[0], P[1], P[2]);
}


/**
 * Create the minimum enclosing circle using Welzl's recursive algorithm.
 * We have 2 vectors of points - to_process and boundary.
 * to_process contains the points that are yes to be processed.
 * boundary contains the points that determine the boundaries of the minimum enclosing circle created
 * by the given points.
 * We pick random points from to_process and create circles, until all points are inside.
 * @param to_process
 * @param boundary
 * @param size - the number of points in to_process that are not yet processed.
 * @return the minimum enclosing circle
 */
Circle welzl(vector<Point>& to_process, vector<Point> boundary, int size) {
    // base case when all points processed or |boundary| = 3 and thus we have a single circle.
    if ((size == 0) || (boundary.size() == 3)) {
        return min_circle_trivial(boundary);
    }

    // pick a random point randomly
    int idx = rand() % size;
    Point p = to_process[idx];

    /* put the picked point at the end of to_process since it's more efficient than deleting from the
     * middle of the vector. */
    swap(to_process[idx], to_process[size - 1]);

    /* Get the minimum enclosing circle d from the set of points to_process - {p}.
     we send size - 1 because we deleted one point. */
    Circle d = welzl(to_process, boundary, size - 1);

    // if our circle d contains p, it's a good circle, we return it.
    if (is_inside(d, p)) {
        return d;
    }

    // otherwise, it's out. It must be on the boundary of our minimum enclosing circle, so we insert it to boundary.
    boundary.push_back(p);

    // return the minimum enclosing circle for to_process - {p} and the new boundary (with {p})
    return welzl(to_process, boundary, size - 1);
}


Circle findMinCircle(Point** points,size_t size) {
    // creating a vector from the given points
    vector<Point> v_points(size);
    for (int i = 0; i < size; i++) {
        v_points[i] = *points[i];
    }
    int v_size = int(size);

    // sending it to welzl's algorithm
    return welzl(v_points, {}, v_size);
}
