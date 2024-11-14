#include <iostream>
#include <queue>
#include <cstdint>
#include <winsock2.h>
#include <windows.h>
#include <vector>
#include "Packet.h" // Ensure this header contains all necessary packet types

CRITICAL_SECTION recvQueueCS;
CRITICAL_SECTION sendQueueCS;

std::queue<void*> RecvQueue; // Generic queue for different packet types
std::queue<ObjectInfo_Packet> SendQueue;

// Function to identify packet type and return the correct size
size_t GetPacketSize(uint8_t packetType) {
    switch (packetType) {
    case 1: return sizeof(Input_Packet);
    case 2: return sizeof(PlayerIDRequest_Packet);
    case 11: return sizeof(GTime_Packet);
    case 12: return sizeof(PlayerIDResponsePacket);
    case 13: return sizeof(ObjectInfo_Packet);
    case 101: return sizeof(EndGame_Packet);
    default: return 0; // Unknown packet type
    }
}

void* ParsePacket(const char* buffer, uint8_t packetType) {
    void* packet = nullptr;
    switch (packetType) {
    case 1: {
        packet = new Input_Packet;
        memcpy(packet, buffer, sizeof(Input_Packet));
        break;
    }
    case 2: {
        packet = new PlayerIDRequest_Packet;
        memcpy(packet, buffer, sizeof(PlayerIDRequest_Packet));
        break;
    }
    case 11: {
        packet = new GTime_Packet;
        memcpy(packet, buffer, sizeof(GTime_Packet));
        break;
    }
    case 12: {
        packet = new PlayerIDResponsePacket;
        memcpy(packet, buffer, sizeof(PlayerIDResponsePacket));
        break;
    }
    case 13: {
        packet = new ObjectInfo_Packet;
        memcpy(packet, buffer, sizeof(ObjectInfo_Packet));
        break;
    }
    case 101: {
        packet = new EndGame_Packet;
        memcpy(packet, buffer, sizeof(EndGame_Packet));
        break;
    }
    default: {
        std::cerr << "Unknown packet type: " << (int)packetType << std::endl;
        break;
    }
    }
    return packet;
}

DWORD WINAPI CommunicationThread(LPVOID lpParam) {
    SOCKET clientSocket = (SOCKET)lpParam;
    char recvBuffer[1024];
    int bytesReceived;

    while (true) {
        // Receive data
        bytesReceived = recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
        if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
            std::cerr << "recv() failed or connection closed: " << WSAGetLastError() << std::endl;
            break;
        }

        // Determine packet type
        uint8_t packetType = recvBuffer[0];
        size_t packetSize = GetPacketSize(packetType);
        if (packetSize == 0 || bytesReceived < packetSize) {
            std::cerr << "Invalid or incomplete packet received." << std::endl;
            continue;
        }

        // Parse the packet
        void* packet = ParsePacket(recvBuffer, packetType);
        if (packet) {
            EnterCriticalSection(&recvQueueCS);
            RecvQueue.push(packet);
            LeaveCriticalSection(&recvQueueCS);
        }
    }
    closesocket(clientSocket);
    return 0;
}