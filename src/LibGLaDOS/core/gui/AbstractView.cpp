#include "AbstractView.h"

namespace GLaDOS {
    AbstractView::AbstractView() {
    }

    AbstractView::AbstractView(int width, int height) {
    }

    AbstractView::~AbstractView() {
    }

    Vec2 AbstractView::globalPosition() const {
        return Vec2();
    }

    Vec2 AbstractView::position() const {
        return Vec2();
    }

    Size<int> AbstractView::size() const {
        return Size<int>();
    }

    bool AbstractView::isRefresh() const {
        return false;
    }

    bool AbstractView::isDestroing() const {
        return false;
    }

    int AbstractView::referenceCount() const {
        return 0;
    }

    int AbstractView::subviewCount() const {
        return 0;
    }

    AbstractView* AbstractView::super() const {
        return nullptr;
    }

    AbstractView* AbstractView::firstSubview() const {
        return nullptr;
    }

    AbstractView* AbstractView::lastSubview() const {
        return nullptr;
    }

    std::string AbstractView::name() const {
        return std::string();
    }

    void AbstractView::setRefresh(bool refresh) {
    }

    void AbstractView::setName(const std::string& name) {
    }

    void AbstractView::setPosition(int x, int y) {
    }

    void AbstractView::setPosition(const Vec2& pos) {
    }

    void AbstractView::setSize(int width, int height) {
    }

    void AbstractView::setSize(const Size<int>& size) {
    }

    void AbstractView::resize(int width, int height) {
    }

    void AbstractView::resize(Size<int> size) {
    }

    void AbstractView::moveTo(int x, int y) {
    }

    void AbstractView::moveTo(const Vec2& pos) {
    }

    void AbstractView::sizeUpdate(const AbstractView* source, const AbstractView* target, int width, int height) {
    }
}