#ifndef network_iobuffer_h
#define network_iobuffer_h
#include "Include/Def/Inct.h"
#include <list>

namespace network
{
	class NetBuffer
	{
		struct _t_block
		{
			const byte* _buffer;
			int 		_size;
			int			_start;
		};
		typedef std::list<_t_block > _t_list_pool;
		
	public:
		NetBuffer();
		virtual ~NetBuffer();

	public:
		size_t clear();
		size_t get_size();
		size_t append_buffer(const byte*, int);
		size_t append_buffer_nocopy(const byte*, int);
		size_t get_first_block(const byte**, int*);
		size_t release_first_block();
		size_t consume_first_block_size(int);

	private:
		size_t 			_size;
		_t_list_pool	_pool;
	};

	class NetBufferArray
	{
	public:
		NetBufferArray();
		virtual ~NetBufferArray();

	public:
		size_t clear();
		size_t get_size();
		size_t append_buffer(const byte*, int);
		size_t get_buffer(const byte**);
		size_t consume_buffer(int);

	private:
		byte* 	_pool;
		int 	_start;
		int 	_end;
		int 	_capacity;
	};
}
#endif//network_iobuffer_h