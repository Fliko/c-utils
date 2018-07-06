#include "../ringBuffer.h"

int main() {
  buffer_t buf = rb_init(32,12);

  int16_t readingsA[12] = {-6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5};
  int16_t readingsB[12] = {-2, -5, -2, -3, -2, -2, 0, 2, 2, 2, 4, 2};

  rb_put(&buf, readingsA);
  rb_put(&buf, readingsB);

  int16_t *readingAPopped = (int16_t *)malloc(12 * sizeof(int16_t));
  int resultA = rb_get(&buf, readingAPopped);

  printf("A[0] %d\n", readingAPopped[0]);

  if(resultA != 0) return -1;
  if(readingAPopped[0] != readingsA[0]) return -1;

  rb_put(&buf, readingsA);
  rb_put(&buf, readingsA);
  rb_put(&buf, readingsB);
  rb_put(&buf, readingsB);

  int resultA1 = rb_peek(&buf, readingAPopped);

  printf("A1[0] %d\n", readingAPopped[0]);

  if(resultA1 != 0) return -1;
  if(readingAPopped[0] != readingsB[0]) return -1;

  int16_t *readingBPopped = (int16_t *)malloc(15 * 12 * sizeof(int16_t));
  int resultB = rb_peekn(&buf, readingBPopped, 15);
  printf("B %d\n", resultB);
  for(int i = 0; i < resultB; i++) {
    printf("%d\n", readingBPopped[i*12]);
  }

  if (resultB != 5) return -1;

  int resultC = rb_getn(&buf, readingBPopped, 15);
  printf("C %d\n", resultC);
  for(int i = 0; i < resultC; i++) {
    printf("%d\n", readingBPopped[i*12]);
  }

  if (resultC != 5) return -1;
  rb_destroy(&buf);
  free(readingAPopped);
  free(readingBPopped);
}