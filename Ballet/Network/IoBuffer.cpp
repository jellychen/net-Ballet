#include "IoBuffer.h"

namespace network
{
	NetBuffer::NetBuffer()
	{
		this->_size = 0;
	}

	NetBuffer::~NetBuffer()
	{
		this->clear();
	}

	size_t NetBuffer::clear()
	{
		while (this->_pool.size() > 0)
		{
			_t_block& _first = this->_pool.front();
			if (_first._buffer) free((void*)_first._buffer);
			this->_pool.pop_front();
		}
		return 0;
	}

	size_t NetBuffer::get_size()
	{
		return this->_size;
	}

	size_t NetBuffer::append_buffer(const byte* _buffer, int _size)
	{
		if_return(_buffer == nullptr_t || 0 == _size, this->_size);
		byte* _buffer_malloc = (byte*)malloc(_size);
		if_return(_buffer_malloc == nullptr_t, this->_size);
		memcpy(_buffer_malloc, _buffer, _size);
		_t_block _block = {_buffer_malloc, _size, 0};
		this->_size += _size;
		this->_pool.push_back(_block);
		return this->_size;
	}

	size_t NetBuffer::append_buffer_nocopy(const byte* _buffer, int _size)
	{
		if_return(_buffer == nullptr_t || 0 == _size, this->_size);
		_t_block _block = {_buffer, _size, 0};
		this->_size += _size;
		this->_pool.push_back(_block);
		return this->_size;
	}

	size_t NetBuffer::get_first_block(const byte** _buffer, int* _size)
	{
		if_return(_buffer == nullptr_t || _size == nullptr_t, this->_size);
		if (_buffer) *_buffer = nullptr_t;
		if (_size) *_size = 0;
		if_return(this->_pool.size() <= 0, this->_size);
		
		_t_block& _first = this->_pool.front();
		if (_size) *_size = _first._size - _first._start;
		if (_buffer) *_buffer = _first._buffer + _first._start;
		return this->_size;
	}

	size_t NetBuffer::release_first_block()
	{
		if_return(this->_pool.size() < 1, 0);
		_t_block& _first = this->_pool.front();
		if (_first._buffer) free((void*)_first._buffer);
		this->_size -= (_first._size - _first._start);
		this->_pool.pop_front();
		return this->_size;
	}

	size_t NetBuffer::consume_first_block_size(int _size)
	{
		if_return(_size <= 0, this->_size);
		_t_block& _first = this->_pool.front();
		if (_size >= (_first._size - _first._start))
		{
			if (_first._buffer) free((void*)_first._buffer);
			this->_size -= (_first._size - _first._start);
			this->_pool.pop_front();
		}
		else
		{
			_first._start += _size;
			this->_size -= _size;
		}
		return this->_size;
	}

	NetBufferArray::NetBufferArray()
	{
		this->_start = 0;
		this->_end = 0;
		this->_capacity = 32;
		this->_pool = (byte*)malloc(this->_capacity);
	}

	NetBufferArray::~NetBufferArray()
	{
		if (this->_pool != nullptr_t)
		{
			free(this->_pool);
		}
	}

	size_t NetBufferArray::clear()
	{
		if (this->_pool != nullptr_t)
		{
			free(this->_pool);
		}

		this->_start = 0;
		this->_end = 0;
		this->_capacity = 32;
		this->_pool = (byte*)malloc(this->_capacity);
		return 0;
	}

	size_t NetBufferArray::get_size()
	{
		return (size_t)(this->_end - this->_start);
	}

	size_t NetBufferArray::append_buffer(const byte* _buffer, int _size)
	{
		if (_buffer != nullptr_t && 0 != _size)
		{
			if (this->_capacity - this->_end >= _size)
			{
				memcpy(_pool + this->_end, _buffer, _size);
				this->_end += _size;
			}
			else if (this->_start + (this->_capacity - this->_end) > _size)
			{
				int _data_length = this->_end - this->_start;
				memcpy(_pool, _pool + this->_start, _data_length);
				this->_start = 0;
				this->_end = _data_length;
				memcpy(_pool + _data_length, _buffer, _size);
				this->_end += _size;
			}
			else
			{
				int _data_length = this->_end - this->_start;
				int _need_size = _data_length + _size;
				int _malloc_size = this->_capacity;
				while (_malloc_size <= _need_size)
				{
					_malloc_size *= 1.5;
				}

				byte* _pool_malloc = (byte*)malloc(_malloc_size);
				if (_pool_malloc == nullptr_t) 
				{
					return (size_t)(_data_length);
				}

				memcpy(_pool_malloc, _pool + this->_start, _data_length);
				memcpy(_pool_malloc + _data_length, _buffer, _size);
				free(this->_pool);

				this->_pool = _pool_malloc;
				this->_start = 0;
				this->_end = _need_size;
				this->_capacity = _malloc_size;
			}
		}
		return (size_t)(this->_end - this->_start);
	}

	size_t NetBufferArray::get_buffer(const byte** _buffer)
	{
		if (_buffer != nullptr_t) 
		{
			*_buffer = this->_pool + this->_start;
		}
		return (size_t)(this->_end - this->_start);
	}

	size_t NetBufferArray::consume_buffer(int _size)
	{
		this->_start += _size;
		if (this->_start > this->_end) 
		{
			this->_start = this->_end;
		}
		return (size_t)(this->_end - this->_start);
	}
}