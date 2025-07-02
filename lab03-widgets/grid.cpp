#include "grid.hpp"
#include <utility>
#include <vector>

namespace widgets {
[[nodiscard]] int grid::width() const {
    return m_width;
}

[[nodiscard]] int grid::height() const {
    return m_height;
}

[[nodiscard]] int grid::rows() const {
    return m_rows;
}

[[nodiscard]] int grid::columns() const {
    return m_columns;
}

grid::grid(int rows, int columns)
    : m_rows(rows),
      m_columns(columns),
      m_width(0),
      m_height(0),
      rows_height(m_rows),
      columns_width(m_columns) {
    children.resize(rows);
    for (auto &row : children) {
        row.resize(columns);
    }
}

void grid::update_layout() {
    int width = 0;
    int height = 0;
    rows_height.assign(m_rows, 0);
    columns_width.assign(m_columns, 0);
    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_columns; j++) {
            auto &child = children[i][j];
            if (child != nullptr) {
                rows_height[i] = std::max(rows_height[i], child->height());
                columns_width[j] = std::max(columns_width[j], child->width());
            }
            if (i == m_rows - 1) {
                width += columns_width[j];
            }
        }
        height += rows_height[i];
    }
    m_width = width;
    m_height = height;
}

[[nodiscard]] widget *grid::get(int row, int column) const {
    return children[row][column].get();
}

widget *grid::add(std::unique_ptr<widget> child, int row, int column) {
    children[row][column] = std::move(child);
    int columns_width_now = 0;
    for (int i = 0; i < m_rows; i++) {
        auto &child_now = children[i][column];
        if (child_now != nullptr) {
            columns_width_now = std::max(columns_width_now, child_now->width());
        }
    }
    m_width += (columns_width_now - columns_width[column]);
    columns_width[column] = columns_width_now;
    int rows_height_now = 0;
    for (int j = 0; j < m_columns; j++) {
        auto &child_now = children[row][j];
        if (child_now != nullptr) {
            rows_height_now = std::max(rows_height_now, child_now->height());
        }
    }
    m_height += (rows_height_now - rows_height[row]);
    rows_height[row] = rows_height_now;
    return children[row][column].get();
}

std::unique_ptr<widget> grid::remove(int row, int column) {
    std::unique_ptr<widget> child_removed = std::move(children[row][column]);
    children[row][column] = nullptr;
    int columns_width_now = 0;
    for (int i = 0; i < m_rows; i++) {
        auto &child_now = children[i][column];
        if (child_now != nullptr) {
            columns_width_now = std::max(columns_width_now, child_now->width());
        }
    }
    m_width += (columns_width_now - columns_width[column]);
    columns_width[column] = columns_width_now;
    int rows_height_now = 0;
    for (int j = 0; j < m_columns; j++) {
        auto &child_now = children[row][j];
        if (child_now != nullptr) {
            rows_height_now = std::max(rows_height_now, child_now->height());
        }
    }
    m_height += (rows_height_now - rows_height[row]);
    rows_height[row] = rows_height_now;
    return child_removed;
}

widget *grid::child_at(int x, int y) {
    int x_now = 0;  // cppcheck-suppress [variableScope,unreadVariable]
    int y_now = 0;
    for (int i = 0; i < m_rows; i++) {
        x_now = 0;
        for (int j = 0; j < m_columns; j++) {
            auto &child = children[i][j];
            if (child != nullptr) {
                if ((x_now <= x && x < x_now + child->width()) && y_now <= y &&
                    y < y_now + child->height()) {
                    return child->child_at(x - x_now, y - y_now);
                }
            }
            x_now += columns_width[j];
        }
        y_now += rows_height[i];
    }

    return nullptr;
}

[[nodiscard]] std::unique_ptr<grid> make_grid(int rows, int columns) {
    return std::make_unique<grid>(rows, columns);
}
}  // namespace widgets
