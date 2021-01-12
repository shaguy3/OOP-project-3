#pragma once
#include <iostream>
using namespace std;

template <class T>
class DynamicArray
{
private:
	void resize() {
		_physicalSize *= 2;
		T* temp = new T[_physicalSize];
		for (int i = 0; i < _logicalSize; i++)
			temp[i] = _arr[i];

		delete[] _arr;
		_arr = temp;
	}

	void resize(int new_size) {
		_physicalSize = new_size;
		T* temp = new T[_physicalSize];
		for (int i = 0; i < _logicalSize; i++)
			temp[i] = _arr[i];

		delete[] _arr;
		_arr = temp;
	}

	T* _arr;
	int _logicalSize;
	int _physicalSize;

public:
	DynamicArray(int size = 2) : _logicalSize(0), _physicalSize(size), _arr(new T[size]) {}

	DynamicArray(const DynamicArray& other) : _arr(nullptr) {
		*this = other;
	}
	~DynamicArray() {
		delete[] _arr;
	}

	const DynamicArray& operator=(const DynamicArray& other) {
		if (this != &other) {
			_logicalSize = other._logicalSize;
			_physicalSize = other._physicalSize;
			delete[] _arr;
			_arr = new T[_physicalSize];
			for (int i = 0; i < _logicalSize; i++)
				_arr[i] = other._arr[i];
		}
		return *this;
	}

	const T& operator[](int i) const { return _arr[i]; }
	T& operator[](int i) { return _arr[i]; }

	void push_back(const T& value) {
		if (_logicalSize == _physicalSize)
			resize();
		_arr[_logicalSize++] = value;
	}

	const T& front()    const { return _arr[0]; }
	int      size()     const { return _logicalSize; }
	int      capacity() const { return _physicalSize; }
	bool     empty()    const { return _logicalSize == 0; }
	void     clear() { _logicalSize = 0; }

	void set_size(int new_size) {
		if (new_size > _physicalSize) { resize(2 * new_size); }

		_logicalSize = new_size;
	}

	// example iterator implementation:
	class iterator
	{
	private:
		DynamicArray* _da;
		int				_i;
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		// other options exist, e.g., std::forward_iterator_tag
		using different_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;

		iterator(DynamicArray& arr, int i) : _da(&arr), _i(i) {}
		iterator(const iterator& other) : _da(other._da), _i(other._i) {}

		friend class const_iterator;

		const iterator& operator=(const iterator& other) {
			_da = other._da;
			_i = other._i;
			return *this;
		}

		// comparison with another iterator
		bool operator==(const iterator& other) const {
			return (_da == other._da) && (_i == other._i);
		}
		bool operator!=(const iterator& other) const {
			return !(*this == other);
		}

		// smart-pointer iterator methods
		T& operator*() {
			return _da->_arr[_i];
		}
		T* operator->() {
			return &_da->_arr[_i];
		}

		// increment-decrement iterator methods
		iterator& operator++() {
			++_i;
			return *this;
		}

		iterator operator++(int) {
			iterator temp(*this);
			++_i;
			return temp;
		}

		iterator& operator--() {
			--_i;
			return *this;
		}
		iterator operator--(int) {
			iterator temp(*this);
			--_i;
			return temp;
		}
	};
	// and here we duplicate the 'iterator' class to 'const_iterator' 
	class const_iterator {
	private:
		DynamicArray* _da;
		int				_i;
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using different_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;

		const_iterator(DynamicArray& arr, int i) : _da(&arr), _i(i) {}
		const_iterator(const const_iterator& other) : _da(other._da), _i(other._i) {}
		const_iterator(const iterator& other) : _da(other._da), _i(other._i) {}


		const iterator& operator=(const const_iterator& other) {
			_da = other._da;
			_i = other._i;
			return *this;
		}

		// comparison with another iterator
		bool operator==(const const_iterator& other) const {
			return (_da == other._da) && (_i == other._i);
		}
		bool operator!=(const const_iterator& other) const {
			return !(*this == other);
		}

		// smart-pointer iterator methods
		T& operator*() {
			return _da->_arr[_i];
		}
		T* operator->() {
			return &_da->_arr[_i];
		}

		// increment-decrement iterator methods
		const_iterator& operator++() {
			++_i;
			return *this;
		}
		const_iterator operator++(int) {
			iterator temp(*this);
			++_i;
			return temp;
		}
		const_iterator& operator--() {
			--_i;
			return *this;
		}
		const_iterator operator--(int) {
			iterator temp(*this);
			--_i;
			return temp;
		}
	};

	void insert(const iterator& pos, const T& val) {			
		if (_logicalSize == _physicalSize)
			resize();

		iterator itrEnd = end();
		iterator itrCurrent = itrEnd, itrPrev = --itrEnd;
		while (itrCurrent != pos)
		{
			*itrCurrent = *itrPrev;
			itrCurrent = itrPrev--;
		}

		iterator p = pos;
		*p = val;
		++_logicalSize;
	}

	void insert(const const_iterator& pos, const T& val) {
		if (_logicalSize == _physicalSize)
			resize();

		iterator itrEnd = end();
		iterator itrCurrent = itrEnd, itrPrev = --itrEnd;
		while (itrCurrent != pos)
		{
			*itrCurrent = *itrPrev;
			itrCurrent = itrPrev--;
		}

		iterator p = pos;
		*p = val;
		++_logicalSize;
	}
		
	const iterator& erase(const iterator& iter) 
	{
		iterator itrEnd = end();
		iterator temp = iter;
		iterator itrCurrent = temp, itrNext = ++temp;
		while (itrNext != itrEnd)
		{
			*itrCurrent = *itrNext;
			itrCurrent = itrNext++;
		}

		_logicalSize--;

		return itrCurrent;

	}
	const iterator& erase(const iterator& first, const iterator& last)	//	[1,2,3,4,5] -> (1,3) -> [1,5]
	{
		int counter = 0;
		iterator itrEnd = end();
		iterator itrTemp = last;
		iterator itrCount = first;
		iterator itrCurrentFirst = first;
		iterator itrCurrentLast = ++itrTemp;

		while (itrCount != last)
		{
			counter++;
			itrCount++;
		}
		
		while (itrCurrentLast != itrEnd)
		{
			*itrCurrentFirst = *itrCurrentLast;
			itrCurrentFirst++;
			itrCurrentLast++;
			counter++;
		}
		
		_logicalSize -= counter;
		return itrCurrentFirst;
	}

	const const_iterator& erase(const const_iterator& iter)
	{
		iterator itrEnd = end();
		iterator temp = iter;
		iterator itrCurrent = temp, itrNext = ++temp;
		while (itrNext != itrEnd)
		{
			*itrCurrent = *itrNext;
			itrCurrent = itrNext++;
		}

		_logicalSize--;

		return itrCurrent;
	}

	const const_iterator& erase(const const_iterator& first, const iterator& last)
	{
		int counter = 0;
		iterator itrEnd = end();
		iterator itrTemp = last;
		iterator itrCount = first;
		iterator itrCurrentFirst = first;
		iterator itrCurrentLast = ++itrTemp;

		while (itrCount != last)
		{
			counter++;
			itrCount++;
		}

		while (itrCurrentLast != itrEnd)
		{
			*itrCurrentFirst = *itrCurrentLast;
			itrCurrentFirst++;
			itrCurrentLast++;
			counter++;
		}

		_logicalSize -= counter;
		return itrCurrentFirst;
	}



	iterator begin() {
		return iterator(*this, 0); 
	}
	iterator end() {
		return iterator(*this, _logicalSize); 
	}
	const_iterator cbegin() const {
		return const_iterator(*this, 0);
	}
	const_iterator cend() const {
		return const_iterator(*this, _logicalSize);
	}

	iterator rbegin() {
		return iterator(*this, _logicalSize - 1);
	}

	iterator rend() {
		return iterator(*this, -1);
	}

	const_iterator rcbegin() {
		return const_iterator(*this, _logicalSize - 1);
	}

	const_iterator rcend() {
		return const_iterator(*this, -1);
	}

	void print() const {
		for (int i = 0; i < _logicalSize; i++)
			cout << _arr[i] << " ";
		cout << endl;
	}
};