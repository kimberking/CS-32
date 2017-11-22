#ifndef SET_H
#define SET_H
#include <string>
using namespace std;
typedef std::string ItemType;

class Set
{
public:
	Set();
	Set(const Set& source);
	~Set();
	Set& operator=(const Set& source);
	bool empty() const;
	int size() const;
	bool insert(const ItemType& value);
	bool erase(const ItemType& value);
	bool contains(const ItemType& value) const;
	bool get(int pos, ItemType& value) const;
	void swap(Set& other);

private:
	struct Node
	{
		ItemType mData;
		Node* next;
		Node* previous;
	};

	Node* mHead;
	Node* mTail;
	int mSize;
	void EmptySet();
	Node *finder(const ItemType& value);
};

void subtract(const Set& s1, const Set& s2, Set& result);
void unite(const Set& s1, const Set& s2, Set& result);

#endif