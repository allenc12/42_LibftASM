#include <string.h>

// Cmocka needs these
// clang-format off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <cmocka.h>
// clang-format on

extern void ft_bzero(void *b, size_t len);

#define N 1024

static char buf1[N];
static char buf2[N];

static char *aligned(void *p) { return (char *)(((intptr_t)p + 63) & -64); }

static void test_align(int align, size_t len) {
  char *s = aligned(buf1 + 64) + align;
  char *want = aligned(buf2 + 64) + align;
  char *p __unused;
  int i;

  assert_false(len + 64 > (size_t)(buf1 + N - s));
  assert_false(len + 64 > (size_t)(buf2 + N - want));

  for (i = 0; i < N; ++i)
    buf1[i] = buf2[i] = ' ';
  for (i = 0; i < (int)len; ++i)
    want[i] = '\0';
  ft_bzero(s, len);
  for (i = -64; i < (int)len + 64; ++i)
    assert_int_equal(s[i], want[i]);
}

static void bzero_aligned_test(void **state __unused) {
  for (int i = 0; i < 32; ++i)
    for (size_t j = 0; j < 512; ++j)
      test_align(i, j);
}

static void check_input_(int c, void **state __unused) {
  ft_bzero(buf1, 10);
  for (int i = 0; i < 10; ++i) {
    assert_int_equal(buf1[i], c);
  }
}

static void check_input(void **state) { check_input_(0, state); }

int bzero_tests(void) {
  const struct CMUnitTest bzero_tests[] = {
      cmocka_unit_test(bzero_aligned_test),
      cmocka_unit_test(check_input),
  };

  return cmocka_run_group_tests(bzero_tests, NULL, NULL);
}

#ifdef SINGLE_TEST
int test_main(void) { return bzero_tests(); }
#endif
