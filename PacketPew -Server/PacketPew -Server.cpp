//author: Kushagra

//cpp headers
#include <iostream>
#include <list>

//sfml headers
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

const int SELECTOR_WAIT_TIME_MS = 200;
const int MAX_CLIENTS = 2;

void AddClients(sf::TcpListener& serverListener, list<sf::TcpSocket*>& clients, sf::SocketSelector& selector) {
    auto* client = new sf::TcpSocket();
    client->setBlocking(false);
    sf::Packet welcomePacket;
    string msg;
    if (serverListener.accept(*client) == sf::Socket::Done) {
        if (clients.size() < MAX_CLIENTS) {
            msg = "1";
            welcomePacket << msg;
            if (client->send(welcomePacket) == sf::Socket::Done) {
                printf("A client connected at port %d and address %s\n", client->getRemotePort(), client->getRemoteAddress().toString().data());
                clients.push_back(client);
                std::cout << "Total clients connected " << clients.size() << std::endl;
                selector.add(*client);
            }
        }
        else {
            msg = "-1";
            welcomePacket << msg;
            client->send(welcomePacket);
            client->disconnect();
            delete client;
        }
    }
    else {
        delete client;
    }
}

string RecieveFromClient(sf::TcpSocket& client, list<sf::TcpSocket*>& clients, sf::SocketSelector& selector) {
    sf::Packet clientDataPacket;
    sf::Socket::Status tcpRecvStat = client.receive(clientDataPacket);
    if (tcpRecvStat == sf::Socket::Disconnected) {
        //error...
        cerr << "A client disconnected\n";
        clients.remove(&client);
        selector.remove(client);
        client.disconnect();
        printf("waiting for a new connection\n");
        return "-1";
    }
    string msg;
    clientDataPacket >> msg;
    return msg;
}

void BroadcastToAll(list<sf::TcpSocket*>& clients, string& msg, sf::SocketSelector& selector) {
    sf::Packet sendPacket;
    sendPacket << msg;
    list<sf::TcpSocket*> clientsCopy = clients;
    for (std::list<sf::TcpSocket*>::iterator SendIt = clientsCopy.begin(); SendIt != clientsCopy.end(); ++SendIt) {
        sf::TcpSocket& sendClient = **SendIt;
        sf::Socket::Status tcpSendStat = sendClient.send(sendPacket);
        if (tcpSendStat == sf::Socket::Disconnected) {
            //error...
            cerr << "removed a client from the list\n";
            clients.remove(&sendClient);
            selector.remove(sendClient);
            sendClient.disconnect();
            continue;
        }
    }
}

int main()
{
    //setting project name and version
    string versionNo = "1.0";
    string projectFullName = "PacketPew Server ";
    projectFullName.append(versionNo);

    //setting server variables
    int port = 6969;
    sf::IpAddress serverIP = sf::IpAddress("127.0.0.1"); //TODO: change this in a way that you can connect to the same machines on the network
    sf::TcpListener serverListener;

    //clients
    std::list<sf::TcpSocket*> clients;

    //selector (for non blocking)
    sf::SocketSelector selector;

    // bind the server to a port
    if (serverListener.listen(port, serverIP) != sf::Socket::Done)
    {
        // error...
        cerr << "\nerror in binding the server port";
    }
    else {
        std::string addr = serverIP.toString();
        printf("\nlistening at port %d and address %s\n", port, addr.data());
        selector.add(serverListener);
    }

    while (true) {
        if (selector.wait(sf::milliseconds(SELECTOR_WAIT_TIME_MS))) {
            //handling new connections
            if (selector.isReady(serverListener)) {
                AddClients(serverListener, clients, selector);
            }
            else {
                list<sf::TcpSocket*> clientsCopy = clients;
                for (std::list<sf::TcpSocket*>::iterator it = clientsCopy.begin(); it != clientsCopy.end(); ++it) {
                    sf::TcpSocket& client = **it;
                    if (selector.isReady(client)) {
                        string msg = RecieveFromClient(client, clients, selector);
                        if (msg == "-1") {
                            cerr << "error in recieving from a client\n";
                        }
                        else {
                            BroadcastToAll(clients, msg, selector);
                        }
                    }
                }
            }
        }
    }

    return 0;
}
