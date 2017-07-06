#ifndef PUFFS_GIF_H
#define PUFFS_GIF_H

// Code generated by puffs-gen-c. DO NOT EDIT.

#ifndef PUFFS_BASE_HEADER_H
#define PUFFS_BASE_HEADER_H

// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file.

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

// Puffs requires a word size of at least 32 bits because it assumes that
// converting a u32 to usize will never overflow. For example, the size of a
// decoded image is often represented, explicitly or implicitly in an image
// file, as a u32, and it is convenient to compare that to a buffer size.
//
// Similarly, the word size is at most 64 bits because it assumes that
// converting a usize to u64 will never overflow.
#if __WORDSIZE < 32
#error "Puffs requires a word size of at least 32 bits"
#elif __WORDSIZE > 64
#error "Puffs requires a word size of at most 64 bits"
#endif

// PUFFS_VERSION is the major.minor version number as a uint32. The major
// number is the high 16 bits. The minor number is the low 16 bits.
//
// The intention is to bump the version number at least on every API / ABI
// backwards incompatible change.
//
// For now, the API and ABI are simply unstable and can change at any time.
//
// TODO: don't hard code this in base-header.h.
#define PUFFS_VERSION (0x00001)

// puffs_base_buf1 is a 1-dimensional buffer (a pointer and length) plus
// additional indexes into that buffer.
//
// A value with all fields NULL or zero is a valid, empty buffer.
typedef struct {
  uint8_t* ptr;  // Pointer.
  size_t len;    // Length.
  size_t wi;     // Write index. Invariant: wi <= len.
  size_t ri;     // Read  index. Invariant: ri <= wi.
  bool closed;   // No further writes are expected.
} puffs_base_buf1;

// puffs_base_limit1 provides a limited view of a 1-dimensional byte stream:
// its first N bytes. That N can be greater than a buffer's current read or
// write capacity. N decreases naturally over time as bytes are read from or
// written to the stream.
//
// A value with all fields NULL or zero is a valid, unlimited view.
typedef struct puffs_base_limit1 {
  uint64_t* ptr_to_len;            // Pointer to N.
  struct puffs_base_limit1* next;  // Linked list of limits.
} puffs_base_limit1;

typedef struct {
  puffs_base_buf1* buf;
  puffs_base_limit1 limit;
} puffs_base_reader1;

typedef struct {
  puffs_base_buf1* buf;
  puffs_base_limit1 limit;
} puffs_base_writer1;

#endif  // PUFFS_BASE_HEADER_H

#ifdef __cplusplus
extern "C" {
#endif

// ---------------- Status Codes

// Status codes are int32_t values:
//  - the sign bit indicates a non-recoverable status code: an error
//  - bits 10-30 hold the packageid: a namespace
//  - bits 8-9 are reserved
//  - bits 0-7 are a package-namespaced numeric code
//
// Do not manipulate these bits directly. Use the API functions such as
// puffs_gif_status_is_error instead.
typedef int32_t puffs_gif_status;

#define puffs_gif_packageid 1017222  // 0x000f8586

#define PUFFS_GIF_STATUS_OK 0                               // 0x00000000
#define PUFFS_GIF_ERROR_BAD_VERSION -2147483647             // 0x80000001
#define PUFFS_GIF_ERROR_BAD_RECEIVER -2147483646            // 0x80000002
#define PUFFS_GIF_ERROR_BAD_ARGUMENT -2147483645            // 0x80000003
#define PUFFS_GIF_ERROR_CONSTRUCTOR_NOT_CALLED -2147483644  // 0x80000004
#define PUFFS_GIF_ERROR_UNEXPECTED_EOF -2147483643          // 0x80000005
#define PUFFS_GIF_STATUS_SHORT_READ 6                       // 0x00000006
#define PUFFS_GIF_STATUS_SHORT_WRITE 7                      // 0x00000007
#define PUFFS_GIF_ERROR_CLOSED_FOR_WRITES -2147483640       // 0x80000008

#define PUFFS_GIF_ERROR_BAD_GIF_BLOCK -1105848320            // 0xbe161800
#define PUFFS_GIF_ERROR_BAD_GIF_EXTENSION_LABEL -1105848319  // 0xbe161801
#define PUFFS_GIF_ERROR_BAD_GIF_HEADER -1105848318           // 0xbe161802
#define PUFFS_GIF_ERROR_BAD_LZW_LITERAL_WIDTH -1105848317    // 0xbe161803
#define PUFFS_GIF_ERROR_TODO_UNSUPPORTED_LOCAL_COLOR_TABLE \
  -1105848316                                                     // 0xbe161804
#define PUFFS_GIF_ERROR_LZW_CODE_IS_OUT_OF_RANGE -1105848315      // 0xbe161805
#define PUFFS_GIF_ERROR_LZW_PREFIX_CHAIN_IS_CYCLICAL -1105848314  // 0xbe161806

bool puffs_gif_status_is_error(puffs_gif_status s);

const char* puffs_gif_status_string(puffs_gif_status s);

// ---------------- Structs

typedef struct {
  // Do not access the private_impl's fields directly. There is no API/ABI
  // compatibility or safety guarantee if you do so. Instead, use the
  // puffs_gif_lzw_decoder_etc functions.
  //
  // In C++, these fields would be "private", but C does not support that.
  //
  // It is a struct, not a struct*, so that it can be stack allocated.
  struct {
    puffs_gif_status status;
    uint32_t magic;
    uint32_t f_literal_width;
    uint8_t f_stack[4096];
    uint8_t f_suffixes[4096];
    uint16_t f_prefixes[4096];

    struct {
      uint32_t coro_state;
      uint32_t v_clear_code;
      uint32_t v_end_code;
      uint32_t v_save_code;
      uint32_t v_prev_code;
      uint32_t v_width;
      uint32_t v_bits;
      uint32_t v_n_bits;
      uint32_t v_code;
      uint32_t v_s;
      uint32_t v_c;
    } c_decode[1];
  } private_impl;
} puffs_gif_lzw_decoder;

typedef struct {
  // Do not access the private_impl's fields directly. There is no API/ABI
  // compatibility or safety guarantee if you do so. Instead, use the
  // puffs_gif_decoder_etc functions.
  //
  // In C++, these fields would be "private", but C does not support that.
  //
  // It is a struct, not a struct*, so that it can be stack allocated.
  struct {
    puffs_gif_status status;
    uint32_t magic;
    uint32_t f_width;
    uint32_t f_height;
    uint8_t f_background_color_index;
    uint8_t f_gct[768];
    puffs_gif_lzw_decoder f_lzw;

    struct {
      uint32_t coro_state;
      uint8_t v_c;
    } c_decode[1];
    struct {
      uint32_t coro_state;
      uint8_t v_c[6];
      uint32_t v_i;
    } c_decode_header[1];
    struct {
      uint32_t coro_state;
      uint8_t v_c[7];
      uint32_t v_i;
      uint32_t v_gct_size;
    } c_decode_lsd[1];
    struct {
      uint32_t coro_state;
      uint8_t v_label;
      uint8_t v_block_size;
    } c_decode_extension[1];
    struct {
      uint32_t coro_state;
      uint8_t v_c[9];
      uint32_t v_i;
      bool v_interlace;
      uint8_t v_lw;
      uint8_t v_block_size;
      uint64_t l_lzw_src;
      puffs_base_reader1 v_lzw_src;
    } c_decode_id[1];
  } private_impl;
} puffs_gif_decoder;

// ---------------- Public Constructor and Destructor Prototypes

// puffs_gif_lzw_decoder_constructor is a constructor function.
//
// It should be called before any other puffs_gif_lzw_decoder_* function.
//
// Pass PUFFS_VERSION and 0 for puffs_version and for_internal_use_only.
void puffs_gif_lzw_decoder_constructor(puffs_gif_lzw_decoder* self,
                                       uint32_t puffs_version,
                                       uint32_t for_internal_use_only);

void puffs_gif_lzw_decoder_destructor(puffs_gif_lzw_decoder* self);

// puffs_gif_decoder_constructor is a constructor function.
//
// It should be called before any other puffs_gif_decoder_* function.
//
// Pass PUFFS_VERSION and 0 for puffs_version and for_internal_use_only.
void puffs_gif_decoder_constructor(puffs_gif_decoder* self,
                                   uint32_t puffs_version,
                                   uint32_t for_internal_use_only);

void puffs_gif_decoder_destructor(puffs_gif_decoder* self);

// ---------------- Public Function Prototypes

puffs_gif_status puffs_gif_decoder_decode(puffs_gif_decoder* self,
                                          puffs_base_writer1 a_dst,
                                          puffs_base_reader1 a_src);

void puffs_gif_lzw_decoder_set_literal_width(puffs_gif_lzw_decoder* self,
                                             uint32_t a_lw);

puffs_gif_status puffs_gif_lzw_decoder_decode(puffs_gif_lzw_decoder* self,
                                              puffs_base_writer1 a_dst,
                                              puffs_base_reader1 a_src);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // PUFFS_GIF_H

// C HEADER ENDS HERE.

#ifndef PUFFS_BASE_IMPL_H
#define PUFFS_BASE_IMPL_H

// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file.

// Use switch cases for coroutine state, similar to the technique in
// https://www.chiark.greenend.org.uk/~sgtatham/coroutines.html
//
// We use a trivial macro instead of an explicit assignment and case statement
// so that clang-format doesn't get confused by the unusual "case"s.
#define PUFFS_COROUTINE_STATE(n) \
  coro_state = n;                \
  case n:

#define PUFFS_LOW_BITS(x, n) ((x) & ((1 << (n)) - 1))

// Clang also defines "__GNUC__".
#if defined(__GNUC__)
#define PUFFS_LIKELY(expr) (__builtin_expect(!!(expr), 1))
#define PUFFS_UNLIKELY(expr) (__builtin_expect(!!(expr), 0))
#else
#define PUFFS_LIKELY(expr) (expr)
#define PUFFS_UNLIKELY(expr) (expr)
#endif

#endif  // PUFFS_BASE_IMPL_H

// ---------------- Status Codes Implementations

bool puffs_gif_status_is_error(puffs_gif_status s) {
  return s < 0;
}

const char* puffs_gif_status_strings0[9] = {
    "gif: ok",
    "gif: bad version",
    "gif: bad receiver",
    "gif: bad argument",
    "gif: constructor not called",
    "gif: unexpected EOF",
    "gif: short read",
    "gif: short write",
    "gif: closed for writes",
};

const char* puffs_gif_status_strings1[7] = {
    "gif: bad GIF block",
    "gif: bad GIF extension label",
    "gif: bad GIF header",
    "gif: bad LZW literal width",
    "gif: TODO: unsupported Local Color Table",
    "gif: LZW code is out of range",
    "gif: LZW prefix chain is cyclical",
};

const char* puffs_gif_status_string(puffs_gif_status s) {
  const char** a = NULL;
  uint32_t n = 0;
  switch ((s >> 10) & 0x1fffff) {
    case 0:
      a = puffs_gif_status_strings0;
      n = 9;
      break;
    case puffs_gif_packageid:
      a = puffs_gif_status_strings1;
      n = 7;
      break;
  }
  uint32_t i = s & 0xff;
  return i < n ? a[i] : "gif: unknown status";
}

// ---------------- Private Constructor and Destructor Prototypes

// ---------------- Private Function Prototypes

puffs_gif_status puffs_gif_decoder_decode_header(puffs_gif_decoder* self,
                                                 puffs_base_reader1 a_src);

puffs_gif_status puffs_gif_decoder_decode_lsd(puffs_gif_decoder* self,
                                              puffs_base_reader1 a_src);

puffs_gif_status puffs_gif_decoder_decode_extension(puffs_gif_decoder* self,
                                                    puffs_base_reader1 a_src);

puffs_gif_status puffs_gif_decoder_decode_id(puffs_gif_decoder* self,
                                             puffs_base_writer1 a_dst,
                                             puffs_base_reader1 a_src);

// ---------------- Constructor and Destructor Implementations

// PUFFS_MAGIC is a magic number to check that constructors are called. It's
// not foolproof, given C doesn't automatically zero memory before use, but it
// should catch 99.99% of cases.
//
// Its (non-zero) value is arbitrary, based on md5sum("puffs").
#define PUFFS_MAGIC (0xCB3699CCU)

// PUFFS_ALREADY_ZEROED is passed from a container struct's constructor to a
// containee struct's constructor when the container has already zeroed the
// containee's memory.
//
// Its (non-zero) value is arbitrary, based on md5sum("zeroed").
#define PUFFS_ALREADY_ZEROED (0x68602EF1U)

void puffs_gif_lzw_decoder_constructor(puffs_gif_lzw_decoder* self,
                                       uint32_t puffs_version,
                                       uint32_t for_internal_use_only) {
  if (!self) {
    return;
  }
  if (puffs_version != PUFFS_VERSION) {
    self->private_impl.status = PUFFS_GIF_ERROR_BAD_VERSION;
    return;
  }
  if (for_internal_use_only != PUFFS_ALREADY_ZEROED) {
    memset(self, 0, sizeof(*self));
  }
  self->private_impl.magic = PUFFS_MAGIC;
  self->private_impl.f_literal_width = 8;
}

void puffs_gif_lzw_decoder_destructor(puffs_gif_lzw_decoder* self) {
  if (!self) {
    return;
  }
}

void puffs_gif_decoder_constructor(puffs_gif_decoder* self,
                                   uint32_t puffs_version,
                                   uint32_t for_internal_use_only) {
  if (!self) {
    return;
  }
  if (puffs_version != PUFFS_VERSION) {
    self->private_impl.status = PUFFS_GIF_ERROR_BAD_VERSION;
    return;
  }
  if (for_internal_use_only != PUFFS_ALREADY_ZEROED) {
    memset(self, 0, sizeof(*self));
  }
  self->private_impl.magic = PUFFS_MAGIC;
  puffs_gif_lzw_decoder_constructor(&self->private_impl.f_lzw, PUFFS_VERSION,
                                    PUFFS_ALREADY_ZEROED);
}

void puffs_gif_decoder_destructor(puffs_gif_decoder* self) {
  if (!self) {
    return;
  }
  puffs_gif_lzw_decoder_destructor(&self->private_impl.f_lzw);
}

// ---------------- Function Implementations

puffs_gif_status puffs_gif_decoder_decode(puffs_gif_decoder* self,
                                          puffs_base_writer1 a_dst,
                                          puffs_base_reader1 a_src) {
  if (!self) {
    return PUFFS_GIF_ERROR_BAD_RECEIVER;
  }
  puffs_gif_status status = self->private_impl.status;
  if (status < 0) {
    return status;
  }
  if (self->private_impl.magic != PUFFS_MAGIC) {
    status = PUFFS_GIF_ERROR_CONSTRUCTOR_NOT_CALLED;
    goto exit;
  }

  uint8_t v_c;

  uint8_t* b_rptr_src = NULL;
  uint8_t* b_rend_src = NULL;
  if (a_src.buf) {
    b_rptr_src = a_src.buf->ptr + a_src.buf->ri;
    size_t len = a_src.buf->wi - a_src.buf->ri;
    puffs_base_limit1* lim;
    for (lim = &a_src.limit; lim; lim = lim->next) {
      if (lim->ptr_to_len && (len > *lim->ptr_to_len)) {
        len = *lim->ptr_to_len;
      }
    }
    b_rend_src = b_rptr_src + len;
  }

  uint32_t coro_state = self->private_impl.c_decode[0].coro_state;
  if (coro_state) {
    v_c = self->private_impl.c_decode[0].v_c;
  }
  switch (coro_state) {
    PUFFS_COROUTINE_STATE(0);

    PUFFS_COROUTINE_STATE(1);
    if (a_src.buf) {
      size_t n = b_rptr_src - (a_src.buf->ptr + a_src.buf->ri);
      a_src.buf->ri += n;
      puffs_base_limit1* lim;
      for (lim = &a_src.limit; lim; lim = lim->next) {
        if (lim->ptr_to_len) {
          *lim->ptr_to_len -= n;
        }
      }
    }
    status = puffs_gif_decoder_decode_header(self, a_src);
    if (a_src.buf) {
      b_rptr_src = a_src.buf->ptr + a_src.buf->ri;
      size_t len = a_src.buf->wi - a_src.buf->ri;
      puffs_base_limit1* lim;
      for (lim = &a_src.limit; lim; lim = lim->next) {
        if (lim->ptr_to_len && (len > *lim->ptr_to_len)) {
          len = *lim->ptr_to_len;
        }
      }
      b_rend_src = b_rptr_src + len;
    }
    if (status) {
      goto suspend;
    }
    PUFFS_COROUTINE_STATE(2);
    if (a_src.buf) {
      size_t n = b_rptr_src - (a_src.buf->ptr + a_src.buf->ri);
      a_src.buf->ri += n;
      puffs_base_limit1* lim;
      for (lim = &a_src.limit; lim; lim = lim->next) {
        if (lim->ptr_to_len) {
          *lim->ptr_to_len -= n;
        }
      }
    }
    status = puffs_gif_decoder_decode_lsd(self, a_src);
    if (a_src.buf) {
      b_rptr_src = a_src.buf->ptr + a_src.buf->ri;
      size_t len = a_src.buf->wi - a_src.buf->ri;
      puffs_base_limit1* lim;
      for (lim = &a_src.limit; lim; lim = lim->next) {
        if (lim->ptr_to_len && (len > *lim->ptr_to_len)) {
          len = *lim->ptr_to_len;
        }
      }
      b_rend_src = b_rptr_src + len;
    }
    if (status) {
      goto suspend;
    }
    while (true) {
      PUFFS_COROUTINE_STATE(3);
      if (PUFFS_UNLIKELY(b_rptr_src == b_rend_src)) {
        goto short_read_src;
      }
      uint8_t t_0 = *b_rptr_src++;
      v_c = t_0;
      if (v_c == 33) {
        PUFFS_COROUTINE_STATE(4);
        if (a_src.buf) {
          size_t n = b_rptr_src - (a_src.buf->ptr + a_src.buf->ri);
          a_src.buf->ri += n;
          puffs_base_limit1* lim;
          for (lim = &a_src.limit; lim; lim = lim->next) {
            if (lim->ptr_to_len) {
              *lim->ptr_to_len -= n;
            }
          }
        }
        status = puffs_gif_decoder_decode_extension(self, a_src);
        if (a_src.buf) {
          b_rptr_src = a_src.buf->ptr + a_src.buf->ri;
          size_t len = a_src.buf->wi - a_src.buf->ri;
          puffs_base_limit1* lim;
          for (lim = &a_src.limit; lim; lim = lim->next) {
            if (lim->ptr_to_len && (len > *lim->ptr_to_len)) {
              len = *lim->ptr_to_len;
            }
          }
          b_rend_src = b_rptr_src + len;
        }
        if (status) {
          goto suspend;
        }
      } else if (v_c == 44) {
        PUFFS_COROUTINE_STATE(5);
        if (a_src.buf) {
          size_t n = b_rptr_src - (a_src.buf->ptr + a_src.buf->ri);
          a_src.buf->ri += n;
          puffs_base_limit1* lim;
          for (lim = &a_src.limit; lim; lim = lim->next) {
            if (lim->ptr_to_len) {
              *lim->ptr_to_len -= n;
            }
          }
        }
        status = puffs_gif_decoder_decode_id(self, a_dst, a_src);
        if (a_src.buf) {
          b_rptr_src = a_src.buf->ptr + a_src.buf->ri;
          size_t len = a_src.buf->wi - a_src.buf->ri;
          puffs_base_limit1* lim;
          for (lim = &a_src.limit; lim; lim = lim->next) {
            if (lim->ptr_to_len && (len > *lim->ptr_to_len)) {
              len = *lim->ptr_to_len;
            }
          }
          b_rend_src = b_rptr_src + len;
        }
        if (status) {
          goto suspend;
        }
      } else if (v_c == 59) {
        status = PUFFS_GIF_STATUS_OK;
        goto suspend;
      } else {
        status = PUFFS_GIF_ERROR_BAD_GIF_BLOCK;
        goto suspend;
      }
    }
    coro_state = 0;
  }

  goto suspend;
suspend:
  self->private_impl.c_decode[0].coro_state = coro_state;
  self->private_impl.c_decode[0].v_c = v_c;

  if (a_src.buf) {
    size_t n = b_rptr_src - (a_src.buf->ptr + a_src.buf->ri);
    a_src.buf->ri += n;
    puffs_base_limit1* lim;
    for (lim = &a_src.limit; lim; lim = lim->next) {
      if (lim->ptr_to_len) {
        *lim->ptr_to_len -= n;
      }
    }
  }

  goto exit;
exit:
  self->private_impl.status = status;
  return status;

short_read_src:
  status = ((a_src.buf->closed) && (a_src.buf->ri == a_src.buf->wi))
               ? PUFFS_GIF_ERROR_UNEXPECTED_EOF
               : PUFFS_GIF_STATUS_SHORT_READ;
  goto suspend;
}

puffs_gif_status puffs_gif_decoder_decode_header(puffs_gif_decoder* self,
                                                 puffs_base_reader1 a_src) {
  puffs_gif_status status = self->private_impl.status;

  uint8_t v_c[6];
  uint32_t v_i;

  uint8_t* b_rptr_src = NULL;
  uint8_t* b_rend_src = NULL;
  if (a_src.buf) {
    b_rptr_src = a_src.buf->ptr + a_src.buf->ri;
    size_t len = a_src.buf->wi - a_src.buf->ri;
    puffs_base_limit1* lim;
    for (lim = &a_src.limit; lim; lim = lim->next) {
      if (lim->ptr_to_len && (len > *lim->ptr_to_len)) {
        len = *lim->ptr_to_len;
      }
    }
    b_rend_src = b_rptr_src + len;
  }

  uint32_t coro_state = self->private_impl.c_decode_header[0].coro_state;
  if (coro_state) {
    memcpy(v_c, self->private_impl.c_decode_header[0].v_c, 6);
    v_i = self->private_impl.c_decode_header[0].v_i;
  }
  switch (coro_state) {
    PUFFS_COROUTINE_STATE(0);

    {
      size_t i;
      for (i = 0; i < 6; i++) {
        v_c[i] = 0;
      }
    };
    v_i = 0;
    while (v_i < 6) {
      PUFFS_COROUTINE_STATE(1);
      if (PUFFS_UNLIKELY(b_rptr_src == b_rend_src)) {
        goto short_read_src;
      }
      uint8_t t_0 = *b_rptr_src++;
      v_c[v_i] = t_0;
      v_i += 1;
    }
    if ((v_c[0] != 71) || (v_c[1] != 73) || (v_c[2] != 70) || (v_c[3] != 56) ||
        ((v_c[4] != 55) && (v_c[4] != 57)) || (v_c[5] != 97)) {
      return PUFFS_GIF_ERROR_BAD_GIF_HEADER;
    }
    coro_state = 0;
  }

  goto suspend;
suspend:
  self->private_impl.c_decode_header[0].coro_state = coro_state;
  memcpy(self->private_impl.c_decode_header[0].v_c, v_c, 6);
  self->private_impl.c_decode_header[0].v_i = v_i;

  if (a_src.buf) {
    size_t n = b_rptr_src - (a_src.buf->ptr + a_src.buf->ri);
    a_src.buf->ri += n;
    puffs_base_limit1* lim;
    for (lim = &a_src.limit; lim; lim = lim->next) {
      if (lim->ptr_to_len) {
        *lim->ptr_to_len -= n;
      }
    }
  }

  goto exit;
exit:
  return status;

short_read_src:
  status = ((a_src.buf->closed) && (a_src.buf->ri == a_src.buf->wi))
               ? PUFFS_GIF_ERROR_UNEXPECTED_EOF
               : PUFFS_GIF_STATUS_SHORT_READ;
  return status;
}

puffs_gif_status puffs_gif_decoder_decode_lsd(puffs_gif_decoder* self,
                                              puffs_base_reader1 a_src) {
  puffs_gif_status status = self->private_impl.status;

  uint8_t v_c[7];
  uint32_t v_i;
  uint32_t v_gct_size;

  uint8_t* b_rptr_src = NULL;
  uint8_t* b_rend_src = NULL;
  if (a_src.buf) {
    b_rptr_src = a_src.buf->ptr + a_src.buf->ri;
    size_t len = a_src.buf->wi - a_src.buf->ri;
    puffs_base_limit1* lim;
    for (lim = &a_src.limit; lim; lim = lim->next) {
      if (lim->ptr_to_len && (len > *lim->ptr_to_len)) {
        len = *lim->ptr_to_len;
      }
    }
    b_rend_src = b_rptr_src + len;
  }

  uint32_t coro_state = self->private_impl.c_decode_lsd[0].coro_state;
  if (coro_state) {
    memcpy(v_c, self->private_impl.c_decode_lsd[0].v_c, 7);
    v_i = self->private_impl.c_decode_lsd[0].v_i;
    v_gct_size = self->private_impl.c_decode_lsd[0].v_gct_size;
  }
  switch (coro_state) {
    PUFFS_COROUTINE_STATE(0);

    {
      size_t i;
      for (i = 0; i < 7; i++) {
        v_c[i] = 0;
      }
    };
    v_i = 0;
    while (v_i < 7) {
      PUFFS_COROUTINE_STATE(1);
      if (PUFFS_UNLIKELY(b_rptr_src == b_rend_src)) {
        goto short_read_src;
      }
      uint8_t t_0 = *b_rptr_src++;
      v_c[v_i] = t_0;
      v_i += 1;
    }
    self->private_impl.f_width =
        (((uint32_t)(v_c[0])) | (((uint32_t)(v_c[1])) << 8));
    self->private_impl.f_height =
        (((uint32_t)(v_c[2])) | (((uint32_t)(v_c[3])) << 8));
    self->private_impl.f_background_color_index = v_c[5];
    if ((v_c[4] & 128) != 0) {
      v_gct_size = (((uint32_t)(1)) << (1 + (v_c[4] & 7)));
      v_i = 0;
      while (v_i < v_gct_size) {
        PUFFS_COROUTINE_STATE(2);
        if (PUFFS_UNLIKELY(b_rptr_src == b_rend_src)) {
          goto short_read_src;
        }
        uint8_t t_1 = *b_rptr_src++;
        self->private_impl.f_gct[(3 * v_i) + 0] = t_1;
        PUFFS_COROUTINE_STATE(3);
        if (PUFFS_UNLIKELY(b_rptr_src == b_rend_src)) {
          goto short_read_src;
        }
        uint8_t t_2 = *b_rptr_src++;
        self->private_impl.f_gct[(3 * v_i) + 1] = t_2;
        PUFFS_COROUTINE_STATE(4);
        if (PUFFS_UNLIKELY(b_rptr_src == b_rend_src)) {
          goto short_read_src;
        }
        uint8_t t_3 = *b_rptr_src++;
        self->private_impl.f_gct[(3 * v_i) + 2] = t_3;
        v_i += 1;
      }
    }
    coro_state = 0;
  }

  goto suspend;
suspend:
  self->private_impl.c_decode_lsd[0].coro_state = coro_state;
  memcpy(self->private_impl.c_decode_lsd[0].v_c, v_c, 7);
  self->private_impl.c_decode_lsd[0].v_i = v_i;
  self->private_impl.c_decode_lsd[0].v_gct_size = v_gct_size;

  if (a_src.buf) {
    size_t n = b_rptr_src - (a_src.buf->ptr + a_src.buf->ri);
    a_src.buf->ri += n;
    puffs_base_limit1* lim;
    for (lim = &a_src.limit; lim; lim = lim->next) {
      if (lim->ptr_to_len) {
        *lim->ptr_to_len -= n;
      }
    }
  }

  goto exit;
exit:
  return status;

short_read_src:
  status = ((a_src.buf->closed) && (a_src.buf->ri == a_src.buf->wi))
               ? PUFFS_GIF_ERROR_UNEXPECTED_EOF
               : PUFFS_GIF_STATUS_SHORT_READ;
  return status;
}

puffs_gif_status puffs_gif_decoder_decode_extension(puffs_gif_decoder* self,
                                                    puffs_base_reader1 a_src) {
  puffs_gif_status status = self->private_impl.status;

  uint8_t v_label;
  uint8_t v_block_size;

  uint8_t* b_rptr_src = NULL;
  uint8_t* b_rend_src = NULL;
  if (a_src.buf) {
    b_rptr_src = a_src.buf->ptr + a_src.buf->ri;
    size_t len = a_src.buf->wi - a_src.buf->ri;
    puffs_base_limit1* lim;
    for (lim = &a_src.limit; lim; lim = lim->next) {
      if (lim->ptr_to_len && (len > *lim->ptr_to_len)) {
        len = *lim->ptr_to_len;
      }
    }
    b_rend_src = b_rptr_src + len;
  }

  uint32_t coro_state = self->private_impl.c_decode_extension[0].coro_state;
  if (coro_state) {
    v_label = self->private_impl.c_decode_extension[0].v_label;
    v_block_size = self->private_impl.c_decode_extension[0].v_block_size;
  }
  switch (coro_state) {
    PUFFS_COROUTINE_STATE(0);

    PUFFS_COROUTINE_STATE(1);
    if (PUFFS_UNLIKELY(b_rptr_src == b_rend_src)) {
      goto short_read_src;
    }
    uint8_t t_0 = *b_rptr_src++;
    v_label = t_0;
    if (v_label == 1) {
    } else if (v_label == 249) {
    } else if (v_label == 254) {
    } else if (v_label == 255) {
    } else {
      return PUFFS_GIF_ERROR_BAD_GIF_EXTENSION_LABEL;
    }
    while (true) {
      PUFFS_COROUTINE_STATE(2);
      if (PUFFS_UNLIKELY(b_rptr_src == b_rend_src)) {
        goto short_read_src;
      }
      uint8_t t_1 = *b_rptr_src++;
      v_block_size = t_1;
      if (v_block_size == 0) {
        goto label_0_break;
      }
      PUFFS_COROUTINE_STATE(3);
      size_t t_2 = ((uint32_t)(v_block_size));
      if (t_2 > b_rend_src - b_rptr_src) {
        t_2 -= b_rend_src - b_rptr_src;
        a_src.buf->ri = a_src.buf->wi;
        status = a_src.buf->closed ? PUFFS_GIF_ERROR_UNEXPECTED_EOF
                                   : PUFFS_GIF_STATUS_SHORT_READ;
        return status;
      }
      b_rptr_src += t_2;
    }
  label_0_break:;
    coro_state = 0;
  }

  goto suspend;
suspend:
  self->private_impl.c_decode_extension[0].coro_state = coro_state;
  self->private_impl.c_decode_extension[0].v_label = v_label;
  self->private_impl.c_decode_extension[0].v_block_size = v_block_size;

  if (a_src.buf) {
    size_t n = b_rptr_src - (a_src.buf->ptr + a_src.buf->ri);
    a_src.buf->ri += n;
    puffs_base_limit1* lim;
    for (lim = &a_src.limit; lim; lim = lim->next) {
      if (lim->ptr_to_len) {
        *lim->ptr_to_len -= n;
      }
    }
  }

  goto exit;
exit:
  return status;

short_read_src:
  status = ((a_src.buf->closed) && (a_src.buf->ri == a_src.buf->wi))
               ? PUFFS_GIF_ERROR_UNEXPECTED_EOF
               : PUFFS_GIF_STATUS_SHORT_READ;
  return status;
}

puffs_gif_status puffs_gif_decoder_decode_id(puffs_gif_decoder* self,
                                             puffs_base_writer1 a_dst,
                                             puffs_base_reader1 a_src) {
  puffs_gif_status status = self->private_impl.status;

  uint8_t v_c[9];
  uint32_t v_i;
  bool v_interlace;
  uint8_t v_lw;
  uint8_t v_block_size;
  uint64_t l_lzw_src;
  puffs_base_reader1 v_lzw_src;

  uint8_t* b_rptr_src = NULL;
  uint8_t* b_rend_src = NULL;
  if (a_src.buf) {
    b_rptr_src = a_src.buf->ptr + a_src.buf->ri;
    size_t len = a_src.buf->wi - a_src.buf->ri;
    puffs_base_limit1* lim;
    for (lim = &a_src.limit; lim; lim = lim->next) {
      if (lim->ptr_to_len && (len > *lim->ptr_to_len)) {
        len = *lim->ptr_to_len;
      }
    }
    b_rend_src = b_rptr_src + len;
  }

  uint32_t coro_state = self->private_impl.c_decode_id[0].coro_state;
  if (coro_state) {
    memcpy(v_c, self->private_impl.c_decode_id[0].v_c, 9);
    v_i = self->private_impl.c_decode_id[0].v_i;
    v_interlace = self->private_impl.c_decode_id[0].v_interlace;
    v_lw = self->private_impl.c_decode_id[0].v_lw;
    v_block_size = self->private_impl.c_decode_id[0].v_block_size;
    l_lzw_src = self->private_impl.c_decode_id[0].l_lzw_src;
    v_lzw_src = self->private_impl.c_decode_id[0].v_lzw_src;
  }
  switch (coro_state) {
    PUFFS_COROUTINE_STATE(0);

    {
      size_t i;
      for (i = 0; i < 9; i++) {
        v_c[i] = 0;
      }
    };
    v_i = 0;
    while (v_i < 9) {
      PUFFS_COROUTINE_STATE(1);
      if (PUFFS_UNLIKELY(b_rptr_src == b_rend_src)) {
        goto short_read_src;
      }
      uint8_t t_0 = *b_rptr_src++;
      v_c[v_i] = t_0;
      v_i += 1;
    }
    v_interlace = ((v_c[8] & 64) != 0);
    if (v_interlace) {
    }
    if ((v_c[8] & 128) != 0) {
      return PUFFS_GIF_ERROR_TODO_UNSUPPORTED_LOCAL_COLOR_TABLE;
    }
    PUFFS_COROUTINE_STATE(2);
    if (PUFFS_UNLIKELY(b_rptr_src == b_rend_src)) {
      goto short_read_src;
    }
    uint8_t t_1 = *b_rptr_src++;
    v_lw = t_1;
    if ((v_lw < 2) || (8 < v_lw)) {
      return PUFFS_GIF_ERROR_BAD_LZW_LITERAL_WIDTH;
    }
    puffs_gif_lzw_decoder_set_literal_width(&self->private_impl.f_lzw,
                                            ((uint32_t)(v_lw)));
    while (true) {
      PUFFS_COROUTINE_STATE(3);
      if (PUFFS_UNLIKELY(b_rptr_src == b_rend_src)) {
        goto short_read_src;
      }
      uint8_t t_2 = *b_rptr_src++;
      v_block_size = t_2;
      if (v_block_size == 0) {
        goto label_0_break;
      }
      l_lzw_src = ((uint64_t)(v_block_size));
      v_lzw_src = (puffs_base_reader1){
          .buf = a_src.buf,
          .limit = (puffs_base_limit1){
              .ptr_to_len = &l_lzw_src, .next = &a_src.limit,
          }};
      PUFFS_COROUTINE_STATE(4);
      if (a_src.buf) {
        size_t n = b_rptr_src - (a_src.buf->ptr + a_src.buf->ri);
        a_src.buf->ri += n;
        puffs_base_limit1* lim;
        for (lim = &a_src.limit; lim; lim = lim->next) {
          if (lim->ptr_to_len) {
            *lim->ptr_to_len -= n;
          }
        }
      }
      status = puffs_gif_lzw_decoder_decode(&self->private_impl.f_lzw, a_dst,
                                            v_lzw_src);
      if (a_src.buf) {
        b_rptr_src = a_src.buf->ptr + a_src.buf->ri;
        size_t len = a_src.buf->wi - a_src.buf->ri;
        puffs_base_limit1* lim;
        for (lim = &a_src.limit; lim; lim = lim->next) {
          if (lim->ptr_to_len && (len > *lim->ptr_to_len)) {
            len = *lim->ptr_to_len;
          }
        }
        b_rend_src = b_rptr_src + len;
      }
      if (status < 0) {
        return status;
      }
    }
  label_0_break:;
    coro_state = 0;
  }

  goto suspend;
suspend:
  self->private_impl.c_decode_id[0].coro_state = coro_state;
  memcpy(self->private_impl.c_decode_id[0].v_c, v_c, 9);
  self->private_impl.c_decode_id[0].v_i = v_i;
  self->private_impl.c_decode_id[0].v_interlace = v_interlace;
  self->private_impl.c_decode_id[0].v_lw = v_lw;
  self->private_impl.c_decode_id[0].v_block_size = v_block_size;
  self->private_impl.c_decode_id[0].l_lzw_src = l_lzw_src;
  self->private_impl.c_decode_id[0].v_lzw_src = v_lzw_src;

  if (a_src.buf) {
    size_t n = b_rptr_src - (a_src.buf->ptr + a_src.buf->ri);
    a_src.buf->ri += n;
    puffs_base_limit1* lim;
    for (lim = &a_src.limit; lim; lim = lim->next) {
      if (lim->ptr_to_len) {
        *lim->ptr_to_len -= n;
      }
    }
  }

  goto exit;
exit:
  return status;

short_read_src:
  status = ((a_src.buf->closed) && (a_src.buf->ri == a_src.buf->wi))
               ? PUFFS_GIF_ERROR_UNEXPECTED_EOF
               : PUFFS_GIF_STATUS_SHORT_READ;
  return status;
}

void puffs_gif_lzw_decoder_set_literal_width(puffs_gif_lzw_decoder* self,
                                             uint32_t a_lw) {
  if (!self) {
    return;
  }
  if (self->private_impl.status < 0) {
    return;
  }
  if (self->private_impl.magic != PUFFS_MAGIC) {
    self->private_impl.status = PUFFS_GIF_ERROR_CONSTRUCTOR_NOT_CALLED;
    return;
  }
  if (a_lw < 2 || a_lw > 8) {
    self->private_impl.status = PUFFS_GIF_ERROR_BAD_ARGUMENT;
    return;
  }

  self->private_impl.f_literal_width = a_lw;
}

puffs_gif_status puffs_gif_lzw_decoder_decode(puffs_gif_lzw_decoder* self,
                                              puffs_base_writer1 a_dst,
                                              puffs_base_reader1 a_src) {
  if (!self) {
    return PUFFS_GIF_ERROR_BAD_RECEIVER;
  }
  puffs_gif_status status = self->private_impl.status;
  if (status < 0) {
    return status;
  }
  if (self->private_impl.magic != PUFFS_MAGIC) {
    status = PUFFS_GIF_ERROR_CONSTRUCTOR_NOT_CALLED;
    goto exit;
  }

  uint32_t v_clear_code;
  uint32_t v_end_code;
  uint32_t v_save_code;
  uint32_t v_prev_code;
  uint32_t v_width;
  uint32_t v_bits;
  uint32_t v_n_bits;
  uint32_t v_code;
  uint32_t v_s;
  uint32_t v_c;

  uint8_t* b_wptr_dst = NULL;
  uint8_t* b_wend_dst = NULL;
  if (a_dst.buf) {
    b_wptr_dst = a_dst.buf->ptr + a_dst.buf->wi;
    size_t len = a_dst.buf->len - a_dst.buf->wi;
    puffs_base_limit1* lim;
    for (lim = &a_dst.limit; lim; lim = lim->next) {
      if (lim->ptr_to_len && (len > *lim->ptr_to_len)) {
        len = *lim->ptr_to_len;
      }
    }
    b_wend_dst = b_wptr_dst + len;
  }
  uint8_t* b_rptr_src = NULL;
  uint8_t* b_rend_src = NULL;
  if (a_src.buf) {
    b_rptr_src = a_src.buf->ptr + a_src.buf->ri;
    size_t len = a_src.buf->wi - a_src.buf->ri;
    puffs_base_limit1* lim;
    for (lim = &a_src.limit; lim; lim = lim->next) {
      if (lim->ptr_to_len && (len > *lim->ptr_to_len)) {
        len = *lim->ptr_to_len;
      }
    }
    b_rend_src = b_rptr_src + len;
  }

  uint32_t coro_state = self->private_impl.c_decode[0].coro_state;
  if (coro_state) {
    v_clear_code = self->private_impl.c_decode[0].v_clear_code;
    v_end_code = self->private_impl.c_decode[0].v_end_code;
    v_save_code = self->private_impl.c_decode[0].v_save_code;
    v_prev_code = self->private_impl.c_decode[0].v_prev_code;
    v_width = self->private_impl.c_decode[0].v_width;
    v_bits = self->private_impl.c_decode[0].v_bits;
    v_n_bits = self->private_impl.c_decode[0].v_n_bits;
    v_code = self->private_impl.c_decode[0].v_code;
    v_s = self->private_impl.c_decode[0].v_s;
    v_c = self->private_impl.c_decode[0].v_c;
  }
  switch (coro_state) {
    PUFFS_COROUTINE_STATE(0);

    v_clear_code = (((uint32_t)(1)) << self->private_impl.f_literal_width);
    v_end_code = (v_clear_code + 1);
    v_save_code = v_end_code;
    v_prev_code = 0;
    v_width = (self->private_impl.f_literal_width + 1);
    v_bits = 0;
    v_n_bits = 0;
  label_0_continue:;
    while (true) {
      while (v_n_bits < v_width) {
        PUFFS_COROUTINE_STATE(1);
        if (PUFFS_UNLIKELY(b_rptr_src == b_rend_src)) {
          goto short_read_src;
        }
        uint8_t t_0 = *b_rptr_src++;
        v_bits |= (((uint32_t)(t_0)) << v_n_bits);
        v_n_bits += 8;
      }
      v_code = PUFFS_LOW_BITS(v_bits, v_width);
      v_bits >>= v_width;
      v_n_bits -= v_width;
      if (v_code < v_clear_code) {
        PUFFS_COROUTINE_STATE(2);
        if (b_wptr_dst == b_wend_dst) {
          status = PUFFS_GIF_STATUS_SHORT_WRITE;
          goto suspend;
        }
        *b_wptr_dst++ = ((uint8_t)(v_code));
        if (v_save_code <= 4095) {
          self->private_impl.f_suffixes[v_save_code] = ((uint8_t)(v_code));
          self->private_impl.f_prefixes[v_save_code] =
              ((uint16_t)(v_prev_code));
        }
      } else if (v_code == v_clear_code) {
        v_save_code = v_end_code;
        v_prev_code = 0;
        v_width = (self->private_impl.f_literal_width + 1);
        goto label_0_continue;
      } else if (v_code == v_end_code) {
        status = PUFFS_GIF_STATUS_OK;
        goto suspend;
      } else if (v_code <= v_save_code) {
        v_s = 4095;
        v_c = v_code;
        if (v_code == v_save_code) {
          v_s -= 1;
          v_c = v_prev_code;
        }
        while (v_c >= v_clear_code) {
          self->private_impl.f_stack[v_s] = self->private_impl.f_suffixes[v_c];
          if (v_s == 0) {
            status = PUFFS_GIF_ERROR_LZW_PREFIX_CHAIN_IS_CYCLICAL;
            goto suspend;
          }
          v_s -= 1;
          v_c = ((uint32_t)(self->private_impl.f_prefixes[v_c]));
        }
        self->private_impl.f_stack[v_s] = ((uint8_t)(v_c));
        if (v_code == v_save_code) {
          self->private_impl.f_stack[4095] = ((uint8_t)(v_c));
        }
        PUFFS_COROUTINE_STATE(3);
        if (a_dst.buf->closed) {
          status = PUFFS_GIF_ERROR_CLOSED_FOR_WRITES;
          goto suspend;
        }
        if ((b_wend_dst - b_wptr_dst) <
            (sizeof(self->private_impl.f_stack) - v_s)) {
          status = PUFFS_GIF_STATUS_SHORT_WRITE;
          goto suspend;
        }
        memmove(b_wptr_dst, self->private_impl.f_stack + v_s,
                sizeof(self->private_impl.f_stack) - v_s);
        b_wptr_dst += sizeof(self->private_impl.f_stack) - v_s;
        if (v_save_code <= 4095) {
          self->private_impl.f_suffixes[v_save_code] = ((uint8_t)(v_c));
          self->private_impl.f_prefixes[v_save_code] =
              ((uint16_t)(v_prev_code));
        }
      } else {
        status = PUFFS_GIF_ERROR_LZW_CODE_IS_OUT_OF_RANGE;
        goto suspend;
      }
      if (v_save_code <= 4095) {
        v_save_code += 1;
        if ((v_save_code == (((uint32_t)(1)) << v_width)) && (v_width < 12)) {
          v_width += 1;
        }
      }
      v_prev_code = v_code;
    }
    coro_state = 0;
  }

  goto suspend;
suspend:
  self->private_impl.c_decode[0].coro_state = coro_state;
  self->private_impl.c_decode[0].v_clear_code = v_clear_code;
  self->private_impl.c_decode[0].v_end_code = v_end_code;
  self->private_impl.c_decode[0].v_save_code = v_save_code;
  self->private_impl.c_decode[0].v_prev_code = v_prev_code;
  self->private_impl.c_decode[0].v_width = v_width;
  self->private_impl.c_decode[0].v_bits = v_bits;
  self->private_impl.c_decode[0].v_n_bits = v_n_bits;
  self->private_impl.c_decode[0].v_code = v_code;
  self->private_impl.c_decode[0].v_s = v_s;
  self->private_impl.c_decode[0].v_c = v_c;

  if (a_dst.buf) {
    size_t n = b_wptr_dst - (a_dst.buf->ptr + a_dst.buf->wi);
    a_dst.buf->wi += n;
    puffs_base_limit1* lim;
    for (lim = &a_dst.limit; lim; lim = lim->next) {
      if (lim->ptr_to_len) {
        *lim->ptr_to_len -= n;
      }
    }
  }
  if (a_src.buf) {
    size_t n = b_rptr_src - (a_src.buf->ptr + a_src.buf->ri);
    a_src.buf->ri += n;
    puffs_base_limit1* lim;
    for (lim = &a_src.limit; lim; lim = lim->next) {
      if (lim->ptr_to_len) {
        *lim->ptr_to_len -= n;
      }
    }
  }

  goto exit;
exit:
  self->private_impl.status = status;
  return status;

short_read_src:
  status = ((a_src.buf->closed) && (a_src.buf->ri == a_src.buf->wi))
               ? PUFFS_GIF_ERROR_UNEXPECTED_EOF
               : PUFFS_GIF_STATUS_SHORT_READ;
  goto suspend;
}
