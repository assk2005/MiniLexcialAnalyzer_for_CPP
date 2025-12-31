# Mini Lexical Analyzer for C++

## Overview
This project implements a compiler-style lexical analyzer in C++ that processes C++ source code and converts it into a stream of meaningful tokens. It simulates the lexical analysis phase of a compiler front-end, focusing on correctness and core compiler principles.

## Features
- Removes comments, whitespace, and preprocessor directives
- Identifies keywords, identifiers, operators, numeric literals, and special symbols
- Generates a clean token stream
- Handles invalid tokens gracefully
- Modular and testable design

## Core Concepts Used
- Lexical Analysis
- Compiler Front-End Design
- Deterministic Finite Automata (DFA)
- Token Classification
- C++ File Handling

## Repository Structure
MiniLexcialAnalyzer_for_CPP/
├── codes/
│   ├── main.cpp
│   ├── lexer.cpp
│   ├── lexer.h
├── testing/
└── README.md
