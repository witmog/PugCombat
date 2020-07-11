#pragma once

// This is dependent on "logger.h" for logging support.
// I don't intend on putting any effort into breaking this dependence. 
// You are of course free to do so
#include "logger.h"

#include <stdlib.h>
#include <stdint.h>

// ALWAYS set a vector to VECTOR_EMPTY (or NULL) before using it
#define VECTOR_EMPTY NULL

#define vector(type) type *

// Private macros, not to be used by the user
#define vector_start_ptr(v) &((size_t*)(v))[-2]
#define vector_set_count(v, count) ((size_t*)(v))[-2] = (count)
#define vector_set_capacity(v, cap) ((size_t*)(v))[-1] = (cap)

// Returns the number of elements in the vector (the vector's length)
#define vector_count(v) ((v) ? (((size_t*)(v))[-2]) : 0)
//
// Returns the max amount of elements the vector has space for before it needs to resize
#define vector_capacity(v) ((v) ? (((size_t*)(v))[-1]) : 0)

// Determines the size of the auto-resized vector
// E.g., when an element is appended to a full vector, the vector's capacity doubles to make 
// room for the new element(s).
#define default_resize_amount(v) (vector_capacity((v)) * 2)

// Reserves a specified number of slots in the vector.
// This must be called on a NULL vector to make it usable.
#define vector_reserve(v, capacity)					\
do									\
{									\
	log_assert((v) == VECTOR_EMPTY);					\
	size_t* ___data_ = malloc(sizeof(*(v))*capacity + sizeof(size_t)*2); \
	if (!___data_) { break; }					\
	(v) = (void*)(&___data_[2]);					\
	vector_set_capacity((v), capacity);				\
	vector_set_count((v), 0);					\
} while(0);

// Deletes the vector 
#define vector_free(v)				\
do						\
{						\
	log_assert((v) != VECTOR_EMPTY);		\
	vector_set_count((v), 0);		\
	vector_set_capacity((v), 0);		\
	free(vector_start_ptr((v)));		\
	(v) = VECTOR_EMPTY;			\
} while(0);

// Expands the size of the vector. This is also done automatically as elements are added
// to the vector
#define vector_resize(v, capacity)					\
do									\
{									\
	log_assert((v) != VECTOR_EMPTY);					\
	size_t *___temp_ = realloc(vector_start_ptr((v)),		\
				   sizeof(*(v))*capacity + sizeof(size_t)*2); \
	if (___temp_ == NULL)						\
	{								\
		log_error("Failed to resize vector. Count: %ld. Capacity: %ld.", \
			  vector_count((v)), vector_capacity((v)));		\
		break;							\
	}								\
	(v) = (void*)(&___temp_[2]);					\
	vector_set_capacity((v), capacity);				\
	log_trace("Vector was resized. New capacity: %ld", vector_capacity((v))); \
} while(0);

// Adds an element to the end of the vector
#define vector_push(v, e)						\
do									\
{									\
	log_assert((v) != VECTOR_EMPTY);					\
	if (vector_count((v)) >= vector_capacity((v)))			\
	{								\
		vector_resize((v), default_resize_amount((v))); \
	}								\
	v[vector_count((v))] = e;					\
	vector_set_count((v), vector_count((v))+1);			\
} while(0);								\

// Adds an element to the vector at a specific index
#define vector_insert(v, pos, e) \
do \
{  \
	log_assert((v) != VECTOR_EMPTY); \
	log_assert((pos) < vector_count((v))); \
	if (vector_count((v)) >= vector_capacity((v))) \
	{\
		vector_resize((v), default_resize_amount((v))); \
	}\
	for (int ___i_ = vector_count((v)) - 1; ___i_ >= (pos); --___i_) \
	{ \
		v[___i_ + 1] = v[___i_]; \
	} \
	v[pos] = (e); \
	vector_set_count((v), vector_count((v))+1); \
} while(0);

// Removes an element at a particular index from the vector
#define vector_remove(v, pos) \
do \
{  \
	log_assert((v) != VECTOR_EMPTY); \
	log_assert((pos) < vector_count((v))); \
	for (int ___i_ = (pos); ___i_ < vector_count((v)) - 1; ++___i_) \
	{ \
		(v)[___i_] = (v)[___i_ + 1]; \
	} \
	vector_set_count((v), vector_count((v))-1); \
} while(0);


// Removes the last element from the vector. Returns the value of the element
#define vector_pop(v)							\
	(((v) && (vector_count((v)) > 0)) ?					\
	 ((vector_set_count((v), vector_count((v))-1)), (v)[vector_count((v))]) : \
	 (log_fatal("Attempt to pop empty vector"), 0))


// Searches for the element by value and removes it from the vector if it exists
#define vector_find_remove(v, element)					\
do									\
{									\
	log_assert((v) != VECTOR_EMPTY);					\
	int ___found_index_ = -1;					\
	for (int ___i_ = 0; ___i_ < vector_count((v)); ++___i_)		\
	{								\
		if ((v)[___i_] == element)					\
		{								\
			___found_index_ = ___i_;				\
			break;							\
		}								\
	}								\
	vector_remove((v), ___found_index_); \
} while (0);

