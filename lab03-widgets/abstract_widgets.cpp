#include "abstract_widgets.hpp"

namespace widgets {

widget *widget::child_at(int x, int y) {
    if (x < 0 || width() <= x || y < 0 || height() <= y) {
        return nullptr;
    }

    return this;
}

}  // namespace widgets
