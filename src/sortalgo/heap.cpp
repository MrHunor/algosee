/*
 *   algosee; a algorithm visulizer
 *   Copyright (C) 2026  MrHunor, siryanni, Vilmos45 (as equals)
 *   "Es mejor morir de pie que vivir toda una vida arrodillado" ~ Emiliano Zapata.
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.(root/LICENSE)  If not, see
 * <https://www.gnu.org/licenses/>.
 */
#include "../defs/general.h"
#include "../defs/types.h"
#include "../stream/stream.h"
#include "../utils/image/image.h"
#include <algorithm>
#include <nlohmann/json.hpp>
#include <utility>
#include <vector>

//-----------------------NUMBERS--------------------------------------------------

/*  Tree
      0
      |
      |
 Index(Highest)
    |    |
    |    |
    |    |
I(next)  I(next)

*/

/* Again dont ask me why these formulas work, i dont quite understand either
Parent =	(i - 1) / 2
Left child	= 2 * i + 1
Right child	= 2 * i + 2*/

// you need to call this recusivly to sort because you
// might move elements that then do not satisfy the max
// heap condition, and before you laught at me yes it
// appears heapify is he trivial name for this function
void heapify(std::vector<int> &arr, int size, int largestIndex)
{
    int originalParent = largestIndex;
    int leftChild = 2 * originalParent + 1;
    int rightChild = 2 * originalParent + 2;

    if (leftChild < size)
    {
        if (arr[leftChild] > arr[largestIndex])
        {
            largestIndex = leftChild;
        }
    }
    if (rightChild < size)
    {
        if (arr[rightChild] > arr[largestIndex])
        {
            largestIndex = rightChild;
        }
    }
    if (largestIndex != originalParent)
    {
        std::swap(arr[largestIndex], arr[originalParent]);
        heapify(arr, size,
                largestIndex); // sice we just moved the parent to one of the children we
                               // must check that the move still satifies the heap propery
                               // at that childrens index; (variable name might be
                               // confusing, the "largest index" represents the index of
                               // the childs node that was promoted to parent) (effectivly
                               // doing a top to bottom check)
    }
}
void buildHeap(std::vector<int> &unsorted) // adjesent matrix
{
    int lastNonLeafNodeIndex =
        unsorted.size() / 2 -
        1; // dont ask me why this works, we just treat the input as a binary tree but i
           // dont understand why this formula works
    for (int i = lastNonLeafNodeIndex; i >= 0; i--) // bottomUp
    {
        heapify(unsorted, unsorted.size(), i);
    }
}

void HeapSort(std::vector<int> &unsorted, json &response, StreamClass &stream)
{
    size_t size = unsorted.size();
    buildHeap(unsorted);
    for (int i = size; i > 0; i--)
    {
        std::vector<int> heap(unsorted.begin(), unsorted.end() - i);
        std::vector<int> arr(unsorted.end() - i, unsorted.end());
        response["HEAP"].push_back(std::move(heap));
        response["ARRAY"].push_back(std::move(arr));
        if (response["HEAP"].size() > 100)
        {
            stream.sendChunk(std::move(response));
            response.clear();
        }
        std::swap(unsorted[0], unsorted[i - 1]);
        heapify(unsorted, i - 1,
                0); // this call is pretty smart, it generates the heap again, but leaves
                    // the now sorted element at the end out
    }
}

//-----------------------------------IMAGE-------------------------------------------
void heapify(indexPixel &arr, int size, int largestIndex)
{
    int originalParent = largestIndex;
    int leftChild = 2 * originalParent + 1;
    int rightChild = 2 * originalParent + 2;

    if (leftChild < size)
    {
        if (arr[leftChild].first > arr[largestIndex].first)
        {
            largestIndex = leftChild;
        }
    }
    if (rightChild < size)
    {
        if (arr[rightChild].first > arr[largestIndex].first)
        {
            largestIndex = rightChild;
        }
    }
    if (largestIndex != originalParent)
    {
        std::swap(arr[largestIndex], arr[originalParent]);
        heapify(arr, size,
                largestIndex); // sice we just moved the parent to one of the children we
                               // must check that the move still satifies the heap propery
                               // at that childrens index; (variable name might be
                               // confusing, the "largest index" represents the index of
                               // the childs node that was promoted to parent) (effectivly
                               // doing a top to bottom check)
    }
}
void buildHeap(indexPixel &unsorted) // adjesent matrix
{
    int lastNonLeafNodeIndex =
        unsorted.size() / 2 -
        1; // dont ask me why this works, we just treat the input as a binary tree but i
           // dont understand why this formula works
    for (int i = lastNonLeafNodeIndex; i >= 0; i--) // bottomUp
    {
        heapify(unsorted, unsorted.size(), i);
    }
}

void HeapSort(indexPixel &unsorted, json &response, StreamClass &stream)
{
    size_t size = unsorted.size();
    buildHeap(unsorted);
    for (int i = size; i > 0; i--)
    {
        indexPixel heap(unsorted.begin(), unsorted.end() - i);
        indexPixel arr(unsorted.end() - i, unsorted.end());
        response["HEAP"].push_back(IndexPixelArrToJson(std::move(heap)));
        response["ARRAY"].push_back(IndexPixelArrToJson(std::move(arr)));
        if (response["HEAP"].size() > 100)
        {
            stream.sendChunk(std::move(response));
            response.clear();
        }
        std::swap(unsorted[0], unsorted[i - 1]);
        heapify(unsorted, i - 1,
                0); // this call is pretty smart, it generates the heap again, but leaves
                    // the now sorted element at the end out
    }
}