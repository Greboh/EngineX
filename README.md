# C++ Game Engine Project

# Setup Steps

1. **Cloning the Repository**: To properly clone the repository, you have two options:
    - Download the full files from GitHub.
    - Use one of the the following commands in your command-line interface:
    
      If you want the main branch
        ```
        git clone --recursive https://github.com/Greboh/EngineX
        ```
      If you want another branch than main
        ```
        git clone -[branch] --recursive https://github.com/Greboh/EngineX
        ```


2. **Running Setup Script**: Navigate to the `EngineX` directory, then proceed to `Scripts`, and execute the `Setup` script.

# Tool Usages

In this section, I'll outline the tools utilized in this project and the reasons behind their selection.

## Build System Configuration

### Premake
- **Tool**: [premake](https://github.com/premake/premake-core)
- **Description**: Premake is utilized as the build configuration tool for this project.
- **Reason for Usage**: Premake simplifies the process of generating project files for various IDEs and build systems, allowing for easy project setup and configuration. Furthermore it reduces the files needed in the repository such as bin files while having an extremely detailed Wiki
- **Alternative Considered**: Although [CMake](https://github.com/Kitware/CMake) was evaluated, despite it being the industry standard the general consensus is that it's extremly complicated and therefor i opted for premake.

# Library Usages

In this section, I'll outline the libraries utilized in this project and the reasons behind their selection.

## Logging
- **Library**: [spdlog](https://github.com/gabime/spdlog)
- **Description**: spdlog was chosen primarily for its detailed and well-written documentation, making it easy to integrate and use within the project. Additionally, it's widely favored within the community.
- **Alternative Considered**: Although [Boost.Log](https://github.com/boostorg/log) was evaluated, spdlog emerged as the preferred choice due to its suitability for the project's requirements.

## Window & Input
- **Library**: [GLFW](https://github.com/glfw/glfw)
- **Description**: GLFW was selected for its cross-platform compatibility and robust input handling capabilities. The decision was influenced by its comprehensive documentation and widespread adoption within the industry.

## Graphics API
- **Status**: Currently, there is a basic implementation of OpenGL. Further down the line i want to properly fully swap over to Vulkan and potentially support DirectX.
