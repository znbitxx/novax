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

#include <log.hpp>
#include <mutex>
#include <stdio.h>

#include "datacenter.h"

NVX_NS_BEGIN
dc_client::dc_client( ipub* p_ )
    : market( p_ ) {
}

nvx_st dc_client::start() {
    return send_event( _bev, dc::start_dc_event() );
}

nvx_st dc_client::stop() {
    return send_event( _bev, dc::stop_dc_event() );
}

nvx_st dc_client::subscribe( const code& code_ ) {
    dc::sub_event s;
    s.c = code_;

    //    {
    //      std::unique_lock<std::mutex> _lck{ _mtx };
    //    _allsubs.emplace( code_, false );
    //}
    return send_event( _bev, s );
}

nvx_st dc_client::unsubscribe( const code& code_ ) {
    dc::unsub_event um;
    um.c = code_;

    //    {
    //        std::unique_lock<std::mutex> _lck{ _mtx };
    //        _allsubs.erase( code_ );
    //   }

    return send_event( _bev, um );
}

void dc_client::on_event( const dc::event* m_, struct bufferevent* bev ) {
    switch ( m_->id ) {
    default: return;
    case dc::event_t::ack:
        return on_ack( reinterpret_cast<const dc::ack_event*>( m_ )->req, reinterpret_cast<const dc::ack_event*>( m_ )->rc );
    case dc::event_t::data_tick:
        return on_tick( reinterpret_cast<const dc::qut_event*>( m_ )->qut );
    }
}

void dc_client::on_ack( dc::event_t req_, char rc_ ) {
    // if ( req_ == mid_t::sub_data && 0 == rc_ ) {
    // }
    if ( rc_ != 0 ) {
        printf( "req failed,error=%d\n", rc_ );
    }
}

void dc_client::on_tick( const tick& qut_ ) {
    // todo
    // delegator()->update( qut_ );
}

void endpoint::read_cb( struct bufferevent* bev_, void* ctx ) {
    auto msg = dc::recv_event( bev_ );
    if ( !msg ) return;

    endpoint* cli = reinterpret_cast<endpoint*>( ctx );
    // cli->on_event( msg );
}

void endpoint::event_cb( struct bufferevent* bev, short event_, void* ctx ) {
    printf( "dcclient on event:%d\n", event_ );
}

void endpoint::attach( struct bufferevent* bev_ ) {
    _bev = bev_;

    bufferevent_setcb( _bev, &endpoint::read_cb, nullptr, &endpoint::event_cb, this );
    bufferevent_enable( _bev, EV_READ | EV_WRITE );
}

endpoint::endpoint() {
}

nvx_st dc_client::run() {
    struct event_base* base = event_base_new();

    struct sockaddr_un addr;
    memset( &addr, 0, sizeof( addr ) );
    addr.sun_family = AF_UNIX;
    strcpy( addr.sun_path, DC_SERVER_ADDR );

    int sock = socket( AF_UNIX, SOCK_STREAM, 0 );
    ::connect( sock, ( struct sockaddr* )&addr, sizeof( addr ) );
    // evutil_make_listen_socket_reuseable( sock );

    struct bufferevent* bev = bufferevent_socket_new( base, sock, BEV_OPT_CLOSE_ON_FREE );

    attach( bev );
    // bufferevent_setcb( bev, &dc_client::read_cb, nullptr, &dc_client::event_cb, this );
    // bufferevent_enable( bev, EV_READ | EV_WRITE );

    // bufferevent_write( bev, "subscribe", 9 );

    return event_base_dispatch( base );
}

NVX_NS_END