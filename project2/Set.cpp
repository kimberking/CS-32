#include "Set.h"
#include <iostream>


Set::Set()
{
	//create the node stuff
	mSize = 0;
	//start off by setting the head and tail to null
	mHead = NULL;
	mTail = NULL;
}

Set::Set(const Set& source)
{
	mSize = source.mSize;

	//copy the source over to the object
	Node *currentOGnode = source.mHead;
	Node *Node2 = NULL;
	for (int i = 0; i <mSize; i++) {

		//make a new node to store the datas
		Node *newNode = new Node;
		newNode->mData = currentOGnode->mData;
		currentOGnode = currentOGnode->next;


		if (i == 0) {
			//rearrange the nodes
			Node2 = newNode;
			newNode->next = newNode;
			newNode->previous = newNode;
			mHead = newNode;
			mTail = newNode;
			continue; //have the for loop run again
		}

		newNode->previous = Node2;
		Node2->next = newNode;
		Node2 = newNode;

		if (i == mSize - 1) {
			//if it is equal run it unless it's 1 item
			mTail = newNode;
			newNode->next = mHead;
			mHead->previous = mTail;
		}
	}
}

Set::~Set()
{
	if (mHead == NULL) {
		return;
	}
	Node * currentNode = mHead->next;
	while (currentNode != mHead) {
		delete currentNode->previous;
		currentNode = currentNode->next;
	}
	delete mTail;
	mHead = NULL;
	mTail = NULL;
}

Set& Set::operator=(const Set& source)
 {
	if (&source == this) {
		return *this;
	}
	this->~Set();
	mSize = source.mSize;
	//copy the source over to the object
	Node *currentOGnode = source.mHead;
	Node *Node2 = NULL;
	for (int i = 0; i <mSize; i++) {
		//make a new node
		Node *newNode = new Node;
		newNode->mData = currentOGnode->mData;
		currentOGnode = currentOGnode->next;
		if (i == 0) {
			//set the nodes for the first item
			Node2 = newNode;
			newNode->next = newNode;
			newNode->previous = newNode;
			mHead = newNode;
			mTail = newNode;
			continue; //run the loops again
		}
		//set a node for the next time
		newNode->previous = Node2;
		Node2->next = newNode;
		Node2 = newNode;

		if (i == mSize - 1) { //do unless it is a one item set
			mTail = newNode;
			newNode->next = mHead;
			mHead->previous = mTail;
		}
	}

	return *this;
}

bool Set::empty() const
{
	return (mSize == 0); //return when size is zero
}

int Set::size() const
{
	return mSize; //return the size
}

bool Set::insert(const ItemType& value)
{
	if (contains(value)) //if the value is in the set
	{
		return false;
	}
	else {
		//if the value is not in the set then make a new node to store the value
		Node* newNode = new Node;
		newNode->mData = value;
		int insertIndex = 0;

		Node *firstBigger = mHead;
		for (; insertIndex < mSize; insertIndex++) {

			//arrange the set by order so it makes the get function easier
			if ((firstBigger != NULL) && (firstBigger->mData > value)) {
				break;
			}
			else {
				firstBigger = firstBigger->next;
			}
		}

		if (insertIndex == 0) { //insert in head
			if (mSize == 0) { //special case
				//rearrange the pointers
				mTail = newNode;
				mHead = newNode;
				newNode->next = newNode;
				newNode->previous = newNode;
				mSize++;
				return true;
			}
			mHead = newNode;
		}

		if (insertIndex == mSize) { //insert in tail
			mTail = newNode;
		}

		//rearrange the pointers after insertion
		newNode->next = firstBigger;
		newNode->previous = firstBigger->previous;
		firstBigger->previous = newNode;
		newNode->previous->next = newNode;

		mSize++; //increase the size by one

		return true;
	}
}

bool Set::erase(const ItemType& value)
{
	if (contains(value))
	{

		//look for node that stores value
		Node* find = finder(value);
		if (find != NULL) { //if it isnt a null pointer
			//rearrange the pointers
			find->previous->next = find->next;
			find->next->previous = find->previous;

			if (find == mHead) {
				mHead = find->next;
				mHead->previous = mTail;
			}
			if (find == mTail) {
				mTail = find->previous;
			}

			delete find; //delete find after using it
			mSize--;

			if (mSize == 0) { //special case
				mHead = NULL;
				mTail = NULL;
			}
			return true;
		}
	}
	else //if value is not in the set
	{
		return false;
	}
}

bool Set::contains(const ItemType& value) const
{
	if (mHead == NULL) {
		return false;
	}

	Node *find = mHead->next;
	for (int i = 0; i<mSize; i++)
	{
		if (find->mData == value)
		{
			return true;
		}
		else
		{
			find = find->next;
		}
	}
	return false;

}

bool Set::get(int pos, ItemType& value) const
{
	if (pos < 0 || pos >= size()) //if it is out of bounds
		return false;

	Node *find = mHead;
	//search through the loop to find the data
	for (int i = 0; i < pos; i++)
	{
		find = find->next;
	}
	value = find->mData;

	return true;
}

void Set::swap(Set& other)
{
	//swap the heads
	Node* mOtherHead = other.mHead;
	other.mHead = this->mHead;
	mHead = mOtherHead;

	//swap the tails
	Node* mOtherTail = other.mTail;
	other.mTail = this->mTail;
	mTail = mOtherTail;

	//swaps the size of the two linked lists
	int otherSize = other.mSize;
	other.mSize = mSize;
	mSize = otherSize;
}

void unite(const Set& s1, const Set& s2, Set& result)
{
	Set newResult;
	ItemType copy;
	//seach the two sets to see what's the same
	for (int i = 0; i <s1.size(); i++)
	{
		s1.get(i, copy);
		newResult.insert(copy); //insert it into the set if it is the same
	}

	for (int i = 0; i<s2.size(); i++)
	{
		s2.get(i, copy);
		newResult.insert(copy); // insert it into the set if it is the same
	}

	result.swap(newResult); //put newresult into result
}

void subtract(const Set& s1, const Set& s2, Set& result)
{
	ItemType currentS1;
	
	//make three new things to prevent aliasing
	Set newS1 = s1;
	Set newS2 = s2;
	Set newS3 = Set();

	if (s1.size() == 0) { //if it is 0 
		return;
	}

	if (s2.size() == 0) { //second special case
		result = s1;
		return;
	}

	for (int i = 0; i < s1.size(); i++) {
		s1.get(i, currentS1);
		if (!s2.contains(currentS1)) {
			//if it isnt in the set put it in the set
			newS3.insert(currentS1);
	
		}
	}

	result = newS3; //set result to the news3 to prevent aliasing
}

void Set::EmptySet()
{
	//definiting if the set is empty
	mSize = 0;
	mHead = new Node;
	mHead->next = mHead;
	mHead->previous = mHead;
}

Set::Node* Set::finder(const ItemType& value)
{
	Node* find = mHead;
	//make sure the loop runs once to prevent skipping the head
	do { 
		if (find->mData == value) { //find the value
			return find;
		}
		find = find->next;

	} while (find != mHead); //as long is it's not the head
	return NULL;
}