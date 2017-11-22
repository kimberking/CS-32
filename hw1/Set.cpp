#include "Set.h"
#include <string>
#include <iostream>
using namespace std;

Set::Set() {
	mSize = 0;
}

bool Set::empty() const {
	if (mSize == 0)
		return true;
	else
	return false;
}

int Set::size() const {
	return mSize;
}

bool Set::insert(const ItemType& value) {

	for (int j = 0; j < mSize; j++) //check 0th~mSize item (theoretically up to 199th element) 
	{
		if (set[j] == value)
			return false;
	}

	if (mSize >= DEFAULT_MAX_ITEMS) {
		return false;
	}

	else {
		set[mSize] = value;
		mSize++;
		return true;
	}
	return false;
}

bool Set::erase(const ItemType& value)
{
	if (this->contains(value))
	{
		int position = 0;
		for (int j = 0; j < mSize; j++) //find the position
		{
			if (set[j] == value)
			{
				position = j;
			}
		}

		for (; position + 1 < mSize; position++)
		{
			set[position] = set[position + 1];
		}
		mSize--;
		return true;
	}
	else
	{
		return false;
	}
}

bool Set::contains(const ItemType& value) const
{
	for (int j = 0; j < mSize; j++)
	{
		if (set[j] == value)
			return true;
	}
	return false;
}

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

	ItemType temp;
	int size2 = (other.mSize > this->mSize)? other.mSize :this->mSize;

	for (int i = 0; i < size2; i++) {
		temp = other.set[i];
		other.set[i] = this->set[i];
		this->set[i] = temp;
	}
	
	int temp2 = other.mSize;
	other.mSize = this->mSize;
	this->mSize = temp2;
	
}