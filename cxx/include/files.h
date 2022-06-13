#ifndef FILES_H
#define FILES_H

#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include <ios>

template <typename T>
class File;

template <typename T>
struct Rider {
	File<T>* file = nullptr;
	T first = 0;
	int pos = 0;
	bool eof = false;
	bool eor = false;

	Rider() = default;
	void open_file(const std::string& filename);
	void create_file(const std::string& filename);
	void write(const T& value);
	void read(T& value);
	void set_file(File<T>* file, std::size_t _pos);
	void clear_file();
};

////////////////////////////////////////////////////////////////////////////////////

template <typename T>
class File
{
	std::fstream _file;
	bool _is_open = false;
	bool _is_read = true;
	std::size_t _size = 0;
	std::size_t _data_size = sizeof(T);

public:
	File() = default;
	File(const std::string& _filename, bool _write) {
		if (_write)
			create(_filename);
		else
			open(_filename);
	}
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
			_size /= _data_size;
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

	bool read(std::size_t pos, T& value) {
		if (!_is_read)
			return false;
		if (pos < _size)
			_file.seekg(pos * _data_size, _file.beg);
		_file.read(reinterpret_cast<char*>(&value), _data_size);
		return _file ? true : false;
	}

	bool write(std::size_t pos, const T& value) {
		if (_is_read)
			return false;
		if (pos < _size)
			_file.seekp(pos * _data_size,  _file.beg);
		_file.write(reinterpret_cast<const char*>(&value), _data_size);
		if (_file) {
			_size++;
			return true;
		}
		return false;
	}

	void set(Rider<T>& rider, std::size_t pos) {
		rider.file = this;
		rider.eof = false;
		rider.pos = pos >= 0 ? (pos < _size ? pos : _size) : 0;
	}

	void close() {
		_file.close();
	}
};

////////////////////////////////////////////////////////////////////////////////////

template <typename T>
inline void Rider<T>::open_file(const std::string& filename) {
	clear_file();
	File<T>* _file = new File<T>(filename, false);
	if (_file)
		set_file(_file, 0);
}

template <typename T>
inline void Rider<T>::create_file(const std::string& filename) {
	clear_file();
	File<T>* _file = new File<T>(filename, true);
	_file->set(*this, 0);
}

template <typename T>
inline void Rider<T>::clear_file() {
	delete file;
	file = nullptr;
}

template <typename T>
inline void Rider<T>::write(const T& value) {
	if (file->write(pos, value))
		pos++;
	else
		eof = true;
}

template <typename T>
inline void Rider<T>::read(T& value) {
	if (file->read(pos, value))
		pos++;
	else
		eof = true;
}

template <typename T>
inline void Rider<T>::set_file(File<T>* _file, std::size_t _pos) {
	// file_set
	file = _file;
	eof = false;
	pos = _pos >= 0 ? (_pos < file->size() ? _pos : file->size()) : 0;
	//
	read(first);
	eor = eof;
}

////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void copy_value(Rider<T>& src, Rider<T>& dst) {
	dst.first = src.first;
	dst.write(dst.first);
	src.read(src.first);
	src.eor = src.eof || (src.first < dst.first);
}

template <typename T>
void copy_run(Rider<T>& src, Rider<T>& dst) {
	do {
		copy_value(src, dst);
	} while (!src.eor);
}

///////////////////////////////////////////////////////////////////////////////////

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

template <typename T>
bool is_sorted(const std::string& filename) {
	std::fstream file(filename, std::ios_base::in | std::ios_base::binary);
	if (!file.is_open())
		return false;
	T first;
	if (!file.eof())
		file.read(reinterpret_cast<char*>(&first), sizeof(T));
	while (!file.eof()) {
		T second;
		file.read(reinterpret_cast<char*>(&second), sizeof(T));
		if (first > second) {
			file.close();
			return false;
		}
		first = second;
	}
	file.close();
	return true;
}

#endif // FILES_H
