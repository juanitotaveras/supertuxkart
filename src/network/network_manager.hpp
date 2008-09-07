//  $Id: network_manager.hpp 2128 2008-06-13 00:53:52Z cosmosninja $
//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2008 Joerg Henrichs, Stephen Leak
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef HEADER_NETWORK_MANAGER_H
#define HEADER_NETWORK_MANAGER_H

#include <string>
#include <vector>

#include "network/remote_kart_info.hpp"

#ifdef HAVE_ENET
#  include "enet/enet.h"
#endif

class NetworkManager
{
public:
    // The mode the network manager is operating in
    enum NetworkMode {NW_SERVER, NW_CLIENT, NW_NONE};

    // States for the finite state machine. First for server:
    enum NetworkState {NS_ACCEPT_CONNECTIONS, NS_KART_INFO_BARRIER,
         // Then client only states:
                       NS_CHARACTER_CONFIRMED, 
                       NS_CHARACTER_REJECTED,
                       NS_WAITING_FOR_RACE_DATA,
         // Shared states   
                       NS_CHARACTER_SELECT, NS_READY_SET_GO_BARRIER, NS_RACING};
private:

    NetworkMode                 m_mode;
    NetworkState                m_state;
    int                         m_port;
    std::string                 m_server_address;
    int                         m_num_clients;
    std::vector<RemoteKartInfo> m_kart_info;
    int                         m_host_id;
    std::vector<std::string>    m_client_names;
    int                         m_barrier_count;
#ifdef HAVE_ENET
    ENetHost                   *m_host;
#endif

    bool         initServer();
    bool         initClient();
    void         handleNewConnection(ENetEvent *event);
    void         handleServerMessage(ENetEvent *event);
    void         handleClientMessage(ENetEvent *event);
    void         handleDisconnection(ENetEvent *event);
    void         testSetCharacter   (ENetEvent *event);

public:
                 NetworkManager();
                ~NetworkManager();
    void         setMode(NetworkMode m)            {m_mode = m;             }
    NetworkMode  getMode() const                   {return m_mode;          }
    void         setState(NetworkState s)          {m_state = s;            }
    NetworkState getState() const                  {return m_state;         }
    int          getHostId() const                 {return m_host_id;       }
    unsigned int getNumClients() const             {return m_num_clients;   }
    const std::string& 
                 getClientName(int i) const        {return m_client_names[i];}
    void         setPort(int p)                    {m_port=p;               }
    void         setServerIP(const std::string &s) {m_server_address=s;     }
    void         setKartInfo(int player_id, const std::string& kart, 
                             const std::string& user="", int hostid=-1);
    bool         initialiseConnections();
    void         update(float dt);
    void         sendKartsInformationToServer();
    void         waitForKartsInformation();
    void         sendRaceInformationToClients();
    void         waitForRaceInformation();
    void         switchToReadySetGoBarrier();
    void         switchToCharacterSelection();
    void         switchToRaceDataSynchronisation();

};

extern NetworkManager *network_manager;

#endif

