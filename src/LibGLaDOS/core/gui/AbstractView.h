#ifndef GLADOS_ABSTRACTVIEW_H
#define GLADOS_ABSTRACTVIEW_H

#include <atomic>
#include <string>

#include "core/gui/constant/Constant.h"
#include "math/Size.hpp"
#include "math/Vec2.h"

namespace GLaDOS {
  class AbstractWindow;
  class AbstractView {
  public:
    AbstractView();
    AbstractView(int width, int height);
    virtual ~AbstractView();

    Vec2 globalPosition() const;
    Vec2 position() const;
    Size<int> size() const;
    bool isRefresh() const;
    bool isDestroing() const;
    int referenceCount() const;
    int subviewCount() const;
    AbstractView* super() const;
    AbstractView* firstSubview() const;
    AbstractView* lastSubview() const;
    std::string name() const;

    void setRefresh(bool refresh);
    void setName(const std::string& name);
    void setPosition(int x, int y);
    void setPosition(const Vec2& pos);
    void setSize(int width, int height);
    void setSize(const Size<int>& size);

    void resize(int width, int height);
    void resize(Size<int> size);
    void moveTo(int x, int y);
    void moveTo(const Vec2& pos);

  protected:
    virtual void sizeUpdate(const AbstractView* source, const AbstractView* target, int width, int height);
    virtual Response draw(AbstractWindow* context) = 0;

  private:
    std::atomic_bool mIsRefresh;
    bool mIsDestroying;
    short mViewType;
    int mReferenceCount;
    int mSubviewCount;
    AbstractView* mSuper;
    AbstractView* mPrevious;
    AbstractView* mNext;
    AbstractView* mFirstSubview;
    AbstractView* mLastSubview;
    Vec2 mPosition;
    Size<int> mSize;
    std::string mName;
  };
}  // namespace GLaDOS

#endif  //GLADOS_ABSTRACTVIEW_H
