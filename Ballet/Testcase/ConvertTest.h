#include "Include/Def/Inct.h"
#include "Include/Common/Inct.h"
#include "Include/Network/IEventPoll.h"

class ConvertTest
{
public:
    bool test()
    {
        __sp<network::IEventPoll> _poll;
        common::let_object(_type_id(network::IEventPoll), (void**)&_poll);
        if (_poll) printf("%s\n", "let_object success");
        __sp<IObject> _object;
        for (int i =0; i < 50000000; ++i)
        {
            _object = nullptr_t;
            _poll->convert(_type_id(IObject), (void**)&_object);
        }
        return true;
    }
};