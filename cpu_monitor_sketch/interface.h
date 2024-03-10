#pragma once

constexpr auto HELLO_MESSAGE = "CPU_MONITOR";
constexpr auto ACK_MESSAGE = "ACK";
constexpr auto SWITCH_SCREEN_MESSAGE = "SWITCH";

enum STATES
{
    WAIT_FOR_CONNECTION,
    SHOW_SCREEN,
    CHANGE_SCREEN,
};
