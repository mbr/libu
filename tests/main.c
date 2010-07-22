#include <check.h>

TCase* testringbuf();
int main() {
	Suite *s = suite_create("libu_suite");
	suite_add_tcase(s, testringbuf());

	SRunner *sr = srunner_create(s);
	srunner_run_all(sr, CK_NORMAL);

	return srunner_ntests_failed(sr);
}
