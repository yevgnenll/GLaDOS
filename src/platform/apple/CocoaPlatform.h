#ifndef GAMEENGINE_COCOAPLATFORM_H
#define GAMEENGINE_COCOAPLATFORM_H

#include "platform/OSTypes.h"

#ifdef PLATFORM_MACOS

#import <Cocoa/Cocoa.h>
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>
#include <string>
#import "platform/Platform.h"

@interface CocoaWindow : NSWindow
@end

@interface MetalView : NSView <NSWindowDelegate, NSApplicationDelegate>
@end

namespace GameEngine {
  class Platform;
  class CocoaPlatform {
    friend class Platform;

  public:
    CocoaPlatform();
    ~CocoaPlatform();

    bool initialize(const PlatformParams& params);
    NSWindowStyleMask makeWindowStyle(WindowStyle windowStyle) const;
    std::pair<int, int> centerOfScreen() const;
    std::pair<int, int> getScreenSize() const;
    static NSString* toString(const std::string& str);
    static CVReturn displayLinkCb(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext);
    static NSApplication* getAppInstance();
    static Platform* getPlatformInstance();

  private:
    static NSApplication* applicationInstance;
    static CocoaPlatform* cocoaPlatformInstance;
    static Platform* platformInstance;

    NSWindow* mWindow{nullptr};
    CAMetalLayer* mMetalLayer{nullptr};
    MetalView* mMetalView{nullptr};
    CVDisplayLinkRef mDisplayLink;
    id<MTLDevice> mMetalDevice;
    id<MTLCommandQueue> mMetalCommandQueue;
    NSAutoreleasePool* mAutoReleasePool{nullptr};
  };
}

#endif

#endif
