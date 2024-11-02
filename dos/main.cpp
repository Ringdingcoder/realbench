// wcl -s -xs -bt=dos -bcl=dos main.cpp

#include <bios.h>
#include <iostream>
#include <stdio.h>

#include "picosha2.h"

using std::cout;
using std::endl;

char emptybuf[512];

int main()
{
    long tv, tv2;

    _bios_timeofday(_TIME_GETCLOCK, &tv);

    picosha2::hash256_one_by_one hasher;
    for (long n=0; n<50000L; n++)
        hasher.process(emptybuf, emptybuf+512);
    hasher.finish();
    std::vector<unsigned char> hash(picosha2::k_digest_size);
    hasher.get_hash_bytes(hash.begin(), hash.end());

    cout << picosha2::get_hash_hex_string(hasher).c_str() << endl;
    _bios_timeofday(_TIME_GETCLOCK, &tv2);
    cout << tv2-tv << " ticks" << endl;

    tv2 -= tv;
    tv2 = (tv2<<16) / 11932;
    printf("%u.%02u seconds\n", (unsigned) (tv2/100), (unsigned) (tv2%100));

    return 0;
}
