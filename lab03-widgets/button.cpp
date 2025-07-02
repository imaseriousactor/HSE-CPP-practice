#include "box.hpp"
#include <utility>
#include <vector>

namespace widgets {
[[nodiscard]] int box::width() const {
    return m_width;
}

[[nodiscard]] int box::height() const {
    return m_height;
}

box::box(box::kind kind) : m_kind(kind), m_width(0), m_height(0) {
}

void box::update_layout() {
    int width = 0;
    int height = 0;
    if (m_kind == kind::HORIZONTAL) {
        for (auto &child : children) {
            width += child->width();
            height = std::max(height, child->height());
        }
    } else {
        for (auto &child : children) {
            width = std::max(width, child->width());
            height += child->height();
        }
    }
    m_width = width;
    m_height = height;
}

[[nodiscard]] widget *box::get(int index) const {
    return children[index].get();
}

widget *box::add(std::unique_ptr<widget> child) {
    if (m_kind == kind::HORIZONTAL) {
        m_width += child->width();
        m_height = std::max(m_height, child->height());

    } else {
        m_width = std::max(m_width, child->width());
        m_height += child->height();
    }
    children.push_back(std::move(child));
    return children[children.size() - 1].get();
}

std::unique_ptr<widget> box::remove(int index) {
    std::unique_ptr<widget> child_removed = std::move(children[index]);
    children.erase(children.begin() + index);
    box::update_layout();
    return child_removed;
}

widget *box::child_at(int x, int y) {
    int x_now = 0;
    int y_now = 0;
    for (auto &child : children) {
        if (m_kind == kind::HORIZONTAL) {
            y_now = (m_height - child->height()) / 2;
        } else {
            x_now = (m_width - child->width()) / 2;
        }
        if ((x_now <= x && x < x_now + child->width()) && y_now <= y &&
            y < y_now + child->height()) {
            return child->child_at(x - x_now, y - y_now);
        }
        if (m_kind == kind::HORIZONTAL) {
            x_now += child->width();
        } else {
            y_now += child->height();
        }
    }

    return nullptr;
}

[[nodiscard]] std::unique_ptr<box> make_box(box::kind kind) {
    return std::make_unique<box>(kind);
}
}  // namespace widgets
