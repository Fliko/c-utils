/**
 * Buffer.c
 *  created by Trace Baxter 07/03/18
 *  Copyright © 2018 ParkHub. All rights reserved.
*/
#include "Buffer.h"

// Initialize buffer
buffer_t rb_init(size_t length, size_t size) {
  buffer_t cbuf;
  cbuf.size = size;
  cbuf.length = length;
  cbuf.buffer = (int16_t **)malloc(length * size * sizeof(int16_t));
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
    cbuf->tail = (cbuf->tail + 1) % cbuf->length;
  }

  cbuf->buffer[cbuf->head] = data;
  cbuf->head = (cbuf->head + 1) % cbuf->length;
}

// Assign data to given variable and advance tail pointer
int rb_get(buffer_t *cbuf, int16_t *data) {
  if (cbuf->empty) return -1;

  *data = *cbuf->buffer[cbuf->tail];

  if (cbuf->tail == cbuf->head)
    rb_reset(cbuf);
  else
    cbuf->tail = (cbuf->tail + 1) % cbuf->length;

  return 0;
}

// Get the next n values or as many as you can, if less than n are left return the number fetched
int rb_getn(buffer_t *cbuf, int16_t **data, int n) {
  if (cbuf->empty) return -1;

  if (n > 0) {
    for (int i = 0; i < n; i++) {
      int res = rb_get(cbuf, &data[i]);

      printf("%d, %d, %s\n", cbuf->head, cbuf->tail, data[i]);
      if (res == -1)
        return i;
    }
  }
  return 0;
}

// Look at the next element but do not move head
int rb_peek(buffer_t *cbuf, int16_t *data) {
  if (cbuf->empty) return -1;

  data = cbuf->buffer[cbuf->tail];
  return 0;
}

// Look n elements in the future, if rerturn is above 0 that is the number of elements peeked
int rb_peekn(buffer_t *cbuf, int16_t **data, int n){
  if (cbuf->empty) return -1;

  if (n > 0) {
    for (int i = 0; i < n; i++) {
      int storageIndex = (cbuf->tail + i) % cbuf->length;
      data[i] = cbuf->buffer[storageIndex];

      if(storageIndex == cbuf->head) return i;
    }
  }

  return 0;
}
