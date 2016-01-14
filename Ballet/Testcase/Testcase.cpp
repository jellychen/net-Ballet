#include "Include/Def/Inct.h"
#include "Include/Common/Inct.h"
#include "Include/Network/IEventPoll.h"
#include "ConvertTest.h"
#include "TimerTest.h"
#include "SignalTest.h"
#include "TaskTest.h"
#include "TcpServerTest.h"
#include "TelnetTest.h"
#include "HttpServerTest.h"
#include "TcpClientTest.h"

#include "Include/Common/IThread.h"
#include "Include/Common/IThreadSolo.h"

#include <map>
#include <string>
#include <iostream>

using namespace std;
using namespace common;

__serialize_def_begin(s_object3)
__number_object_uint32_t    _v1;
__number_object_uint32_t    _v2;
__string_object_t           _name;
__serialize_def_end_3(_v1, _v2, _name)

__serialize_def_begin(s_object4)
__number_object_uint32_t    _v1;
__number_object_uint32_t    _v2;
__string_object_t           _name;
s_object3                   _object;
__serialize_def_end_4(_v1, _v2, _name, _object)

__serialize_def_begin(s_object5)
__number_object_uint32_t    _v1;
__number_object_uint32_t    _v2;
__string_object_t           _name;
__list_object_t<s_object3>  _object;
__serialize_def_end_4(_v1, _v2, _name, _object)


__list_object_t<__number_object_uint32_t>  _test_object;

int i;
TaskQueueEntry* _entry = nullptr_t;

struct _task_test
{
    void do_task()
    {
        //printf("%s %lu\n", "do_task", pthread_self());
    }
    
    void do_task_complete()
    { 
        i++;
        if (i % 1000 == 0)
            printf("%s %lu  > %d\n", "do_task_complete", pthread_self(),  i);
        //common::thread::execute_task(TaskExecutEcho<_task_test>::create(_entry, nullptr_t));
    }
};


int main()
{
    
    i = 0;
    printf("%s\n", "Testcase");
    common::init();
    common::load_module("libNetwork.so");

    HttpServerTest _test;  
    _test.test();
    return 0;

/*
    __sp<network::IEventPoll> _poll;
    common::let_object(_type_id(network::IEventPoll), (void**)&_poll);
    if (_poll) printf("%s\n", "let_object IEventPoll success");
    _poll->init();
    _poll->event_loop_entry(&_entry);

    //for (int i = 0; i < 100; ++i)
    {
        common::thread::execute_task(TaskExecutEcho<_task_test>::create(_entry, nullptr_t));
    }
   
    __sp<common::IThread> _thread;
    common::let_object(_type_id(common::IThread), (void**)&_thread);
   
    if (_thread) printf("%s\n", "let_object IThread success");

    for (int i = 0; i < 1000000; ++i)
    _thread->do_task(TaskExecutEcho<_task_test>::create(_entry, nullptr_t));
	
    _poll->event_loop();
    return 0;

/*
    s_object3 _object;
    _object._v1 = 10000;
    _object._v2 = 2;
    _object._name = "guodongasdasdasdasdasdasdasdasdasdasdasdasdksadjflkasdjflkasdl;kfjal;ksdjfl;kajsdfl;kajsdf";

    s_object5 _object4;
    _object4._name = "gsdzkljfklsadjfklsjdkfljskdlfjklsdfjklasdasdasdasdasdasasasdasdasdasd";
    for (int i = 0; i < 37; ++i)
    {
        _object4._object._raw_data.push_back(_object);
    }

    std::string _str;
    _object4.serialize_to_string(_str);

    for (int i = 0; i < 1000000; ++i)
    {
        uint32_t __len = (uint32_t)_str.length();
        uint32_t __offset = 0;
        s_object5 _data_out;
        bool _ret = _data_out.unserialize_from_buffer(_str.c_str(), __len, __offset);

    }
    return 0;

*/
    


    int _test_id = -1;
    printf("%s", "input test case id: ");
    scanf("%d", &_test_id);
    
    switch(_test_id)
    {
    case 0:
        {
            TimerTest _test;
            _test.test();
            break;  
        }

    case 1:
        {
            SignalTest _test;
            _test.test();
            break;
        }

    case 2:
        {
            TaskTest _test;
            _test.test();
            break;
        }
    case 3:
        {
            TcpServerTest _test;
            _test.test();
        }
    case 4:
        {
            TelnetTest _test;
            _test.test();
        }
    case 5:
        {
            HttpServerTest _test;
            _test.test();
            break;
        }
    case 6:
        {
            TcpClientTest _test;
            _test.test();
        }
    }
    printf("%s\n", "test   end");
    return 0;
}