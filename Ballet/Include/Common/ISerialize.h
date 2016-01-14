#ifndef _include_common_serialize_h
#define _include_common_serialize_h
#include "Include/Def/Inct.h"
#include <string>
#include <vector>

namespace common
{
    typedef union __host_littel_endian_t
    {
        int _i; char _byte;
    } __host_littel_endian_t;

    inline int16_t htoni(int16_t _data)
    {
        __host_littel_endian_t _t; _t._i = 1;
        if_return(1 != _t._byte, _data);
        return ((_data & 0x00FF) << 8)|((_data & 0xFF00) >>8);
    }

    inline int32_t htoni(int32_t _data)
    {
        __host_littel_endian_t _t; _t._i = 1;
        if_return(1 != _t._byte, _data);
        return 
            ((_data & 0x000000FF) << 24)|((_data & 0xFF000000) >> 24)|
            ((_data & 0x0000FF00) << 8)|((_data & 0x00FF0000) >> 8);
    }

    inline int64_t htoni(int64_t _data)
    {
        __host_littel_endian_t _t; _t._i = 1;
        if_return(1 != _t._byte, _data);
        return 
            ((_data & 0x00000000000000FF) << 56)|((_data & 0xFF00000000000000) >> 56)|
            ((_data & 0x000000000000FF00) << 40)|((_data & 0x00FF000000000000) >> 40)|
            ((_data & 0x0000000000FF0000) << 24)|((_data & 0x0000FF0000000000) >> 24)|
            ((_data & 0x00000000FF000000) << 8) |((_data & 0x000000FF00000000) >> 8);
    }

    inline int16_t ntohi(int16_t _data)
    {
        __host_littel_endian_t _t; _t._i = 1;
        if_return(1 != _t._byte, _data);
        return ((_data & 0x00FF) << 8)|((_data & 0xFF00) >>8);
    }

    inline int32_t ntohi(int32_t _data)
    {
        __host_littel_endian_t _t; _t._i = 1;
        if_return(1 != _t._byte, _data);
        return 
            ((_data & 0x000000FF) << 24)|((_data & 0xFF000000) >> 24)|
            ((_data & 0x0000FF00) << 8) |((_data & 0x00FF0000) >> 8);
    }

    inline int64_t ntohi(int64_t _data)
    {
        __host_littel_endian_t _t; _t._i = 1;
        if_return(1 != _t._byte, _data);
        return 
            ((_data & 0x00000000000000FF) << 56)|((_data & 0xFF00000000000000) >> 56)|
            ((_data & 0x000000000000FF00) << 40)|((_data & 0x00FF000000000000) >> 40)|
            ((_data & 0x0000000000FF0000) << 24)|((_data & 0x0000FF0000000000) >> 24)|
            ((_data & 0x00000000FF000000) << 8) |((_data & 0x000000FF00000000) >> 8);
    }

    template <typename _t> struct __number_object_t
    {
        _t                      _raw_data;
        static const uint32_t   _object_size = sizeof(_t);

        __number_object_t():_raw_data(0){}
        __number_object_t(_t _data):_raw_data(_data){}
        __number_object_t(const __number_object_t& _data):_raw_data(_data._raw_data){}

        void operator= (_t _data) {_raw_data = _data;}
        void operator= (const __number_object_t& _data) {_raw_data = _data._raw_data;}
        operator _t() const {return _raw_data;}

        inline bool calc_size(uint32_t& _size){_size = _object_size; return true;}
        inline bool serialize_to_string(std::string& _str);
        inline bool serialize_to_buffer(const char* _buffer, uint32_t& _size, uint32_t& _offset);
        inline bool unserialize_from_buffer(const char* _buffer, uint32_t& _size, uint32_t& _offset);
    };

    typedef __number_object_t<uint8_t>  __number_object_uint8_t;    typedef __number_object_uint8_t     s_uint8_t;
    typedef __number_object_t<uint16_t> __number_object_uint16_t;   typedef __number_object_uint16_t    s_uint16_t;
    typedef __number_object_t<uint32_t> __number_object_uint32_t;   typedef __number_object_uint32_t    s_uint32_t;
    typedef __number_object_t<uint64_t> __number_object_uint64_t;   typedef __number_object_uint64_t    s_uint64_t;
    typedef __number_object_t<int8_t>   __number_object_int8_t;     typedef __number_object_int8_t      s_int8_t;
    typedef __number_object_t<int16_t>  __number_object_int16_t;    typedef __number_object_int16_t     s_int16_t;
    typedef __number_object_t<int32_t>  __number_object_int32_t;    typedef __number_object_int32_t     s_int32_t;
    typedef __number_object_t<int64_t>  __number_object_int64_t;    typedef __number_object_int64_t     s_int64_t;
    typedef __number_object_t<float>    __number_object_float_t;    typedef __number_object_float_t     s_float_t;
    typedef __number_object_t<double>   __number_object_double_t;   typedef __number_object_double_t    s_double_t;

    template<> inline bool __number_object_uint8_t::serialize_to_string(std::string& _str)
    {
        _str.append((char*)(&_raw_data), __number_object_uint8_t::_object_size);
        return true;
    }

    template<> inline bool __number_object_uint8_t::serialize_to_buffer(const char* _buffer, uint32_t& _size, uint32_t& _offset)
    {
        if_return(_buffer == nullptr_t || _size < __number_object_uint8_t::_object_size, false);
        *((char*)_buffer + _offset) = _raw_data;
        _size -= __number_object_uint8_t::_object_size; _offset += __number_object_uint8_t::_object_size;
        return true;
    }

    template<> inline bool __number_object_uint8_t::unserialize_from_buffer(const char* _buffer, uint32_t& _size, uint32_t& _offset)
    {
        if_return(_buffer == nullptr_t || _size < __number_object_uint8_t::_object_size, false);
        _raw_data = (uint8_t)(*((char*)_buffer + _offset));
        _size -= __number_object_uint8_t::_object_size; _offset += __number_object_uint8_t::_object_size;
        return true;
    }

    template<> inline bool __number_object_uint16_t::serialize_to_string(std::string& _str)
    {
        uint16_t _net_uint = htons(_raw_data);
        _str.append((char*)(&_net_uint), __number_object_uint16_t::_object_size);
        return true;
    }

    template<> inline bool __number_object_uint16_t::serialize_to_buffer(const char* _buffer, uint32_t& _size, uint32_t& _offset)
    {
        if_return(_buffer == nullptr_t || _size < __number_object_uint16_t::_object_size, false);
        uint16_t _net_uint = htons(_raw_data);
        memcpy((char*)_buffer + _offset, &_net_uint, __number_object_uint16_t::_object_size);
        _size -= __number_object_uint16_t::_object_size; _offset += __number_object_uint16_t::_object_size;
        return true;
    }

    template<> inline bool __number_object_uint16_t::unserialize_from_buffer(const char* _buffer, uint32_t& _size, uint32_t& _offset)
    {
        if_return(_buffer == nullptr_t || _size < __number_object_uint16_t::_object_size, false);
        memcpy(&_raw_data, (char*)_buffer + _offset, __number_object_uint16_t::_object_size);
        _raw_data = ntohs(_raw_data);
        _size -= __number_object_uint16_t::_object_size; _offset += __number_object_uint16_t::_object_size;
        return true;
    }

    template<> inline bool __number_object_uint32_t::serialize_to_string(std::string& _str)
    {
        uint32_t _net_uint = htonl(_raw_data);
        _str.append((char*)(&_net_uint), __number_object_uint32_t::_object_size);
        return true;
    }

    template<> inline bool __number_object_uint32_t::serialize_to_buffer(const char* _buffer, uint32_t& _size, uint32_t& _offset)
    {
        if_return(_buffer == nullptr_t || _size < __number_object_uint32_t::_object_size, false);
        uint32_t _net_uint = htonl(_raw_data);
        memcpy((char*)_buffer + _offset, &_net_uint, __number_object_uint32_t::_object_size);
        _size -= __number_object_uint32_t::_object_size; _offset += __number_object_uint32_t::_object_size;
        return true;
    }

    template<> inline bool __number_object_uint32_t::unserialize_from_buffer(const char* _buffer, uint32_t& _size, uint32_t& _offset)
    {
        if_return(_buffer == nullptr_t || _size < __number_object_uint32_t::_object_size, false);
        memcpy(&_raw_data, (char*)_buffer + _offset, __number_object_uint32_t::_object_size);
        _raw_data = ntohl(_raw_data);
        _size -= __number_object_uint32_t::_object_size; _offset += __number_object_uint32_t::_object_size;
        return true;
    }

    template<> inline bool __number_object_uint64_t::serialize_to_string(std::string& _str)
    {
        uint32_t _net_uint = htonl(_raw_data >> 32);
        _str.append((char*)(&_net_uint), sizeof(uint32_t));
        _net_uint = htonl(_raw_data);
        _str.append((char*)(&_net_uint), sizeof(uint32_t));
        return true;
    }

    template<> inline bool __number_object_uint64_t::serialize_to_buffer(const char* _buffer, uint32_t& _size, uint32_t& _offset)
    {
        if_return(_buffer == nullptr_t || _size < __number_object_uint64_t::_object_size, false);
        uint32_t _net_uint = htonl(_raw_data >> 32);
        memcpy((char*)_buffer + _offset, &_net_uint, sizeof(uint32_t));
        _size -= sizeof(uint32_t); _offset += sizeof(uint32_t);
        _net_uint = htonl(_raw_data);
        memcpy((char*)_buffer + _offset, &_net_uint, sizeof(uint32_t));
        _size -= sizeof(uint32_t); _offset += sizeof(uint32_t);
        return true;
    }

    template<> inline bool __number_object_uint64_t::unserialize_from_buffer(const char* _buffer, uint32_t& _size, uint32_t& _offset)
    {
        if_return(_buffer == nullptr_t || _size < __number_object_uint64_t::_object_size, false);
        uint32_t _net_uint = 0;
        memcpy(&_net_uint, (char*)_buffer + _offset, sizeof(uint32_t));
        _raw_data = ntohl(_net_uint); _raw_data = _raw_data << 32;
        _size -= sizeof(uint32_t); _offset += sizeof(uint32_t);
        memcpy(&_net_uint, (char*)_buffer + _offset, sizeof(uint32_t));
        _raw_data += ntohl(_net_uint);
        _size -= sizeof(uint32_t); _offset += sizeof(uint32_t);
        return true;
    }

    template<> inline bool __number_object_int8_t::serialize_to_string(std::string& _str)
    {
        _str.append((char*)(&_raw_data), __number_object_int8_t::_object_size);
        return true;
    }

    template<> inline bool __number_object_int8_t::serialize_to_buffer(const char* _buffer, uint32_t& _size, uint32_t& _offset)
    {
        if_return(_buffer == nullptr_t || _size < __number_object_int8_t::_object_size, false);
        *((char*)_buffer + _offset) = _raw_data;
        _size -= __number_object_int8_t::_object_size; _offset += __number_object_int8_t::_object_size;
        return true;
    }

    template<> inline bool __number_object_int8_t::unserialize_from_buffer(const char* _buffer, uint32_t& _size, uint32_t& _offset)
    {
        if_return(_buffer == nullptr_t || _size < __number_object_int8_t::_object_size, false);
        _raw_data = (uint8_t)(*((char*)_buffer + _offset));
        _size -= __number_object_int8_t::_object_size; _offset += __number_object_int8_t::_object_size;
        return true;
    }

    template<> inline bool __number_object_int16_t::serialize_to_string(std::string& _str)
    {
        int16_t _net_int = htoni(_raw_data);
        _str.append((char*)(&_net_int), __number_object_int16_t::_object_size);
        return true;
    }

    template<> inline bool __number_object_int16_t::serialize_to_buffer(const char* _buffer, uint32_t& _size, uint32_t& _offset)
    {
        if_return(_buffer == nullptr_t || _size < __number_object_int16_t::_object_size, false);
        int16_t _net_int = htoni(_raw_data);
        memcpy((char*)_buffer + _offset, &_net_int, __number_object_int16_t::_object_size);
        _size -= __number_object_int16_t::_object_size; _offset += __number_object_int16_t::_object_size;
        return true;
    }

    template<> inline bool __number_object_int16_t::unserialize_from_buffer(const char* _buffer, uint32_t& _size, uint32_t& _offset)
    {
        if_return(_buffer == nullptr_t || _size < __number_object_int16_t::_object_size, false);
        memcpy(&_raw_data, (char*)_buffer + _offset, __number_object_int16_t::_object_size);
        _raw_data = ntohi(_raw_data);
        _size -= __number_object_int16_t::_object_size; _offset += __number_object_int16_t::_object_size;
        return true;
    }

    template<> inline bool __number_object_int32_t::serialize_to_string(std::string& _str)
    {
        int32_t _net_int = htoni(_raw_data);
        _str.append((char*)(&_net_int), __number_object_int32_t::_object_size);
        return true;
    }

    template<> inline bool __number_object_int32_t::serialize_to_buffer(const char* _buffer, uint32_t& _size, uint32_t& _offset)
    {
        if_return(_buffer == nullptr_t || _size < __number_object_int32_t::_object_size, false);
        int32_t _net_int = htoni(_raw_data);
        memcpy((char*)_buffer + _offset, &_net_int, __number_object_int32_t::_object_size);
        _size -= __number_object_int32_t::_object_size; _offset += __number_object_int32_t::_object_size;
        return true;
    }

    template<> inline bool __number_object_int32_t::unserialize_from_buffer(const char* _buffer, uint32_t& _size, uint32_t& _offset)
    {
        if_return(_buffer == nullptr_t || _size < __number_object_int32_t::_object_size, false);
        memcpy(&_raw_data, (char*)_buffer + _offset, __number_object_int32_t::_object_size);
        _raw_data = ntohi(_raw_data);
        _size -= __number_object_int32_t::_object_size; _offset += __number_object_int32_t::_object_size;
        return true;
    }

    template<> inline bool __number_object_int64_t::serialize_to_string(std::string& _str)
    {
        int64_t _net_int = htoni(_raw_data);
        _str.append((char*)(&_net_int), __number_object_int64_t::_object_size);
        return true;
    }

    template<> inline bool __number_object_int64_t::serialize_to_buffer(const char* _buffer, uint32_t& _size, uint32_t& _offset)
    {
        if_return(_buffer == nullptr_t || _size < __number_object_int64_t::_object_size, false);
        int64_t _net_int = htoni(_raw_data);
        memcpy((char*)_buffer + _offset, &_net_int, __number_object_int64_t::_object_size);
        _size -= __number_object_int64_t::_object_size; _offset += __number_object_int64_t::_object_size;
        return true;
    }

    template<> inline bool __number_object_int64_t::unserialize_from_buffer(const char* _buffer, uint32_t& _size, uint32_t& _offset)
    {
        if_return(_buffer == nullptr_t || _size < __number_object_int64_t::_object_size, false);
        memcpy(&_raw_data, (char*)_buffer + _offset, __number_object_int64_t::_object_size);
        _raw_data = ntohi(_raw_data);
        _size -= __number_object_int64_t::_object_size; _offset += __number_object_int64_t::_object_size;
        return true;
    }

    template<> inline bool __number_object_float_t::serialize_to_string(std::string& _str)
    {
        _str.append((char*)(&_raw_data), __number_object_float_t::_object_size);
        return true;
    }

    template<> inline bool __number_object_float_t::serialize_to_buffer(const char* _buffer, uint32_t& _size, uint32_t& _offset)
    {
        if_return(_buffer == nullptr_t || _size < __number_object_float_t::_object_size, false);
        memcpy((char*)_buffer + _offset, &_raw_data, sizeof(float));
        _size -= __number_object_float_t::_object_size; _offset += __number_object_float_t::_object_size;
        return true;
    }

    template<> inline bool __number_object_float_t::unserialize_from_buffer(const char* _buffer, uint32_t& _size, uint32_t& _offset)
    {
        if_return(_buffer == nullptr_t || _size < __number_object_float_t::_object_size, false);
        memcpy(&_raw_data, (char*)_buffer + _offset, sizeof(float));
        _size -= __number_object_int64_t::_object_size; _offset += __number_object_float_t::_object_size;
        return true;
    }

    template<> inline bool __number_object_double_t::serialize_to_string(std::string& _str)
    {
        _str.append((char*)(&_raw_data), __number_object_double_t::_object_size);
        return true;
    }

    template<> inline bool __number_object_double_t::serialize_to_buffer(const char* _buffer, uint32_t& _size, uint32_t& _offset)
    {
        if_return(_buffer == nullptr_t || _size < __number_object_double_t::_object_size, false);
        memcpy((char*)_buffer + _offset, &_raw_data, sizeof(double));
        _size -= __number_object_double_t::_object_size; _offset += __number_object_double_t::_object_size;
        return true;
    }

    template<> inline bool __number_object_double_t::unserialize_from_buffer(const char* _buffer, uint32_t& _size, uint32_t& _offset)
    {
        if_return(_buffer == nullptr_t || _size < __number_object_double_t::_object_size, false);
        memcpy(&_raw_data, (char*)_buffer + _offset, sizeof(double));
        _size -= __number_object_int64_t::_object_size; _offset += __number_object_double_t::_object_size;
        return true;
    }

    struct __string_object_t
    {
        std::string _raw_data;

        __string_object_t() {}
        __string_object_t(std::string& _str) {_raw_data = _str;}
        __string_object_t(const char* _str) {_raw_data = _str;}
        __string_object_t(const __string_object_t& _obj) {_raw_data = _obj._raw_data;}

        void operator= (std::string& _str) {_raw_data = _str;}
        void operator= (const char* _str) {_raw_data = _str;}
        void operator= (const __string_object_t& _obj) {_raw_data = _obj._raw_data;}

        inline bool calc_size(uint32_t& _size)
        {
            _size = sizeof(uint32_t) + (uint32_t)_raw_data.length(); 
            return true;
        }

        inline bool serialize_to_string(std::string& _str)
        {
            uint32_t _size = htonl((uint32_t)_raw_data.length());
            _str.append((char*)&_size, sizeof(uint32_t));
            _str.append((char*)_raw_data.c_str(), _raw_data.length());
            return true;
        }

        inline bool serialize_to_buffer(const char* _buffer, uint32_t& _size, uint32_t& _offset)
        {
            uint32_t _len = (uint32_t)_raw_data.length();
            if_return(nullptr_t == _buffer || _size < sizeof(uint32_t) + _len, false);
            uint32_t _str_size = _len; _str_size = htonl(_str_size);
            memcpy((char*)_buffer + _offset, (void*)&_str_size, sizeof(uint32_t));
            _size -= sizeof(uint32_t); _offset += sizeof(uint32_t);
            memcpy((char*)_buffer + _offset, (char*)_raw_data.c_str(), _len);
            _size -= _len; _offset += _len;
            return true;
        }

        inline bool unserialize_from_buffer(const char* _buffer, uint32_t& _size, uint32_t& _offset)
        {
            _raw_data.clear();
            if_return(_buffer == nullptr_t || _size < sizeof(uint32_t), false);

            uint32_t _str_size = 0;
            memcpy((void*)&_str_size, (char*)_buffer + _offset, sizeof(uint32_t));
            _str_size = ntohl(_str_size);
            _size -= sizeof(uint32_t); _offset += sizeof(uint32_t);
            if_return(_size < _str_size, false);

            if (_str_size > 0)
            {
                _raw_data.append((char*)_buffer + _offset, _str_size);
                _size -= _str_size; _offset += _str_size;
            }
            return true;
        }
    };
    typedef __string_object_t   s_string_t;

    struct  __buffer_object_t
    {
        std::string _raw_data;

        __buffer_object_t(){}
        __buffer_object_t(std::string& _buffer) {_raw_data = _buffer;}
        __buffer_object_t(const __buffer_object_t& _obj) {_raw_data = _obj._raw_data;}

        void operator= (std::string& _buffer) {_raw_data = _buffer;}
        void operator= (const __buffer_object_t& _obj) {_raw_data = _obj._raw_data;}

        bool append_buffer(const char* _buffer, uint32_t _size)
        {
            if_return(nullptr_t == _buffer || 0 == _size, false);
            _raw_data.append(_buffer, _size);
            return true;
        }

        bool clear()
        {
            _raw_data.clear();
            return true;
        }

        inline bool calc_size(uint32_t& _size)
        {
            _size = sizeof(uint32_t) + (uint32_t)_raw_data.size(); 
            return true;
        }

        inline bool serialize_to_string(std::string& _str)
        {
            uint32_t _size = htonl((uint32_t)_raw_data.size());
            _str.append((char*)&_size, sizeof(uint32_t));
            _str.append((char*)_raw_data.c_str(), _raw_data.size());
            return true;
        }

        inline bool serialize_to_buffer(const char* _buffer, uint32_t& _size, uint32_t& _offset)
        {
            uint32_t _len = (uint32_t)_raw_data.size();
            if_return(nullptr_t == _buffer || _size < sizeof(uint32_t) + _len, false);
            uint32_t _str_size = _len; _str_size = htonl(_str_size);
            memcpy((char*)_buffer + _offset, (void*)&_str_size, sizeof(uint32_t));
            _size -= sizeof(uint32_t); _offset += sizeof(uint32_t);
            memcpy((char*)_buffer + _offset, (char*)_raw_data.c_str(), _len);
            _size -= _len; _offset += _len;
            return true;
        }

        inline bool unserialize_from_buffer(const char* _buffer, uint32_t& _size, uint32_t& _offset)
        {
            _raw_data.clear();
            if_return(_buffer == nullptr_t || _size < sizeof(uint32_t), false);

            uint32_t _str_size = 0;
            memcpy((void*)&_str_size, (char*)_buffer + _offset, sizeof(uint32_t));
            _str_size = ntohl(_str_size);
            _size -= sizeof(uint32_t); _offset += sizeof(uint32_t);
            if_return(_size < _str_size, false);

            if (_str_size > 0)
            {
                _raw_data.append((char*)_buffer + _offset, _str_size);
                _size -= _str_size; _offset += _str_size;
            }
            return true;
        }
    };
    typedef __buffer_object_t   s_buffer_t;

    template<typename _t> struct __list_object_t
    {
        std::vector<_t > _raw_data;

        __list_object_t() {}
        __list_object_t(const __list_object_t& _obj) {_raw_data = _obj._raw_data;}

        inline bool calc_size(uint32_t& _size)
        {
            _size = sizeof(uint32_t);
            for (size_t i = 0; i < _raw_data.size(); ++i)
            {
                uint32_t _item_size = 0;
                _raw_data[i].calc_size(_item_size);
                _size += _item_size;
            }
            return true;
        }

        inline bool serialize_to_string(std::string& _str)
        {
            uint32_t _vec_size = htonl((uint32_t)_raw_data.size());
            _str.append((char*)&_vec_size, sizeof(uint32_t));
            for (size_t i = 0; i < _raw_data.size(); ++i)
            {
                _raw_data[i].serialize_to_string(_str);
            }
            return true;
        }

        inline bool serialize_to_buffer(const char* _buffer, uint32_t& _size, uint32_t& _offset)
        {
            if_return(nullptr_t == _buffer || _size < sizeof(uint32_t), false);
            uint32_t _vec_size = htonl((uint32_t)_raw_data.size());
            memcpy((char**)_buffer + _offset, (char*)&_vec_size, sizeof(uint32_t));
            _size -= sizeof(uint32_t); _offset += sizeof(uint32_t);
            for (size_t i = 0; i < _raw_data.size(); ++i)
            {
                if_return(!_raw_data[i].serialize_to_buffer(_buffer, _size, _offset), false);
            }
            return true;
        }

        inline bool unserialize_from_buffer(const char* _buffer, uint32_t& _size, uint32_t& _offset)
        {
            _raw_data.clear();
            if_return(_buffer == nullptr_t || _size < sizeof(uint32_t), false);
            uint32_t _vec_size = 0;
            memcpy((void*)&_vec_size, (char*)_buffer + _offset, sizeof(uint32_t));
            _vec_size = ntohl(_vec_size);
            _size -= sizeof(uint32_t); _offset += sizeof(uint32_t);
            if_return(0 == _vec_size, true);
            _raw_data.resize(_vec_size);
            for (size_t i = 0; i < _vec_size; ++i)
            {
                if_return(!_raw_data[i].unserialize_from_buffer(_buffer, _size, _offset), false);
            }
            return true;
        }
    };

    template<typename _t> struct expand: public _t
    {
        inline bool serialize_to_string(std::string& _str)
        {
            return _t::serialize_to_string(_str);
        }

        inline bool serialize_to_buffer(const char* _buffer, uint32_t _size)
        {
            uint32_t _offset = 0;
            uint32_t _inner_size = _size;
            return _t::serialize_to_buffer(_buffer, _inner_size, _offset);
        }

        inline bool unserialize_from_buffer(const char* _buffer, uint32_t _size)
        {
            uint32_t _offset = 0;
            uint32_t _inner_size = _size;
            return _t::unserialize_from_buffer(_buffer, _inner_size, _offset);
        }
    };
}

#define __serialize_calc_size_begin()       inline bool calc_size(uint32_t& _size){ 
#define __serialize_calc_size(_v)                                                           \
    {                                                                                       \
        uint32_t _v_size = 0;                                                               \
        if (false == _v.calc_size(_v_size)) return false;                                   \
        _size += _v_size;                                                                   \
    }
#define __serialize_calc_size_end()         return true;}
#define __serialize_to_string_begin()       inline bool serialize_to_string(std::string& _str){
#define __serialize_to_string(_v)           if (!_v.serialize_to_string(_str)) return false;
#define __serialize_to_string_end()         return true;}
#define __serialize_to_buffer_begin()       inline bool serialize_to_buffer(const char* _buffer, uint32_t& _size, uint32_t& _offset){
#define __serialize_to_buffer(_v)           if (!_v.serialize_to_buffer(_buffer, _size, _offset)) return false;
#define __serialize_to_buffer_end()         return true;}
#define __unserialize_from_buffer_begin()   inline bool unserialize_from_buffer(const char* _buffer, uint32_t& _size, uint32_t& _offset){
#define __unserialize_from_buffer(_v)       if (!_v.unserialize_from_buffer(_buffer, _size, _offset)) return false;
#define __unserialize_from_buffer_end()     return true;}

#define __serialize_def_begin(_name)        struct _name{
#define __serialize_def_end_0()             };

#define __serialize_def_end_1(v0)                                                           \
        __serialize_calc_size_begin()                                                       \
        __serialize_calc_size(v0)                                                           \
        __serialize_calc_size_end()                                                         \
        __serialize_to_string_begin()                                                       \
        __serialize_to_string(v0)                                                           \
        __serialize_to_string_end()                                                         \
        __serialize_to_buffer_begin()                                                       \
        __serialize_to_buffer(v0)                                                           \
        __serialize_to_buffer_end()                                                         \
        __unserialize_from_buffer_begin()                                                   \
        __unserialize_from_buffer(v0)                                                       \
        __unserialize_from_buffer_end()};

#define __serialize_def_end_2(v0, v1)                                                       \
        __serialize_calc_size_begin()                                                       \
        __serialize_calc_size(v0)                                                           \
        __serialize_calc_size(v1)                                                           \
        __serialize_calc_size_end()                                                         \
        __serialize_to_string_begin()                                                       \
        __serialize_to_string(v0)                                                           \
        __serialize_to_string(v1)                                                           \
        __serialize_to_string_end()                                                         \
        __serialize_to_buffer_begin()                                                       \
        __serialize_to_buffer(v0)                                                           \
        __serialize_to_buffer(v1)                                                           \
        __serialize_to_buffer_end()                                                         \
        __unserialize_from_buffer_begin()                                                   \
        __unserialize_from_buffer(v0)                                                       \
        __unserialize_from_buffer(v1)                                                       \
        __unserialize_from_buffer_end()};

#define __serialize_def_end_3(v0, v1, v2)                                                   \
        __serialize_calc_size_begin()                                                       \
        __serialize_calc_size(v0)                                                           \
        __serialize_calc_size(v1)                                                           \
        __serialize_calc_size(v2)                                                           \
        __serialize_calc_size_end()                                                         \
        __serialize_to_string_begin()                                                       \
        __serialize_to_string(v0)                                                           \
        __serialize_to_string(v1)                                                           \
        __serialize_to_string(v2)                                                           \
        __serialize_to_string_end()                                                         \
        __serialize_to_buffer_begin()                                                       \
        __serialize_to_buffer(v0)                                                           \
        __serialize_to_buffer(v1)                                                           \
        __serialize_to_buffer(v2)                                                           \
        __serialize_to_buffer_end()                                                         \
        __unserialize_from_buffer_begin()                                                   \
        __unserialize_from_buffer(v0)                                                       \
        __unserialize_from_buffer(v1)                                                       \
        __unserialize_from_buffer(v2)                                                       \
        __unserialize_from_buffer_end()};

#define __serialize_def_end_4(v0, v1, v2, v3)                                               \
        __serialize_calc_size_begin()                                                       \
        __serialize_calc_size(v0)                                                           \
        __serialize_calc_size(v1)                                                           \
        __serialize_calc_size(v2)                                                           \
        __serialize_calc_size(v3)                                                           \
        __serialize_calc_size_end()                                                         \
        __serialize_to_string_begin()                                                       \
        __serialize_to_string(v0)                                                           \
        __serialize_to_string(v1)                                                           \
        __serialize_to_string(v2)                                                           \
        __serialize_to_string(v3)                                                           \
        __serialize_to_string_end()                                                         \
        __serialize_to_buffer_begin()                                                       \
        __serialize_to_buffer(v0)                                                           \
        __serialize_to_buffer(v1)                                                           \
        __serialize_to_buffer(v2)                                                           \
        __serialize_to_buffer(v3)                                                           \
        __serialize_to_buffer_end()                                                         \
        __unserialize_from_buffer_begin()                                                   \
        __unserialize_from_buffer(v0)                                                       \
        __unserialize_from_buffer(v1)                                                       \
        __unserialize_from_buffer(v2)                                                       \
        __unserialize_from_buffer(v3)                                                       \
        __unserialize_from_buffer_end()};

#define __serialize_def_end_5(v0, v1, v2, v3, v4)                                           \
        __serialize_calc_size_begin()                                                       \
        __serialize_calc_size(v0)                                                           \
        __serialize_calc_size(v1)                                                           \
        __serialize_calc_size(v2)                                                           \
        __serialize_calc_size(v3)                                                           \
        __serialize_calc_size(v4)                                                           \
        __serialize_calc_size_end()                                                         \
        __serialize_to_string_begin()                                                       \
        __serialize_to_string(v0)                                                           \
        __serialize_to_string(v1)                                                           \
        __serialize_to_string(v2)                                                           \
        __serialize_to_string(v3)                                                           \
        __serialize_to_string(v4)                                                           \
        __serialize_to_string_end()                                                         \
        __serialize_to_buffer_begin()                                                       \
        __serialize_to_buffer(v0)                                                           \
        __serialize_to_buffer(v1)                                                           \
        __serialize_to_buffer(v2)                                                           \
        __serialize_to_buffer(v3)                                                           \
        __serialize_to_buffer(v4)                                                           \
        __serialize_to_buffer_end()                                                         \
        __unserialize_from_buffer_begin()                                                   \
        __unserialize_from_buffer(v0)                                                       \
        __unserialize_from_buffer(v1)                                                       \
        __unserialize_from_buffer(v2)                                                       \
        __unserialize_from_buffer(v3)                                                       \
        __unserialize_from_buffer(v4)                                                       \
        __unserialize_from_buffer_end()};

#define __serialize_def_end_6(v0, v1, v2, v3, v4, v5)                                       \
        __serialize_calc_size_begin()                                                       \
        __serialize_calc_size(v0)                                                           \
        __serialize_calc_size(v1)                                                           \
        __serialize_calc_size(v2)                                                           \
        __serialize_calc_size(v3)                                                           \
        __serialize_calc_size(v4)                                                           \
        __serialize_calc_size(v5)                                                           \
        __serialize_calc_size_end()                                                         \
        __serialize_to_string_begin()                                                       \
        __serialize_to_string(v0)                                                           \
        __serialize_to_string(v1)                                                           \
        __serialize_to_string(v2)                                                           \
        __serialize_to_string(v3)                                                           \
        __serialize_to_string(v4)                                                           \
        __serialize_to_string(v5)                                                           \
        __serialize_to_string_end()                                                         \
        __serialize_to_buffer_begin()                                                       \
        __serialize_to_buffer(v0)                                                           \
        __serialize_to_buffer(v1)                                                           \
        __serialize_to_buffer(v2)                                                           \
        __serialize_to_buffer(v3)                                                           \
        __serialize_to_buffer(v4)                                                           \
        __serialize_to_buffer(v5)                                                           \
        __serialize_to_buffer_end()                                                         \
        __unserialize_from_buffer_begin()                                                   \
        __unserialize_from_buffer(v0)                                                       \
        __unserialize_from_buffer(v1)                                                       \
        __unserialize_from_buffer(v2)                                                       \
        __unserialize_from_buffer(v3)                                                       \
        __unserialize_from_buffer(v4)                                                       \
        __unserialize_from_buffer(v5)                                                       \
        __unserialize_from_buffer_end()};

#define __serialize_def_end_7(v0, v1, v2, v3, v4, v5, v6)                                   \
        __serialize_calc_size_begin()                                                       \
        __serialize_calc_size(v0)                                                           \
        __serialize_calc_size(v1)                                                           \
        __serialize_calc_size(v2)                                                           \
        __serialize_calc_size(v3)                                                           \
        __serialize_calc_size(v4)                                                           \
        __serialize_calc_size(v5)                                                           \
        __serialize_calc_size(v6)                                                           \
        __serialize_calc_size_end()                                                         \
        __serialize_to_string_begin()                                                       \
        __serialize_to_string(v0)                                                           \
        __serialize_to_string(v1)                                                           \
        __serialize_to_string(v2)                                                           \
        __serialize_to_string(v3)                                                           \
        __serialize_to_string(v4)                                                           \
        __serialize_to_string(v5)                                                           \
        __serialize_to_string(v6)                                                           \
        __serialize_to_string_end()                                                         \
        __serialize_to_buffer_begin()                                                       \
        __serialize_to_buffer(v0)                                                           \
        __serialize_to_buffer(v1)                                                           \
        __serialize_to_buffer(v2)                                                           \
        __serialize_to_buffer(v3)                                                           \
        __serialize_to_buffer(v4)                                                           \
        __serialize_to_buffer(v5)                                                           \
        __serialize_to_buffer(v6)                                                           \
        __serialize_to_buffer_end()                                                         \
        __unserialize_from_buffer_begin()                                                   \
        __unserialize_from_buffer(v0)                                                       \
        __unserialize_from_buffer(v1)                                                       \
        __unserialize_from_buffer(v2)                                                       \
        __unserialize_from_buffer(v3)                                                       \
        __unserialize_from_buffer(v4)                                                       \
        __unserialize_from_buffer(v5)                                                       \
        __unserialize_from_buffer(v6)                                                       \
        __unserialize_from_buffer_end()};

#define __serialize_def_end_8(v0, v1, v2, v3, v4, v5, v6, v7)                               \
        __serialize_calc_size_begin()                                                       \
        __serialize_calc_size(v0)                                                           \
        __serialize_calc_size(v1)                                                           \
        __serialize_calc_size(v2)                                                           \
        __serialize_calc_size(v3)                                                           \
        __serialize_calc_size(v4)                                                           \
        __serialize_calc_size(v5)                                                           \
        __serialize_calc_size(v6)                                                           \
        __serialize_calc_size(v7)                                                           \
        __serialize_calc_size_end()                                                         \
        __serialize_to_string_begin()                                                       \
        __serialize_to_string(v0)                                                           \
        __serialize_to_string(v1)                                                           \
        __serialize_to_string(v2)                                                           \
        __serialize_to_string(v3)                                                           \
        __serialize_to_string(v4)                                                           \
        __serialize_to_string(v5)                                                           \
        __serialize_to_string(v6)                                                           \
        __serialize_to_string(v7)                                                           \
        __serialize_to_string_end()                                                         \
        __serialize_to_buffer_begin()                                                       \
        __serialize_to_buffer(v0)                                                           \
        __serialize_to_buffer(v1)                                                           \
        __serialize_to_buffer(v2)                                                           \
        __serialize_to_buffer(v3)                                                           \
        __serialize_to_buffer(v4)                                                           \
        __serialize_to_buffer(v5)                                                           \
        __serialize_to_buffer(v6)                                                           \
        __serialize_to_buffer(v7)                                                           \
        __serialize_to_buffer_end()                                                         \
        __unserialize_from_buffer_begin()                                                   \
        __unserialize_from_buffer(v0)                                                       \
        __unserialize_from_buffer(v1)                                                       \
        __unserialize_from_buffer(v2)                                                       \
        __unserialize_from_buffer(v3)                                                       \
        __unserialize_from_buffer(v4)                                                       \
        __unserialize_from_buffer(v5)                                                       \
        __unserialize_from_buffer(v6)                                                       \
        __unserialize_from_buffer(v7)                                                       \
        __unserialize_from_buffer_end()};

#define __serialize_def_end_9(v0, v1, v2, v3, v4, v5, v6, v7, v8)                           \
        __serialize_calc_size_begin()                                                       \
        __serialize_calc_size(v0)                                                           \
        __serialize_calc_size(v1)                                                           \
        __serialize_calc_size(v2)                                                           \
        __serialize_calc_size(v3)                                                           \
        __serialize_calc_size(v4)                                                           \
        __serialize_calc_size(v5)                                                           \
        __serialize_calc_size(v6)                                                           \
        __serialize_calc_size(v7)                                                           \
        __serialize_calc_size(v8)                                                           \
        __serialize_calc_size_end()                                                         \
        __serialize_to_string_begin()                                                       \
        __serialize_to_string(v0)                                                           \
        __serialize_to_string(v1)                                                           \
        __serialize_to_string(v2)                                                           \
        __serialize_to_string(v3)                                                           \
        __serialize_to_string(v4)                                                           \
        __serialize_to_string(v5)                                                           \
        __serialize_to_string(v6)                                                           \
        __serialize_to_string(v7)                                                           \
        __serialize_to_string(v8)                                                           \
        __serialize_to_string_end()                                                         \
        __serialize_to_buffer_begin()                                                       \
        __serialize_to_buffer(v0)                                                           \
        __serialize_to_buffer(v1)                                                           \
        __serialize_to_buffer(v2)                                                           \
        __serialize_to_buffer(v3)                                                           \
        __serialize_to_buffer(v4)                                                           \
        __serialize_to_buffer(v5)                                                           \
        __serialize_to_buffer(v6)                                                           \
        __serialize_to_buffer(v7)                                                           \
        __serialize_to_buffer(v8)                                                           \
        __serialize_to_buffer_end()                                                         \
        __unserialize_from_buffer_begin()                                                   \
        __unserialize_from_buffer(v0)                                                       \
        __unserialize_from_buffer(v1)                                                       \
        __unserialize_from_buffer(v2)                                                       \
        __unserialize_from_buffer(v3)                                                       \
        __unserialize_from_buffer(v4)                                                       \
        __unserialize_from_buffer(v5)                                                       \
        __unserialize_from_buffer(v6)                                                       \
        __unserialize_from_buffer(v7)                                                       \
        __unserialize_from_buffer(v8)                                                       \
        __unserialize_from_buffer_end()};

#define __serialize_def_end_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9)                      \
        __serialize_calc_size_begin()                                                       \
        __serialize_calc_size(v0)                                                           \
        __serialize_calc_size(v1)                                                           \
        __serialize_calc_size(v2)                                                           \
        __serialize_calc_size(v3)                                                           \
        __serialize_calc_size(v4)                                                           \
        __serialize_calc_size(v5)                                                           \
        __serialize_calc_size(v6)                                                           \
        __serialize_calc_size(v7)                                                           \
        __serialize_calc_size(v8)                                                           \
        __serialize_calc_size(v9)                                                           \
        __serialize_calc_size_end()                                                         \
        __serialize_to_string_begin()                                                       \
        __serialize_to_string(v0)                                                           \
        __serialize_to_string(v1)                                                           \
        __serialize_to_string(v2)                                                           \
        __serialize_to_string(v3)                                                           \
        __serialize_to_string(v4)                                                           \
        __serialize_to_string(v5)                                                           \
        __serialize_to_string(v6)                                                           \
        __serialize_to_string(v7)                                                           \
        __serialize_to_string(v8)                                                           \
        __serialize_to_string(v9)                                                           \
        __serialize_to_string_end()                                                         \
        __serialize_to_buffer_begin()                                                       \
        __serialize_to_buffer(v0)                                                           \
        __serialize_to_buffer(v1)                                                           \
        __serialize_to_buffer(v2)                                                           \
        __serialize_to_buffer(v3)                                                           \
        __serialize_to_buffer(v4)                                                           \
        __serialize_to_buffer(v5)                                                           \
        __serialize_to_buffer(v6)                                                           \
        __serialize_to_buffer(v7)                                                           \
        __serialize_to_buffer(v8)                                                           \
        __serialize_to_buffer(v9)                                                           \
        __serialize_to_buffer_end()                                                         \
        __unserialize_from_buffer_begin()                                                   \
        __unserialize_from_buffer(v0)                                                       \
        __unserialize_from_buffer(v1)                                                       \
        __unserialize_from_buffer(v2)                                                       \
        __unserialize_from_buffer(v3)                                                       \
        __unserialize_from_buffer(v4)                                                       \
        __unserialize_from_buffer(v5)                                                       \
        __unserialize_from_buffer(v6)                                                       \
        __unserialize_from_buffer(v7)                                                       \
        __unserialize_from_buffer(v8)                                                       \
        __unserialize_from_buffer(v9)                                                       \
        __unserialize_from_buffer_end()};

#define __serialize_def_end_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10)                 \
        __serialize_calc_size_begin()                                                       \
        __serialize_calc_size(v0)                                                           \
        __serialize_calc_size(v1)                                                           \
        __serialize_calc_size(v2)                                                           \
        __serialize_calc_size(v3)                                                           \
        __serialize_calc_size(v4)                                                           \
        __serialize_calc_size(v5)                                                           \
        __serialize_calc_size(v6)                                                           \
        __serialize_calc_size(v7)                                                           \
        __serialize_calc_size(v8)                                                           \
        __serialize_calc_size(v9)                                                           \
        __serialize_calc_size(v10)                                                          \
        __serialize_calc_size_end()                                                         \
        __serialize_to_string_begin()                                                       \
        __serialize_to_string(v0)                                                           \
        __serialize_to_string(v1)                                                           \
        __serialize_to_string(v2)                                                           \
        __serialize_to_string(v3)                                                           \
        __serialize_to_string(v4)                                                           \
        __serialize_to_string(v5)                                                           \
        __serialize_to_string(v6)                                                           \
        __serialize_to_string(v7)                                                           \
        __serialize_to_string(v8)                                                           \
        __serialize_to_string(v9)                                                           \
        __serialize_to_string(v10)                                                          \
        __serialize_to_string_end()                                                         \
        __serialize_to_buffer_begin()                                                       \
        __serialize_to_buffer(v0)                                                           \
        __serialize_to_buffer(v1)                                                           \
        __serialize_to_buffer(v2)                                                           \
        __serialize_to_buffer(v3)                                                           \
        __serialize_to_buffer(v4)                                                           \
        __serialize_to_buffer(v5)                                                           \
        __serialize_to_buffer(v6)                                                           \
        __serialize_to_buffer(v7)                                                           \
        __serialize_to_buffer(v8)                                                           \
        __serialize_to_buffer(v9)                                                           \
        __serialize_to_buffer(v10)                                                          \
        __serialize_to_buffer_end()                                                         \
        __unserialize_from_buffer_begin()                                                   \
        __unserialize_from_buffer(v0)                                                       \
        __unserialize_from_buffer(v1)                                                       \
        __unserialize_from_buffer(v2)                                                       \
        __unserialize_from_buffer(v3)                                                       \
        __unserialize_from_buffer(v4)                                                       \
        __unserialize_from_buffer(v5)                                                       \
        __unserialize_from_buffer(v6)                                                       \
        __unserialize_from_buffer(v7)                                                       \
        __unserialize_from_buffer(v8)                                                       \
        __unserialize_from_buffer(v9)                                                       \
        __unserialize_from_buffer(v10)                                                      \
        __unserialize_from_buffer_end()};
#endif//_include_common_serialize_h