#include <stdlib.h>
#include <stdio.h>
#include <check.h>
#include <struct/ringbuf.h>

/* should be at least 3 */
static const int ringbuf_size = 128;

static ringbuf_t *rb;
static char *data;
static char *pattern;

static int overflow_callback_called;
static int underflow_callback_called;

static void overflow_callback() {
	fail_if(overflow_callback_called); /* if called wasn't reset, we made an error in the test code */
	overflow_callback_called = 1;
}

static void underflow_callback() {
	fail_if(underflow_callback_called);
	underflow_callback_called = 1;
}

void setup() {
	// ringbuf:
	rb = (ringbuf_t*) malloc(sizeof(ringbuf_t));	
	
	// data area:
	data = (char*) malloc(ringbuf_size);
	ringbuf_init(rb, data, ringbuf_size, NULL, NULL);

	// some data
	pattern = (char*) malloc(2*ringbuf_size);
	for(int i = 0; i < 2*ringbuf_size; ++i) {
		pattern[i] = (char) (i%255);
	}

	overflow_callback_called = 0;
	underflow_callback_called = 0;
}

void teardown() {
	free(data);	
	free(rb);
	free(pattern);
}

START_TEST(count_init) {
	fail_unless(0 == rb->wcount);
	fail_unless(0 == rb->rcount);
}
END_TEST

START_TEST(put_one_get_one)
{
	ringbuf_putc(rb, 'a');
	fail_unless('a' == ringbuf_getc(rb));
}
END_TEST

START_TEST(put_many_get_many)
{
	for(int i = 0; i != _i; ++i) {
		ringbuf_putc(rb, pattern[i]);
		fail_unless(i+1 == rb->wcount);
		fail_unless(0 == rb->rcount);
	}

	for(int i = 0; i != _i; ++i) {
		fail_unless(ringbuf_getc(rb) == pattern[i]);
		fail_unless(_i == rb->wcount);
		fail_unless(i+1 == rb->rcount);
	}
}
END_TEST

START_TEST(mixed_read_write)
{
	int w = 0;
	int r = 0;

	for(int i = 0; i < ringbuf_size/4; ++i) {
		ringbuf_putc(rb, pattern[w++]);
		fail_unless(rb->wcount == w);
		fail_unless(rb->rcount == r);
	}

	for(int i = 0; i < ringbuf_size/4; ++i) {
		fail_unless(ringbuf_getc(rb) == pattern[r++]);
		ringbuf_putc(rb, pattern[w++]);
		fail_unless(rb->wcount == w);
		fail_unless(rb->rcount == r);
	}

	for(int i = 0; i < ringbuf_size/4; ++i) {
		fail_unless(ringbuf_getc(rb) == pattern[r++]);
		fail_unless(rb->wcount == w);
		fail_unless(rb->rcount == r);
	}
}
END_TEST

START_TEST(overflow_callback_gets_called) {
	int i;
	ringbuf_set_overflow_callback(rb, overflow_callback);

	fail_if(overflow_callback_called);
	for(i = 0; i < ringbuf_size; ++i) {
		ringbuf_putc(rb, pattern[i]);
	}

	/* ringbuf is now full */
	fail_if(overflow_callback_called);
	ringbuf_putc(rb, pattern[++i]);
	fail_unless(overflow_callback_called);
}
END_TEST

START_TEST(underflow_callback_gets_called) {
	fail_if(underflow_callback_called);
	ringbuf_set_underflow_callback(rb, underflow_callback);
	ringbuf_getc(rb);
	fail_unless(underflow_callback_called);
}
END_TEST

START_TEST(counter_overflow) {
	/* this may be fragile, as its modifying the internal state directly. */
	rb->rcount = UINT16_MAX-2;	
	rb->wcount = UINT16_MAX-2;	

	ringbuf_putc(rb, 'a');
	ringbuf_putc(rb, 'a');

	fail_unless(ringbuf_size - 2 == ringbuf_available(rb));
	fail_unless(rb->wcount == UINT16_MAX);

	/* now we add one more, capacity should decrease by one */
	ringbuf_putc(rb, 'a');
	fail_unless(ringbuf_size - 3 == ringbuf_available(rb));

	ringbuf_getc(rb);
	ringbuf_getc(rb);
	ringbuf_getc(rb);
	fail_unless(ringbuf_size == ringbuf_available(rb));
}
END_TEST

START_TEST(availability) {
	fail_unless(ringbuf_size == ringbuf_available(rb));
	ringbuf_putc(rb, 'a');
	ringbuf_getc(rb);
	fail_unless(ringbuf_size == ringbuf_available(rb));

	/* fill almost to capacity */
	for(int i = 0; i != ringbuf_size; ++i) {
		ringbuf_putc(rb, pattern[i]);
		fail_unless(ringbuf_size-i-1 == ringbuf_available(rb));
	}

	for(int i = 0; i != ringbuf_size; ++i) {
		ringbuf_getc(rb);
		fail_unless(i+1 == ringbuf_available(rb));
	}

	fail_unless(ringbuf_size == ringbuf_available(rb));
}
END_TEST

TCase *testringbuf() {
	TCase *tc = tcase_create("testringbuf");
	tcase_add_test(tc, count_init);
	tcase_add_checked_fixture(tc, setup, teardown);
	tcase_add_test(tc, put_one_get_one);
	tcase_add_test(tc, mixed_read_write);
	tcase_add_test(tc, overflow_callback_gets_called);
	tcase_add_test(tc, underflow_callback_gets_called);
	tcase_add_test(tc, counter_overflow);
	tcase_add_test(tc, availability);

	/* try with 1-ringbuf_size elements in buffer */
	tcase_add_loop_test(tc, put_many_get_many, 1, ringbuf_size+1);

	return tc;
}
