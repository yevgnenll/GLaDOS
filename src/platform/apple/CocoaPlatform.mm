#include "CocoaPlatform.h"
#include <iostream>
#include "utils/Utility.h"

#ifdef PLATFORM_MACOS

namespace GameEngine {
  NSApplication* CocoaPlatform::applicationInstance = nullptr;
  CocoaPlatform* CocoaPlatform::cocoaPlatformInstance = nullptr;
  Platform* CocoaPlatform::platformInstance = nullptr;
  Platform* Platform::instance = nullptr;

  CocoaPlatform::CocoaPlatform() {
    mAutoReleasePool = [[NSAutoreleasePool alloc] init];
  }

  CocoaPlatform::~CocoaPlatform() {
    CVDisplayLinkStop(mDisplayLink);
    CVDisplayLinkRelease(mDisplayLink);
    [mMetalCommandQueue release];
    [mMetalDevice release];
    [mAutoReleasePool drain];
  }

  bool CocoaPlatform::initialize(const PlatformParams& params) {
    mMetalDevice = MTLCreateSystemDefaultDevice();
    if (mMetalDevice == nullptr) {
      std::cout << "System does not support metal.\n";
      return false;
    }

    mMetalCommandQueue = [mMetalDevice newCommandQueue];
    if (mMetalCommandQueue == nullptr) {
      std::cout << "System does not support metal.\n";
      return false;
    }

    applicationInstance = [NSApplication sharedApplication];
    [applicationInstance setActivationPolicy:NSApplicationActivationPolicyRegular];
    [applicationInstance activateIgnoringOtherApps:YES];

    if (params.width <= 0 || params.height <= 0) {
      std::cout << "platform width and height should not be less than 0.\n";
      return false;
    }

    platformInstance->mMultisample = params.multisample;
    platformInstance->mWindowStyle = params.windowStyle;
    platformInstance->mIsFullScreen = params.isFullscreen;
    CocoaPlatform::platformInstance->setIsShowCursor(params.isShowCursor);
    NSRect contentSize;
    if (platformInstance->mIsFullScreen) {
      NSRect screenRect = [[NSScreen mainScreen] frame];
      platformInstance->mWidth = static_cast<int>(screenRect.size.width);
      platformInstance->mHeight = static_cast<int>(screenRect.size.height);
      contentSize = NSMakeRect(0, 0, platformInstance->mWidth, platformInstance->mHeight);
    } else {
      platformInstance->mWidth = params.width;
      platformInstance->mHeight = params.height;
      const auto [x, y] = centerOfScreen();
      contentSize = NSMakeRect(x, y, platformInstance->mWidth, platformInstance->mHeight);
    }
    platformInstance->mLastWidth = platformInstance->mWidth;
    platformInstance->mLastHeight = platformInstance->mHeight;
    mMetalView = [[MetalView alloc] initWithFrame:contentSize];
    [mMetalView setWantsLayer:YES];  // you must still call the setWantsLayer: method to let the view know that it should use layers.
    mMetalLayer = [CAMetalLayer layer];
    if (mMetalLayer == nullptr) {
      std::cout << "System does not support metal.\n";
      return false;
    }
    mMetalLayer.device = mMetalDevice;
    mMetalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    [mMetalView setLayer:mMetalLayer];
    [applicationInstance setDelegate:mMetalView];

    // make platform
    NSWindowStyleMask windowStyle = platformInstance->mIsFullScreen ? NSWindowStyleMaskBorderless : makeWindowStyle(params.windowStyle);
    mWindow = [[CocoaWindow alloc] initWithContentRect:contentSize styleMask:windowStyle backing:NSBackingStoreBuffered defer:YES];
    CocoaPlatform::platformInstance->setTitleName(params.titleName);
    [mWindow setOpaque:YES];
    [mWindow setContentView:mMetalView];
    [mWindow setDelegate:mMetalView];
    [mWindow makeMainWindow];
    [mWindow makeFirstResponder:nil];
    if (platformInstance->mIsFullScreen) {
      [mWindow setLevel:NSMainMenuWindowLevel + 1];
      [mWindow setHidesOnDeactivate:YES];
    } else {
      [mWindow setLevel:NSNormalWindowLevel];
      [mWindow setHidesOnDeactivate:NO];
    }

    [mWindow makeKeyAndOrderFront:nil];
    [mWindow orderFrontRegardless];

    CVDisplayLinkCreateWithActiveCGDisplays(&mDisplayLink);
    CVDisplayLinkSetOutputCallback(mDisplayLink, &displayLinkCb, nullptr);
    CVDisplayLinkSetCurrentCGDisplay(mDisplayLink, 0);
    CVDisplayLinkStart(mDisplayLink);

    return true;
  }

  NSWindowStyleMask CocoaPlatform::makeWindowStyle(WindowStyle windowStyle) const {
    NSWindowStyleMask style = NSWindowStyleMaskBorderless;
    if (windowStyle & WindowStyle::Resizable) {
      style |= NSWindowStyleMaskResizable;
    }
    if (windowStyle & WindowStyle::TitleBar) {
      style |= NSWindowStyleMaskTitled;
    }
    if (windowStyle & WindowStyle::Closable) {
      style |= NSWindowStyleMaskClosable;
    }
    if (windowStyle & WindowStyle::Maximizable) {
      // no effect for cocoa sdk
    }
    if (windowStyle & WindowStyle::Minimizable) {
      style |= NSWindowStyleMaskMiniaturizable;
    }
    if (windowStyle & WindowStyle::Borderless) {
      style |= NSWindowStyleMaskBorderless;
    }

    return style;
  }

  std::pair<int, int> CocoaPlatform::centerOfScreen() const {
    const auto [w, h] = getScreenSize();
    return std::make_pair((w - platformInstance->mWidth) / 2, (h - platformInstance->mHeight) / 2);
  }

  std::pair<int, int> CocoaPlatform::getScreenSize() const {
    int width = static_cast<int>([[NSScreen mainScreen] frame].size.width);
    int height = static_cast<int>([[NSScreen mainScreen] frame].size.height);
    return std::make_pair(width, height);
  }

  NSString* CocoaPlatform::toString(const std::string& str) {
    return [NSString stringWithCString:str.c_str() encoding:[NSString defaultCStringEncoding]];
  }

  CVReturn CocoaPlatform::displayLinkCb(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext) {
    CocoaPlatform::platformInstance->render();
    return kCVReturnSuccess;
  }

  NSApplication* CocoaPlatform::getAppInstance() {
    return applicationInstance;
  }

  Platform* CocoaPlatform::getPlatformInstance() {
    return platformInstance;
  }

  Platform::Platform() {
    Platform::instance = this;
    CocoaPlatform::platformInstance = Platform::instance;
    registerKeyMap();
    CocoaPlatform::cocoaPlatformInstance = NEW_T(CocoaPlatform);
  }

  Platform::~Platform() {
    DELETE_T(CocoaPlatform::cocoaPlatformInstance, CocoaPlatform);
  }

  void Platform::registerKeyMap() {
    for (int i = 0; i < static_cast<int>(KeyCode::KEY_MAX); i++) {
      mKeys[i] = false;
      mLocalKeymap[i] = KeyCode::KEY_UNDEFINED;
    }

    for (bool& mMouseButton : mMouseButtons) {
      mMouseButton = false;
    }

    {  // optimized to Apple Extended Keyboard II (domestic layout)
      mLocalKeymap[0] = KeyCode::KEY_A;
      mLocalKeymap[1] = KeyCode::KEY_S;
      mLocalKeymap[2] = KeyCode::KEY_D;
      mLocalKeymap[3] = KeyCode::KEY_F;
      mLocalKeymap[4] = KeyCode::KEY_H;
      mLocalKeymap[5] = KeyCode::KEY_G;
      mLocalKeymap[6] = KeyCode::KEY_Z;
      mLocalKeymap[7] = KeyCode::KEY_X;
      mLocalKeymap[8] = KeyCode::KEY_C;
      mLocalKeymap[9] = KeyCode::KEY_V;
      mLocalKeymap[10] = KeyCode::KEY_UNDEFINED;  // for historical reason
      mLocalKeymap[11] = KeyCode::KEY_B;
      mLocalKeymap[12] = KeyCode::KEY_Q;
      mLocalKeymap[13] = KeyCode::KEY_W;
      mLocalKeymap[14] = KeyCode::KEY_E;
      mLocalKeymap[15] = KeyCode::KEY_R;
      mLocalKeymap[16] = KeyCode::KEY_Y;
      mLocalKeymap[17] = KeyCode::KEY_T;
      mLocalKeymap[18] = KeyCode::KEY_1;
      mLocalKeymap[19] = KeyCode::KEY_2;
      mLocalKeymap[20] = KeyCode::KEY_3;
      mLocalKeymap[21] = KeyCode::KEY_4;
      mLocalKeymap[22] = KeyCode::KEY_6;
      mLocalKeymap[23] = KeyCode::KEY_5;
      mLocalKeymap[24] = KeyCode::KEY_EQUALS;
      mLocalKeymap[25] = KeyCode::KEY_9;
      mLocalKeymap[26] = KeyCode::KEY_7;
      mLocalKeymap[27] = KeyCode::KEY_MINUS;
      mLocalKeymap[28] = KeyCode::KEY_8;
      mLocalKeymap[29] = KeyCode::KEY_0;
      mLocalKeymap[30] = KeyCode::KEY_RIGHTBRACKET;
      mLocalKeymap[31] = KeyCode::KEY_O;
      mLocalKeymap[32] = KeyCode::KEY_U;
      mLocalKeymap[33] = KeyCode::KEY_LEFTBRACKET;
      mLocalKeymap[34] = KeyCode::KEY_I;
      mLocalKeymap[35] = KeyCode::KEY_P;
      mLocalKeymap[36] = KeyCode::KEY_RETURN;
      mLocalKeymap[37] = KeyCode::KEY_L;
      mLocalKeymap[38] = KeyCode::KEY_J;
      mLocalKeymap[39] = KeyCode::KEY_APOSTROPHE;
      mLocalKeymap[40] = KeyCode::KEY_K;
      mLocalKeymap[41] = KeyCode::KEY_SEMICOLON;
      mLocalKeymap[42] = KeyCode::KEY_BACKSLASH;
      mLocalKeymap[43] = KeyCode::KEY_COMMA;
      mLocalKeymap[44] = KeyCode::KEY_SLASH;
      mLocalKeymap[45] = KeyCode::KEY_N;
      mLocalKeymap[46] = KeyCode::KEY_M;
      mLocalKeymap[47] = KeyCode::KEY_PERIOD;
      mLocalKeymap[48] = KeyCode::KEY_TAB;
      mLocalKeymap[49] = KeyCode::KEY_SPACE;
      mLocalKeymap[50] = KeyCode::KEY_GRAVE_ACCENT;
      mLocalKeymap[51] = KeyCode::KEY_BACKSPACE;
      mLocalKeymap[52] = KeyCode::KEY_KP_ENTER;
      mLocalKeymap[53] = KeyCode::KEY_ESCAPE;
      mLocalKeymap[54] = KeyCode::KEY_RSUPER;
      mLocalKeymap[55] = KeyCode::KEY_LSUPER;
      mLocalKeymap[56] = KeyCode::KEY_LSHIFT;
      mLocalKeymap[57] = KeyCode::KEY_CAPSLOCK;
      mLocalKeymap[58] = KeyCode::KEY_LALT;
      mLocalKeymap[59] = KeyCode::KEY_LCTRL;
      mLocalKeymap[60] = KeyCode::KEY_RSHIFT;
      mLocalKeymap[61] = KeyCode::KEY_RALT;
      mLocalKeymap[62] = KeyCode::KEY_RCTRL;
      mLocalKeymap[63] = KeyCode::KEY_UNDEFINED;  // KEY_RGUI
      mLocalKeymap[64] = KeyCode::KEY_UNDEFINED;  // KEY_UNKNOWN
      mLocalKeymap[65] = KeyCode::KEY_UNDEFINED;  // KEY_KP_PERIOD
      mLocalKeymap[66] = KeyCode::KEY_UNDEFINED;  // KEY_UNKNOWN
      mLocalKeymap[67] = KeyCode::KEY_KP_MULTIPLY;
      mLocalKeymap[68] = KeyCode::KEY_UNDEFINED;  // KEY_UNKNOWN
      mLocalKeymap[69] = KeyCode::KEY_KP_ADD;
      mLocalKeymap[70] = KeyCode::KEY_UNDEFINED;  // KEY_UNKNOWN
      mLocalKeymap[71] = KeyCode::KEY_UNDEFINED;  // KEY_NUMLOCKCLEAR
      mLocalKeymap[72] = KeyCode::KEY_UNDEFINED;  // KEY_VOLUMEUP
      mLocalKeymap[73] = KeyCode::KEY_UNDEFINED;  // KEY_VOLUMEDOWN
      mLocalKeymap[74] = KeyCode::KEY_UNDEFINED;  // KEY_MUTE
      mLocalKeymap[75] = KeyCode::KEY_KP_DIVIDE;
      mLocalKeymap[76] = KeyCode::KEY_KP_ENTER;
      mLocalKeymap[77] = KeyCode::KEY_UNDEFINED;  // KEY_UNKNOWN
      mLocalKeymap[78] = KeyCode::KEY_KP_SUBTRACT;
      mLocalKeymap[79] = KeyCode::KEY_UNDEFINED;  // KEY_UNKNOWN
      mLocalKeymap[80] = KeyCode::KEY_UNDEFINED;  // KEY_UNKNOWN
      mLocalKeymap[81] = KeyCode::KEY_KP_EQUAL;
      mLocalKeymap[82] = KeyCode::KEY_UNDEFINED;  // KEY_KP_0
      mLocalKeymap[83] = KeyCode::KEY_UNDEFINED;  // KEY_KP_1
      mLocalKeymap[84] = KeyCode::KEY_UNDEFINED;  // KEY_KP_2
      mLocalKeymap[85] = KeyCode::KEY_UNDEFINED;  // KEY_KP_3
      mLocalKeymap[86] = KeyCode::KEY_UNDEFINED;  // KEY_KP_4
      mLocalKeymap[87] = KeyCode::KEY_UNDEFINED;  // KEY_KP_5
      mLocalKeymap[88] = KeyCode::KEY_UNDEFINED;  // KEY_KP_6
      mLocalKeymap[89] = KeyCode::KEY_UNDEFINED;  // KEY_KP_7
      mLocalKeymap[90] = KeyCode::KEY_UNDEFINED;  // KEY_UNKNOWN
      mLocalKeymap[91] = KeyCode::KEY_UNDEFINED;  // KEY_KP_8
      mLocalKeymap[92] = KeyCode::KEY_UNDEFINED;  // KEY_KP_9
      mLocalKeymap[93] = KeyCode::KEY_UNDEFINED;  // KEY_INTERNATIONAL3
      mLocalKeymap[94] = KeyCode::KEY_UNDEFINED;  // KEY_INTERNATIONAL1
      mLocalKeymap[95] = KeyCode::KEY_UNDEFINED;  // KEY_KP_COMMA
      mLocalKeymap[96] = KeyCode::KEY_F5;
      mLocalKeymap[97] = KeyCode::KEY_F6;
      mLocalKeymap[98] = KeyCode::KEY_F7;
      mLocalKeymap[99] = KeyCode::KEY_F3;
      mLocalKeymap[100] = KeyCode::KEY_F8;
      mLocalKeymap[101] = KeyCode::KEY_F9;
      mLocalKeymap[102] = KeyCode::KEY_UNDEFINED;  // KEY_LANG2
      mLocalKeymap[103] = KeyCode::KEY_F11;
      mLocalKeymap[104] = KeyCode::KEY_UNDEFINED;  // KEY_LANG1
      mLocalKeymap[105] = KeyCode::KEY_PRINTSCREEN;
      mLocalKeymap[106] = KeyCode::KEY_UNDEFINED;  // KEY_F16
      mLocalKeymap[107] = KeyCode::KEY_UNDEFINED;  // KEY_SCROLLLOCK
      mLocalKeymap[108] = KeyCode::KEY_UNDEFINED;  // KEY_UNKNOWN
      mLocalKeymap[109] = KeyCode::KEY_F10;
      mLocalKeymap[110] = KeyCode::KEY_UNDEFINED;  // KEY_APPLICATION
      mLocalKeymap[111] = KeyCode::KEY_F12;
      mLocalKeymap[112] = KeyCode::KEY_UNDEFINED;  // KEY_UNKNOWN
      mLocalKeymap[113] = KeyCode::KEY_PAUSE;
      mLocalKeymap[114] = KeyCode::KEY_INSERT;
      mLocalKeymap[115] = KeyCode::KEY_HOME;
      mLocalKeymap[116] = KeyCode::KEY_PAGEUP;
      mLocalKeymap[117] = KeyCode::KEY_DELETE;
      mLocalKeymap[118] = KeyCode::KEY_F4;
      mLocalKeymap[119] = KeyCode::KEY_END;
      mLocalKeymap[120] = KeyCode::KEY_F2;
      mLocalKeymap[121] = KeyCode::KEY_PAGEDOWN;
      mLocalKeymap[122] = KeyCode::KEY_F1;
      mLocalKeymap[123] = KeyCode::KEY_LEFT;
      mLocalKeymap[124] = KeyCode::KEY_RIGHT;
      mLocalKeymap[125] = KeyCode::KEY_DOWN;
      mLocalKeymap[126] = KeyCode::KEY_UP;
      mLocalKeymap[127] = KeyCode::KEY_UNDEFINED;  // KEY_POWER
    }
  }

  bool Platform::initialize(const PlatformParams& params) {
    return CocoaPlatform::cocoaPlatformInstance->initialize(params);
  }

  void Platform::render() {
    id<CAMetalDrawable> drawable = [CocoaPlatform::cocoaPlatformInstance->mMetalLayer nextDrawable];
    id<MTLTexture> texture = drawable.texture;

    MTLRenderPassDescriptor* passDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];
    passDescriptor.colorAttachments[0].texture = texture;
    passDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
    passDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
    passDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(1.f, 0.3f, 0.3f, 1.0f);

    id<MTLCommandBuffer> commandBuffer = [CocoaPlatform::cocoaPlatformInstance->mMetalCommandQueue commandBuffer];

    id<MTLRenderCommandEncoder> commandEncoder = [commandBuffer renderCommandEncoderWithDescriptor:passDescriptor];
    [commandEncoder endEncoding];

    [commandBuffer presentDrawable:drawable];
    [commandBuffer commit];
  }

  void Platform::update() {
    NSEvent* event;
    do {
      event = [CocoaPlatform::getAppInstance() nextEventMatchingMask:NSEventMaskAny untilDate:[NSDate distantPast] inMode:NSDefaultRunLoopMode dequeue:YES];
      [CocoaPlatform::getAppInstance() sendEvent:event];
    } while (event != nil);
  }

  void Platform::setWidthHeight(int width, int height) {
    if (mWidth == width && mHeight == height) return;
    NSRect frame = [CocoaPlatform::cocoaPlatformInstance->mWindow frame];
    frame.origin.y += frame.size.height;  // origin.y is top Y coordinate now
    frame.origin.y -= height;             // new Y coordinate for the origin
    frame.size = CGSizeMake(static_cast<CGFloat>(width), static_cast<CGFloat>(height));
    [CocoaPlatform::cocoaPlatformInstance->mWindow setFrame:frame display:YES];
    mWidth = width;
    mHeight = height;
    mLastWidth = mWidth;
    mLastHeight = mHeight;
  }

  void Platform::setTitleName(const std::string& titleName) {
    if (mTitleName == titleName) return;
    [CocoaPlatform::cocoaPlatformInstance->mWindow setTitle:CocoaPlatform::toString(titleName)];
    mTitleName = titleName;
  }

  void Platform::setIsShowCursor(bool isShowCursor) {
    if (mIsShowCursor == isShowCursor) return;
    if (isShowCursor) {
      [NSCursor unhide];
    } else {
      [NSCursor hide];
    }
    mIsShowCursor = isShowCursor;
  }

  void Platform::setIsFullScreen(bool isFullScreen) {
    if (mIsFullScreen == isFullScreen) return;
    NSWindow* window = CocoaPlatform::cocoaPlatformInstance->mWindow;

    if (isFullScreen) {
      NSRect screenRect = [[NSScreen mainScreen] frame];
      [window orderOut:nil];
      [window setStyleMask:NSWindowStyleMaskBorderless];
      [window makeKeyAndOrderFront:nil];
      [window setFrame:screenRect display:YES];
      [window setLevel:NSMainMenuWindowLevel + 1];
      [window setOpaque:YES];
      [window setHidesOnDeactivate:YES];
      mWidth = static_cast<int>(screenRect.size.width);
      mHeight = static_cast<int>(screenRect.size.height);
    } else {
      mWidth = mLastWidth;
      mHeight = mLastHeight;
      const auto [x, y] = CocoaPlatform::cocoaPlatformInstance->centerOfScreen();
      [window setFrame:NSMakeRect(x, y, mWidth, mHeight) display:YES];
      [window orderOut:nil];
      [window setStyleMask:CocoaPlatform::cocoaPlatformInstance->makeWindowStyle(mWindowStyle)];
      [window makeKeyAndOrderFront:nil];
      [window setLevel:NSNormalWindowLevel];
      [window setOpaque:YES];
      [window setHidesOnDeactivate:NO];
    }
    // TODO: destroy and reinitialize metalView
    mIsFullScreen = isFullScreen;
  }
}

@implementation CocoaWindow
- (BOOL)canBecomeMainWindow {
  return YES;
}
- (BOOL)canBecomeKeyWindow {
  return YES;
}
- (BOOL)acceptsFirstResponder {
  return YES;
}

- (void)keyDown:(NSEvent*)event {
  unsigned short keyCode = [event keyCode];
  GameEngine::KeyCode localKey = GameEngine::CocoaPlatform::getPlatformInstance()->getKeyCode(keyCode);
  GameEngine::CocoaPlatform::getPlatformInstance()->setKeyDown(localKey);
}

- (void)keyUp:(NSEvent*)event {
  unsigned short keyCode = [event keyCode];
  GameEngine::KeyCode localKey = GameEngine::CocoaPlatform::getPlatformInstance()->getKeyCode(keyCode);
  GameEngine::CocoaPlatform::getPlatformInstance()->setKeyUp(localKey);
}
@end

@implementation MetalView
- (void)mouseMoved:(NSEvent*)event {
  NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
  std::cout << "[mouseMoved] Mouse pos: " << point.x << ", " << point.y << '\n';
}

- (void)mouseDragged:(NSEvent*)event {
  NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
  std::cout << "[mouseDragged] Mouse pos: " << point.x << ", " << point.y << '\n';
}

- (void)scrollWheel:(NSEvent*)event {
  NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
  std::cout << "[scrollWheel] Mouse wheel at: " << point.x << ", " << point.y << ", Delta: " << [event deltaY] * 10.0 << '\n';
}

- (void)mouseDown:(NSEvent*)event {
  GameEngine::CocoaPlatform::getPlatformInstance()->leftMouseDown(true);
}

- (void)mouseUp:(NSEvent*)event {
  GameEngine::CocoaPlatform::getPlatformInstance()->leftMouseDown(false);
}

- (void)rightMouseDown:(NSEvent*)event {
  GameEngine::CocoaPlatform::getPlatformInstance()->rightMouseDown(true);
}

- (void)rightMouseUp:(NSEvent*)event {
  GameEngine::CocoaPlatform::getPlatformInstance()->rightMouseDown(false);
}

- (void)otherMouseDown:(NSEvent*)event {
  GameEngine::CocoaPlatform::getPlatformInstance()->middleMouseDown(true);
}

- (void)otherMouseUp:(NSEvent*)event {
  GameEngine::CocoaPlatform::getPlatformInstance()->middleMouseDown(false);
}

- (void)windowDidResize:(NSNotification*)notification {
  std::cout << "[windowDidResize]" << '\n';
}

- (void)windowWillMiniaturize:(NSNotification*)notification {
  std::cout << "[windowWillMiniaturize]" << '\n';
}

- (void)windowDidMiniaturize:(NSNotification*)notification {
  std::cout << "[windowDidMiniaturize]" << '\n';
}

- (void)windowDidDeminiaturize:(NSNotification*)notification {
  std::cout << "[windowDidDeminiaturize]" << '\n';
}

- (void)windowWillMove:(NSNotification*)notification {
  std::cout << "[windowWillMove]" << '\n';
}

- (void)windowDidMove:(NSNotification*)notification {
  std::cout << "[windowDidMove]" << '\n';
}

- (void)windowDidChangeScreen:(NSNotification*)notification {
  std::cout << "[windowDidChangeScreen]" << '\n';
}

- (void)applicationWillBecomeActive:(NSNotification*)notification {
  GameEngine::CocoaPlatform::getPlatformInstance()->setIsFocused(true);
}

- (void)applicationWillResignActive:(NSNotification*)notification {
  GameEngine::CocoaPlatform::getPlatformInstance()->setIsFocused(false);
}
@end

#endif