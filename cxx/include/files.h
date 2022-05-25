#ifndef FILES_H
#define FILES_H

#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include <ios>

class File;

template <typename T>
struct Rider {
	File* file;
	T first = 0;
	int pos = 0;
	bool eof = false;
	bool eor = false;

	Rider() = default;
};

class File
{
	std::fstream _file;
	bool _is_open = false;
	bool _is_read = true;
	std::size_t _size = 0;

public:
	File() = default;
	File(const File& other) = delete;
	File& operator = (const File& other) = delete;
	File(File&& other): _file(std::move(other._file)), _is_open(other._is_open), _is_read(other._is_read), _size(other._size) {}
	File& operator = (File&& other) {
		if (&other != this) {
			_file = std::move(other._file);
			_is_open = other._is_open;
			_is_read = other._is_read;
			_size = other._size;
		}
		return *this;
	}
	~File() { _file.close(); }

	bool open(const std::string& filename) { // input
		_file.open(filename, std::ios_base::in | std::ios_base::binary);
		if (_file.is_open()) {
			_is_open = true;
			_is_read = true;
			_file.seekg(0, _file.end);
			_size = _file.tellg();
			_file.seekg(0, _file.beg);
			return true;
		}
		return false;
	}

	bool create(const std::string& filename) { // output
		_file.open(filename, std::ios_base::out | std::ios_base::binary);
		if (_file.is_open()) {
			_is_open = true;
			_is_read = false;
			_size = 0;
			return true;
		}
		return false;
	}
	std::size_t size() const { return _size; }
	bool eof() const { return _file.eof(); }

	template <typename T>
	bool read(T& value) {
		if (!_is_read)
			return false;
		_file.read(reinterpret_cast<char*>(&value), sizeof (T));
		return _file ? true : false;
	}

	template <typename T>
	bool write(const T& value) {
		if (_is_read)
			return false;
		_file.write(reinterpret_cast<const char*>(&value), sizeof (T));
		if (_file) {
			++_size;
			return true;
		}
		return false;
	}

	void close() {
		_file.close();
	}
};

template <typename T>
void write_int(Rider<T>& rider, int value) {
	if (rider.file->write(value))
		rider.pos++;
	else
		rider.eof = true;
}

template <typename T>
void read_int(Rider<T>& rider, int& value) {
	if (rider.file->read(value))
		rider.pos++;
	else
		rider.eof = true;
}

template <typename T>
void file_set(Rider<T>& rider, File* file, std::size_t pos) {
	rider.file = file;
	rider.eof = false;
	rider.pos = pos >= 0 ? (pos < file->size() ? pos : file->size()) : 0;
}

template <typename T>
void run_set(Rider<T>& rider, File* file) {
	file_set(rider, file, 0);
	read_int(rider, rider.first);
	rider.eor = rider.eof;
}

template <typename T>
void copy_value(Rider<T>& src, Rider<T>& dst) {
	dst.first = src.first;
	write_int(dst, dst.first);
	read_int(src, src.first);
	src.eor = src.eof || (src.first < dst.first);
}

template <typename T>
void copy_run(Rider<T>& src, Rider<T>& dst) {
	do {
		copy_value(src, dst);
	} while (!src.eor);
}

template <typename T, typename Out>
bool binary_file_to_string(const std::string& filename, Out& out) {
	std::fstream file(filename, std::ios_base::in | std::ios_base::binary);
	if (!file.is_open())
		return false;
	while (file) {
		T data;
		file.read(reinterpret_cast<char*>(&data), sizeof(T));
		out << data << std::endl;
	}
	file.close();
	return true;
}

#endif // FILES_H
