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

#ifndef C2E26F98_58D2_4FB6_9B05_CB4ED59A65C3
#define C2E26F98_58D2_4FB6_9B05_CB4ED59A65C3

#include "definitions.h"
#include "models.h"

NVX_NS_BEGIN
struct aspect;
struct position;

struct context {
    virtual aspect* load( const code& symbol_, const period& period_, int count_ ) = 0;
    virtual nvx_st  calloff( const oid& id_ )                                      = 0;

    virtual oid shorting( const code& c_, vol qty_, price price_, ord_type type_, const text& remark_ )                                            = 0;
    virtual oid longing( const code& c_, vol qty_, price price_, ord_type type_, const text& remark_ )                                             = 0;
    virtual oid trapping( const code& c_, vol qty_, price limit_, ord_dir dir_, price stop_, stop_dir sdir_, ord_type type_, const text& remark_ ) = 0;

    virtual position* qry_long( const code& c_ )  = 0;
    virtual position* qry_short( const code& c_ ) = 0;
    virtual ~context() {}
};

NVX_NS_END

#endif /* C2E26F98_58D2_4FB6_9B05_CB4ED59A65C3 */
