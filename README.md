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
|---|---|
| [Overview](#overview) | What the project does |
| [Features](#features) | Main system operations |
| [RPG Attributes](#rpg-attributes) | Optional book stats |
| [Project Layout](#project-layout) | Folder organization |
| [Build and Run](#build-and-run) | Bash and PowerShell commands |
| [Data Flow](#data-flow) | How memory and files interact |
| [Core Functions](#core-functions) | Main implemented functions |
| [Team](#team) | Group members |

---

## Overview

**Magic Library** is a backend system written in C for managing a magical book inventory in an RPG game.

Each book is stored dynamically in memory and can be saved to or loaded from a file. The project follows the required structure from the assignment while also adding optional RPG attributes to make the system more creative and game-like.

The program uses a fixed inventory with `100` slots. Each slot stores either:

```txt
NULL                  -> empty slot
MagicBook pointer     -> dynamically allocated book
```

---

## Features

| Option | Action | What it does |
|---:|---|---|
| `1` | Register book | Allocates and stores a new magical book |
| `2` | Delete book | Finds a book by ID and frees its memory |
| `3` | Display book | Shows all information about one book |
| `4` | Update book | Allows selective editing of book fields |
| `5` | List book titles | Displays all registered book IDs and titles |
| `6` | Save and exit | Writes the library to disk and closes the program |

---

## RPG Attributes

Books may optionally provide RPG attributes. A book does **not** need to have all attributes.

| Code | Attribute | Meaning |
|---|---|---|
| `FOR` | Strength | Physical power and melee potential |
| `DES` | Dexterity | Agility, reflexes and balance |
| `CON` | Constitution | Health and resistance |
| `INT` | Intelligence | Reasoning, memory and knowledge |
| `SAB` | Wisdom | Instinct, intuition and perception |
| `CAR` | Charisma | Presence, willpower and persuasion |
| `MAG` | Magic | Magical potential |

Each attribute has a value from `1` to `20`.

Example:

```txt
FOR / Strength: 12
INT / Intelligence: 17
MAG / Magic: 19
```

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

---

## Build and Run

### Bash

```bash
bash build.sh
./library data/magicLibrary.txt
```

### PowerShell

```powershell
.\build.ps1
.\library.exe data\magicLibrary.txt
```

If PowerShell blocks script execution:

```powershell
powershell -ExecutionPolicy Bypass -File .\build.ps1
.\library.exe data\magicLibrary.txt
```

---

## Data Flow

```mermaid
flowchart LR
    A["User input"] --> B["Menu"]
    B --> C["Library operations"]
    C --> D["MagicBook pointers"]
    D --> E["Memory inventory: 100 slots"]
    E --> F["Save file"]
    F --> G["Encrypted title in hex"]
    G --> H["data/magicLibrary.txt"]
    H --> I["Load on startup"]
    I --> E
```

---

## Core Data Structures

<details>
<summary><strong>Date</strong></summary>

```c
typedef struct
{
    int day;
    int month;
    int year;
} Date;
```

</details>

<details>
<summary><strong>Author</strong></summary>

```c
typedef struct
{
    char name[TEXT_SIZE];
    Date birthDate;
} Author;
```

</details>

<details>
<summary><strong>MagicBook</strong></summary>

```c
typedef struct
{
    int id;
    char title[TEXT_SIZE];
    Author author;
    Date writingDate;
    BookAttributes attributes;
} MagicBook;
```

</details>

---

## Core Functions

| Module | Functions |
|---|---|
| `library.c` | `registerBook`, `deleteBookById`, `displayBookById`, `updateBookById`, `listBookTitles` |
| `files.c` | `saveLibraryToFile`, `loadLibraryFromFile` |
| `encryption.c` | `encryptString`, `decryptString` |
| `utils.c` | `clearInputBuffer`, `readLine`, `copyText`, `isValidDate` |

---

## Save System

The program receives the save file as a command-line argument:

```bash
./library data/magicLibrary.txt
```

When the user chooses option `6`, the program:

1. Encrypts each book title.
2. Converts the encrypted title to hexadecimal.
3. Saves all registered books to `data/magicLibrary.txt`.
4. Frees all allocated memory.
5. Exits the program.

---

## Encryption

The title encryption uses the complement of `255`.

```c
(char)(255 - (unsigned char)c)
```

The same operation is used for encryption and decryption.

To keep the save file readable and stable, encrypted titles are stored in hexadecimal format.

---

## Test Guide

Recommended tests:

| Test | Expected result |
|---|---|
| Register a book | Book appears in the list |
| Register duplicated ID | System rejects the ID |
| Display existing book | Full book data is shown |
| Display missing ID | Error message is shown |
| Update only title | Other fields remain unchanged |
| Delete book | Slot becomes empty |
| Save and run again | Data is loaded from file |
| Add only one RPG attribute | Only that attribute is displayed |

More detailed test cases are available in:

```txt
docs/test-cases.md
```

---

## Team

| Member | Role |
|---|---|
| Member 1 | Development |
| Member 2 | Development |
| Member 3 | Development |
| Member 4 | Development |

---

## Repository

```txt
https://github.com/Euosaullo/mini-projeto-ip-magic-library
```

---

<div align="center">

**Magic Library**  
A terminal-based RPG inventory backend built with C.

</div>
