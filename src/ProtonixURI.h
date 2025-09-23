#pragma once

#include <Arduino.h>

namespace Qybercom {
	namespace Protonix {
		class ProtonixURI {
			private:
				String _scheme;
				String _username;
				String _password;
				String _host;
				unsigned int _port;
				String _path;
				String _query;
				String _fragment;

			public:
				ProtonixURI ();
				ProtonixURI (String uri);
				ProtonixURI (String host, unsigned int port);
				ProtonixURI (String host, unsigned int port, String path);

				void Scheme (String scheme);
				String Scheme ();

				void Username (String username);
				String Username ();

				void Password (String password);
				String Password ();

				void Host (String host);
				String Host ();

				void Port (unsigned int port);
				unsigned int Port ();
				unsigned int PortConnection ();

				void Path (String path);
				String Path ();

				void Query (String query);
				String Query ();
				String Query (bool includePath);

				void Fragment (String fragment);
				String Fragment ();

				String URI ();

				bool EmptySocket ();
		};
	}
}