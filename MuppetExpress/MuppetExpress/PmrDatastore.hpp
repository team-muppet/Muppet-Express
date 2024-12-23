//#pragma once
//#include <memory_resource>
//#include <vector>
//#include <iostream>
//#include <format>
//#include <list>
//#include <locale>
//#include <string>
//#include <chrono>
//#include "PokemonModel.hpp"
//
//
//
//template <typename T, size_t storeSize = 100>
//class PmrDatastore {
//public:
//	PmrDatastore(std::pmr::memory_resource* mr) {
//		_mr = mr;
//		_data-> = _mr->allocate(sizeof(T) * _size, alignof(T));
//	}
//	//PmrDatastore(const PmrDatastore&) {
//	//	_mr = mr;
//	//	_mr->allocate(sizeof(T) * _size, alignof(T));
//	//}
//	//PmrDatastore& operator=(const PmrDatastore&) {
//	//	_mr = mr;
//	//	_mr->allocate(sizeof(T) * _size, alignof(T));
//	//}
//	//PmrDatastore(PmrDatastore&&) {
//	//	_mr = mr;
//	//	_mr->allocate(sizeof(T) * _size, alignof(T));
//	//}
//	//PmrDatastore& operator=(PmrDatastore&&) {
//	//	_mr = mr;
//	//	_mr->allocate(sizeof(T) * _size, alignof(T));
//	//}
//	~PmrDatastore() {
//		_mr->deallocate(_data.data(), sizeof(T) * _data.size(), alignof(T));
//	}
//	void push_back(T&& item) {
//		_data.push_back(std::move(item));
//	}
//	void push_back(const T& item) {
//		_data.push_back(item);
//	}
//	void erase(const T& item) {
//		_data.erase(std::remove(_data.begin(), _data.end(), item), _data.end());
//	}
//	void remove(T&& item) {
//		_data.erase(std::remove(_data.begin(), _data.end(), item), _data.end());
//	}
//	void clear() {
//		_data.clear();
//	}
//	std::vector<T> get() const {
//		return _data;
//	}
//	//begin iterrator
//	auto begin() {
//		return _data.begin();
//	}
//	//end iterator
//	auto end() {
//		return _data.end();
//	}
//	//size of the vector
//	size_t size() {
//		return _data.size();
//	}
//	//empty check
//	bool empty() {
//		return _data.empty();
//	}
//	//find the item
//	auto find(const T& item) {
//		return std::find(_data.begin(), _data.end(), item);
//	}
//	//find the item
//	auto find(T&& item) {
//		return std::find(_data.begin(), _data.end(), item);
//	}
//	// [] operator iterator
//	auto operator[](size_t index) {
//		return _data[index];
//	}
//	auto erase(typename std::vector<T>::iterator first, typename std::vector<T>::iterator last)
//		-> typename std::vector<T>::iterator
//	{
//		return _data.erase(first, last);
//	}
//
//
//private:
//	std::pmr::memory_resource* _mr;
//	std::vector<T> _data;
//	size_t _size = storeSize;
//};
//
//
