/**
 *  Buffer.h
 *
 *  created by Trace Baxter 07/03/18
 *  Copyright © 2018 ParkHub. All rights reserved.
*/

/**
 //
 // EXAMPLE USAGE
 //

 // Call initialize to allocate buffer's memory and get the buffer

// ***************FIRST ARGUMENT MUST BE A POWER OF 2*********************
 buffer_t buffer = rb_init(32,12);

 // Create some readings
 int16_t readingsA[12] = { -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5 };
 int16_t readingsB[12] = { -2, -5, -2, -3, -2, -2, 0, 2, 2, 2, 4, 2 };

 // Add to buffer
 rb_put(&buffer, &readingsA);
 rb_put(&buffer, &readingsB);

 // Read from buffer
 int16_t readingAPopped[12];
 int resultA = rb_get(&buffer, &readingAPopped);

 // Push a bunch of readings
 rb_put(&buffer, &readingsA);
 rb_put(&buffer, &readingsA);
 rb_put(&buffer, &readingsB);
 rb_put(&buffer, &readingsB);

 // Pop (or peek) multiple at the same time
 int16_t readingBPopped[15][12];
 int resultB = rb_getn(&buffer, &readingBPopped, 15);

 // Do something with each reading
 for (int i = 0; i < 15; i++) {
 // readingBPopped[i] ...
 }

 rb_destroy();
 */
#ifndef RING_BUFFER_H
#define RING_BUFFER_H
#endif

#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int16_t *buffer;
  size_t head;
  size_t tail;
  size_t length;
  size_t stride;
  size_t empty;
} buffer_t;

/**
 * Initializes and returns a buffer of specified length and size
 * @param length - the number of elements a buffer will hold
 * @param size - the length of each element
*/
buffer_t rb_init(size_t length, size_t size);

/**
 * Sets buffer state to empty, does not free buffer
 * @param cbuf - the buffer to be accessed
*/
void rb_reset(buffer_t *cbuf);

/**
 * Frees buffer... forever
 * @param cbuf - the buffer to be accessed
*/
void rb_destroy(buffer_t *cbuf);

/**
 * Adds data to the next available spot in the buffer, will overwrite oldest data if necessary
 * @param cbuf - the buffer to be accessed
 * @param data - the data that will be written to buffer, an array of size not to exceed buffer size
*/
void rb_put(buffer_t *cbuf, int16_t *data);

/**
 * Retrieves the oldest data in buffer
 * @param cbuf - the buffer to be accessed
 * @param data - place where data is to be written
 * returned is 0 for success and -1 for failure(empty buffer)
*/
int rb_get(buffer_t *cbuf, int16_t *data);

/**
 * Retrieve the next available n data or until buffer is empty
 * @param cbuf - the buffer to be accessed
 * @param data - place where data will be stored
 * @param n - the number of elements to grab
 * return is -1 for failed(empty buffer) 0 for success and >0 for the number of elements grabbed before emptying buffer
*/
int rb_getn(buffer_t *cbuf, int16_t *data, int n);

/**
 * peek and peekn are the same as get and getn but do not move read pointer
*/
int rb_peek(buffer_t *cbuf, int16_t *data);
int rb_peekn(buffer_t *cbuf, int16_t *data, int n);