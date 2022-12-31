target("HW1")
    set_kind("binary")
    add_files("HW1/*.cpp")
    add_packages("fmt" ,"glad", "glfw", "glm", "spdlog", "stb")

target("HW2")
    set_kind("binary")
    add_files("HW2/*.cpp")
    add_packages("fmt" ,"glad", "glfw", "glm", "spdlog", "stb")

target("HW3")
    set_kind("binary")
    add_files("HW3/*.cpp")
    add_files("HW3/*/*.cpp")
    -- Override for res
    set_rundir("HW3")
    add_packages("fmt" ,"glad", "glfw", "glm", "spdlog", "stb", "imgui", "assimp")