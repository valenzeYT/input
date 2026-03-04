#include "../include/interpreter.h"
#include <vector>
#include "../include/module_registry.h"
#include <algorithm>
#include <string>
#include <windows.h>

namespace {
void sendKeyEvent(WORD key, DWORD flags) {
    INPUT input = {};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;
    input.ki.dwFlags = flags;
    SendInput(1, &input, sizeof(INPUT));
}

void sendMouseEvent(DWORD flags, LONG dx = 0, LONG dy = 0, DWORD data = 0) {
    INPUT input = {};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = flags;
    input.mi.dx = dx;
    input.mi.dy = dy;
    input.mi.mouseData = data;
    SendInput(1, &input, sizeof(INPUT));
}
} // namespace

namespace input_lib {

void key_down(WORD key) { sendKeyEvent(key, 0); }
void key_up(WORD key) { sendKeyEvent(key, KEYEVENTF_KEYUP); }
void key_press(WORD key) {
    key_down(key);
    key_up(key);
}

void key_code_down(int key) { key_down(static_cast<WORD>(key)); }
void key_code_up(int key) { key_up(static_cast<WORD>(key)); }
void key_code_press(int key) { key_press(static_cast<WORD>(key)); }

void type(const std::string& text) {
    for (char c : text) {
        SHORT keyInfo = VkKeyScanA(c);
        if (keyInfo == -1) {
            continue;
        }
        BYTE vk = LOBYTE(keyInfo);
        BYTE modifiers = HIBYTE(keyInfo);
        if (modifiers & 1) key_down(VK_SHIFT);
        if (modifiers & 2) key_down(VK_CONTROL);
        if (modifiers & 4) key_down(VK_MENU);
        key_press(static_cast<WORD>(vk));
        if (modifiers & 4) key_up(VK_MENU);
        if (modifiers & 2) key_up(VK_CONTROL);
        if (modifiers & 1) key_up(VK_SHIFT);
    }
}

void left_down() { sendMouseEvent(MOUSEEVENTF_LEFTDOWN); }
void left_up() { sendMouseEvent(MOUSEEVENTF_LEFTUP); }
void left_click() {
    left_down();
    left_up();
}

void right_down() { sendMouseEvent(MOUSEEVENTF_RIGHTDOWN); }
void right_up() { sendMouseEvent(MOUSEEVENTF_RIGHTUP); }
void right_click() {
    right_down();
    right_up();
}

void middle_down() { sendMouseEvent(MOUSEEVENTF_MIDDLEDOWN); }
void middle_up() { sendMouseEvent(MOUSEEVENTF_MIDDLEUP); }
void middle_click() {
    middle_down();
    middle_up();
}

void move(int x, int y) {
    int screenX = std::max(GetSystemMetrics(SM_CXSCREEN) - 1, 1);
    int screenY = std::max(GetSystemMetrics(SM_CYSCREEN) - 1, 1);
    LONG absX = static_cast<LONG>((x * 65535LL) / screenX);
    LONG absY = static_cast<LONG>((y * 65535LL) / screenY);
    sendMouseEvent(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, absX, absY);
}

void move_rel(int dx, int dy) { sendMouseEvent(MOUSEEVENTF_MOVE, dx, dy); }
void scroll(int amount) { sendMouseEvent(MOUSEEVENTF_WHEEL, 0, 0, static_cast<DWORD>(amount)); }
void hscroll(int amount) { sendMouseEvent(MOUSEEVENTF_HWHEEL, 0, 0, static_cast<DWORD>(amount)); }

int cursor_x() {
    POINT p = {};
    GetCursorPos(&p);
    return p.x;
}

int cursor_y() {
    POINT p = {};
    GetCursorPos(&p);
    return p.y;
}

#define DEFINE_LETTER_FUNCS(letter, vk) \
void letter##_down() { key_down(vk); } \
void letter##_up() { key_up(vk); } \
void letter##_press() { key_press(vk); }

DEFINE_LETTER_FUNCS(a, 'A')
DEFINE_LETTER_FUNCS(b, 'B')
DEFINE_LETTER_FUNCS(c, 'C')
DEFINE_LETTER_FUNCS(d, 'D')
DEFINE_LETTER_FUNCS(e, 'E')
DEFINE_LETTER_FUNCS(f, 'F')
DEFINE_LETTER_FUNCS(g, 'G')
DEFINE_LETTER_FUNCS(h, 'H')
DEFINE_LETTER_FUNCS(i, 'I')
DEFINE_LETTER_FUNCS(j, 'J')
DEFINE_LETTER_FUNCS(k, 'K')
DEFINE_LETTER_FUNCS(l, 'L')
DEFINE_LETTER_FUNCS(m, 'M')
DEFINE_LETTER_FUNCS(n, 'N')
DEFINE_LETTER_FUNCS(o, 'O')
DEFINE_LETTER_FUNCS(p, 'P')
DEFINE_LETTER_FUNCS(q, 'Q')
DEFINE_LETTER_FUNCS(r, 'R')
DEFINE_LETTER_FUNCS(s, 'S')
DEFINE_LETTER_FUNCS(t, 'T')
DEFINE_LETTER_FUNCS(u, 'U')
DEFINE_LETTER_FUNCS(v, 'V')
DEFINE_LETTER_FUNCS(w, 'W')
DEFINE_LETTER_FUNCS(x, 'X')
DEFINE_LETTER_FUNCS(y, 'Y')
DEFINE_LETTER_FUNCS(z, 'Z')

#undef DEFINE_LETTER_FUNCS

#define DEFINE_VK_FUNCS(name, vk) \
void name##_down() { key_down(vk); } \
void name##_up() { key_up(vk); } \
void name##_press() { key_press(vk); }

DEFINE_VK_FUNCS(enter, VK_RETURN)
DEFINE_VK_FUNCS(space, VK_SPACE)
DEFINE_VK_FUNCS(tab, VK_TAB)
DEFINE_VK_FUNCS(esc, VK_ESCAPE)
DEFINE_VK_FUNCS(shift, VK_SHIFT)
DEFINE_VK_FUNCS(ctrl, VK_CONTROL)
DEFINE_VK_FUNCS(alt, VK_MENU)
DEFINE_VK_FUNCS(backspace, VK_BACK)
DEFINE_VK_FUNCS(del, VK_DELETE)
DEFINE_VK_FUNCS(arrow_up, VK_UP)
DEFINE_VK_FUNCS(arrow_down, VK_DOWN)
DEFINE_VK_FUNCS(arrow_left, VK_LEFT)
DEFINE_VK_FUNCS(arrow_right, VK_RIGHT)
DEFINE_VK_FUNCS(home, VK_HOME)
DEFINE_VK_FUNCS(end, VK_END)
DEFINE_VK_FUNCS(page_up, VK_PRIOR)
DEFINE_VK_FUNCS(page_down, VK_NEXT)

#undef DEFINE_VK_FUNCS

} // namespace input_lib

extern "C" __declspec(dllexport)
void register_module() {
    module_registry::registerModule("input", [](Interpreter& interp) {
                    auto add0 = [&interp](const std::string& name, const std::function<void()>& fn) {
                        interp.registerModuleFunction("input", name, [&interp, name, fn](const std::vector<Value>& args) -> Value {
                            interp.expectArity(args, 0, "input." + name);
                            fn();
                            interp.fireEvent("input.event." + name);
                            return Value::fromNumber(0.0);
                        });
                    };
                    auto add1Num = [&interp](const std::string& name, const std::function<void(double)>& fn) {
                        interp.registerModuleFunction("input", name, [&interp, name, fn](const std::vector<Value>& args) -> Value {
                            interp.expectArity(args, 1, "input." + name);
                            double value = interp.expectNumber(args[0], "input." + name + " expects number");
                            fn(value);
                            interp.fireEvent("input.event." + name, {args[0]});
                            return Value::fromNumber(0.0);
                        });
                    };
                    auto add2Num = [&interp](const std::string& name, const std::function<void(double, double)>& fn) {
                        interp.registerModuleFunction("input", name, [&interp, name, fn](const std::vector<Value>& args) -> Value {
                            interp.expectArity(args, 2, "input." + name);
                            double a = interp.expectNumber(args[0], "input." + name + " expects number args");
                            double b = interp.expectNumber(args[1], "input." + name + " expects number args");
                            fn(a, b);
                            interp.fireEvent("input.event." + name, {args[0], args[1]});
                            return Value::fromNumber(0.0);
                        });
                    };
                    auto add1Str = [&interp](const std::string& name, const std::function<void(const std::string&)>& fn) {
                        interp.registerModuleFunction("input", name, [&interp, name, fn](const std::vector<Value>& args) -> Value {
                            interp.expectArity(args, 1, "input." + name);
                            std::string text = interp.expectString(args[0], "input." + name + " expects string");
                            fn(text);
                            interp.fireEvent("input.event." + name, {args[0]});
                            return Value::fromNumber(0.0);
                        });
                    };
                    auto add0Num = [&interp](const std::string& name, const std::function<double()>& fn) {
                        interp.registerModuleFunction("input", name, [&interp, name, fn](const std::vector<Value>& args) -> Value {
                            interp.expectArity(args, 0, "input." + name);
                            Value out = Value::fromNumber(fn());
                            interp.fireEvent("input.event." + name, {out});
                            return out;
                        });
                    };

                    add0("left_down", []() { input_lib::left_down(); });
                    add0("left_up", []() { input_lib::left_up(); });
                    add0("left_click", []() { input_lib::left_click(); });
                    add0("right_down", []() { input_lib::right_down(); });
                    add0("right_up", []() { input_lib::right_up(); });
                    add0("right_click", []() { input_lib::right_click(); });
                    add0("middle_down", []() { input_lib::middle_down(); });
                    add0("middle_up", []() { input_lib::middle_up(); });
                    add0("middle_click", []() { input_lib::middle_click(); });

                    add2Num("move", [](double x, double y) { input_lib::move(static_cast<int>(x), static_cast<int>(y)); });
                    add2Num("move_rel", [](double dx, double dy) { input_lib::move_rel(static_cast<int>(dx), static_cast<int>(dy)); });
                    add1Num("scroll", [](double amount) { input_lib::scroll(static_cast<int>(amount)); });
                    add1Num("hscroll", [](double amount) { input_lib::hscroll(static_cast<int>(amount)); });
                    add0Num("cursor_x", []() { return static_cast<double>(input_lib::cursor_x()); });
                    add0Num("cursor_y", []() { return static_cast<double>(input_lib::cursor_y()); });

                    add1Num("key_down", [](double code) { input_lib::key_code_down(static_cast<int>(code)); });
                    add1Num("key_up", [](double code) { input_lib::key_code_up(static_cast<int>(code)); });
                    add1Num("key_press", [](double code) { input_lib::key_code_press(static_cast<int>(code)); });
                    add1Str("type", [](const std::string& text) { input_lib::type(text); });

                    for (char c = 'a'; c <= 'z'; ++c) {
                        std::string base(1, c);
                        WORD vk = static_cast<WORD>('A' + (c - 'a'));

                        add0(base + "_down", [vk]() { input_lib::key_down(vk); });
                        add0(base + "_up", [vk]() { input_lib::key_up(vk); });
                        add0(base + "_press", [vk]() { input_lib::key_press(vk); });
                    }

                    struct NamedVk {
                        const char* name;
                        WORD vk;
                    };
                    const NamedVk namedVks[] = {
                        {"enter", VK_RETURN}, {"space", VK_SPACE}, {"tab", VK_TAB}, {"esc", VK_ESCAPE},
                        {"shift", VK_SHIFT}, {"ctrl", VK_CONTROL}, {"alt", VK_MENU},
                        {"lshift", VK_LSHIFT}, {"rshift", VK_RSHIFT},
                        {"lctrl", VK_LCONTROL}, {"rctrl", VK_RCONTROL},
                        {"lalt", VK_LMENU}, {"ralt", VK_RMENU},
                        {"backspace", VK_BACK}, {"del", VK_DELETE}, {"insert", VK_INSERT},
                        {"arrow_up", VK_UP}, {"arrow_down", VK_DOWN}, {"arrow_left", VK_LEFT}, {"arrow_right", VK_RIGHT},
                        {"home", VK_HOME}, {"end", VK_END}, {"page_up", VK_PRIOR}, {"page_down", VK_NEXT},
                        {"caps_lock", VK_CAPITAL}, {"num_lock", VK_NUMLOCK}, {"scroll_lock", VK_SCROLL},
                        {"print_screen", VK_SNAPSHOT}, {"pause", VK_PAUSE},
                        {"lwin", VK_LWIN}, {"rwin", VK_RWIN}, {"apps", VK_APPS},
                        {"digit0", '0'}, {"digit1", '1'}, {"digit2", '2'}, {"digit3", '3'}, {"digit4", '4'},
                        {"digit5", '5'}, {"digit6", '6'}, {"digit7", '7'}, {"digit8", '8'}, {"digit9", '9'},
                        {"num0", VK_NUMPAD0}, {"num1", VK_NUMPAD1}, {"num2", VK_NUMPAD2}, {"num3", VK_NUMPAD3}, {"num4", VK_NUMPAD4},
                        {"num5", VK_NUMPAD5}, {"num6", VK_NUMPAD6}, {"num7", VK_NUMPAD7}, {"num8", VK_NUMPAD8}, {"num9", VK_NUMPAD9},
                        {"num_mul", VK_MULTIPLY}, {"num_add", VK_ADD}, {"num_sub", VK_SUBTRACT}, {"num_dec", VK_DECIMAL}, {"num_div", VK_DIVIDE},
                        {"f1", VK_F1}, {"f2", VK_F2}, {"f3", VK_F3}, {"f4", VK_F4}, {"f5", VK_F5}, {"f6", VK_F6},
                        {"f7", VK_F7}, {"f8", VK_F8}, {"f9", VK_F9}, {"f10", VK_F10}, {"f11", VK_F11}, {"f12", VK_F12},
                        {"f13", VK_F13}, {"f14", VK_F14}, {"f15", VK_F15}, {"f16", VK_F16}, {"f17", VK_F17}, {"f18", VK_F18},
                        {"f19", VK_F19}, {"f20", VK_F20}, {"f21", VK_F21}, {"f22", VK_F22}, {"f23", VK_F23}, {"f24", VK_F24},
                        {"semicolon", VK_OEM_1}, {"plus", VK_OEM_PLUS}, {"comma", VK_OEM_COMMA}, {"minus", VK_OEM_MINUS},
                        {"period", VK_OEM_PERIOD}, {"slash", VK_OEM_2}, {"backtick", VK_OEM_3},
                        {"lbracket", VK_OEM_4}, {"backslash", VK_OEM_5}, {"rbracket", VK_OEM_6}, {"quote", VK_OEM_7}
                    };
                    for (const auto& key : namedVks) {
                        std::string base = key.name;
                        add0(base + "_down", [vk = key.vk]() { input_lib::key_down(vk); });
                        add0(base + "_up", [vk = key.vk]() { input_lib::key_up(vk); });
                        add0(base + "_press", [vk = key.vk]() { input_lib::key_press(vk); });
                    }

    });
}
