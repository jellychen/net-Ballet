#ifndef network_tcpconnection_h
#define network_tcpconnection_h
#include "Include/Def/Inct.h"
#include "Include/Common/Factory.h"
#include "Include/Network/ITcpConnection.h"
#include "Current.h"
#include "IoBuffer.h"
#include "Module.h"

namespace network
{
	_implement_bind(TcpConnection, 7BE4E60CA4524B4B, 839DE7C7F8D18251)
	_support ITcpConnection, public Current
    {
    	_implement_def(TcpConnection)
        _implement_support_begin()
        _implement_support(ITcpConnection)
        _implement_support_end()

    public:
    	TcpConnection();
    	virtual ~TcpConnection();
    	bool internal_init();

    public:
    	ret init					();
		ret close					(bool _right_now);
		ret follow_fd				(int _fd, ITcpServer* _server);
		ret use_ipv6				(bool* _ipv6);
		ret get_status				(int* _status);
		ret send_buffer 			(const byte* _buffer, int _size);
		ret send_raw_buffer			(const byte* _buffer, int _size);
		ret set_delay_delete		(bool _delay);
		ret get_delay_delete		(bool* _delay);
        ret get_local_info          (std::string* _addr, int* _port);
        ret get_peer_info           (std::string* _addr, int* _port);
		ret wait_for_event			(IEventPoll* _poll);

	private:
		ret on_read					(int fd, IEventPoll* _poll);
		ret on_write				(int fd, IEventPoll* _poll);
		ret on_time 				(int id, IEventPoll* _poll);
		ret on_set_close			(int fd, IEventPoll* _poll, bool _accord);

	private:
		int 						fd_;
		int 						connection_status_;
		int 						want_read_buffer_size_;
		bool 						delay_delete_;
		bool						set_time_out_;
        bool                        wait_for_write_;
        int64_t                     last_read_time_;
		NetBuffer					write_buffer_;
		NetBufferArray				read_buffer_;
		ITcpConnection*				connection_self_;
		__sp<IEventPoll>			poll_;
		__wp<ITcpServer>			tcp_server_;
		CTimerCallbackPtr<__self>	timer_callback_key_;
		CEventCallbackPtr<__self>	event_callback_key_;
    };
    _implement_register_ref_object(ITcpConnection, TcpConnection, __rtns, __module_register)
}
#endif//network_tcpconnection_h