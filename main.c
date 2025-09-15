#include<stdio.h>

int main(int argc, char* argv[])
{
    unsigned char data[BUFSIZ];
    size_t bytes_read = 0;

    if (argc != 3) {
        printf("Not enough arguments\n");
    }

    FILE* fin = fopen(argv[1], "rb");
    if (fin == NULL) {
        printf("File is not open\n");
        return 1;
    }

    FILE* fout = fopen(argv[2], "wb");
    if (fin == NULL) {
        printf("File is not open\n");
        fclose(fin);
        return 1;
    }

    do {
        bytes_read = fread(data, 1, BUFSIZ, fin);
        fwrite(data, 1, bytes_read, fout);
    } while (bytes_read != 0);

    fclose(fin);
    fclose(fout);

	return 0;
}