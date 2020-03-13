#ifndef SATURN_EDITOR_LOG_WINDOW_HPP_
#define SATURN_EDITOR_LOG_WINDOW_HPP_

#include <phobos/util/log_interface.hpp>

#include <stl/vector.hpp>
#include <string>

class LogWindow : public ph::log::LogInterface {
public:
    LogWindow();

    void show_gui();

    void write(ph::log::Severity severity, std::string_view str) override;
private:
    bool shown = true;
    bool auto_scroll = true;

    static constexpr stl::size_t max_messages = 100;
    stl::vector<std::string> messages;
};

#endif