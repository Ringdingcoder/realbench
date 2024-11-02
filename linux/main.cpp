#include <iostream>

#include "picosha2.h"

using std::cout;
using std::endl;

char emptybuf[512];

int main()
{
    picosha2::hash256_one_by_one hasher;
    for (long n=0; n<50000L; n++)
        hasher.process(emptybuf, emptybuf+512);
    hasher.finish();
    std::vector<unsigned char> hash(picosha2::k_digest_size);
    hasher.get_hash_bytes(hash.begin(), hash.end());

    cout << picosha2::get_hash_hex_string(hasher) << endl;

    return 0;
}
