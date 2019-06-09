#ifndef __UTIL_H
#define __UTIL_H
#pragma once
#include <cmath>
#include <vector>

double det2x2(double a, double b, double c, double d);
double det2x2(double v[]);

class UnionFind {
public:
	UnionFind(int numElements) :
		mParent(std::vector<int>(numElements)),
		mRank(std::vector<int>(numElements)),
		mMarked(std::vector<bool>(numElements))
	{
		for (int i = 0; i < numElements; ++i)
		{
			mParent[i] = i;
			mRank[i] = 0;
			mMarked[i] = false;
		}
	}

	int find(int elementIndex)
	{
		while (mParent[elementIndex] != elementIndex)
		{
			elementIndex = mParent[elementIndex];
		}
		return elementIndex;
	}

	void merge(int firstElementIndex, int secondElementIndex)
	{
		int firstSetRepresentant = find(firstElementIndex);
		int secondSetRepresentant = find(secondElementIndex);

		if (firstSetRepresentant == secondSetRepresentant)
			return;
		
		//actual union by rank
		if (mRank[firstSetRepresentant] < mRank[secondSetRepresentant])
		{
			mParent[firstSetRepresentant] = secondSetRepresentant;
		}
		else
		{
			mParent[secondSetRepresentant] = firstSetRepresentant;
		}
		
		
		if(mRank[firstSetRepresentant] == mRank[secondSetRepresentant])
		{
			++mRank[firstSetRepresentant];
		}
	}

private:
	std::vector<int> mParent;
	std::vector<int> mRank;
	std::vector<bool> mMarked;
};

#endif