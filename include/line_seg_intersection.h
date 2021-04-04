#ifndef __LINE_SEG_INTERSECTION_H
#define __LINE_SEG_INTERSECTION_H
#include <line.hpp>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <algorithm>

using std::map;
using std::set;
using std::pair;
using std::vector;

/**
 * @brief Points comparator class, based on lexicographic order
*/
struct ComparePts {
	/**
	 * @brief Lexicographic comparator
	 * @param p lhs
	 * @param q rhs
	 * @return (lhs.y > rhs.y) or (lhs.y == rhs.y && lhs.x < rhs.x)
	*/
	bool operator()(const Point& p, const Point & q) const;
};

/**
 * @brief Representing priority queue as an ordered map
*/
using PriorityQueue = map<Point, vector<Segment>, ComparePts>;

/**
 * @brief Printing the content of a priority queue
 * @param os Output stream reference
 * @param p priority queue
 * @return Output stream reference
*/
std::ostream& operator<<(std::ostream& os, const PriorityQueue& p);

/**
 * @brief Get min point in lexicographic order
 * @param p lhs
 * @param q rhs
 * @return the min between lhs and rhs according to the lexicographic order
*/
Point getMin(const Point & p, const Point & q);

/**
 * @brief Get max point in lexicographic order
 * @param p lhs
 * @param q rhs
 * @return the max between lhs and rhs according to the lexicographic order
*/
Point getMax(const Point & p, const Point & q);

/**
 * @brief Get the upper point of a segment according to lexicographic order
 * @param s Segment
 * @return upper point
*/
Point getUp(const Segment &s);

/**
 * @brief Get the lower point of a segment according to lexicographic order
 * @param s Segment
 * @return lower point
*/
Point getDown(const Segment & s);

/**
 * @brief Comparator class for segments assuming they both intersect an horizontal line
*/
struct SegmentComparator {
	/**
	 * @brief Segment comparator, it is assumed the segments are not horizontal
	 * @param s lhs
	 * @param r rhs
	 * @return lhs < rhs
	*/
	bool operator()(const Segment & s, const Segment & r) const;
	bool * above; //<! a boolean flag affecting the order of the segments intersecting each other on the line
	float * y; //<! horizontal line coordinate
};

using SweepLine = set<Segment, SegmentComparator>;
using SweepLineIter = SweepLine::iterator;
constexpr float THRESHOLD = 0.0001f; //<! Floating point threshold

/**
 * @brief Status structure for line segments intersection
*/
struct StatusStructure {

	StatusStructure();

	/**
	 * @brief Get iterator to the first segment incident to p
	 * @param p Point on the sweepline
	 * @return Iterator to the first segment containing p
	*/
	SweepLineIter getIncident(const Point& p);

	/**
	 * @brief Insert segment in the status structure
	 * @param s Segment to be inserted
	*/
	void insertSegment(const Segment& s);

	/**
	 * @brief Find the first neighbour on the left of a given segment on the sweepline
	 * @param x coordinate on the sweepline
	 * @param sl left neighbour
	 * @return true if there's a segment on the left (not nil)
	*/
	bool findLeftNeighboor(float x, Segment& sl) const;

	/**
	 * @brief Find the first neighbour on the right of a given segment on the sweepline
	 * @param x coordinate on the sweepline
	 * @param sr right neighbour
	 * @return true if there's a segment on the right (not nil)
	*/
	bool findRightNeighboor(float x, Segment& sr) const;

	/**
	 * @brief Find leftmost and right most segments of a star of segments passing through a given point on the sweepline
	 * @param pt Point on the sweepline
	 * @param it_l Iterator to the left most segment
	 * @param it_r Iterator to the right most segment
	*/
	void findLeftmostAndRightmost(const Point& pt, SweepLineIter& it_l, SweepLineIter& it_r);
	
	bool above; //!< boolean representing if the sweepline is infinitesimally above the specified y coordinate
	float y_line; //!< y coordinate of the sweepline
	SegmentComparator segComp; //!< segments comparator
	SweepLine sweepLine; //!< Balanced tree like structure representing the ordered lines in the sweepline
	SweepLineIter nil; //!< nil pointer
	float angleUpperBound; //!< auxiliary variable used to speedup the search of segments intersecting a given point on a sweepline
	float angleLowerBound; //!< auxiliary variable used to speedup the search of segments intersecting a given point on a sweepline
};

std::ostream& operator<<(std::ostream& os, const StatusStructure& tau);

/**
 * @brief Line segments intersection algorithm as explained in DeBerg at all.
 * @param S List of segments
 * @return Vector of intersection points
*/
vector<Point> computeIntersection(vector<Segment> & S);

#endif