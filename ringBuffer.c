/**
 * Buffer.c
 *  created by Trace Baxter 07/03/18
 *  Copyright © 2018 ParkHub. All rights reserved.
*/
#include "ringBuffer.h"

// Initialize buffer
// ***************LENGTH MUST BE A POWER OF 2*********************
buffer_t rb_init(size_t length, size_t stride) {
  buffer_t cbuf;
  cbuf.stride = stride;
  cbuf.length = length;
  cbuf.buffer = (int16_t *)malloc(length * stride * sizeof(int16_t));
  rb_reset(&cbuf);
  return cbuf;
}

// reset buffer
void rb_reset(buffer_t *cbuf) {
  cbuf->head = 0;
  cbuf->tail = 0;
  cbuf->empty = 1;
}

// Free allocated memory of buffer
void rb_destroy(buffer_t *cbuf) {
  free(cbuf->buffer);
}

// Add data and advance head pointer
void rb_put(buffer_t *cbuf, int16_t *data) {
  if (cbuf->empty) cbuf->empty = 0;
  else if (cbuf->tail == cbuf->head) {
    cbuf->tail = (cbuf->tail + 1) & (cbuf->length - 1);
  }

  int index = cbuf->head * cbuf->stride;
  for (int i = 0; i < cbuf->stride; i++) {
    cbuf->buffer[index + i] = data[i];
  }

  cbuf->head = (cbuf->head + 1) & (cbuf->length - 1);
}

// Assign data to given variable and advance tail pointer
int rb_get(buffer_t *cbuf, int16_t *data) {
  if (cbuf->empty) return -1;

  int index = cbuf->tail * cbuf->stride;
  for (int i = 0; i < cbuf->stride; i++) {
    data[i] = cbuf->buffer[index + i];
  }

  if (cbuf->tail == cbuf->head)
    rb_reset(cbuf);
  else
    cbuf->tail = (cbuf->tail + 1) & (cbuf->length - 1);

  return 0;
}

// Get the next n values or as many as you can, if less than n are left return the number fetched
int rb_getn(buffer_t *cbuf, int16_t *data, int n) {
  if (cbuf->empty) return -1;

  if (n > 0) {
    for (int i = 0; i < n; i++) {
      int res = rb_get(cbuf, &data[i * cbuf->stride]);

      // If you have emptied the buffer return with the number of elements retrieved
      if (res == -1)
        return i - 1;
    }
  }
  return 0;
}

// Look at the next element but do not move head
int rb_peek(buffer_t *cbuf, int16_t *data) {
  if (cbuf->empty) return -1;

  int index = cbuf->tail * cbuf->stride;
  for (int i = 0; i < cbuf->stride; i++) {
    data[i] = cbuf->buffer[index + i];
  }
  return 0;
}

// Look n elements in the future, if rerturn is above 0 that is the number of elements peeked
int rb_peekn(buffer_t *cbuf, int16_t *data, int n){
  if (cbuf->empty) return -1;

  if (n > 0) {
    for (int i = 0; i < n; i++) {
      int index = (cbuf->tail + i) * cbuf->stride;

      for (int j = 0; j < cbuf->stride; j++) {
        data[(cbuf->stride * i) + j] = cbuf->buffer[index + j];
      }

      if((cbuf->tail + i) == cbuf->head) return i;
    }
  }

  return 0;
}
