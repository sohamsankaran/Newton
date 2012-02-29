//////////////////////////////////////////////////////////////////////////////////
// Ping.c
// This program demonstrates two functions: getting the IP
// number of an http domain, and then pinging the IP number.
//
// This program is written for the Dexter Industries DIWIFI
// More information can be found at www.dexterindustries.com
// More information on the Wifi sensor at http://dexterindustries.com/manual/wifi/
//
// Modify the array "DNS_Name" to get to a specific domain.
///////////////////////////////////////////////////////////////////////////////////

ubyte DNS_Name[] = {'w','w','w','.','s','o','h','.','a','m'};

ubyte IP_num[64];

#include "../drivers/common.h"
#include "New_utils.h"
#include "DIWIFI-Ping.h"

task main(){
  get_IP_address();
  while(true){
    ping_IP();
  }

}
