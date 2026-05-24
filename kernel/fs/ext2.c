#include "fs.h"

static uint32_t rd32(const uint8_t* p) {
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}
static uint16_t rd16(const uint8_t* p) {
    return (uint16_t)p[0] | ((uint16_t)p[1] << 8);
}

fs_status_t ext2_parse_superblock(const uint8_t* sb, ext2_info_t* out) {
    if (!sb || !out) return FS_ERR;
    out->inodes_count = rd32(sb + 0x00);
    out->blocks_count = rd32(sb + 0x04);
    out->log_block_size = rd32(sb + 0x18);
    out->magic = rd16(sb + 0x38);
    return (out->magic == 0xEF53) ? FS_OK : FS_ERR;
}

fs_status_t ext2_probe(void) {
    return FS_ERR;
}
