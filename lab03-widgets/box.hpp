#ifndef BOX_HPP_
#define BOX_HPP_

#include <memory>
#include <vector>
#include "abstract_widgets.hpp"

namespace widgets {
struct box final : container {
    enum class kind { HORIZONTAL, VERTICAL };

    [[nodiscard]] int width() const final;
    [[nodiscard]] int height() const final;

    explicit box(kind kind_);

    void update_layout() override;

    [[nodiscard]] int size() const {
        return static_cast<int>(children.size());
    }

    [[nodiscard]] widget *get(int index) const;

    widget *add(std::unique_ptr<widget> child);

    std::unique_ptr<widget> remove(int index);

    widget *child_at(int x, int y) override;

private:
    std::vector<std::unique_ptr<widget>> children;
    kind m_kind;
    int m_width;
    int m_height;
};

[[nodiscard]] std::unique_ptr<box> make_box(box::kind kind);
}  // namespace widgets

#endif  // BOX_HPP_
