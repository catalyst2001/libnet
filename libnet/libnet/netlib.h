/**
* netlib
* 
* Purpose: reliable and unreliable network data transmittion
* Author: Deryabin K.
* Date: 30.12.24
* Version: 0.1
*/
#pragma once
#include <stdint.h>

#define NETLIB_DEFAULT (-1)

/**
* global data
*/
typedef struct netlib_globaldata_s {
	uint32_t num_connections;
	uint32_t max_queue_memory;
	uint32_t max_fragments_memory;
	void* (*p_memalloc_func)(size_t count, size_t size, int flags);
	void  (*p_memfree_func)(void *p_ptr);
} netlib_globaldata_t;

/**
* netlib operations status
*/
typedef enum NETLIB_STATUS_E {
	NETLIB_STATUS_OK = 0,
	NETLIB_STATUS_INVALID_PARAMETER,
	NETLIB_STATUS_OUT_OF_MEMORY,
} NETLIB_STATUS;

/**
* basic handle definitions macro
*/
#define NETLIB_DECL_HANDLE(type) typedef struct { void *p_ptr; } type;

NETLIB_DECL_HANDLE(h_netobj)

/**
* netlib_initialize
* 
* initialize network library
*/
NETLIB_STATUS netlib_initialize();

/**
* netlib_deinitialize
* 
* 
*/
NETLIB_STATUS netlib_deinitialize();



