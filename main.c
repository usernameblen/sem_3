#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

int main(int argc, char* argv[])
{
	FILE* file = fopen(argv[1], "wb");
	if (file == NULL)
	{
		printf("file is not open :)lol");
		return -1;
	}

	unsigned char buf[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
	unsigned char buf_for_read[4] = { 0 };
	size_t c = fwrite(buf, sizeof(buf[0]), sizeof(buf) / sizeof(buf[0]), file);

	if (c != sizeof(buf) / sizeof(buf[0]))
	{
		printf("not all objects were recorded");
		return 1;
	}
	fclose(file);

	file = fopen(argv[1], "rb");
	if (file == NULL)
	{
		printf("file is not open :)lol");
		return -1;
	}

	unsigned char byte_buf;

	while (fread(&byte_buf, sizeof(byte_buf), 1, file))
	{
		printf("Byte: %u\n", byte_buf);
		printf("file base (buffer begin): %p\n", file->_base); //
		printf("file buf size: %d\n", file->_bufsiz);
		printf("file char buf: %d\n", file->_charbuf);
		printf("file cnt (колво ост байтов в буффере): %d\n", file->_cnt);
		printf("file file (description): %d\n", file->_file);
		printf("file flag: %d\n", file->_flag);
		printf("file ptr (now position buffer): %p\n", file->_ptr);
		printf("file tmpf name: %p\n", file->_tmpfname);
	}

	fclose(file);

	file = fopen(argv[1], "rb");
	if (file == NULL)
	{
		printf("file is not open :)lol");
		return -1;
	}

	fseek(file, 3, SEEK_SET); //0(SEEK_SET) - begin, 1 - current, 2 - end

	fread(buf_for_read, sizeof(unsigned char), 4, file);

	fclose(file);

	printf("The buffer: ");
	for (int i = 0; i < 4; ++i)
	{
		printf("%u", buf_for_read[i]);
	}

	return 0;
}