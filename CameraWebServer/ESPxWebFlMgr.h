#ifndef ESPxWebFlMgr_h
#define ESPxWebFlMgr_h



#include <Arduino.h>
#include <inttypes.h>

// file system default for esp8266 is LittleFS, for ESP32 it is SPIFFS (no time to check...)

#ifdef ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
  #include <FS.h>
  //
  #include <LittleFS.h>
  #define ESPxWebFlMgr_FileSystem LittleFS
  /*
  #include <SPIFFS.h>
  #define ESPxWebFlMgr_FileSystem SPIFFS
  */
#endif

#ifdef ESP32
  #include <WiFi.h>
  #include <WebServer.h>
  #include <FS.h>
  #include <SD_MMC.h> //jz #include <LittleFS.h>
  #define ESPxWebFlMgr_FileSystem SD_MMC //jz #define ESPxWebFlMgr_FileSystem LittleFS
#endif


#ifndef ESPxWebFlMgr_FileSystem
#pragma message ("ESPxWebFlMgr_FileSystem not defined.")
#endif

/* undefine this to save about 10k code space.
   it requires to put the files from "<library>/filemanager" into the FS. No free lunch.
*/
#define fileManagerServerStaticsInternally

// will show the Edit-Button for every file type, even binary and such.
//#define fileManagerEditEverything

class ESPxWebFlMgr {
  private:
    word _Port ;
#ifdef ESP8266
    ESP8266WebServer * fileManager = NULL;
#endif
#ifdef ESP32
    WebServer * fileManager = NULL;
#endif
    bool _ViewSysFiles = false;
    String _SysFileStartPattern = "/.";
    File fsUploadFile;
    String _backgroundColor = "black";

    void fileManagerNotFound(void);
    String dispIntDotted(size_t i); 
    String dispFileString(size_t fs); 
    String CheckFileNameLengthLimit(String fn);

    // the webpage
    void fileManagerIndexpage(void);
    void fileManagerJS(void);
    void fileManagerCSS(void);
    void fileManagerGetBackGround(void);

    // javascript xmlhttp includes
    String colorline(int i);
    String escapeHTMLcontent(String html);
    void fileManagerFileListInsert(void);
    void fileManagerFileEditorInsert(void);
    boolean allowAccessToThisFile(const String filename);
    void fileManagerCommandExecutor(void);
    void fileManagerReceiverOK(void);
    void fileManagerReceiver(void);

    // Zip-File uncompressed/stored
    void getAllFilesInOneZIP(void);
    int WriteChunk(const char* b, size_t l);

    // helper: fs.h from esp32 and esp8266 don't have a compatible solution
    // for getting a file list from a directory
#ifdef ESP32
#define Dir File
#endif
    File nextFile(Dir &dir);
    File firstFile(Dir &dir);
    // and not to get this data about usage...
    size_t totalBytes(void);
    size_t usedBytes(void);

  public:
    ESPxWebFlMgr(word port);
    virtual ~ESPxWebFlMgr();

    void begin();
    void end();
    virtual void handleClient();

    // This must be called before the webpage is loaded in the browser...
    // must be a valid css color name, see https://en.wikipedia.org/wiki/Web_colors
    void setBackGroundColor(const String backgroundColor);

    void setViewSysFiles(bool vsf);
    bool getViewSysFiles(void);

    void setSysFileStartPattern(String sfsp);
    String getSysFileStartPattern(void);
};

#endif
