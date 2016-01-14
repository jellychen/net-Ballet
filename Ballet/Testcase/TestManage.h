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
#include "Include/Service/IServiceManage.h"

using namespace service;

class TestManage
{
public:
	TestManage():_callback(this)
	{
		_callback->hook_on_init(&TestManage::on_init);
		_callback->hook_on_uninit(&TestManage::on_uninit);
		_callback->hook_on_start(&TestManage::on_start);
		_callback->hook_on_run(&TestManage::on_run);
		_callback->hook_on_stoped(&TestManage::on_stoped);
	}

	virtual ~TestManage(){}

public:
	ret on_init(IService* _service)
	{
		return ret_success;
	}

    ret on_uninit(IService* _service)
	{
		return ret_success;
	}
	
    ret on_start(IService* _service)
	{
		return ret_success;
	}
	
    ret on_run(IService* _service)
	{
		return ret_success;
	}
	
    ret on_stoped(IService* _service)
	{
		return ret_success;
	}

	
public:
	CServiceCallbackPtr<TestManage > 	_callback;
	__sp<IService>						_service;
};