#pragma once

#include <Arduino.h>
#include <ArduinoWebsockets.h>

namespace Qybercom {
  namespace Protonix {
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

    class INetwork {
        public:
            virtual bool Connect();
            virtual bool Connected();
            virtual String AddressMAC();
            virtual String AddressIP();
            // https://stackoverflow.com/a/12772708/2097055
            static void ParseMAC (String mac, uint8_t out[6]) {
                char buffer[18];
                mac.toCharArray(buffer, 18);

                sscanf(buffer, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &out[0], &out[1], &out[2], &out[3], &out[4], &out[5]);
            }
    };

    class IProtocol {
        public:
            virtual bool Connect(URI* uri);
            virtual bool Connected();
            virtual void Pipe();
    };

    namespace Networks {
      class NWiFi: public Qybercom::Protonix::INetwork {
        private:
          String _ssid;
          String _password;
          String _mac;
          String _hostname;
        
        public:
          NWiFi(String ssid, String password, String mac, String hostname);

          bool Connect();

          bool Connected();

          String AddressMAC();

          String AddressIP();
      };
    }

    namespace Protocols {
      class PWebSocket: public Qybercom::Protonix::IProtocol {
        private:
          websockets::WebsocketsClient _client;

        public:
          bool Connect(URI* uri);
          bool Connected();
          void Pipe();
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

        void Pipe(uint tick);

        typedef void(*NetworkConnectCallback)(Device*);
        Device* OnNetworkConnect(NetworkConnectCallback callback);
        typedef void(*ProtocolConnectCallback)(Device*);
        Device* OnProtocolConnect(ProtocolConnectCallback callback);

        //void Command();

    private:
        INetwork* _network;
        bool _networkConnected;
        IProtocol* _protocol;
        bool _protocolConnected;
        URI* _uri;
        String _id;
        String _passphrase;
        NetworkConnectCallback _onNetworkConnect;
        ProtocolConnectCallback _onProtocolConnect;
    };
  }
}