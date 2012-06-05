#include "clar_libgit2.h"
#include "odb.h"
#include "git2/odb_backend.h"
#include "pack.h"

static git_odb *_odb;

void test_odb_foreach__initialize(void)
{
	git_odb_backend *backend = NULL;

	cl_git_pass(git_odb_new(&_odb));
	cl_git_pass(git_odb_backend_one_pack(&backend, cl_fixture("testrepo.git/objects/pack/pack-a81e489679b7d3418f9ab594bda8ceb37dd4c695.idx")));
	cl_git_pass(git_odb_add_backend(_odb, backend, 1));
}

void test_odb_foreach__cleanup(void)
{
	git_odb_free(_odb);
}

static int foreach_cb(git_oid *oid, void *data)
{
	char str[GIT_OID_HEXSZ+1] = {0};
	GIT_UNUSED(data);

	git_oid_fmt(str, oid);
	puts(str);
	return 0;
}

void test_odb_foreach__foreach(void)
{
	git_odb_foreach(_odb, foreach_cb, NULL);
}
