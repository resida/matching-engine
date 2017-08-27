// Copyright (c) 2001-2010 quickfixengine.org  All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in
//    the documentation and/or other materials provided with the
//    distribution.
//
// 3. The end-user documentation included with the redistribution,
//    if any, must include the following acknowledgment:
//      "This product includes software developed by
//       quickfixengine.org (http://www.quickfixengine.org/)."
//   Alternately, this acknowledgment may appear in the software itself,
//   if and wherever such third-party acknowledgments normally appear.
//
// 4. The names "QuickFIX" and "quickfixengine.org" must
//    not be used to endorse or promote products derived from this
//    software without prior written permission. For written
//    permission, please contact ask@quickfixengine.org
//
// 5. Products derived from this software may not be called "QuickFIX",
//    nor may "QuickFIX" appear in their name, without prior written
//    permission of quickfixengine.org
//
// THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED.  IN NO EVENT SHALL QUICKFIXENGINE.ORG OR
// ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
// USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
// OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

#ifndef FIX_SOCKETINITIATOR_H
#define FIX_SOCKETINITIATOR_H

#ifdef _MSC_VER
#pragma warning( disable : 4503 4355 4786 4290 )
#endif

#include "Initiator.h"
#include "SocketConnector.h"
#include "SocketConnection.h"

namespace FIX
{
/// Socket implementation of Initiator.
class SocketInitiator : public Initiator, SocketConnector::Strategy
{
public:
  SocketInitiator( Application&, MessageStoreFactory&,
                   const SessionSettings& ) throw( ConfigError );
  SocketInitiator( Application&, MessageStoreFactory&,
                   const SessionSettings&, LogFactory& ) throw( ConfigError );

  virtual ~SocketInitiator();

private:
  typedef std::map < int, SocketConnection* > SocketConnections;
  typedef std::map < SessionID, int > SessionToHostNum;

  void onConfigure( const SessionSettings& ) throw ( ConfigError );
  void onInitialize( const SessionSettings& ) throw ( RuntimeError );

  void onStart();
  bool onPoll( double timeout );
  void onStop();

  void doConnect( const SessionID&, const Dictionary& d );
  void onConnect( SocketConnector&, int );
  void onWrite( SocketConnector&, int );
  bool onData( SocketConnector&, int );
  void onDisconnect( SocketConnector&, int );
  void onError( SocketConnector& );
  void onTimeout( SocketConnector& );

  void getHost( const SessionID&, const Dictionary&, std::string&, short& );

  SessionSettings m_settings;
  SessionToHostNum m_sessionToHostNum;
  SocketConnector m_connector;
  SocketConnections m_pendingConnections;
  SocketConnections m_connections;
  time_t m_lastConnect;
  int m_reconnectInterval;
  bool m_noDelay;
  int m_sendBufSize;
  int m_rcvBufSize;
  bool m_stop;
};
/*! @} */
}

#endif //FIX_SOCKETINITIATOR_H
