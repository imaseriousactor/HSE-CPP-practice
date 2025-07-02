#ifndef GRID_HPP_
#define GRID_HPP_

#include <memory>
#include <vector>
#include "abstract_widgets.hpp"

namespace widgets {
struct grid final : container {
    [[nodiscard]] int width() const final;
    [[nodiscard]] int height() const final;

    explicit grid(int rows, int columns);

    [[nodiscard]] int rows() const;
    [[nodiscard]] int columns() const;

    void update_layout() override;

    [[nodiscard]] widget *get(int row, int column) const;

    widget *add(std::unique_ptr<widget> child, int row, int column);

    std::unique_ptr<widget> remove(int row, int column);

    widget *child_at(int x, int y) override;

private:
    int m_rows;
    int m_columns;
    int m_width;
    int m_height;
    std::vector<int> rows_height;
    std::vector<int> columns_width;
    std::vector<std::vector<std::unique_ptr<widget>>> children;
};

[[nodiscard]] std::unique_ptr<grid> make_grid(int rows, int columns);
}  // namespace widgets

#endif  // GRID_HPP_
