--
-- Author: Amélie Heinrich
-- Company: Amélie Games
-- License: MIT
-- Create Time: 24/02/2023 15:25
--

add_rules("mode.debug", "mode.release")

target("ImGui")
    set_kind("static")
    add_files("external/*.cpp")
    add_syslinks("user32", "d3d11", "dxgi")

target("Twitch Sound Player")
    add_files("src/*.cpp")
    add_deps("ImGui")
    add_syslinks("Winmm", "Ws2_32", "gdi32", "user32", "kernel32", "dsound", "d3d11", "dxgi")
    add_includedirs("external")
    set_rundir(".")

    if is_mode("debug") then
        set_symbols("debug")
        set_optimize("none")
    end

    if is_mode("release") then
        set_symbols("hidden")
        set_optimize("fastest")
        set_strip("all")
    end
