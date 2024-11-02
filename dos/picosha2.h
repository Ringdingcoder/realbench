/*
The MIT License (MIT)

Copyright (C) 2017 okdshin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#ifndef PICOSHA2_H
#define PICOSHA2_H
// picosha2:20140213

#ifndef PICOSHA2_BUFFER_SIZE_FOR_INPUT_ITERATOR
#define PICOSHA2_BUFFER_SIZE_FOR_INPUT_ITERATOR \
    1048576  //=1024*1024: default is 1MB memory
#endif

#include <algorithm>
#include <cassert>
#include <iterator>
#include <string>
#include <strstrea>
#include <vector>
#include <fstream>

#include <stdlib.h>
#include <string.h>

namespace picosha2 {
typedef unsigned long word_t;
typedef unsigned char byte_t;

typedef unsigned size_t;

static const size_t k_digest_size = 32;

namespace detail {
inline byte_t mask_8bit(byte_t x) { return x & 0xff; }

inline word_t mask_32bit(word_t x) { return x & 0xffffffffUL; }

const word_t add_constant[64] = {
    0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL, 0x3956c25bUL, 0x59f111f1UL,
    0x923f82a4UL, 0xab1c5ed5UL, 0xd807aa98UL, 0x12835b01UL, 0x243185beUL, 0x550c7dc3UL,
    0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL, 0xc19bf174UL, 0xe49b69c1UL, 0xefbe4786UL,
    0x0fc19dc6UL, 0x240ca1ccUL, 0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL,
    0x983e5152UL, 0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL, 0xc6e00bf3UL, 0xd5a79147UL,
    0x06ca6351UL, 0x14292967UL, 0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL, 0x53380d13UL,
    0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL, 0xa2bfe8a1UL, 0xa81a664bUL,
    0xc24b8b70UL, 0xc76c51a3UL, 0xd192e819UL, 0xd6990624UL, 0xf40e3585UL, 0x106aa070UL,
    0x19a4c116UL, 0x1e376c08UL, 0x2748774cUL, 0x34b0bcb5UL, 0x391c0cb3UL, 0x4ed8aa4aUL,
    0x5b9cca4fUL, 0x682e6ff3UL, 0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
    0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL};

const word_t initial_message_digest[8] = {0x6a09e667UL, 0xbb67ae85UL, 0x3c6ef372UL,
                                          0xa54ff53aUL, 0x510e527fUL, 0x9b05688cUL,
                                          0x1f83d9abUL, 0x5be0cd19UL};

inline word_t ch(word_t x, word_t y, word_t z) { return (x & y) ^ ((~x) & z); }

inline word_t maj(word_t x, word_t y, word_t z) {
    return (x & y) ^ (x & z) ^ (y & z);
}

inline word_t rotr(word_t x, std::size_t n) {
    assert(n < 32);
    return mask_32bit((x >> n) | (x << (32 - n)));
}

inline word_t bsig0(word_t x) { return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22); }

inline word_t bsig1(word_t x) { return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25); }

inline word_t shr(word_t x, std::size_t n) {
    assert(n < 32);
    return x >> n;
}

inline word_t ssig0(word_t x) { return rotr(x, 7) ^ rotr(x, 18) ^ shr(x, 3); }

inline word_t ssig1(word_t x) { return rotr(x, 17) ^ rotr(x, 19) ^ shr(x, 10); }

template <typename RaIter1, typename RaIter2>
void hash256_block(RaIter1 message_digest, RaIter2 first, RaIter2 last) {
    assert(first + 64 == last);
    static_cast<void>(last);  // for avoiding unused-variable warning
    word_t w[64];
    std::fill(w, w + 64, word_t(0));
    for (std::size_t i = 0; i < 16; ++i) {
        w[i] = (static_cast<word_t>(mask_8bit(*(first + i * 4))) << 24) |
               (static_cast<word_t>(mask_8bit(*(first + i * 4 + 1))) << 16) |
               (static_cast<word_t>(mask_8bit(*(first + i * 4 + 2))) << 8) |
               (static_cast<word_t>(mask_8bit(*(first + i * 4 + 3))));
    }
    for (std::size_t i = 16; i < 64; ++i) {
        w[i] = mask_32bit(ssig1(w[i - 2]) + w[i - 7] + ssig0(w[i - 15]) +
                          w[i - 16]);
    }

    word_t a = *message_digest;
    word_t b = *(message_digest + 1);
    word_t c = *(message_digest + 2);
    word_t d = *(message_digest + 3);
    word_t e = *(message_digest + 4);
    word_t f = *(message_digest + 5);
    word_t g = *(message_digest + 6);
    word_t h = *(message_digest + 7);

    for (std::size_t i = 0; i < 64; ++i) {
        word_t temp1 = h + bsig1(e) + ch(e, f, g) + add_constant[i] + w[i];
        word_t temp2 = bsig0(a) + maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = mask_32bit(d + temp1);
        d = c;
        c = b;
        b = a;
        a = mask_32bit(temp1 + temp2);
    }
    *message_digest += a;
    *(message_digest + 1) += b;
    *(message_digest + 2) += c;
    *(message_digest + 3) += d;
    *(message_digest + 4) += e;
    *(message_digest + 5) += f;
    *(message_digest + 6) += g;
    *(message_digest + 7) += h;
    for (std::size_t i = 0; i < 8; ++i) {
        *(message_digest + i) = mask_32bit(*(message_digest + i));
    }
}

}  // namespace detail

template <typename InIter>
void output_hex(InIter first, InIter last, std::ostream& os) {
    os.setf(std::ios::hex, std::ios::basefield);
    while (first != last) {
        os.width(2);
        os.fill('0');
        os << static_cast<unsigned int>(*first);
        ++first;
    }
    os.setf(std::ios::dec, std::ios::basefield);
}

template <typename InIter>
void bytes_to_hex_string(InIter first, InIter last, std::string& hex_str) {
    std::ostrstream oss;
    output_hex(first, last, oss);
    size_t len = oss.tellp();
    char *buf = (char*) malloc(len + 1);
    buf[len] = '\0';
    memcpy(buf, oss.str(), len);
    hex_str.assign(buf);
}

class hash256_one_by_one {
   public:
    hash256_one_by_one() { init(); }

    void init() {
        buffer_.clear();
        std::fill(data_length_digits_, data_length_digits_ + 4, word_t(0));
        std::copy(detail::initial_message_digest,
                  detail::initial_message_digest + 8, h_);
    }

    template <typename RaIter>
    void process(RaIter first, RaIter last) {
        add_to_data_length(static_cast<word_t>(std::distance(first, last)));
        std::copy(first, last, std::back_inserter(buffer_));
        std::size_t i = 0;
        for (; i + 64 <= buffer_.size(); i += 64) {
            detail::hash256_block(h_, buffer_.begin() + i,
                                  buffer_.begin() + i + 64);
        }
        buffer_.erase(buffer_.begin(), buffer_.begin() + i);
    }

    void finish() {
        byte_t temp[64];
        std::fill(temp, temp + 64, byte_t(0));
        std::size_t remains = buffer_.size();
        std::copy(buffer_.begin(), buffer_.end(), temp);
        temp[remains] = 0x80;

        if (remains > 55) {
            std::fill(temp + remains + 1, temp + 64, byte_t(0));
            detail::hash256_block(h_, temp, temp + 64);
            std::fill(temp, temp + 64 - 4, byte_t(0));
        } else {
            std::fill(temp + remains + 1, temp + 64 - 4, byte_t(0));
        }

        write_data_bit_length(&(temp[56]));
        detail::hash256_block(h_, temp, temp + 64);
    }

    template <typename OutIter>
    void get_hash_bytes(OutIter first, OutIter last) const {
        for (const word_t* iter = h_; iter != h_ + 8; ++iter) {
            for (std::size_t i = 0; i < 4 && first != last; ++i) {
                *(first++) = detail::mask_8bit(
                    static_cast<byte_t>((*iter >> (24 - 8 * i))));
            }
        }
    }

   private:
    void add_to_data_length(word_t n) {
        word_t carry = 0;
        data_length_digits_[0] += n;
        for (std::size_t i = 0; i < 4; ++i) {
            data_length_digits_[i] += carry;
            if (data_length_digits_[i] >= 65536u) {
                carry = data_length_digits_[i] >> 16;
                data_length_digits_[i] &= 65535u;
            } else {
                break;
            }
        }
    }
    void write_data_bit_length(byte_t* begin) {
        word_t data_bit_length_digits[4];
        std::copy(data_length_digits_, data_length_digits_ + 4,
                  data_bit_length_digits);

        // convert byte length to bit length (multiply 8 or shift 3 times left)
        word_t carry = 0;
        for (std::size_t i = 0; i < 4; ++i) {
            word_t before_val = data_bit_length_digits[i];
            data_bit_length_digits[i] <<= 3;
            data_bit_length_digits[i] |= carry;
            data_bit_length_digits[i] &= 65535u;
            carry = (before_val >> (16 - 3)) & 65535u;
        }

        // write data_bit_length
        for (int i = 3; i >= 0; --i) {
            (*begin++) = static_cast<byte_t>(data_bit_length_digits[i] >> 8);
            (*begin++) = static_cast<byte_t>(data_bit_length_digits[i]);
        }
    }
    std::vector<byte_t> buffer_;
    word_t data_length_digits_[4];  // as 64bit integer (16bit x 4 integer)
    word_t h_[8];
};

inline void get_hash_hex_string(const hash256_one_by_one& hasher,
                                std::string& hex_str) {
    byte_t hash[k_digest_size];
    hasher.get_hash_bytes(hash, hash + k_digest_size);
    return bytes_to_hex_string(hash, hash + k_digest_size, hex_str);
}

inline std::string get_hash_hex_string(const hash256_one_by_one& hasher) {
    std::string hex_str;
    get_hash_hex_string(hasher, hex_str);
    return hex_str;
}

}// namespace picosha2
#endif  // PICOSHA2_H
