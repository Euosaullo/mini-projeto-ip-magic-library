<div align="center">

# MAGIC LIBRARY

### RPG Book Inventory Backend in C

`structs` · `pointers` · `dynamic allocation` · `file handling` · `encryption`

<img src="https://img.shields.io/badge/language-C-00599C?style=for-the-badge">
<img src="https://img.shields.io/badge/compiler-GCC-orange?style=for-the-badge">
<img src="https://img.shields.io/badge/interface-terminal-222222?style=for-the-badge">
<img src="https://img.shields.io/badge/status-finished-brightgreen?style=for-the-badge">

</div>

---

```txt
+====================================================+
| MAGIC LIBRARY                                      |
| Enchanted Book Inventory                           |
+====================================================+
| Registered books: 000/100                          |
+----------------------------------------------------+
| [1] Register book                                  |
| [2] Delete book                                    |
| [3] Display book                                   |
| [4] Update book                                    |
| [5] List book titles                               |
| [6] Save and exit                                  |
+====================================================+
Choose an option:
```

---

## Quick Navigation

| Section | Description |
|:---:|---|
| [Overview](#overview) | General explanation of the project |
| [Features](#features) | Menu options and system operations |
| [RPG Attributes](#rpg-attributes) | Optional magical attributes for each book |
| [Project Layout](#project-layout) | Folder and file organization |
| [Build and Run](#build-and-run) | Bash and PowerShell commands |
| [Memory Model](#memory-model) | How books are stored in memory |
| [Save System](#save-system) | How file persistence works |
| [Encryption](#encryption) | How book titles are protected |
| [Team](#team) | Project members |

---

## Overview

**Magic Library** is a terminal-based backend system written in C for managing magical books in a fictional RPG inventory.

The project follows the requirements of Mini Project 03, using:

- `struct`
- pointers
- dynamic memory allocation
- array of pointers
- file handling
- encrypted data persistence

The program works like a small game backend. While it is running, the books are stored in memory. When the user chooses to save and exit, the system writes the current library state to a file. When the program is opened again with the same file, the saved books are loaded back into memory.

This means the project simulates a simple **save game system**.

---

## Features

| Menu Option | Feature | What it does |
|:---:|:---:|---|
| `1` | Register book | Allocates memory for a new book and stores it in the library |
| `2` | Delete book | Searches a book by ID, frees its memory and clears its slot |
| `3` | Display book | Shows all information about a specific book |
| `4` | Update book | Allows selective editing of title, author, dates and RPG attributes |
| `5` | List book titles | Shows all registered book IDs and titles |
| `6` | Save and exit | Saves the library to a file, frees memory and closes the program |

Each operation is accessed through the main menu. The menu keeps running in a loop until the user selects option `6`.

---

## RPG Attributes

Besides the required book data, each book may optionally provide RPG attributes.

A book does **not** need to have all attributes. For example:

```txt
Book A -> MAG only
Book B -> FOR and CON
Book C -> INT, SAB and MAG
```

This is handled by storing two pieces of information for each attribute:

```txt
hasAttribute -> tells if the book has that attribute
value        -> stores the attribute value
```

Available attributes:

| Code | Attribute | Meaning |
|:---:|:---:|---|
| `FOR` | Strength | Physical power and melee potential |
| `DES` | Dexterity | Agility, reflexes and balance |
| `CON` | Constitution | Health and resistance |
| `INT` | Intelligence | Reasoning, memory and knowledge |
| `SAB` | Wisdom | Instinct, intuition and perception |
| `CAR` | Charisma | Presence, willpower and persuasion |
| `MAG` | Magic | Magical potential |

Each attribute value must be between `1` and `20`.

---

## Project Layout

```txt
mini-projeto-ip-magic-library/
├── data/
│   └── magicLibrary.txt
├── docs/
│   ├── test-cases.md
│   └── video-script.md
├── include/
│   ├── encryption.h
│   ├── files.h
│   ├── library.h
│   ├── structs.h
│   └── utils.h
├── src/
│   ├── encryption.c
│   ├── files.c
│   ├── library.c
│   ├── main.c
│   └── utils.c
├── build.ps1
├── build.sh
├── CMakeLists.txt
├── Makefile
└── README.md
```

The project is separated into folders to make the code easier to understand:

| Folder | Purpose |
|:---:|---|
| `src/` | Source code files |
| `include/` | Header files |
| `data/` | Save file used by the program |
| `docs/` | Extra documentation, test cases and video script |

---

## Build and Run

### Bash

Compile:

```bash
bash build.sh
```

Run:

```bash
./library data/magicLibrary.txt
```

### PowerShell

Compile:

```powershell
.\build.ps1
```

Run:

```powershell
.\library.exe data\magicLibrary.txt
```

If PowerShell blocks script execution:

```powershell
powershell -ExecutionPolicy Bypass -File .\build.ps1
.\library.exe data\magicLibrary.txt
```

---

## Memory Model

The library is stored as an array of `100` pointers.

```c
MagicBook *library[LIBRARY_SIZE];
```

Each position can contain:

```txt
NULL              -> empty slot
MagicBook pointer -> registered book
```

When a book is registered, the program finds a free slot and allocates memory dynamically using `malloc`.

When a book is deleted, the program uses `free` and sets the position back to `NULL`.

This prevents memory leaks and keeps the inventory organized.

---

## Save System

The program receives the save file through the command line:

```bash
./library data/magicLibrary.txt
```

When the user selects option `6`, the program:

1. Goes through the library array.
2. Saves every registered book.
3. Encrypts each book title before writing it.
4. Writes the data to `data/magicLibrary.txt`.
5. Frees all dynamically allocated memory.
6. Exits the program.

When the program starts again, it reads the file and rebuilds the library in memory.

---

## Encryption

The title of each book is encrypted before being saved.

The encryption uses the complement of `255`:

```c
(char)(255 - (unsigned char)c)
```

This operation is reversible. Applying it again decrypts the text.

To avoid problems with special characters in text files, the encrypted title is stored in hexadecimal format.

---

## Core Functions

| Module | Main functions |
|:---:|---|
| `library.c` | `registerBook`, `deleteBookById`, `displayBookById`, `updateBookById`, `listBookTitles` |
| `files.c` | `saveLibraryToFile`, `loadLibraryFromFile` |
| `encryption.c` | `encryptString`, `decryptString` |
| `utils.c` | `clearInputBuffer`, `readLine`, `copyText`, `isValidDate` |

---

## Data Flow

```mermaid
flowchart LR
    A["User input"] --> B["Main menu"]
    B --> C["Library operations"]
    C --> D["Array of MagicBook pointers"]
    D --> E["Dynamic memory"]
    E --> F["Save file"]
    F --> G["Encrypted title in hex"]
    G --> H["data/magicLibrary.txt"]
    H --> I["Load on startup"]
    I --> D
```

---

## Test Guide

Recommended tests:

| Test | Expected result |
|:---:|---|
| Register a book | Book appears in the list |
| Register duplicated ID | System rejects the duplicated ID |
| Display existing book | Full book data is shown |
| Display missing ID | Error message is shown |
| Update only title | Other fields remain unchanged |
| Delete book | Book is removed and memory is freed |
| Save and run again | Data is loaded from file |
| Add only one RPG attribute | Only that attribute is displayed |

More detailed test cases are available in:

```txt
docs/test-cases.md
```

---

## Team

| Member | Name |
|:---:|---|
| Member 1 | Saullo Luiz de Moura |
| Member 2 | Manuela Renovato Amaral |

---

## Repository

```txt
https://github.com/Euosaullo/mini-projeto-ip-magic-library
```

---

<div align="center">

**Magic Library**  
A terminal-based RPG inventory backend built with C.

</div
```
