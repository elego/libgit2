#include "clar_libgit2.h"
#include "fileops.h"
#include "odb.h"
#include "pack_data_one.h"
#include "pack.h"

void test_odb_indexer_odb__odb_from_index(void)
{
	git_indexer_stream *idx;
	char buffer[4096];
	git_indexer_stats stats;
	int bytes, fd;
	unsigned int i;
	git_odb *odb;

	cl_fixture_sandbox("testrepo.git");
	cl_git_pass(git_futils_mkdir_r("testrepo.git/out", NULL, 0777));
	cl_git_pass(git_indexer_stream_new(&idx, "testrepo.git/out"));

	/* Index the packfile */
	memset(&stats, 0, sizeof(stats));
	fd = p_open(cl_fixture("testrepo.git/objects/pack/pack-a81e489679b7d3418f9ab594bda8ceb37dd4c695.pack"), O_RDONLY);
	cl_assert(fd > 0);
	do {
		bytes = read(fd, buffer, sizeof(buffer));
		cl_git_pass(git_indexer_stream_add(idx, buffer, bytes, &stats));
	} while (bytes > 0);

	cl_git_pass(git_indexer_stream_finalize(idx, &stats));
	cl_git_pass(git_indexer_stream_odb(&odb, idx));

	/* The loop is from test_odb_packed_one__mass_read() */
	for (i = 0; i < ARRAY_SIZE(packed_objects_one); ++i) {
		git_oid id;
		git_odb_object *obj;

		cl_git_pass(git_oid_fromstr(&id, packed_objects_one[i]));
		cl_assert(git_odb_exists(odb, &id) == 1);
		cl_git_pass(git_odb_read(&obj, odb, &id));

		git_odb_object_free(obj);
	}

	git_odb_free(odb);
	git_indexer_stream_free(idx);
}

void test_odb_indexer_odb__cleanup(void)
{
	cl_fixture_cleanup("testrepo.git");
}
