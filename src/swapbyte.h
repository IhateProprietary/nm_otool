#ifndef SWAP_BYTE_H
# define SWAP_BYTE_H

# define SWAP16(x) ((((x) & 0xff00) >> 8) | (((x) & 0xff) << 8))
# define SWAP32(x) ((SWAP16(x) << 16) | (SWAP16((x) >> 16)))
# define SWAP64(x) ((SWAP32(x) << 32) | (SWAP32((x) >> 32)))

#endif
