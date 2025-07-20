# RandomWriter — Word-Level Markov Text Generator in C++

📄 RandomWriter is a C++ application that generates logical text output using a word-level Markov model. By training on different source texts, it captures stylistic and statistical patterns of word transitions and uses them to extend or simulate writing in a consistent manner.

## 🔍 How It Works

- Users select a writing style based on preloaded training `.txt` files.
- The program builds a Markov map of word transitions from the selected file.
- Users then provide an input file to extend or complete.
- The program identifies the most frequent words in the user input and merges them into the trained model.
- It then generates new text using the blended Markov model.

## ✅ Features

- Word-level Markov model for generating coherent text
- Trains on any number of user-supplied `.txt` style files
- Automatically extracts and merges most frequent words from input documents
- Produces consistent and statistically grounded sequences
- Easy to customize constants (merge size, word count, training sources)

## 💡 Example Use Cases

- Text generation from a selected writing corpus  
- Extension of incomplete articles, reports, or student papers  
- Modeling writing patterns across multiple authors  
- Studying transition probabilities in natural language

## 🧰 Requirements

- C++ (C++11 or later)
- Stanford C++ libraries: `console.h`, `simpio.h`, etc.
- Compatible build environment (e.g., Qt, Visual Studio, or terminal with proper setup)

## 📂 Customization

- Add more `.txt` training files in the source directory
- Modify constants such as:
  - `NUM_OF_WORDS_MERGED`
  - `MIN_WORD_TO_GENERATE`
  - `NUM_OF_TRAINING_FILES`

## 📌 Notes

- All training and input files should be present in the same directory as the `.cpp` source
- Generated text depends on the overlap between the training corpus and input content
- Graceful early exit occurs if the current word has no known next word in the Markov chain

