#pragma once

constexpr auto HELLO_MESSAGE = "CPU_MONITOR";
constexpr auto ACK_MESSAGE = "ACK";

enum STATES
{
    WAIT_FOR_CONNECTION,
    SHOW_SCREEN,
    CHANGE_SCREEN,
};

enum SCREENS
{
    CPU_GPU,
    RAM_VRAM,
    INTERNET_IN_OUT,
    _SCREENS_LEN // THIS IS NOT A SCREEN
};
