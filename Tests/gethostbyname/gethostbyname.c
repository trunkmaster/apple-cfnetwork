#include <CoreFoundation/CFLogUtilities.h>
#include <CFNetwork/CFNetwork.h>

void ClientCallback(CFHostRef theHost, CFHostInfoType typeInfo, const CFStreamError* error, CFStringRef name)
{
  CFArrayRef addrs;
  Boolean hasBeenResolved;

  CFLog(kCFLogLevelInfo, CFSTR("Client callback called"));  
  if (!error->error) {
    CFLog(kCFLogLevelInfo, CFSTR("Addresses resolution finished without error. Good."));
    addrs = CFHostGetAddressing(theHost, &hasBeenResolved);
    CFShow(addrs);
  }
  CFHostSetClient(theHost, NULL, NULL);
}


int main(int argc, char **argv)
{
  CFHostRef host = CFHostCreateWithName(kCFAllocatorDefault, CFSTR("localhost"));
  CFStreamError error;
  CFRunLoopRef runloop = CFRunLoopGetCurrent();
  CFHostClientContext ctxt = {0, NULL, NULL, NULL, NULL};

  CFLog(kCFLogLevelInfo, CFSTR("Setup resolution process..."));
  CFHostSetClient(host, (CFHostClientCallBack)ClientCallback, &ctxt);
  // CFHostSetClient(host, NULL, NULL);
  CFHostScheduleWithRunLoop(host, runloop, kCFRunLoopDefaultMode);

  CFLog(kCFLogLevelInfo, CFSTR("Starting addresses resolution..."));
  if (CFHostStartInfoResolution(host, kCFHostAddresses, &error) != TRUE) {
    CFLog(kCFLogLevelInfo, CFSTR("Failed to start name resolution"));
    exit(1);
  }

  CFRunLoopRun();
  // CFRunLoopRunInMode(kCFRunLoopCommonModes, 10, TRUE);
}