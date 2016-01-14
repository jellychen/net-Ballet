#include "ServiceDom.h"

namespace monitor
{
	ServiceDomNode::ServiceDomNode()
	{
		this->parent_ = nullptr_t;
	}

	ServiceDomNode::~ServiceDomNode()
	{
		map_children_t::iterator _it = this->children_.begin();
		for (; _it != this->children_.end(); ++_it)
		{
			if_continue(nullptr_t == _it->second);
			delete _it->second;
		}
		this->children_.clear();
	}

	bool ServiceDomNode::get_node(std::string& _path, int _offset, ServiceDomNode** _node)
	{
		if_return(nullptr_t == _node, false);
		*_node = nullptr_t;

		std::string _path_node = "";
		int _position = _offset; bool _find = false;
		for (; _position < (int)_path.length(); ++_position)
		{
			if ('/' == _path[_position]) 
			{
				_find = true; break;
			}
			_path_node += _path[_position];
		}
		if_return(_position > _offset && _path_node.length() <= 0, false);

		if (_offset == _position) 
		{
			*_node = (ServiceDomNode*)this;
		}
		else if (_path_node.length() > 0)
		{
			ServiceDomNode* _find_node = nullptr_t;
			map_children_t::iterator _it = this->children_.find(_path_node);
			if (this->children_.end() != _it) _find_node = _it->second;

			if (nullptr_t == _find_node)
			{
				_find_node = new(std::nothrow)ServiceDomNode();
				if_return(nullptr_t == _find_node, false);
				_find_node->parent_ = (ServiceDomNode*)this;
				this->children_[_path_node] = _find_node;
			}

			if (_find)
			{
				return _find_node->get_node(_path, _position+1, _node);
			}
			else
			{
				*_node = (ServiceDomNode*)(_find_node);
			}
		}
		return true;
	}

	ServiceDomTree::ServiceDomTree()
	{
		this->root_ = nullptr_t;
	}

	ServiceDomTree::~ServiceDomTree()
	{
		if (nullptr_t != this->root_)
		{
			delete this->root_;
		}
	}
	
	bool ServiceDomTree::get_node(std::string& _path, ServiceDomNode** _node)
	{
		if_return(nullptr_t == _node, false);
		*_node = nullptr_t;
		if_return(nullptr_t == this->root_, false);

		std::string _route = "";
		for (int i = 0; i < (int)_path.length(); ++i)
		{
			if (isalpha(_path[i]))
			{
				_route += tolower(_path[i]);
			}
			else if ((_path[i] >= '0' && _path[i] <= '9') || '-' == _path[i] || '_' == _path[i])
			{
				_route += _path[i];
			}
			else
			{
				return false;
			}
		}
		if_return(0 == _route.length() || '/' != _route[0], false);
		
		if (nullptr_t == this->root_)
		{
			this->root_ = new(std::nothrow)ServiceDomNode();
			if_return(nullptr_t == this->root_, false);
		}

		return this->root_->get_node(_route, 1, _node);
	}

	bool ServiceDomTree::clean_up_node(ServiceDomNode* _node)
	{
		if_return(nullptr_t == _node, false);
	}
}