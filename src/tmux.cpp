// Display images inside a terminal
// Copyright (C) 2023  JustKidding
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "tmux.hpp"
#include "os.hpp"

#include <sstream>
#include <string>
#include <iostream>

using std::string;

std::string tmux::get_session_id()
{
    std::string cmd =
        "tmux display -p -F '#{session_id}' -t " + tmux::get_pane();
    return os::exec(cmd);
}

bool tmux::is_used()
{
    return !tmux::get_pane().empty();
}

bool tmux::is_window_focused()
{
    std::string cmd =
        "tmux display -p -F '#{window_active},#{pane_in_mode}' -t " + tmux::get_pane();
    std::string output = os::exec(cmd);
    return output == "1,0";
}

std::string tmux::get_pane()
{
    return os::getenv("TMUX_PANE").value_or("");
}

auto tmux::get_client_pids() -> std::optional<std::vector<ProcessInfo>>
{
    if (!tmux::is_window_focused()) return {};

    std::vector<ProcessInfo> pids;
    std::string cmd =
        "tmux list-clients -F '#{client_pid}' -t " + tmux::get_pane();
    std::string output = os::exec(cmd), to;
    std::stringstream ss(output);

    while(std::getline(ss, to, '\n')) {
        pids.push_back(ProcessInfo(std::stoi(to)));
    }

    return pids;
}

auto tmux::get_offset() -> std::pair<const int, const int>
{
    string cmd = "tmux display -p -F '#{pane_top},#{pane_left},\
                                     #{pane_bottom},#{pane_right},\
                                     #{window_height},#{window_width}' \
                                  -t" +  tmux::get_pane();
    string output = os::exec(cmd);
    return std::make_pair(0,0);
}
