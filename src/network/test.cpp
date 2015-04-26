

#include <stdio.h>
#include <stropts.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/netdevice.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
  char buf[100];
  readlink("/proc/self/exe", buf, 100);
  string p=((string)buf);
  p.resize(p.size()-6);
  std::cout<<p<<"\n"<<p.size();
}

