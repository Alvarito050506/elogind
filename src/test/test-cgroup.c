/* SPDX-License-Identifier: LGPL-2.1+ */

#include <unistd.h>

#include "cgroup-setup.h"
#include "cgroup-util.h"
#include "path-util.h"
#include "process-util.h"
#include "string-util.h"
#include "tests.h"

static void test_cg_split_spec(void) {
        char *c, *p;

        log_info("/* %s */", __func__);

        assert_se(cg_split_spec("foobar:/", &c, &p) == 0);
        assert_se(streq(c, "foobar"));
        assert_se(streq(p, "/"));
        c = mfree(c);
        p = mfree(p);

        assert_se(cg_split_spec("foobar:", &c, &p) == 0);
        c = mfree(c);
        p = mfree(p);

        assert_se(cg_split_spec("foobar:asdfd", &c, &p) < 0);
        assert_se(cg_split_spec(":///", &c, &p) < 0);
        assert_se(cg_split_spec(":", &c, &p) < 0);
        assert_se(cg_split_spec("", &c, &p) < 0);
        assert_se(cg_split_spec("fo/obar:/", &c, &p) < 0);

        assert_se(cg_split_spec("/", &c, &p) >= 0);
        assert_se(c == NULL);
        assert_se(streq(p, "/"));
        p = mfree(p);

        assert_se(cg_split_spec("foo", &c, &p) >= 0);
        assert_se(streq(c, "foo"));
        assert_se(p == NULL);
        c = mfree(c);
}

static void test_cg_create(void) {
        char *path;

        log_info("/* %s */", __func__);

        assert_se(cg_create(SYSTEMD_CGROUP_CONTROLLER, "/test-a") == 1);
        assert_se(cg_create(SYSTEMD_CGROUP_CONTROLLER, "/test-a") == 0);
        assert_se(cg_create(SYSTEMD_CGROUP_CONTROLLER, "/test-b") == 1);
        assert_se(cg_create(SYSTEMD_CGROUP_CONTROLLER, "/test-b/test-c") == 1);
        assert_se(cg_create_and_attach(SYSTEMD_CGROUP_CONTROLLER, "/test-b", 0) == 0);

        assert_se(cg_pid_get_path(SYSTEMD_CGROUP_CONTROLLER, getpid_cached(), &path) == 0);
        assert_se(streq(path, "/test-b"));
        free(path);

        assert_se(cg_attach(SYSTEMD_CGROUP_CONTROLLER, "/test-a", 0) == 0);

        assert_se(cg_pid_get_path(SYSTEMD_CGROUP_CONTROLLER, getpid_cached(), &path) == 0);
        assert_se(path_equal(path, "/test-a"));
        free(path);

        assert_se(cg_create_and_attach(SYSTEMD_CGROUP_CONTROLLER, "/test-b/test-d", 0) == 1);

        assert_se(cg_pid_get_path(SYSTEMD_CGROUP_CONTROLLER, getpid_cached(), &path) == 0);
        assert_se(path_equal(path, "/test-b/test-d"));
        free(path);

        assert_se(cg_get_path(SYSTEMD_CGROUP_CONTROLLER, "/test-b/test-d", NULL, &path) == 0);
#if 0 /// elogind uses its own name
        assert_se(path_equal(path, "/sys/fs/cgroup/systemd/test-b/test-d"));
#else // 0
        assert_se(path_equal(path, "/sys/fs/cgroup/elogind/test-b/test-d"));
#endif // 0
        free(path);

        assert_se(cg_is_empty(SYSTEMD_CGROUP_CONTROLLER, "/test-a") > 0);
        assert_se(cg_is_empty(SYSTEMD_CGROUP_CONTROLLER, "/test-b") > 0);
        assert_se(cg_is_empty_recursive(SYSTEMD_CGROUP_CONTROLLER, "/test-a") > 0);
        assert_se(cg_is_empty_recursive(SYSTEMD_CGROUP_CONTROLLER, "/test-b") == 0);

        assert_se(cg_kill_recursive(SYSTEMD_CGROUP_CONTROLLER, "/test-a", 0, 0, NULL, NULL, NULL) == 0);
        assert_se(cg_kill_recursive(SYSTEMD_CGROUP_CONTROLLER, "/test-b", 0, 0, NULL, NULL, NULL) > 0);

        assert_se(cg_migrate_recursive(SYSTEMD_CGROUP_CONTROLLER, "/test-b", SYSTEMD_CGROUP_CONTROLLER, "/test-a", 0) > 0);

        assert_se(cg_is_empty_recursive(SYSTEMD_CGROUP_CONTROLLER, "/test-a") == 0);
        assert_se(cg_is_empty_recursive(SYSTEMD_CGROUP_CONTROLLER, "/test-b") > 0);

        assert_se(cg_kill_recursive(SYSTEMD_CGROUP_CONTROLLER, "/test-a", 0, 0, NULL, NULL, NULL) > 0);
        assert_se(cg_kill_recursive(SYSTEMD_CGROUP_CONTROLLER, "/test-b", 0, 0, NULL, NULL, NULL) == 0);

        cg_trim(SYSTEMD_CGROUP_CONTROLLER, "/", false);

        assert_se(cg_rmdir(SYSTEMD_CGROUP_CONTROLLER, "/test-b") == 0);
        assert_se(cg_rmdir(SYSTEMD_CGROUP_CONTROLLER, "/test-a") < 0);
        assert_se(cg_migrate_recursive(SYSTEMD_CGROUP_CONTROLLER, "/test-a", SYSTEMD_CGROUP_CONTROLLER, "system.slice", 0) > 0);
        assert_se(cg_rmdir(SYSTEMD_CGROUP_CONTROLLER, "/test-a") == 0);
}

int main(int argc, char *argv[]) {
        test_setup_logging(LOG_DEBUG);

        test_cg_split_spec();
        test_cg_create();

        return 0;
}
