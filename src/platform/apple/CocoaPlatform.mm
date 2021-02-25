
#include <platform/Platform.h>

#include "CocoaPlatform.h"
#include "math/Math.h"
#include "platform/render/metal/MetalFrameBuffer.h"
#include "Core/SceneManager.h"

#ifdef PLATFORM_MACOS

namespace GLaDOS {
  NSApplication* CocoaPlatform::applicationInstance = nullptr;
  CocoaPlatform* CocoaPlatform::cocoaPlatformInstance = nullptr;

  CocoaPlatform::CocoaPlatform() {
  }

  CocoaPlatform::~CocoaPlatform() {
    CVDisplayLinkStop(mDisplayLink);
    CVDisplayLinkRelease(mDisplayLink);
  }

  bool CocoaPlatform::initialize(const PlatformParams& params) {
    @autoreleasepool {
      LOG_TRACE("Initialize Cocoa Platform...");

      if (params.width <= 0 || params.height <= 0) {
        LOG_ERROR("Platform width and height should not be less than 0.");
        return false;
      }

      if (!MetalRenderer::getInstance()->initialize()) {
        LOG_ERROR("MetalRenderer initialize failed.");
        return false;
      }

      // create contentView
      mContentView = [[ContentView alloc] initWithFrame:makeViewRect(params.width, params.height, params.isFullscreen)];
      [mContentView setWantsLayer:YES];  // you must still call the setWantsLayer: method to let the view know that it should use layers.
      [mContentView setLayer:MetalRenderer::getInstance()->getMetalLayer()];
      Platform::getInstance()->mMainFrameBuffer = MetalRenderer::getInstance()->createFrameBuffer();
      updateTrackingAreas();

      // create cocoa application
      applicationInstance = [NSApplication sharedApplication];
      [applicationInstance setDelegate:mContentView];
      initCursorMode(params.isShowCursor);
      // 해당 코드가 없을시 windowDelegate 의 windowWillClose, windowDidDeminiaturize 콜백 함수가 호출되지 않음
      [applicationInstance finishLaunching];

      // create window
      NSWindowStyleMask windowStyle = Platform::getInstance()->mIsFullScreen ? NSWindowStyleMaskBorderless : CocoaPlatform::makeWindowStyle(params.windowStyle);
      mWindow = [[CocoaWindow alloc] initWithContentRect:[mContentView frame] styleMask:windowStyle backing:NSBackingStoreBuffered defer:YES];
      Platform::getInstance()->setTitleName(params.titleName);
      [mWindow setOpaque:YES];
      [mWindow setContentView:mContentView];
      [mWindow setDelegate:mContentView];
      [mWindow makeMainWindow];
      [mWindow makeFirstResponder:nil];
      if (Platform::getInstance()->mIsFullScreen) {
        [mWindow setLevel:NSMainMenuWindowLevel + 1];
        [mWindow setHidesOnDeactivate:YES];
      } else {
        [mWindow setLevel:NSNormalWindowLevel];
        [mWindow setHidesOnDeactivate:NO];
      }
      [mWindow makeKeyAndOrderFront:nil];
      [mWindow orderFrontRegardless];

      // 초기 backingScaleFactor 셋팅
      [MetalRenderer::getInstance()->getMetalLayer() setContentsScale:[mWindow backingScaleFactor]];
      Platform::getInstance()->mContentScale = static_cast<real>([mWindow backingScaleFactor]);

      CVDisplayLinkCreateWithActiveCGDisplays(&mDisplayLink);
      CVDisplayLinkSetOutputCallback(mDisplayLink, &displayLinkCb, nullptr);
      CVDisplayLinkSetCurrentCGDisplay(mDisplayLink, 0);
      CVDisplayLinkStart(mDisplayLink);
    }

    return true;
  }

  NSRect CocoaPlatform::makeViewRect(int width, int height, bool isFullScreen) {
    Platform::getInstance()->mIsFullScreen = isFullScreen;
    NSRect contentSize;
    if (Platform::getInstance()->mIsFullScreen) {
      NSRect screenRect = [[NSScreen mainScreen] frame];
      Platform::getInstance()->mWidth = static_cast<int>(screenRect.size.width);
      Platform::getInstance()->mHeight = static_cast<int>(screenRect.size.height);
      contentSize = NSMakeRect(0, 0, Platform::getInstance()->mWidth, Platform::getInstance()->mHeight);
    } else {
      Platform::getInstance()->mWidth = width;
      Platform::getInstance()->mHeight = height;
      const auto [x, y] = CocoaPlatform::centerOfScreen();
      contentSize = NSMakeRect(x, y, Platform::getInstance()->mWidth, Platform::getInstance()->mHeight);
    }
    Platform::getInstance()->mLastWidth = Platform::getInstance()->mWidth;
    Platform::getInstance()->mLastHeight = Platform::getInstance()->mHeight;
    return contentSize;
  }

  void CocoaPlatform::initCursorMode(bool isShowCursor) {
    if (isShowCursor) {
      Platform::getInstance()->mCursorMode = CursorMode::Show;
    } else {
      Platform::getInstance()->mCursorMode = CursorMode::Hidden;
    }
    Platform::getInstance()->showCursor(isShowCursor);
  }

  //////////////////////////////////////////////////////////////
  //// CocoaPlatform delegate methods definition
  //////////////////////////////////////////////////////////////

  void CocoaPlatform::keyDown(unsigned short keycode) {
    KeyCode localKey = Platform::getInstance()->getKeyCode(keycode);
    Platform::getInstance()->setKeyDown(localKey);
  }

  void CocoaPlatform::keyUp(unsigned short keycode) {
    KeyCode localKey = Platform::getInstance()->getKeyCode(keycode);
    Platform::getInstance()->setKeyUp(localKey);
  }

  void CocoaPlatform::mouseMoved(NSPoint point) {
    Platform::getInstance()->mMousePosition.x = static_cast<real>(point.x);
    Platform::getInstance()->mMousePosition.y = static_cast<real>(point.y);
  }

  void CocoaPlatform::scrollWheel(CGFloat deltaX, CGFloat deltaY, bool precise) {
    if (precise) {
      deltaY *= 0.1;
    }

    if (Math::abs(deltaY) > 0.0) {
      Platform::getInstance()->mMousePosition.z = static_cast<real>(deltaY);
    }
  }

  void CocoaPlatform::mouseDown() {
    Platform::getInstance()->leftMouseDown(true);
  }

  void CocoaPlatform::mouseUp() {
    Platform::getInstance()->leftMouseDown(false);
  }

  void CocoaPlatform::rightMouseDown() {
    Platform::getInstance()->rightMouseDown(true);
  }

  void CocoaPlatform::rightMouseUp() {
    Platform::getInstance()->rightMouseDown(false);
  }

  void CocoaPlatform::otherMouseDown() {
    Platform::getInstance()->middleMouseDown(true);
  }

  void CocoaPlatform::otherMouseUp() {
    Platform::getInstance()->middleMouseDown(false);
  }

  void CocoaPlatform::mouseExited() {
    LOG_TRACE("mouse exited from window");
    if (Platform::getInstance()->mCursorMode == GLaDOS::CursorMode::Hidden) {
      Platform::getInstance()->showCursor(true);
    }
  }

  void CocoaPlatform::mouseEntered() {
    LOG_TRACE("mouse entered from window");
    if (Platform::getInstance()->mCursorMode == GLaDOS::CursorMode::Hidden) {
      Platform::getInstance()->showCursor(false);
    }
  }

  void CocoaPlatform::updateTrackingAreas() {
    LOG_TRACE("updateTrackingAreas");
    // To remove out of date tracking areas and add recomputed tracking areas.
    if (mTrackingArea != nullptr) {
      [mContentView removeTrackingArea:mTrackingArea];
      [mTrackingArea release];
    }

    const NSTrackingAreaOptions options = NSTrackingMouseEnteredAndExited |
                                          NSTrackingActiveInKeyWindow |
                                          NSTrackingEnabledDuringMouseDrag |
                                          NSTrackingMouseMoved |
                                          NSTrackingCursorUpdate |
                                          NSTrackingInVisibleRect |
                                          NSTrackingAssumeInside;
    mTrackingArea = [[NSTrackingArea alloc] initWithRect:[mContentView bounds] options:options owner:mContentView userInfo:nil];
    [mContentView addTrackingArea:mTrackingArea];
  }

  void CocoaPlatform::windowDidResize() {
    LOG_TRACE("windowDidResize");
    CGRect frame = [mWindow frame];
    CGRect contentRect = [mWindow contentRectForFrameRect:frame];
    Platform::getInstance()->mLastWidth = Platform::getInstance()->mWidth;
    Platform::getInstance()->mLastHeight = Platform::getInstance()->mHeight;
    Platform::getInstance()->mWidth = static_cast<int>(contentRect.size.width);
    Platform::getInstance()->mHeight = static_cast<int>(contentRect.size.height);
    CGFloat scaleFactor = static_cast<CGFloat>(Platform::getInstance()->mContentScale);
    [MetalRenderer::getInstance()->getMetalLayer() setDrawableSize:NSMakeSize(contentRect.size.width * scaleFactor, contentRect.size.height * scaleFactor)];
  }

  void CocoaPlatform::windowDidMiniaturize() {
    // TODO: 윈도우를 최소화 할 때 호출 되는 콜백
    LOG_TRACE("window miniaturized");
  }

  void CocoaPlatform::windowDidDeminiaturize() {
    // TODO: 윈도우를 최소화를 되돌릴 때 호출 되는 콜백
    LOG_TRACE("window deminiaturized");
  }

  void CocoaPlatform::windowDidMove() {
    // TODO: 윈도우 이동시 호출 되는 콜백
    LOG_TRACE("window moved");
  }

  void CocoaPlatform::windowDidChangeBackingProperties(CGFloat scaleFactor) {
    LOG_TRACE("windowDidChangeBackingProperties");
    // 레티나 디스플레이 scaleFactor 변화를 감지하여 Metal drawable와 MetalLayer의 contentScale을 갱신한다.
    CGFloat oldScaleFactor = static_cast<CGFloat>(Platform::getInstance()->mContentScale);
    if (scaleFactor != oldScaleFactor) {
      Platform::getInstance()->mContentScale = static_cast<real>(scaleFactor);
      CAMetalLayer* metalLayer = MetalRenderer::getInstance()->getMetalLayer();
      [metalLayer setContentsScale:scaleFactor];
      CGRect bounds = metalLayer.bounds;
      [metalLayer setDrawableSize:NSMakeSize(bounds.size.width * scaleFactor, bounds.size.height * scaleFactor)];
    }
  }

  void CocoaPlatform::applicationWillBecomeActive() {
    Platform::getInstance()->mIsFocused = true;
  }

  void CocoaPlatform::applicationWillResignActive() {
    Platform::getInstance()->mIsFocused = false;
  }

  void CocoaPlatform::applicationWillFinishLaunching() {
    [applicationInstance setActivationPolicy:NSApplicationActivationPolicyRegular];
    [applicationInstance setPresentationOptions:NSApplicationPresentationDefault];
    CocoaPlatform::createMenuBar();
  }

  void CocoaPlatform::applicationDidFinishLaunching() {
    [applicationInstance activateIgnoringOtherApps:YES];
  }

  void CocoaPlatform::windowShouldClose() {
    // 윈도우의 빨간색 x 버튼을 클릭시 호출되는 콜백
    Platform::getInstance()->quit();
  }

  void CocoaPlatform::windowWillClose() {
    // 윈도우를 강제종료 할 때 호출되는 콜백
    if (Platform::getInstance()->isRunning()) {
      Platform::getInstance()->quit();
    }
  }

  void CocoaPlatform::windowDidChangeOcclusionState() {
    if ([[applicationInstance mainWindow] occlusionState] & NSWindowOcclusionStateVisible) {
      Platform::getInstance()->mIsOccluded = false;
    } else {
      Platform::getInstance()->mIsOccluded = true;
    }
  }

  void CocoaPlatform::applicationShouldTerminate() {
    // cocoa 메뉴의 quit 버튼을 클릭할때 호출되는 콜백
    if (Platform::getInstance()->isRunning()) {
      Platform::getInstance()->quit();
    }
  }

  //////////////////////////////////////////////////////////////
  //// CocoaPlatform static methods definition
  //////////////////////////////////////////////////////////////

  NSWindowStyleMask CocoaPlatform::makeWindowStyle(WindowStyle windowStyle) {
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

  std::pair<int, int> CocoaPlatform::centerOfScreen() {
    const auto [w, h] = CocoaPlatform::getScreenSize();
    return std::make_pair((w - Platform::getInstance()->mWidth) / 2, (h - Platform::getInstance()->mHeight) / 2);
  }

  std::pair<int, int> CocoaPlatform::getScreenSize() {
    int width = static_cast<int>([[NSScreen mainScreen] frame].size.width);
    int height = static_cast<int>([[NSScreen mainScreen] frame].size.height);
    return std::make_pair(width, height);
  }

  void CocoaPlatform::createMenuBar() {
    @autoreleasepool {
      NSString* appName = CocoaPlatform::toString(Platform::getInstance()->titleName());
      NSMenu* menuBar = [NSMenu new];
      NSMenuItem* appMenuItem = [menuBar addItemWithTitle:@"" action:NULL keyEquivalent:@""];
      [applicationInstance setMainMenu:menuBar];

      NSMenu* appMenu = [NSMenu new];
      [appMenuItem setSubmenu:appMenu];

      // About menu item
      [appMenu addItemWithTitle:[NSString stringWithFormat:@"About %@", appName] action:@selector(orderFrontStandardAboutPanel:) keyEquivalent:@""];
      // Hide menu item
      [appMenu addItemWithTitle:[NSString stringWithFormat:@"Hide %@", appName] action:@selector(hide:) keyEquivalent:@"h"];
      // Hide Others menu item
      [[appMenu addItemWithTitle:@"Hide Others" action:@selector(hideOtherApplications:) keyEquivalent:@"h"]
          setKeyEquivalentModifierMask:(NSEventModifierFlagOption | NSEventModifierFlagCommand)];
      // show all menu item
      [appMenu addItemWithTitle:@"Show All" action:@selector(unhideAllApplications:) keyEquivalent:@""];
      // quit menu item
      [appMenu addItem:[NSMenuItem separatorItem]];
      [appMenu addItemWithTitle:[NSString stringWithFormat:@"Quit %@", appName] action:@selector(terminate:) keyEquivalent:@"q"];

      SEL setAppleMenuSelector = NSSelectorFromString(@"setAppleMenu:");
      [applicationInstance performSelector:setAppleMenuSelector withObject:appMenu];
    }
  }

  NSString* CocoaPlatform::toString(const std::string& str) {
    return [NSString stringWithCString:str.c_str() encoding:[NSString defaultCStringEncoding]];
  }

  CVReturn CocoaPlatform::displayLinkCb(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext) {
    Platform::getInstance()->render();
    return kCVReturnSuccess;
  }

  CocoaPlatform* CocoaPlatform::getCocoaPlatform() {
    return cocoaPlatformInstance;
  }

  //////////////////////////////////////////////////////////////
  //// Platform definition
  //////////////////////////////////////////////////////////////

  Platform::Platform() {
    CocoaPlatform::cocoaPlatformInstance = NEW_T(CocoaPlatform);
    registerKeyMap();
  }

  Platform::~Platform() {
    DELETE_T(CocoaPlatform::cocoaPlatformInstance, CocoaPlatform);
    DELETE_T(mMainFrameBuffer, FrameBuffer);
  }

  void Platform::registerKeyMap() {
    LOG_TRACE("Register MacOS key map...");
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
    mMainFrameBuffer->begin();
    SceneManager::getInstance()->render();
    mMainFrameBuffer->end();
  }

  void Platform::update() {
    // 큐잉 지연 후 큐에서 이벤트를 패치하는 로직, CPU 100% 를 막기위해 사용된다.
    // wait event
//    NSEvent* eventFuture = [CocoaPlatform::applicationInstance nextEventMatchingMask:NSEventMaskAny
//                                                                           untilDate:[NSDate distantFuture]
//                                                                              inMode:NSDefaultRunLoopMode
//                                                                             dequeue:YES];
//    [CocoaPlatform::applicationInstance sendEvent:eventFuture];

    // polling event
    for (;;) {
      NSEvent* eventPast = [CocoaPlatform::applicationInstance nextEventMatchingMask:NSEventMaskAny
                                                                           untilDate:[NSDate distantPast]
                                                                              inMode:NSDefaultRunLoopMode
                                                                             dequeue:YES];
      if (eventPast == nil) break;
      [CocoaPlatform::applicationInstance sendEvent:eventPast];
    }
  }

  void Platform::setViewport(int width, int height) {
    // TODO: 테스트 필요
    if (mWidth == width && mHeight == height) return;
    NSRect contentRect = [CocoaPlatform::cocoaPlatformInstance->mWindow contentRectForFrameRect:[CocoaPlatform::cocoaPlatformInstance->mWindow frame]];
    contentRect.origin.y += contentRect.size.height;  // origin.y is top Y coordinate now
    contentRect.origin.y -= height;                   // new Y coordinate for the origin
    contentRect.size = CGSizeMake(static_cast<CGFloat>(width), static_cast<CGFloat>(height));
    [CocoaPlatform::cocoaPlatformInstance->mWindow setFrame:contentRect display:YES];
    mLastWidth = mWidth;
    mLastHeight = mHeight;
    mWidth = width;
    mHeight = height;
  }

  void Platform::setTitleName(const std::string& titleName) {
    if (mTitleName == titleName) return;
    [CocoaPlatform::cocoaPlatformInstance->mWindow setTitle:CocoaPlatform::toString(titleName)];
    mTitleName = titleName;
  }

  void Platform::showCursor(bool isShowCursor) {
    mIsShowCursor = isShowCursor;
    if (isShowCursor) {
      [NSCursor unhide];
      return;
    }
    [NSCursor hide];
  }

  void Platform::fullScreen(bool isFullScreen) {
    // TODO: 테스트 필요
    // set window viewport
    NSRect frame;
    if (isFullScreen) {
      frame = [[NSScreen mainScreen] frame];
    } else {
      const auto [x, y] = CocoaPlatform::centerOfScreen();
      frame = NSMakeRect(x, y, mWidth, mHeight);
    }
    setViewport(static_cast<int>(frame.size.width), static_cast<int>(frame.size.height));

    // set window options
    NSWindow* window = CocoaPlatform::cocoaPlatformInstance->mWindow;
    if (isFullScreen) {
      [window orderOut:nil];
      [window setStyleMask:CocoaPlatform::makeWindowStyle(WindowStyle::Borderless)];
      [window makeKeyAndOrderFront:nil];
      [window setLevel:NSMainMenuWindowLevel + 1];
      [window setOpaque:YES];
      [window setHidesOnDeactivate:YES];
      // TODO
      //      [window toggleFullScreen:window];
    } else {
      [window orderOut:nil];
      [window setStyleMask:CocoaPlatform::makeWindowStyle(EnumConstant::defaultWindowStyle)];
      [window makeKeyAndOrderFront:nil];
      [window setLevel:NSNormalWindowLevel];
      [window setOpaque:YES];
      [window setHidesOnDeactivate:NO];
    }
    mIsFullScreen = isFullScreen;
  }

  Renderer* Platform::getRenderer() {
    return MetalRenderer::getInstance();
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
  GLaDOS::CocoaPlatform::getCocoaPlatform()->keyDown([event keyCode]);
}

- (void)keyUp:(NSEvent*)event {
  GLaDOS::CocoaPlatform::getCocoaPlatform()->keyUp([event keyCode]);
}
@end

@implementation ContentView
- (void)mouseMoved:(NSEvent*)event {
  NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
  GLaDOS::CocoaPlatform::getCocoaPlatform()->mouseMoved(point);
}

- (void)mouseDragged:(NSEvent*)event {
  [self mouseMoved:event];
}

- (void)scrollWheel:(NSEvent*)event {
  GLaDOS::CocoaPlatform::getCocoaPlatform()->scrollWheel([event scrollingDeltaX], [event scrollingDeltaY], [event hasPreciseScrollingDeltas]);
}

- (void)mouseDown:(NSEvent*)event {
  GLaDOS::CocoaPlatform::getCocoaPlatform()->mouseDown();
}

- (void)mouseUp:(NSEvent*)event {
  GLaDOS::CocoaPlatform::getCocoaPlatform()->mouseUp();
}

- (void)rightMouseDown:(NSEvent*)event {
  GLaDOS::CocoaPlatform::getCocoaPlatform()->rightMouseDown();
}

- (void)rightMouseUp:(NSEvent*)event {
  GLaDOS::CocoaPlatform::getCocoaPlatform()->rightMouseUp();
}

- (void)otherMouseDown:(NSEvent*)event {
  GLaDOS::CocoaPlatform::getCocoaPlatform()->otherMouseDown();
}

- (void)otherMouseUp:(NSEvent*)event {
  GLaDOS::CocoaPlatform::getCocoaPlatform()->otherMouseUp();
}

- (void)mouseExited:(NSEvent*)event {
  GLaDOS::CocoaPlatform::getCocoaPlatform()->mouseExited();
}

- (void)mouseEntered:(NSEvent*)event {
  GLaDOS::CocoaPlatform::getCocoaPlatform()->mouseEntered();
}

- (void)updateTrackingAreas {
  GLaDOS::CocoaPlatform::getCocoaPlatform()->updateTrackingAreas();
  [super updateTrackingAreas];
}

- (void)windowDidResize:(NSNotification*)notification {
  GLaDOS::CocoaPlatform::getCocoaPlatform()->windowDidResize();
}

- (void)windowDidMiniaturize:(NSNotification*)notification {
  GLaDOS::CocoaPlatform::getCocoaPlatform()->windowDidMiniaturize();
}

- (void)windowDidDeminiaturize:(NSNotification*)notification {
  GLaDOS::CocoaPlatform::getCocoaPlatform()->windowDidDeminiaturize();
}

- (void)windowDidMove:(NSNotification*)notification {
  GLaDOS::CocoaPlatform::getCocoaPlatform()->windowDidMove();
}

- (void)windowDidChangeBackingProperties:(NSNotification*)notification {
  // https://developer.apple.com/library/archive/documentation/GraphicsAnimation/Conceptual/HighResolutionOSX/CapturingScreenContents/CapturingScreenContents.html#//apple_ref/doc/uid/TP40012302-CH10-SW20
  NSWindow* theWindow = (NSWindow*)[notification object];
  CGFloat newBackingScaleFactor = [theWindow backingScaleFactor];
  GLaDOS::CocoaPlatform::getCocoaPlatform()->windowDidChangeBackingProperties(newBackingScaleFactor);
}

- (void)applicationWillBecomeActive:(NSNotification*)notification {
  GLaDOS::CocoaPlatform::getCocoaPlatform()->applicationWillBecomeActive();
}

- (void)applicationWillResignActive:(NSNotification*)notification {
  GLaDOS::CocoaPlatform::getCocoaPlatform()->applicationWillResignActive();
}

- (void)applicationWillFinishLaunching:(NSNotification*)notification {
  GLaDOS::CocoaPlatform::getCocoaPlatform()->applicationWillFinishLaunching();
}

- (void)applicationDidFinishLaunching:(NSNotification*)notification {
  GLaDOS::CocoaPlatform::getCocoaPlatform()->applicationDidFinishLaunching();
}

- (BOOL)windowShouldClose:(id)sender {
  GLaDOS::CocoaPlatform::getCocoaPlatform()->windowShouldClose();
}

- (void)windowWillClose:(NSNotification*)notification {
  GLaDOS::CocoaPlatform::getCocoaPlatform()->windowWillClose();
}

- (void)windowDidChangeOcclusionState:(NSNotification*)notification {
  GLaDOS::CocoaPlatform::getCocoaPlatform()->windowDidChangeOcclusionState();
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication*)aNotification {
  GLaDOS::CocoaPlatform::getCocoaPlatform()->applicationShouldTerminate();
  return NSTerminateCancel;
}
@end

#endif