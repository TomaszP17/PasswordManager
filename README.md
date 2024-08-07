# Password Manager

## Overview

Welcome to the Password Manager project! This program, developed by Tomasz, is a console-based application designed to securely store, manage, and organize passwords. The application supports functionalities such as adding, removing, editing, and searching passwords, along with managing password categories. It also offers features like password generation and checking password security.

## Features

- **Secure Password Storage**: Save and encrypt passwords using a user-defined key.
- **Password Management**: Add, edit, and delete passwords as needed.
- **Password Generation**: Automatically generate secure passwords based on user-specified criteria.
- **Search and Sort**: Search for passwords by different parameters and sort them.
- **Category Management**: Manage categories by adding or removing them.
- **Timestamping**: Track the last modification time of the password list.

## Project Structure

- **PasswordManager.h/.cpp**: Contains the main logic for managing passwords and interacting with the user.
- **Password.h/.cpp**: Defines the `Password` class for handling individual password records.
- **Timestamp.h/.cpp**: Provides functionality to get the current time for timestamps.

## Prerequisites

- A C++ compiler that supports C++17 or later.
- A terminal or command prompt to run the application.

## Usage

When you start the application, you will be prompted to enter a file name to load existing passwords or create a new one. You will also need to provide a password to access the file.

### Menu Options

- **Display passwords**: View all stored passwords and the last modification timestamp.
- **Add a new password**: Enter details to add a new password record.
- **Delete password**: Remove one or more password records.
- **Edit password**: Modify an existing password record.
- **Search passwords**: Look for passwords based on specified criteria.
- **Sort passwords**: Arrange passwords by different attributes.
- **Add categories**: Create new categories for organizing passwords.
- **Delete categories**: Remove categories and associated passwords.
- **End program**: Exit the application.

## Security

- The application encrypts passwords using a user-defined key, ensuring that your data remains secure.
- The password strength checker ensures that user-provided passwords are safe by checking for length, uppercase, lowercase, digits, and special characters.

## Contribution

Contributions are welcome! Feel free to submit issues and pull requests to enhance the functionality or fix any bugs.

## License

This project is open-source and available under the [MIT License](LICENSE).
