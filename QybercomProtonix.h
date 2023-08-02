#pragma once

#include <Arduino.h>

namespace Qybercom {
  namespace Protonix {
    class INetwork {
      public:
        virtual bool Connect();
        virtual bool Connected();
        virtual String AddressMAC();
        virtual String AddressIP();
    };

    class IProtocol {
      public:
        virtual void Connect();
    };

    class URI {
      private:
        String _scheme;
        String _username;
        String _password;
        String _host;
        uint _port;
        String _path;

      public:
        URI(String host, uint port);
        URI(String host, uint port, String path);

        void Scheme(String scheme);
        String Scheme();

        void Username(String username);
        String Username();

        void Password(String password);
        String Password();

        void Host(String host);
        String Host();

        void Port(uint port);
        uint Port();

        void Path(String path);
        String Path();
    };

    namespace Networks {
      class NWiFi: public Qybercom::Protonix::INetwork {
        private:
          String _ssid;
          String _password;
          String _mac;
        
        public:
          NWiFi(String ssid, String password, String mac);

          bool Connect();

          bool Connected();

          String AddressMAC();

          String AddressIP();
      };
    }

    namespace Protocols {
      class PWebSocket: public Qybercom::Protonix::IProtocol {
        public:
          void Connect();
      };
    }

    // http://tedfelix.com/software/c++-callbacks.html
    class Device {
      public:
        Device(String id, String passphrase);

        void ID(String id);
        String ID();

        void Passphrase(String passphrase);
        String Passphrase();

        void Network(INetwork* network);
        INetwork* Network();

        void Protocol(IProtocol* protocol);
        IProtocol* Protocol();

        void Server(URI* uri);
        URI* Server();

        void ServerEndpoint(String host, uint port);
        void ServerEndpoint(String host, uint port, String path);

        void Pipe();

        /*Device* OnNetworkConnect(void (*callback)(Device*));
        Device* OnProtocolConnect(void (*callback)(Device*));*/
        typedef void(*NetworkConnectCallback)(Device*);
        Device* OnNetworkConnect(NetworkConnectCallback callback);
        typedef void(*ProtocolConnectCallback)(Device*);
        Device* OnProtocolConnect(ProtocolConnectCallback callback);

        //void Command();

    private:
        INetwork* _network;
        IProtocol* _protocol;
        URI* _uri;
        String _id;
        String _passphrase;
        /*void (_onNetworkConnect)(Device*);
        void (_onProtocolConnect)(Device*);*/
        NetworkConnectCallback _onNetworkConnect;
        ProtocolConnectCallback _onProtocolConnect;
    };
  }
}