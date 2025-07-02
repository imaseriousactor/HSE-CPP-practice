#include "ball_icon.hpp"
#include <cmath>
#include <utility>

namespace widgets {
[[nodiscard]] int ball_icon::width() const {
    return 2 * static_cast<int>(m_radius) + 1;
}

[[nodiscard]] int ball_icon::height() const {
    return 2 * static_cast<int>(m_radius) + 1;
}

ball_icon::ball_icon(int radius) : m_radius(radius) {
}

[[nodiscard]] const int &ball_icon::radius() const {
    return m_radius;
}

void ball_icon::radius(int new_radius) {
    m_radius = new_radius;
}

widget *ball_icon::child_at(int x, int y) {
    const int delta_x = m_radius - x;
    const int delta_y = m_radius - y;
    if (m_radius * m_radius < (delta_x) * (delta_x) + (delta_y) * (delta_y)) {
        return nullptr;
    }

    return this;
}

[[nodiscard]] std::unique_ptr<ball_icon> make_ball_icon(int radius) {
    return std::make_unique<ball_icon>(radius);
}
}  // namespace widgets
