#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>
#include <tuple>
#include <memory>
#include <cstring>

template<typename T, T value = 0, class Allocator = std::allocator<std::tuple<uint64_t, uint64_t, T>>>
class matrix_data {
private:
	std::tuple<uint64_t, uint64_t, T> *items;
	T default_value;
	size_t d_size;
	Allocator alloc;

	void add(uint64_t, uint64_t, T, size_t);
	void del(size_t);

public:
	matrix_data();
	~matrix_data();

	size_t size();
	std::tuple<uint64_t, uint64_t, T>* begin();
	std::tuple<uint64_t, uint64_t, T>* end();
	T get(uint64_t, uint64_t);
	void set(uint64_t, uint64_t, T);

};

template<typename T, T value = 0, class Allocator = std::allocator<std::tuple<uint64_t, uint64_t, T>>>
class Matrix {
private:
	matrix_data<T, value, Allocator> data;

	class proxy {
	private:
		matrix_data<T, value, Allocator> *data;
		uint64_t row;
		uint64_t col;

	public:
		proxy(matrix_data<T, value, Allocator> *data, uint64_t row) {this->data = data; this->row = row;}

		proxy& operator[](uint64_t col) {this->col = col; return *this;}
		proxy& operator=(T val) {
			data->set(row, col, val);
			return *this;
		}
		
		operator T() const {
			return data->get(row, col);
		}
	};

public:
	Matrix() {}
	~Matrix() = default;

	std::tuple<uint64_t, uint64_t, T>* begin();
	std::tuple<uint64_t, uint64_t, T>* end();
	size_t size();
	
	proxy operator[](uint64_t row) {return proxy(&data, row);}
};

template<typename T, T value, class Allocator>
matrix_data<T, value, Allocator>::matrix_data()
{
	this->items = nullptr;
	this->default_value = value;
	this->d_size = 0;
}

template<typename T, T value, class Allocator>
matrix_data<T, value, Allocator>::~matrix_data()
{
	alloc.deallocate(items, d_size);
	items = nullptr;
}

template<typename T, T value, class Allocator>
void matrix_data<T, value, Allocator>::add(uint64_t row, uint64_t col, T val, size_t index)
{
	std::tuple<uint64_t, uint64_t, T> *new_items = alloc.allocate(sizeof(std::tuple<uint64_t, uint64_t, T>) * d_size);

	std::memcpy(static_cast<void *>(new_items), static_cast<void *>(items), index * sizeof(std::tuple<uint64_t, uint64_t, T>));
	std::memcpy(static_cast<void *>(new_items + index + 1), static_cast<void *>(items + index), (d_size - index) * sizeof(std::tuple<uint64_t, uint64_t, T>));
	std::swap(new_items, items);
	alloc.deallocate(new_items, d_size);

	++d_size;

	*(items + index) = std::make_tuple(row, col, val);
}

template<typename T, T value, class Allocator>
void matrix_data<T, value, Allocator>::del(size_t index)
{
	std::tuple<uint64_t, uint64_t, T> *new_items = alloc.allocate(sizeof(std::tuple<uint64_t, uint64_t, T>) * (d_size - 1));
	std::memcpy(static_cast<void *>(new_items), static_cast<void *>(items), index * sizeof(std::tuple<uint64_t, uint64_t, T>));
	std::memcpy(static_cast<void *>(new_items + index), static_cast<void *>(items + index + 1), (d_size - index - 1) * sizeof(std::tuple<uint64_t, uint64_t, T>));
	std::swap(new_items, items);
	alloc.deallocate(new_items, d_size);
	--d_size;

	if (d_size == 0) {
		items = nullptr;
	}
}

template<typename T, T value, class Allocator>
size_t matrix_data<T, value, Allocator>::size() {return d_size;}

template<typename T, T value, class Allocator>
std::tuple<uint64_t, uint64_t, T>* matrix_data<T, value, Allocator>::begin() {return items;}

template<typename T, T value, class Allocator>
std::tuple<uint64_t, uint64_t, T>* matrix_data<T, value, Allocator>::end() {return items + d_size;}

template<typename T, T value, class Allocator>
T matrix_data<T, value, Allocator>::get(uint64_t row, uint64_t col)
{
	for (size_t i = 0; i < d_size; ++i) {
		if (std::get<0>(items[i]) > row) {
			break;
		} else if (std::get<0>(items[i]) == row){
			if (std::get<1>(items[i]) > col)
				break;

			if (std::get<0>(items[i]) == row && std::get<1>(items[i]) == col)
				return std::get<2>(items[i]);
		}
	}
	return default_value;
}

template<typename T, T value, class Allocator>
void matrix_data<T, value, Allocator>::set(uint64_t row, uint64_t col, T val)
{
	if (d_size == 0) {
		if (val != default_value)
			add(row, col, val, (size_t)0);
		return;
	}

	for (size_t i = 0; i < d_size; ++i) {
		if (std::get<0>(items[i]) > row) {
			if (val != default_value)
				add(row, col, val, i);
			return;
		} else if (std::get<0>(items[i]) == row) {
			if (std::get<1>(items[i]) > col) {
				if (val != default_value)
					add(row, col, val, i);
				return;
			}	else if (std::get<0>(items[i]) == row && std::get<1>(items[i]) == col) {
				if (val == default_value)
					del(i);
				else
					std::get<2>(items[i]) = val;
				return;
			}
		}
		if (i == d_size - 1) {
			if (val != default_value)
				add(row, col, val, i + 1);
			return;
		}
	}
}

template<typename T, T value, class Allocator>
std::tuple<uint64_t, uint64_t, T>* Matrix<T, value, Allocator>::begin() {return data.begin();}

template<typename T, T value, class Allocator>
std::tuple<uint64_t, uint64_t, T>* Matrix<T, value, Allocator>::end() {return data.end();}

template<typename T, T value, class Allocator>
size_t Matrix<T, value, Allocator>::size() {return data.size();}

#endif /* MATRIX_H */