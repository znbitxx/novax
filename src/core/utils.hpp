/************************************************************************************
The MIT License

Copyright (c) 2024 YaoZinan  [zinan@outlook.com, nvx-quant.com]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

* \author: yaozn(zinan@outlook.com)
* \date: 2024
**********************************************************************************/

#ifndef A0091236_F594_4A70_BDCB_927CD411D38C
#define A0091236_F594_4A70_BDCB_927CD411D38C
#include <atomic>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>

#include "ns.h"

NVX_NS_BEGIN
struct Spinner {
    void lock() {
        bool exp = false;

        while ( !_flag.compare_exchange_weak( exp, true, std::memory_order::memory_order_release, std::memory_order::memory_order_relaxed ) )
            exp = false;
    }

    void unlock() {
        _flag.store( false, std ::memory_order_release );
    }

private:
    std::atomic_bool _flag = false;
};

template <typename... T>
struct MaxTypeSize;

template <>
struct MaxTypeSize<> {
    enum {
        value = 0
    };
};

template <typename T, typename... R>
struct MaxTypeSize<T, R...> {
    enum {
        value = sizeof( T ) > ( MaxTypeSize<R...>::value ) ? sizeof( T ) : ( MaxTypeSize<R...>::value )
    };
};

inline void dumpHex(
    const uint8_t*                          data_,
    size_t                                  length_,
    int                                     size_per_row_,
    std::function<void( const char* hex_ )> print_ = []( const char* hex_ ) { printf( "%s", hex_ ); } ) {

    char hex[ 8 ];
    for ( int i = 0; i < ( int )length_; ++i ) {
        if ( i != 0 && i % size_per_row_ == 0 ) {
            print_( "\n" );
        }
        sprintf( hex, "%02X ", data_[ i ] );
        print_( hex );
    }

    print_( "\n" );
}

NVX_NS_END

#endif /* A0091236_F594_4A70_BDCB_927CD411D38C */
