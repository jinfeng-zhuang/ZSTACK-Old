#include <zstack/bitstream.h>

int is_byte_aligned(struct bitstream *bitstream)
{
    return (0 == (bitstream->bitpos & 0x7));
}
