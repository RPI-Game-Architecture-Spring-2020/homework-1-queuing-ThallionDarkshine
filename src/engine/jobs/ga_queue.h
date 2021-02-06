#pragma once
#include <mutex>

/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

/*
** Thread-safe queue.
** https://www.research.ibm.com/people/m/michael/podc-1996.pdf
*/

class ga_node {
public:
	void* _data;
	ga_node* _next;
};

class ga_queue
{
public:
	ga_queue(int node_count);
	~ga_queue();

	void push(void* data);
	bool pop(void** data);

	int get_count() const;

private:
	ga_node* _head;
	ga_node* _tail;
	std::mutex _head_lock;
	std::mutex _tail_lock;
	unsigned int _pushed, _popped;
};
