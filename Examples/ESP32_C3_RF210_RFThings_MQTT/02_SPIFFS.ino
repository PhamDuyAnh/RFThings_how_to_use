#define FORMAT_SPIFFS_IF_FAILED true  // uncomment if you want to FORMAT SPIFFS when SPIFFS are not formatted

#include <FS.h>  //this needs to be first, or it all crashes and burns...
#ifdef ESP32
#include <SPIFFS.h>
#endif

bool initSPIFFS() {
  bool c;
  //if (SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
  if (SPIFFS.begin()) {
    c = true;
    dbPrintln("SPIFFS -> Mounted successfully!");
    //SPIFFS.end();
  } else {
    c = false;
    dbPrintln("SPIFFS -> Mounted UNsuccessfully!");
#ifdef FORMAT_SPIFFS_IF_FAILED
    dbPrintln("SPIFFS -> Wait for SPIFFS format...");
    if (SPIFFS.format()) {
      dbPrintln("SPIFFS -> Formatted successfully!");
      c = true;
    } else {
      dbPrintln("SPIFFS ---> ERROR formatting!");
    }
#endif
  }
  dbPrintln("");
  return c;
}