#include "SymbolTable.h"
#include <string>
#include <stack>
#include <vector>
using namespace std;
#define tableSize 19997

struct item {
	string id;
	int lineNum;
	item* next;
	int scope;
};

int hashf(const string &id)
{
	unsigned int hash = 2166136261U;
	int index;
	for (int i = 0; i < id.length(); i++)
	{
		hash = hash + (int)id[i];
		hash *= 16777619;
	}
	index = hash%tableSize;
	return index;
}

class Hash {
public:
	Hash();
	~Hash();
	void deleteScope(const int& scope);
	int find(const string& id, const int& scope) const;
	bool addItem(string id, int &lines, int &scope);
	bool contains(const string& id, const int& scope);

private:

	item* HashTable[tableSize];
	vector <item> current;
};

Hash::Hash() {
	for (int i = 0; i < tableSize; i++) {
		HashTable[i] = new item;
		HashTable[i]->id = "";
		HashTable[i]->lineNum = 0;
		HashTable[i]->scope = 0;
		HashTable[i]->next = nullptr;
	}
}

Hash::~Hash() {

	for (int i = 0; i < tableSize; i++) {
		while (HashTable[i] != nullptr) {
			item* a = HashTable[i];
			HashTable[i] = HashTable[i]->next;
			delete a;
		}
	}
}

bool Hash::addItem(string id, int &lines, int &scope) {

	int i = hashf(id);

	if (HashTable[i]->id == "")
	{
		HashTable[i]->id = id;
		HashTable[i]->lineNum = lines;
		HashTable[i]->scope = scope;
		current.push_back(*HashTable[i]);
		return true;

	} else {

		item *ptr = HashTable[i];

		while (ptr->next != nullptr)
		{
			if (ptr->id == id && ptr->scope == scope)
			{	
				return false;
			}
			ptr = ptr->next;
		}

		if (ptr->id == id && ptr->scope == scope) {
			
			return false;
		}

		item *n = new item;
		n->id = id;
		n->lineNum = lines;
		n->scope = scope;
		n->next = nullptr;
		ptr->next = n;
		current.push_back(*n);
		return true;
	}
}

int Hash::find(const string& id, const int& scope) const {

	int index = hashf(id);
	int lineNum = -1;

	item* ptr = HashTable[index];

	if (ptr == nullptr) {
		return lineNum;
	}

	do {
		if (ptr->id == id && ptr->scope <= scope) {

			lineNum = ptr->lineNum;
		}
		ptr = ptr->next;

	} while (ptr != nullptr);

	return lineNum;
}

void Hash::deleteScope(const int& scope) {

	for (int i = current.size() - 1; i >= 0 && current[i].scope == scope; i--)
	{
		if (current[i].scope == scope)
		{
			string key = current[i].id;
			int lines = current[i].lineNum;
			int index = hashf(key);
			item *ptr1 = HashTable[index];
			item *ptr2 = HashTable[index];

			if (ptr1 != nullptr && ptr1->next == nullptr && ptr2 != nullptr && ptr2->next == nullptr)
			{
				if (ptr1->id == key && ptr1->lineNum == lines)
				{
					ptr1->id = "";
					ptr1->lineNum = 0;
					ptr1->scope = 0;
				}
			}

			else
			{
				if (ptr1 != nullptr) {
					while (ptr1 != nullptr){  
						if (ptr1->id == key && ptr1->lineNum == lines)
						{
							if (ptr1->next == nullptr)
							{
								ptr2->next = nullptr;
								delete ptr1;
								break;
							}

							else
							{
								ptr2->next = ptr1->next;
								delete ptr1;
								break;
							}
						}

						ptr2 = ptr1;
						ptr1 = ptr1->next;
					}
				}
			}
			current.pop_back();
		}
	}
}

bool Hash::contains(const string& id, const int& scope) {

	int index = hashf(id);
	int result = 0;

	if (HashTable[index]->id != "") {

		result++;
		item* ptr = HashTable[index];
		while (ptr->next != nullptr)
		{
			result++;
			ptr = ptr->next;
		}

		if (result == 0) {
			return false;
		}

		else {

			item *ptr = HashTable[index];
			while (ptr != nullptr)
			{
				if (ptr->id == id && ptr->scope <= scope) {
					return true;
				}
				ptr = ptr->next;
			}
		}
	}

	return false;
}

class SymbolTableImpl
{
public:
	SymbolTableImpl() { mHashTable = new Hash(); };
	~SymbolTableImpl() { delete mHashTable; };
	void enterScope() { mScope++; };
	bool exitScope();
	bool declare(const string& id, int lineNum) ;
	int find(const string& id) const;
private:
	Hash* mHashTable;
	int mScope = 0;
};

bool SymbolTableImpl::exitScope() {

	if (mScope == 0) {
		return false;
	}

	else {
		mHashTable->deleteScope(mScope);
		mScope--;
		return true;
	}
}

bool SymbolTableImpl::declare(const string& id, int lineNum) { return ((id.size() == 0) ? false : (mHashTable->addItem(id, lineNum, mScope)) ); }

int SymbolTableImpl::find(const string& id) const { return ((id.size() == 0) ? -1 : (!(mHashTable->contains(id, mScope)) ? -1 : mHashTable->find(id, mScope))); }

SymbolTable::SymbolTable() { m_impl = new SymbolTableImpl; }

SymbolTable::~SymbolTable() { delete m_impl; }

void SymbolTable::enterScope() { m_impl->enterScope(); }

bool SymbolTable::exitScope() { return m_impl->exitScope(); }

bool SymbolTable::declare(const string& id, int lineNum) { return m_impl->declare(id, lineNum); }

int SymbolTable::find(const string& id) const { return m_impl->find(id); }