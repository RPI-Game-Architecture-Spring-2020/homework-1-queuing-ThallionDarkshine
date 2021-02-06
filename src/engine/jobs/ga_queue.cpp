/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

#include "ga_queue.h"
#include <stddef.h>
#include <climits>
#include <cassert>

ga_queue::ga_queue(int node_count)
{
	// allocate dummy node
	ga_node* node = new ga_node();
	node->_next = NULL;
	_head = _tail = node;

	// initialize counter variables
	_pushed = _popped = 0;
}

ga_queue::~ga_queue()
{
	// lock operations so the queue can be freed
	_head_lock.lock();
	_tail_lock.lock();

	// free the individual nodes
	ga_node* next;
	while (_head != NULL) {
		next = _head->_next;
		free(_head);
		_head = next;
	}

	_tail_lock.unlock();
	_head_lock.unlock();
}

void ga_queue::push(void* data)
{
	// allocate new node object
	ga_node* node = new ga_node();
	node->_data = data;
	node->_next = NULL;

	// lock and append to queue
	_tail_lock.lock();

	_tail->_next = node;
	_tail = node;
	++_pushed;

	_tail_lock.unlock();
}

bool ga_queue::pop(void** data)
{
	// lock and remove the queue head
	_head_lock.lock();

	ga_node* node = _head;
	ga_node* new_head = node->_next;

	if (new_head == NULL) {
		_head_lock.unlock();
		return false;
	}

	*data = new_head->_data;
	_head = new_head;
	++_popped;

	_head_lock.unlock();

	return true;
}

int ga_queue::get_count() const
{
	// calculate queue count, accounting for potential overflow
	int count = _pushed - _popped;
	if (count < 0) count += UINT_MAX;

	return count;
}
