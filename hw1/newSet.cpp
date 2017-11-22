#include "newSet.h"
#include <iostream>
using namespace std;

Set::Set() {
	set = new ItemType[DEFAULT_MAX_ITEMS];
	mSize = 0;
	mLimit = DEFAULT_MAX_ITEMS;
}

Set::Set(const Set & source) {

	mLimit = source.mLimit;
	mSize = source.mSize;

	set = new ItemType[mLimit];
	for (int i = 0; i < mSize; i++) {
		set[i] = source.set[i];
	}
}

Set::~Set()
{
	delete[] set;
}

bool Set::empty() const {

	if(mSize == 0)
	return true;
	else
	return false;

}// Return true if the set is empty, otherwise false.

Set& Set::operator=(const Set& source) {
	if (this != &source)
	{
		Set temp(source);
		swap(temp);
	}
	return *this;
}

int Set::size() const {
	return mSize;
}// Return the number of items in the set.

bool Set::insert(const ItemType& value) {
	if (contains(value))
		return false;

	if (mSize >= mLimit)
	{
		return false;
	}

	else
	{
		*(set + mSize) = value;
		mSize++;
		return true;
	}
	return false;
}
// Insert value into the set if it is not already present.  Return
// true if the value was actually inserted.  Return false if the
// value was not inserted (perhaps because it is already in the set
// or because the set has a fixed capacity and is full).

bool Set::erase(const ItemType& value) {

	if (contains(value)) //contains(value) is equivalent to this->contains(value)
	{
		int pos = 0;
		for (int i = 0; i < mSize; i++) //find the position
		{
			if (*(set + i) == value)
			{
				pos = i;
			}
		}

		for (; pos < mSize; pos++)
		{
			*(set + pos) = *(set + pos + 1);
		}
		mSize--;
		return true;
	}
	else
	{
		return false;
	}

}
// Remove the value from the set if present.  Return true if the
// value was removed; otherwise, leave the set unchanged and
// return false.

bool Set::contains(const ItemType& value) const
{
	for (int i = 0; i < mSize; i++)
	{
		if (*(set + i) == value)
			return true;
	}
	return false;
}
//  true if the value is in set

bool Set::get(int j, ItemType &value) const {
	if (j >= 0 && j < mSize) {
		ItemType p;
		ItemType k;
		ItemType arr[200];

		for (int i = 0; i < mSize; i++) {
			p = set[i];
			arr[i] = p;
		}

		for (int i = 0; i < mSize; i++) {
			for (int j = 0; j < mSize; j++) {
				if ((arr[j] < arr[i]) && j>i) {
					k = arr[i];
					arr[i] = arr[j];
					arr[j] = k;
				}
			}
		}

		value = arr[j];
		return true;

	}
	else
		return false;
}

void Set::swap(Set& other) {

	ItemType* tempD = set;
	set = other.set;
	other.set = tempD;

	int temp2 = mSize;
	mSize = other.mSize;
	other.mSize = temp2;

	int temp3 = mLimit;
	mLimit = other.mLimit;
	other.mLimit = temp3;
}