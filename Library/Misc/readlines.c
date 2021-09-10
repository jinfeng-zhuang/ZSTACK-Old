#include <zstack/zstack.h>

#if 0
int readlines(const char *filename, struct index_buffer *ib)
{
    unsigned int file_size;
    char *file_content;
    unsigned int line_count;
    unsigned int i, j;

    file_content = (char *)file_load(filename, (uint64_t *)&file_size);
    if (NULL == file_content) {
        log_info("'%s' open failed\n", filename);
        return 0;
    }

    ib->data = (unsigned char *)file_content;

    line_count = count_char((unsigned char *)file_content, file_size, '\n');
    ib->total = line_count;

    ib->index = (unsigned int *)malloc(line_count * 4);
    if (NULL == ib->index)
        return 0;

    j = 0;

    ib->index[j++] = 0;

    for (i = 0; (i < file_size) && (j < line_count); i++) {
        if (file_content[i] == '\n') {
            ib->index[j++] = i + 1;
        }
    }

    replace_char((unsigned char *)file_content, file_size, '\r', 0);
    replace_char((unsigned char *)file_content, file_size, '\n', 0);

    return line_count;
}
#endif