--
-- Author: Amélie Heinrich
-- Company: Amélie Games
-- License: MIT
-- Create Time: 24/02/2023 15:25
--

target("Twitch Sound Player")
    add_files("src/*.cpp")
    add_syslinks("Winmm", "Ws2_32", "gdi32", "user32", "kernel32", "dsound")
    set_rundir(".")
