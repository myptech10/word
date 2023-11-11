# Wordcount

## Collaborators:
Dhvani Patel (dgp69)
Mehek Palrecha (myp32)

## Design

### How-To

make words

./words [...]

### Project Structure

Part I: file IO that scans files in sequence & opens directories recursively, see recursiveDirectory()

Part II: method that processes each file by counts words using the rules of our language as defined in class, see count_words()

Part III: Hash map data structure that holds nodes of each word containing frequency of word, see hash() and insertHash()

### Some Design Decisions

- program will print the name of every argument that it counts words in
- while searching directories it ignore all files other than .txt files
    - if it cannot read a file that is a directory or .txt file it prints an error message, but continues to count words excluding that file
- language rules
    - for the purposes of this project, since we could not find clear guidelines on whether or not special characters such as @#$%^&*()<>[]{}|/\ were part of words, we decided they were separators and would end not be part of words
        - we considered anything in our array separators[] to be a delimiter
    - _ is not a delimiter
    - considered hyphens between letters to be one word (a-a) but hyphens between apostrophes to be two ('-')


#### Error Handling

Program prints errors for the following cases:
- No arguments supplied
- Arguments supplied but no valid files present (e.g. empty folder)
- Arguments supplied, valid files supplied, but no words to count (e.g. empty text file)

##### Invalid Arguments

- Occasionally while traversing directories the program will encounter some files which will be marked as invalid:
    - files that the program does not have permission to read
    - files not ending in .txt
- All files passed as arguments to the program will be treated as text files regardless of extension


## Testing

### Performance

- ./words all_test_cases/nine to see if directory traversal could handle many recursive calls
-  ./words . to see if the program could handle a large amount of files of varying types and words of varying lengths at once. This was basically an everything test

### Correctness

- loremipsum.txt tests sample text for counting words
- several files test language rules around hyphens and apostrophes
- also copied example files from project writeup, spam.txt, example.txt

### Edge Cases

- same_name_file - > tests files in different directories
- emptyfile -> tests files with no words
- unreadablefile.txt -> error in words.c write() method when fd = -1
- same_name_file.txt and all_test_cases/same_name_file.txt