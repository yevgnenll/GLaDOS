#ifndef GLADOS_COCOAPLATFORM_H
#define GLADOS_COCOAPLATFORM_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#include <Cocoa/Cocoa.h>
#include <string>
#include "platform/Platform.h"

@interface CocoaWindow : NSWindow
@end

@interface ContentView : NSView <NSWindowDelegate, NSApplicationDelegate>
@end

namespace GLaDOS {
  class Platform;
  class CocoaPlatform {
    friend class Platform;

  public:
    CocoaPlatform() = default;
    ~CocoaPlatform();
    CocoaPlatform(const CocoaPlatform&) = delete;
    CocoaPlatform& operator=(const CocoaPlatform&) = delete;

    bool initialize(const PlatformParams& params);

    // delegate methods
    inline void keyDown(unsigned short keycode);
    inline void keyUp(unsigned short keycode);
    inline void viewDidEndLiveResize();
    inline void mouseMoved(NSPoint& point);
    inline void scrollWheel(CGFloat deltaX, CGFloat deltaY, bool precise);
    inline void mouseDown();
    inline void mouseUp();
    inline void rightMouseDown();
    inline void rightMouseUp();
    inline void otherMouseDown();
    inline void otherMouseUp();
    inline void mouseExited();
    inline void mouseEntered();
    inline void updateTrackingAreas();
    inline void windowDidMiniaturize();
    inline void windowDidDeminiaturize();
    inline void windowDidMove();
    inline void windowDidChangeBackingProperties(CGFloat scaleFactor);
    inline void applicationWillBecomeActive();
    inline void applicationWillResignActive();
    inline void applicationWillFinishLaunching();
    inline void applicationDidFinishLaunching();
    inline void windowShouldClose();
    inline void windowWillClose();
    inline void windowDidChangeOcclusionState();
    inline void applicationShouldTerminate();

    inline bool OSVersionIsAtLeast(OSVersion minor);

    // static methods
    static NSWindowStyleMask makeWindowStyle(WindowStyle windowStyle);
    static std::pair<int, int> centerOfScreen();
    static std::pair<int, int> getScreenSize();
    static void createMenuBar();
    static NSString* toString(const std::string& str);
    static CVReturn displayLinkCb(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext);
    static CocoaPlatform* getCocoaPlatform();

  private:
    NSRect makeViewRect(int width, int height, bool isFullScreen);
    void initCursorMode(bool isShowCursor);
    static OSVersion makeOSVersion(uint32_t major, uint32_t minor, uint32_t patch);

    static NSApplication* applicationInstance;
    static CocoaPlatform* cocoaPlatformInstance;

    NSWindow* mWindow{nil};
    ContentView* mContentView{nil};
    CVDisplayLinkRef mDisplayLink{nil};
    NSTrackingArea* mTrackingArea{nil};
    OSVersion mCurrentOSVersion;
  };
}

#endif

#endif
