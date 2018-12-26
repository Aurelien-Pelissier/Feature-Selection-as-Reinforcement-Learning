#include <vector>
using namespace std;

#include "quickselect.h"

/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 　　Quickselect algorithm return the kth smallest element in a vector, and have a average complexity of O(n).                 //
/// 　　This program is a slightly modified quickselect to return the indexes of the k smallest elements in the initial vector.　  //　
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



vector<int> quick_select(const int &k0, vector<pair<float, int>> &Arr0, const bool &make_copy)
//return the indexes of the k0 smallest elements in the initial array, Inputs are:
//  k = number of nearest neighbors desired
//	&Arr0 = pointer to the vector
//  (make_copy) if you dont want your vector to be modified by quickselect
{
    int p = Arr0.size()-1; //element on the left side of the search
    int r = 0; //element on the right side of the search

    if (make_copy)
    {
        vector<pair<float, int>> Arr = Arr0;
        return select(k0, Arr, k0, r, p);
    }
    else
    {
        return select(k0, Arr0, k0, r, p);
    }

}





vector<int> select(const int &k0, vector<pair<float, int>> &Arr, int k, int p, int r)
//return the indexes of the k0 smallest elements in the initial array, Inputs are:
//  k0 = number of nearest neighbors desired
//	&Arr = pointer to the vector
//  k = the searched kth greatest element of the array --> no
//	p = first  -  left
//	r = last  -  right

//	Arr is a vector of pair containing {value, initial index}, the initial index needs to be saved because the quickselect function modify the array

{
    if ( p == r )
    {
        vector<int> n_index;
        for (int ki=0; ki<k0; ki++)
        {
            n_index.push_back(Arr[p-ki].second);
        }
        return n_index;
    }

    int j = partitions(Arr, p, r);
    int length = j - p + 1;

    if ( k > length )
	{
		return select(k0, Arr, k - length, j + 1, r);
	}

    else if ( k < length )
	{
		return select(k0, Arr, k, p, j - 1);
	}

    else
	{
		vector<int> n_index;
		for (int ki=0; ki<k0; ki++)
        {
            n_index.push_back(Arr[j-ki].second);
        }
		return n_index;
	}
}







int partitions(vector<pair<float, int>> &Arr, int p, int r)
//it rearranges the elements so those less than a selected pivot are at lower indices in the array than the pivot
//and those larger than the pivot are at higher indices in the array

// it return the index of the pivot element j

{
    float pivot = Arr[r].first; //The pivot is taken as the right side

    while ( p < r )
    {
        while ( Arr[p].first < pivot )
            p++;

        while ( Arr[r].first > pivot )
            r--;

        if ( Arr[p].first == Arr[r].first )
            p++;
        else if ( p < r ) {
            pair<float, int> tmp = Arr[p];
            Arr[p] = Arr[r];
            Arr[r] = tmp;
        }
    }

    return r;
}
