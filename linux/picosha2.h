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
#include <sstream>
#include <vector>
#include <fstream>
namespace picosha2 {
typedef unsigned long word_t;
typedef unsigned char byte_t;

static const size_t k_digest_size = 32;

namespace detail {
const word_t initial_message_digest[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372,
                                          0xa54ff53a, 0x510e527f, 0x9b05688c,
                                          0x1f83d9ab, 0x5be0cd19};

inline byte_t mask_8bit(byte_t x) { return x & 0xff; }

}  // namespace detail

extern "C"
void hash256_block(unsigned long *message_digest, unsigned char *first, unsigned char *last);

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
    std::ostringstream oss;
    output_hex(first, last, oss);
    hex_str.assign(oss.str());
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
            hash256_block(h_, &*buffer_.begin() + i,
                                  &*buffer_.begin() + i + 64);
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
            hash256_block(h_, temp, temp + 64);
            std::fill(temp, temp + 64 - 4, byte_t(0));
        } else {
            std::fill(temp + remains + 1, temp + 64 - 4, byte_t(0));
        }

        write_data_bit_length(&(temp[56]));
        hash256_block(h_, temp, temp + 64);
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
