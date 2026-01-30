# MarkovText: Stochastic N-Gram Prediction Engine

![Language](https://img.shields.io/badge/language-C++-blue.svg)
![Type](https://img.shields.io/badge/category-NLP-orange.svg)
![Status](https://img.shields.io/badge/status-Active-green.svg)

MarkovText is a C++ application that implements a word-level Markov Chain algorithm to simulate natural language patterns. By analyzing the statistical distribution of word transitions in a source corpus, the engine builds a probability map (N-gram model) to generate coherent, stylistically consistent text extensions.

This project serves as a low-level demonstration of the probabilistic principles behind modern Large Language Models (LLMs).

## Key Features

- **N-Gram Modeling:** Builds a dynamic graph of word states and transition probabilities (Order-K Markov Model) to predict the next token in a sequence.
- **Contextual Adaptation:** Features a "Merge" algorithm that analyzes user input, identifies high-frequency terms, and biases the generation model to mimic the user's specific writing style.
- **Corpus Agnostic:** Capable of ingesting any plain text (.txt) corpus—from Shakespeare to financial reports—and adopting its statistical voice immediately.
- **Efficient Lookups:** Utilizes high-performance hash maps to store thousands of state transitions with O(1) average access time.

## Technical Implementation

### The Algorithm
The engine parses text into a multi-dimensional map structure:
1. **Training:** The source text is scanned to create a frequency table of `Seed -> {Next_Word, Probability}`.
2. **Analysis:** The "Order" (K) determines how many previous words constitute a "state." (e.g., in an Order-2 model, "The quick" predicts "brown").
3. **Generation:** Starting from a seed, the engine performs a weighted random walk through the probability graph to construct new sentences.

### Complexity
- **Time Complexity:** O(N) for training (linear scan of corpus).
- **Space Complexity:** O(U) where U is the number of unique K-gram combinations in the source text.

## Installation & Usage

### Prerequisites
- C++ Compiler (g++ or clang) supporting C++11 standard.
- StanfordCPPLib (Required for console interface and simplified I/O).

### Usage
1. Compile the source code using the provided Makefile or your preferred IDE.
2. Place your training data (e.g., `literature.txt`) in the `res/` directory.
3. Run the executable:

    ./MarkovText

4. Follow the console prompts to select a "Base Style" (training file) and an "Input Context" (file to extend).

## Configuration
The generation behavior can be fine-tuned via constants in the header file:

- **NUM_OF_WORDS_MERGED:** Controls how aggressively the model blends the user's input style with the base corpus.
- **MIN_WORD_TO_GENERATE:** Sets the lower bound for output length.
- **ORDER_K:** (Advanced) Adjusts the Markov chain order (higher K = more coherent but less creative).

## Project Structure

    ├── src/
    │   ├── RandomWriter.cpp    # Core Markov Chain logic and generation loop
    │   └── main.cpp            # Entry point and UI handling
    ├── res/                    # Training corpora (.txt files)
    ├── README.md
    └── LICENSE

## Future Improvements
- Refactor to remove dependencies on educational libraries (Standard STL only).
- Implement character-level generation for code or poetry modeling.
- Add serialization to save trained models to disk for instant loading.

---

**Author:** Luka Aladashvili
