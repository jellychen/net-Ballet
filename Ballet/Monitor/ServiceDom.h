#ifndef monitor_servicedom_h
#define monitor_servicedom_h
#include "Include/Def/Inct.h"
#include <string>
#include <vector>
#include <map>

namespace monitor
{
	class ServiceDomNode
	{
		typedef std::vector<std::string > vector_addr_t;
		typedef std::map<std::string, ServiceDomNode*> map_children_t;
	public:
		ServiceDomNode();
		virtual ~ServiceDomNode();

	public:
		bool get_node(std::string& _path, int _offset, ServiceDomNode**);

	public:
		ServiceDomNode*	parent_;
		vector_addr_t	service_addr_;
		map_children_t 	children_;
	};

	class ServiceDomTree
	{
	public:
		ServiceDomTree();
		virtual ~ServiceDomTree();

	public:
		bool get_node		(std::string& _path, ServiceDomNode**);
		bool clean_up_node	(ServiceDomNode*);

	public:
		ServiceDomNode* root_;
	};
}
#endif//monitor_servicedom_h