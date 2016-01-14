#include <iostream>
#include "HttpLogic/HttpServer.h"
using namespace std;

int main()
{
	moment::HttpServer _http_server;
	_http_server.start();
	return 0;
}