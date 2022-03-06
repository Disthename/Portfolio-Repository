#include <iostream>
#include<algorithm> //std::max

using namespace std;
typedef char Elem;
class ArraySequenceE {
public:
	class Iterator {
	public:
		//int j = 0; // index of the next element to report 
		//bool removable = false; // can remove be called at this time? 
		Elem* v;        // pointer to the dynamic array 
		Iterator(Elem* u)      // //constructorcreate from pointer
		{
			v = u;
		}// an iterator for the list
	public:

		Elem& operator*() { return *v; }      // reference to the element
		Iterator& operator++()
		{
			++v;
			return *this;
		}

		// move to previous position
		Iterator& operator--()
		{
			--v;
			return *this;
		}

		// compare positions
		bool operator==(const Iterator& p) const
		{
			return v == p.v;
		}

		bool operator!=(const Iterator& p) const
		{
			return v != p.v;
		}
		// Define postfix increment operator.  
		template <class E>
		Iterator operator++(E) //post increment operator overloading
		{
			try {
				if (currentSize== currentCapacity) throw invalid_argument("No next element");
				Iterator temp = *this; // save the original value
				++* this;  //increment
				return temp; // return the older value
			}
			catch (...) {
				cout << "No next element " << endl;
			}
		}

    // give ArraySequenceE access

	};

private:
	Elem* dynamicArray;
	int currentSize;
	int currentCapacity;
public:
	ArraySequenceE() {
		currentSize = 0;
		currentCapacity = 10;
		dynamicArray = new Elem[currentCapacity];
	}
	ArraySequenceE(Elem* _dynamicArray_, int _currentSize_, int _currentCapacity_) {
		currentSize = _currentSize_;
		currentCapacity = _currentCapacity_;
		for (int i = 0; i < currentCapacity; i++)
			dynamicArray[i] = _dynamicArray_[i];
	}
	~ArraySequenceE() {
		delete[] dynamicArray;
	}
	ArraySequenceE& operator=(const ArraySequenceE& rhs) {
		if (this != &rhs) {
			currentSize = rhs.currentSize;
			currentCapacity = rhs.currentSize;
			delete[] dynamicArray;
			dynamicArray = new Elem[currentCapacity];
			for (int i = 0; i < currentCapacity; i++)
				dynamicArray[i] = rhs.dynamicArray[i];
		}
		return *this;
	}
	int size() const { return currentSize; }
	bool empty() const { return currentSize == 0; }

	void reserve(int N) {     // reserve at least N spots
		if (currentCapacity >= N) return;      // already big enough
		Elem* B = new Elem[N];       // allocate bigger array
		for (int j = 0; j < currentSize; j++)      // copy contents to new array
			B[j] = dynamicArray[j];
		if (dynamicArray != NULL) delete[] dynamicArray;      // discard old array
		dynamicArray = B;           // make B the new array
		currentCapacity = N;         // set new capacity
	}
	//insertion:
	//void insert(const Iterator& p, const Elem& e) // insert e before p
	void insert(Iterator& p, const Elem& e) // insert e before p
	{
		//note that p is not const so that we can fix it if the original array was expanded.
		int j = 0;
		if (currentSize >= currentCapacity) {      // overflow?
		 //find rank of p and stored to use it later for fixing p after changing the array to the exanded one.
			{
				Iterator q = beginItr();
				while (q != p) {    // until finding p
					++q; ++j;     // advance and count hops
				}
			}
			reserve(max(1, 2 * currentCapacity));    // double array size. Use max just in case currentcapacity was zero
			//fixing p
			p = Iterator(&dynamicArray[j]);
		}
		Iterator next = endItr();
		Iterator current = endItr();

		if (currentSize != 0)
		{
			--current;
			while (next != p) {
				*(next) = *(current);
				if (next != p) {
					--next;
					--current;
				}
			}
		}
		*(next) = e;          // put in empty slot
		++currentSize;          // one more element
	}
	void insertFront(const Elem& e) { Iterator p = beginItr();  insert(p, e);  /*used tmp p becasue neginItr is const. */ }
	void insertBack(const Elem& e) { Iterator p = endItr();  insert(p, e); }
	//deletion (erasing elements)
	void erase(const Iterator& p)    // remove p
	{
		try {
			if (empty()) throw invalid_argument("No element to remove");

			Iterator current = p;
			Iterator next = p;
			++next;
			while (next != endItr())
			{
				*current = *next;
				if (next != endItr())
				{
					++current; ++next;
				}
			}
			//p = end();
			--currentSize;
		}
		catch (...) { cout << "No element to remove " << endl; }
		// one fewer element
	}
	void eraseFront() { erase(beginItr()); }
	void eraseBack() {
		Iterator current = endItr();
		--current;
		erase(current);
	}
	Elem& operator[](int i) { return dynamicArray[i]; }
	Elem& at(int i) {
		try {
			if (i > currentCapacity)
				throw(1);
			return dynamicArray[i];
		}
		catch (...) {
			cout << "Index is out of bounds " << endl;
		}
	};
	Iterator beginItr() const  // begin position is first item
	{
		return Iterator(dynamicArray);
	}

	Iterator endItr() const  // end position is just beyond last
	{
		return Iterator(dynamicArray + currentSize);
	}
	public:
		typename  ArraySequenceE::Iterator atIndex(int i) const;// get position from index
		int indexOf(const typename  ArraySequenceE::Iterator& p) const;// get index from position
	};


	typename ArraySequenceE::Iterator ArraySequenceE::atIndex(int i) const {
		typename  ArraySequenceE::Iterator current = ArraySequenceE::beginItr();
		//comments
		for (int index = 0; index < i; ++index)
			++current;
		return current;
	}


	int ArraySequenceE::indexOf(const typename  ArraySequenceE::Iterator& p) const {
		typename  ArraySequenceE::Iterator current = ArraySequenceE::beginItr();
		int j = 0;
		while (current != p) {
			++current;
			++j;
		}

		return j;
	};


int main()
{
	try {
		std::cout << "Hello World!\n";
		ArraySequenceE E;
		for (char ch = 'A'; ch <= 'F'; ch = ch + 1)
			E.insertBack(ch);
		//['A', 'B', 'C', 'D', 'E','F']
		for (ArraySequenceE::Iterator current = E.beginItr(); current != E.endItr(); ++current)
			cout << *current << endl;
		for (int i = 0; i < E.size(); i++)
			cout << *E.atIndex(i);

		E.insertFront('Z');
		//['Z', 'A', 'B', 'C', 'D', 'E','F']
		ArraySequenceE::Iterator q = E.beginItr();
		E.insert(q, '$');
		//['$', 'Z', 'A', 'B', 'C', 'D', 'E','F']
		ArraySequenceE::Iterator r = E.endItr();
		E.insert(r, '@');
		E.insertBack('%');
		E.insertBack('#');
		E.insertBack('+');
		E.insertBack('+');
		//['$', 'Z', 'A', 'B', 'C', 'D', 'E', 'F', '@', '%', '#', '+', '+']
		for (char ch = 'G'; ch <= 'O'; ch = ch + 1)
			E.insertFront(ch);
		//['O','N','M','L','K','J','I','H','G','$', 'Z', 'A', 'B', 'C', 'D', 'E','F', '@', '%', '#', '+', '+']

		for (ArraySequenceE::Iterator current = E.beginItr(); current != E.endItr(); ++current)
			cout << *current << endl;
	}
	catch (...) {

		cout << "Something went wrong\n";
	}
	system("PAUSE");
	return 0;
}
