#ifndef _include_network_tcpstatus_h
#define _include_network_tcpstatus_h

namespace network
{
    enum tcp_connection_error
    {
        error_none              = 0x0,
        error_input             = 0x1,
        error_decode            = 0x2,
        error_encode            = 0x3,
        error_read_exceed       = 0x4,
        error_write_exceed      = 0x5,
    };

    enum tcp_connection_status
    {
        status_none             = 0x0,
        status_connecting       = 0x1,
        status_establish        = 0x2,
        status_closing          = 0x3,
        status_closed           = 0x4,
    };
}
#endif//_include_network_tcpstatus_h