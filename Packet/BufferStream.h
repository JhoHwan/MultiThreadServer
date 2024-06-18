#pragma once
#include <array>
#include <string>

constexpr int MAX_BUFFER_SIZE = 1024;

class BufferStream
{
private:
	std::array<char, MAX_BUFFER_SIZE> buffer;
	int readPtr;
	int size;

private:
	bool CheackWriteBound(size_t len)
	{
		if (size + len > MAX_BUFFER_SIZE)
			return false;
		return true;
	}

	bool CheackReadBound(size_t len)
	{
		if (readPtr + len > size)
			return false;
		return false;
	}

public:
	BufferStream() : readPtr(0), size(0)
	{
		buffer.fill(0);
	}

	BufferStream(const char* inBuffer, size_t size) : readPtr(0), size(0)
	{
		SetBuffer(inBuffer, size);
	}

	void SetBuffer(const char* inBuffer, size_t size)
	{
		this->size = size;
		buffer.fill(0);
		memcpy(buffer.data(), inBuffer, size);
	}
	
	template <typename T>
	void operator << (const T& value)
	{
		size_t len = sizeof(value);
		if (CheackWriteBound(size) == false)
			return;
		memcpy(buffer.data() + offset, &value, len);
		size += len;
	}

	void operator << (const std::string& str)
	{
		for (auto ch : str)
		{
			*this << ch;
		}
		*this << '\0';
	}

	template <typename T>
	void operator >> (T& value)
	{
		size_t size = sizeof(value);
		if (CheackReadBound(size) == false)
			return;
		memcpy(&value, buffer.data() + readPtr, size);
		readptr += size;
	}

	void operator >> (std::string& str)
	{
		size_t len = 0;
		while (true)
		{
			if (buffer[readPtr + len] == '\0') break;
			len++;
		}
		if (CheackReadBound(len) == false) return;
		str.clear();
		str.assign(buffer.data(), readPtr, size);

		readPtr += size + 1;
	}

	size_t GetSize() const { return size; }
	const char* GetBuffer() const { return buffer.data(); }

};