#ifndef network_tcpserver_h
#define network_tcpserver_h
#include "Include/Def/Inct.h"
#include "Include/Common/Factory.h"
#include "Include/Network/IEventPoll.h"
#include "Include/Network/ITcpServer.h"
#include "Include/Network/ITcpProtocol.h"
#include "Include/Network/ITcpCallback.h"
#include "Include/Network/ITcpConnection.h"
#include "TcpConnection.h"
#include "Module.h"

namespace network
{
	typedef std::map<int, __sp<ITcpConnection> > _t_map_tcp_connection_pool;

	_implement_bind(TcpServer, AB2D2486E70943BE, A85DE2B1B36796C5)
    _support ITcpServer
    {
        _implement_def(TcpServer)
        _implement_support_begin()
        _implement_support(ITcpServer)
        _implement_support_end()
    
    public:
        TcpServer();
        virtual ~TcpServer();

    public:
    	ret init					();
		ret listen					(const char* _addr, int _port, int _backlog);
		ret listen_ipv6				(const char* _addr, int _port, int _backlog);
		ret erase					(int _fd);
		ret close					();
		ret use_ipv6				(bool* _use_ipv6);
		ret set_protocol			(ITcpProtocol* _protocol);
		ret get_protocol			(ITcpProtocol** _protocol);
		ret set_max_read_buffer		(int _max);
		ret set_max_write_buffer	(int _max);
		ret get_max_read_buffer     (int* _max);
		ret get_max_write_buffer	(int* _max);
		ret set_timeout				(int _timeout);
		ret get_timeout				(int* _timeout);
		ret set_callback			(ITcpCallback* _callback);
		ret get_callback			(ITcpCallback** _callback);
		ret wait_for_event			(IEventPoll* _poll);

	private:
		ret on_accept				(int _fd, IEventPoll* poll);

	protected:
		int 					    fd_;
		int 					    max_read_size_;
		int 						max_write_size_;
		int 						time_out_;
		bool						use_ipv6_;
		bool 						listen_accept_;
		__sp<IEventPoll>			poll_;
		__sp<ITcpProtocol>          protocol_;
		__sp<ITcpCallback>			tcp_callback_;
		_t_map_tcp_connection_pool  pool_;
		CEventCallbackPtr<__self>	event_callback_key_;
    };
    _implement_register_ref_object(ITcpServer, TcpServer, __rtns, __module_register)
}
#endif//network_tcpserver_h