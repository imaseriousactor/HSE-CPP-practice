#ifdef _MSC_VER
#include <crtdbg.h>
#endif
#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
enum class Command {
    EXIT,
    SHERLOCK,
    CAT,
    FWD,
    BWD,
    TURN_RIGHT,
    TURN_LEFT,
    STATS,
    START_MACRO,
    RUN_MACRO,
    DRAW_MAP,
    INVALID,
    PUT_WALL,
    REMOVE_WALL
};
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
std::unordered_map<std::string, Command> parse_command{
    {"exit", Command::EXIT},
    {"quit", Command::EXIT},
    {"sherlock", Command::SHERLOCK},
    {"sherlock-holmes", Command::SHERLOCK},
    {"cat", Command::CAT},
    {"cat-jeoffry", Command::CAT},
    {"forward", Command::FWD},
    {"fwd", Command::FWD},
    {"backward", Command::BWD},
    {"bwd", Command::BWD},
    {"turn-right", Command::TURN_RIGHT},
    {"right", Command::TURN_RIGHT},
    {"turn-left", Command::TURN_LEFT},
    {"left", Command::TURN_LEFT},
    {"print-statistics", Command::STATS},
    {"stats", Command::STATS},
    {"start-macro", Command::START_MACRO},
    {"run-macro", Command::RUN_MACRO},
    {"show-map", Command::DRAW_MAP},
    {"put-wall", Command::PUT_WALL},
    {"remove-wall", Command::REMOVE_WALL}};

const std::set<std::string> macro_command{
    "forward",    "fwd",  "backward",         "bwd",   "turn-right", "right",
    "turn-left",  "left", "print-statistics", "stats", "show-map",   "put-wall",
    "remove-wall"};

int x = 0;    // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
int y = 0;    // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
int dir = 0;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
std::set<std::pair<int, int>>
    visited;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
std::map<std::pair<int, int>, std::multiset<int>>
    walls;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

const std::string quote_sherlock =
    "I had had so many reasons to believe in my friend's "
    "subtle "
    "powers of\n"
    "reasoning and extraordinary energy in action that I "
    "felt that he must\n"
    "have some solid grounds for the assured and easy "
    "demeanour with which\n"
    "he treated the singular mystery which he had been "
    "called "
    "upon to\n"
    "fathom. Once only had I known him to fail, in the "
    "case of the King of\n"
    "Bohemia and of the Irene Adler photograph;"
    " but when I looked back to\n"
    "the weird business of the"
    " Sign of Four, and the extraordinary\n"
    "circumstances connected with the Study in Scarlet,"
    " I felt that it would\n"
    "be a strange tangle indeed "
    "which he could not unravel.\n";

const std::string quote_cat = R"(For he is of the tribe of Tiger.
For the Cherub Cat is a term of the Angel Tiger.
For he has the subtlety and hissing of a serpent, which in goodness he suppresses.
For he will not do destruction, if he is well-fed, neither will he spit without provocation.
For he purrs in thankfulness, when God tells him he's a good Cat.
For he is an instrument for the children to learn benevolence upon.
For every house is incomplete without him and a blessing is lacking in the spirit.
For the Lord commanded Moses concerning the cats at the departure of the Children of Israel from Egypt.
For every family had one cat at least in the bag.
For the English Cats are the best in Europe.
)";

struct Direction {
    int dx, dy;
    char dr;
};

const Direction DIRECTIONS[] = {
    {0, 1, '^'},
    {1, 0, '>'},
    {0, -1, 'v'},
    {-1, 0, '<'}};

void forward() {
    const int x_back = x + DIRECTIONS[dir].dx;
    const int y_back = y + DIRECTIONS[dir].dy;
    if (!((walls.contains({x, y}) && walls[{x, y}].contains(dir)) ||
          (walls.contains({x_back, y_back}) &&
           walls[{x_back, y_back}].contains((dir + 2) % 4)))) {
        x += DIRECTIONS[dir].dx;
        y += DIRECTIONS[dir].dy;
        const std::pair<int, int> cur{x, y};
        visited.insert(cur);
    } else {
        std::cout << "Bump!\n";
    }
}

void backward() {
    const int x_back = x - DIRECTIONS[dir].dx;
    const int y_back = y - DIRECTIONS[dir].dy;
    if (!((walls.contains({x, y}) && walls[{x, y}].contains((dir + 2) % 4)) ||
          (walls.contains({x_back, y_back}) &&
           walls[{x_back, y_back}].contains(dir)))) {
        x -= DIRECTIONS[dir].dx;
        y -= DIRECTIONS[dir].dy;
        const std::pair<int, int> cur{x, y};
        visited.insert(cur);
    } else {
        std::cout << "Bump!\n";
    }
}

void stats() {
    std::cout << "Player " << DIRECTIONS[dir].dr << " stands " << abs(x)
              << " away from OY, " << abs(y) << " away from OX. "
              << visited.size() << " cell(s) has been visited.\n";
}

void put_wall() {
    if (walls.contains({x, y})) {
        if (!walls[{x, y}].contains(dir)) {
            walls[{x, y}].insert(dir);
        }
    } else {
        walls[{x, y}].insert(dir);
    }
}

void remove_wall() {
    if (walls.contains({x, y})) {
        if (walls[{x, y}].contains(dir)) {
            walls[{x, y}].erase(dir);
        }
    }
    const int x_back = x - DIRECTIONS[dir].dx;
    const int y_back = y - DIRECTIONS[dir].dy;
    if (walls.contains({x_back, y_back})) {
        if (walls[{x_back, y_back}].contains(dir)) {
            walls[{x_back, y_back}].erase(dir);
        }
    }
}

void show_map() {
    const int size_x_left =
        (std::min_element(
             visited.begin(), visited.end(),
             [](const auto &i, const auto &j) { return i.first < j.first; }
         ))->first;
    const int size_x_right =
        (std::max_element(
             visited.begin(), visited.end(),
             [](const auto &i, const auto &j) { return i.first < j.first; }
         ))->first;
    const int size_y_down =
        (std::min_element(
             visited.begin(), visited.end(),
             [](const auto &i, const auto &j) { return i.second < j.second; }
         ))->second;
    const int size_y_up =
        (std::max_element(
             visited.begin(), visited.end(),
             [](const auto &i, const auto &j) { return i.second < j.second; }
         ))->second;
    std::vector<std::vector<char>> map(2 * (size_y_up - size_y_down + 1) + 1);
    std::vector<char> line(2 * (size_x_right - size_x_left + 1) + 1);
    std::vector<char> line_0(2 * (size_x_right - size_x_left + 1) + 1);
    for (int i = 0; i < (2 * (size_x_right - size_x_left + 1) + 1); i++) {
        if (i % 2 == 0) {
            line[i] = ' ';
        } else {
            line[i] = '.';
        }
    }
    for (int i = 0; i < (2 * (size_x_right - size_x_left + 1) + 1); i++) {
        line_0[i] = ' ';
    }
    for (int i = 0; i < (2 * (size_y_up - size_y_down + 1) + 1); i++) {
        if (i % 2 == 0) {
            map[i] = line_0;
        } else {
            map[i] = line;
        }
    }
    for (auto c : visited) {
        map[2 * (abs(size_y_up) - (c.second)) + 1]
           [2 * (abs(size_x_left) + c.first) + 1] = 'o';
    }
    map[2 * abs(size_y_up) + 1][2 * abs(size_x_left) + 1] = '0';
    map[2 * (abs(size_y_up) - y) + 1][2 * (abs(size_x_left) + x) + 1] =
        DIRECTIONS[dir].dr;
    for (const auto &c : walls) {
        int x_wall = 0;
        int y_wall = 0;
        x_wall = 2 * (abs(size_x_left) + c.first.first) + 1;
        y_wall = 2 * (abs(size_y_up) - c.first.second) + 1;
        for (auto k : c.second) {
            int x_wall_now = x_wall;
            int y_wall_now = y_wall;
            if (k % 2 == 0) {
                x_wall_now -= DIRECTIONS[k].dx;
                y_wall_now -= DIRECTIONS[k].dy;
                map[y_wall_now][x_wall_now] = '-';
                map[y_wall_now][x_wall_now - 1] = '+';
                map[y_wall_now][x_wall_now + 1] = '+';
            } else {
                x_wall_now += DIRECTIONS[k].dx;
                y_wall_now += DIRECTIONS[k].dy;
                map[y_wall_now][x_wall_now] = '|';
                map[y_wall_now - 1][x_wall_now] = '+';
                map[y_wall_now + 1][x_wall_now] = '+';
            }
        }
    }
    for (const auto &lines : map) {
        for (auto symbols : lines) {
            std::cout << symbols;
        }
        std::cout << "\n";
    }
}

void run_macro(const std::vector<Command> &macro) {
    for (const auto &command : macro) {
        switch (command) {
            case Command::FWD:
                forward();
                break;
            case Command::BWD:
                backward();
                break;
            case Command::TURN_RIGHT:
                dir = (dir + 1) % 4;
                break;
            case Command::TURN_LEFT:
                dir = (3 + dir) % 4;
                break;
            case Command::STATS:
                stats();
                break;
            case Command::DRAW_MAP:
                show_map();
                break;
            case Command::PUT_WALL:
                put_wall();
                break;
            case Command::REMOVE_WALL:
                remove_wall();
                break;
            default:
                break;
        }
    }
}

bool invalid_command(const std::string &command) {
    std::string command_check = command;
    bool check = false;
    command_check.erase(
        std::remove_if(command_check.begin(), command_check.end(), ::isspace),
        command_check.end()
    );

    if (!(command_check.empty()) && macro_command.contains(command_check)) {
        check = true;
    }
    return check;
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
int main() {
#ifdef _MSC_VER
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#endif
    std::vector<Command> macro;
    visited.insert({0, 0});
    while (true) {
        std::string command;
        if (!std::getline(std::cin, command)) {
            break;
        }
        std::string command_check = command;
        command_check.erase(
            std::remove_if(
                command_check.begin(), command_check.end(), ::isspace
            ),
            command_check.end()
        );
        if (command_check.empty()) {
            std::cout << "Invalid command: '" << command << "'\n";
            continue;
        }
        bool exit = false;
        Command command_switch = Command::INVALID;
        if (parse_command.contains(command_check)) {
            command_switch = parse_command[command_check];
        }
        switch (command_switch) {
            case Command::EXIT:
                exit = true;
                break;
            case Command::SHERLOCK:
                std::cout << quote_sherlock;
                break;
            case Command::CAT:
                std::cout << quote_cat;
                break;
            case Command::FWD:
                forward();
                break;
            case Command::BWD:
                backward();
                break;
            case Command::TURN_RIGHT:
                dir = (dir + 1) % 4;
                break;
            case Command::TURN_LEFT:
                dir = (3 + dir) % 4;
                break;
            case Command::STATS:
                stats();
                break;
            case Command::START_MACRO:
                macro = {};
                if (!std::getline(std::cin, command)) {
                    std::cout << "New macro of " << std::size(macro)
                              << " command(s)\n";
                    break;
                }
                while (command != "end-macro") {
                    if (!invalid_command(command)) {
                        std::cout << "Invalid command: '" << command << "'\n";
                        if (!std::getline(std::cin, command)) {
                            break;
                        }
                        continue;
                    }
                    macro.push_back(parse_command[command]);
                    std::cout << "Recorded\n";
                    if (!std::getline(std::cin, command)) {
                        break;
                    }
                }
                std::cout << "New macro of " << std::size(macro)
                          << " command(s)\n";
                break;
            case Command::RUN_MACRO:
                run_macro(macro);
                break;
            case Command::DRAW_MAP:
                show_map();
                break;
            case Command::PUT_WALL:
                put_wall();
                break;
            case Command::REMOVE_WALL:
                remove_wall();
                break;
            case Command::INVALID:
                std::cout << "Invalid command: '" << command << "'\n";
                break;
        }
        if (exit) {
            break;
        }
    }
}
