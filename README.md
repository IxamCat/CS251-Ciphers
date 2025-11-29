# CS 251 – Ciphers Project

A C++ command-line tool that encrypts, decrypts, and analyzes text using classical ciphers and heuristic search.

## Overview

This project was built for CS 251 to explore algorithm design, file I/O, and text analysis under tight C++ constraints.  
The program supports both Caesar and substitution ciphers and includes tools to automatically recover plaintext from ciphertext.

## Features

- **Caesar cipher**
  - Encrypt plaintext with a given shift
  - Decrypt ciphertext even when the key is unknown
  - Brute-force search over all shifts

- **Substitution cipher**
  - Apply random substitution keys
  - Decrypt substitution ciphers with:
    - Dictionary-based scoring
    - Quadgram “English-ness” scoring
    - Hill-climbing optimization to improve keys

- **File I/O**
  - Read ciphertext from files
  - Write recovered plaintext to output files

- **Menu-driven interface**
  - Simple text-based menu for all operations
  - Clear prompts and error handling

## Skills & Concepts

- C++ programming
- Function decomposition and modular design
- Algorithm design & heuristic search (hill-climbing)
- Text processing and scoring (quadgrams, dictionaries)
- File I/O and working with test data

## How to Build & Run

```bash
# From the project root
make

# Run the main program
./ciphers_main

# Run tests (if provided)
./ciphers_tests
