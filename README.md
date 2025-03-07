# README

This build of the emulator currently supports Windows only. For support of macOS and Linux, please contribute to this project or wait for future updates.
## Build Requirements

To build this emulator, you will need a C++ compiler that supports the C++23 standard. Ensure your development environment is set up accordingly.
### Compiling with C++23

To compile the emulator with C++23 support, use the following flags with your compiler:

- **GCC**: Add `-std=c++23` to your compile command.
- **Clang**: Add `-std=c++23` to your compile command.
- **MSVC**: Add `/std:c++23` to your compile command.

Ensure your compiler version supports the C++23 standard and update it if necessary.

## GameBoy Architecture Reference

For detailed information on the GameBoy architecture, please refer to the [GameBoy Architecture Reference](https://gbdev.io/pandocs/).

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Project Details

This GameBoy emulator project aims to provide a robust and accurate emulation of the original GameBoy hardware. The emulator is written in C++ and leverages modern development practices to ensure high performance and maintainability. Key features include:

- Support for a wide range of GameBoy games.
- Accurate emulation of the GameBoy CPU, GPU, and sound system.
- Save state functionality.
- Debugging tools for developers.

We are continuously working on improving the emulator and adding new features. Your contributions and feedback are highly appreciated.

## Contributing

We welcome contributions from the community! If you would like to contribute, please follow these steps:

1. Fork the repository.
2. Create a new branch for your feature or bugfix.
3. Make your changes and commit them with clear and concise messages.
4. Push your changes to your fork.
5. Open a pull request to the main repository.

Please ensure your code adheres to the project's coding standards and includes appropriate tests.