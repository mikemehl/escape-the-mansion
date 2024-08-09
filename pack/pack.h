#ifndef __PACK__H__
#define __PACK__H__

#include <stdint.h>

typedef struct PackChunk {
  uint32_t entry_size; // Size of entry
  uint32_t entry_crc;  // crc of entry data (not including header)
  // data follows...
} PackChunk;

typedef struct PackIndexHeader {
  uint32_t size;        // Size of the entire index (after header)
  uint32_t num_entries; // Number of entries
  uint32_t crc32;       // crc for all pack index entries
  // pack index entries[num_entries]
  // pack chunks...
} PackIndex;

typedef struct PackIndexEntry {
  char     name[16]; // name of entry
  uint32_t offset;   // offset to entry from file start in bytes
} PackIndexEntry;

typedef struct PackHeader {
  char     id[4];   // MPF: Mike's Pack Format
  char     version; // Version
  char     year;    // Year created
  char     month;   // Month created
  char     day;     // Day created
  uint32_t size;    // Filesize
  uint32_t crc32;   // crc for entire file (after header)
  // pack index header follows....
} PackHeader;

#endif
