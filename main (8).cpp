#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;
void errHandler (const char * why, const int exitCode = 1)
{
    cerr << why << endl;
    exit(exitCode);
}

int main(int argc, char **argv)
{
    // Подготовка структуры с адресом клиента
    sockaddr_in * selfAddr = new (sockaddr_in);
    selfAddr->sin_family = AF_INET;      // интернет протокол IPv4
    selfAddr->sin_port = 0;              // Любой порт на усмотрение ОС
    selfAddr->sin_addr.s_addr = 0;       // все адреса компьютера

    // Подготовка структуры с адресом сервера
    sockaddr_in * remoteAddr = new (sockaddr_in);
    remoteAddr->sin_family = AF_INET;
    remoteAddr->sin_port = htons(13);
    remoteAddr->sin_addr.s_addr = inet_addr("82.179.90.69"); // локальный адрес сети

    // Подготовка буфера обмена для передачи и приема данных
    char *buf = new char [256];
    strcpy(buf, "how much time?\n");
    int msgLen = strlen(buf);            

    // II этап. Создание сокета IPv4
    int mySocket = socket(AF_INET, SOCK_STREAM, 0); 
    if (mySocket == -1) {
        errHandler("Error open socket", 11);
    }

    //III этап. Привязка сокета к своему адресу
    int rc = bind(mySocket, (const sockaddr*) selfAddr, sizeof (sockaddr_in));
    if (rc == -1) { // проверка
        close(mySocket);
        errHandler("Error bind socket with local address", 12);
    }

    // IV этап. Установка соединения с сервером
    rc = connect(mySocket, (const sockaddr*) remoteAddr, sizeof(sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        errHandler("Error connect socket with remote server", 13);
    }

    // V этап. Выполнение информационного обмена
    rc = send(mySocket, buf, msgLen, 0);
    if (rc == -1) { // проверка
        close(mySocket);
        errHandler("Error send message", 14);
    }
    cout << "We send: " << buf << endl;
    // Принятие ответа
    rc = recv(mySocket, buf, 256, 0);
    if (rc == -1) { // проверка
        close(mySocket);
        errHandler("Error recevie answer", 15);
    }
    buf[rc]= '\0';
    std::cout<<"We receive: "<< buf << std::endl;

    // VI этап. Закрытие сокета
    close(mySocket);
    // Освобождение памяти
    delete selfAddr;
    delete remoteAddr;
    delete[] buf;
    return 0;
}