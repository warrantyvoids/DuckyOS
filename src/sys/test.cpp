#include "test.h"

void test(int x);
void test(double y);

void test_cpp() {
  test(3);
  test(4.0);
}

void test(int x) {
  terminal_writestring("TEST (int)\n");
}

void test(double d) {
  terminal_writestring("TEST (double)\n");
}