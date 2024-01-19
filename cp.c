/* copy.c
 *
 * Compile with:
 * 
 * gcc copy.c -o copy -Wall -g
 *
 * Defines function copy:
 *
 * Copy a source file to a destination file. If the destination file already
 * exists, this clobbers it. If the destination file does not exist, it is
 * created. 
 *
 * Uses a buffer in user-space, so may not perform as well as 
 * copy_file_range, which copies in kernel-space.
 *
 */

#include <stdlib.h>
#include <stdio.h>

#define BUF_SIZE 65536 //2^16

int copy(const char *in_path, const char *out_path)
{
	size_t n;
	FILE *in = NULL, *out = NULL;
	char *buf = calloc(BUF_SIZE, 1);
	if ((in = fopen(in_path, "rb")) && (out = fopen(out_path, "wb")))
		while ((n = fread(buf, 1, BUF_SIZE, in)) &&
		       fwrite(buf, 1, n, out))
			;
	free(buf);
	if (in)
		fclose(in);
	if (out)
		fclose(out);
	return EXIT_SUCCESS;
}

/* Test it out.
 *
 * BASH:
 *
 * gcc copy.c -o copy -Wall -g
 * echo 'Hello, world!' > src.txt
 * ./copy src.txt dst.txt
 * [ -z "$(diff src.txt dst.txt)" ]
 *
 */
int main(int argc, char *argv[argc])
{
	if (argc != 3) {
		printf("Usage: %s <SOURCE> <DESTINATION>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	return copy(argv[1], argv[2]);
}
