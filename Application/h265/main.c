#include <zstack.h>

struct nalu {
    unsigned int prefix;
    unsigned int offset;
    unsigned char* buffer;
    unsigned int length;
    unsigned int type;
};

#define NALU_START_CODE_SIZE	4
#define NALU_TYPE_MASK	        0x1F

struct nalu* nalu_parse_all(unsigned char* buffer, unsigned int length, unsigned int *count)
{
    unsigned int i;
    bool nalu_start_flag;
    unsigned int nalu_start_offset;
    unsigned int nalu_index;
    unsigned char nalu_header;
    unsigned int nalu_count = 0;

    // Step 1: count of nalu
    for (i = 0; i < length - NALU_START_CODE_SIZE; i++) {
        if ((0 == buffer[i]) && (0 == buffer[i + 1]) && (0 == buffer[i + 2]) && (1 == buffer[i + 3])) {
            nalu_start_flag = true;
            nalu_start_offset = i;
            i += 4;
        }
        else if ((0 == buffer[i]) && (0 == buffer[i + 1]) && (1 == buffer[i + 2])) {
            nalu_start_flag = true;
            nalu_start_offset = i;
            i += 3;
        }
        else {
            nalu_start_flag = false;
            nalu_start_offset = 0;
        }

        if (nalu_start_flag)
            nalu_count++;
    }

    // Step 2: generate nalu_array
    struct nalu* nalu_array = (struct nalu*)malloc(nalu_count * sizeof(struct nalu));
    if (NULL == nalu_array) {
        goto END;
    }

    memset(nalu_array, 0, nalu_count * sizeof(struct nalu));

    // Step 3: fill nalu_array
    for (i = 0, nalu_index = 0; i < length - NALU_START_CODE_SIZE; i++) {
        if ((0 == buffer[i]) && (0 == buffer[i + 1]) && (0 == buffer[i + 2]) && (1 == buffer[i + 3])) {
            nalu_array[nalu_index].prefix = 4;
            nalu_array[nalu_index].offset = i + 4;
            nalu_array[nalu_index].buffer = &buffer[i];
            nalu_index++;
            i += 4;
        }
        else if ((0 == buffer[i]) && (0 == buffer[i + 1]) && (1 == buffer[i + 2])) {
            nalu_array[nalu_index].prefix = 3;
            nalu_array[nalu_index].offset = i + 3;
            nalu_array[nalu_index].buffer = &buffer[i];
            nalu_index++;
            i += 3;
        }
        else {
        }
    }

    for (i = 0; i < nalu_count - 1; i++) {
        nalu_array[i].length = nalu_array[i + 1].buffer - nalu_array[i].buffer;
    }
    nalu_array[i].length = &buffer[length] - nalu_array[i].buffer;

    for (i = 0; i < nalu_count; i++) {
        nalu_header = nalu_array[i].buffer[nalu_array[i].prefix];
        //nalu_array[i].type = (enum nal_unit_type)(nalu_header & NALU_TYPE_MASK); // H264
        nalu_array[i].type = (enum nal_unit_type)((nalu_header & 0x7F) >> 1); // H265
    }

END:
    *count = nalu_count;
    return nalu_array;
}

int main(int argc, char *argv[])
{
    unsigned char *content;
    unsigned int file_size;
    unsigned int nalu_count;
    unsigned int i;
    char *filename;
    struct nalu* nalu_array;

    filename = argv[1];

    content = file_load(filename, &file_size);
    if (NULL == content)
        return -1;

    nalu_array = nalu_parse_all(content, file_size, &nalu_count);
    if (NULL == nalu_array)
        return -1;

    for (i = 0; i < nalu_count; i++) {
        printf("nalu_array[%d].type = %d\n", i, nalu_array[i].type);
        //if (nalu_array[i].type == 35) {
        //    printf("nalu_array[%d].type = %d\n", i, nalu_array[i].type);
        //}
    }

    return 0;
}