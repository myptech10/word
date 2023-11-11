# Wordcount

## Collaborators:
Dhvani Patel
Mehek Palrecha

## Design

### How-To

make words
./words [...]

### Project Structure

Part I: file IO that scans files in sequence & opens directories recursively

Part II: method that processes each file by counts words using the rules of our language as defined in class

Part III: Data Structure

### Some Design Decisions

- program will print the name of every argument that it counts words in
- while searching directories it ignore all files other than .txt files
    - if it cannot read a file that is a directory or .txt file it prints an error message, but continues to count words excluding that file
- language rules
    - for the purposes of this project, since we could not find clear guidelines on whether or not special characters such as @#$%^&*()<>[]{}|/\ were part of words, we decided they were separators and would end not be part of words
        - we considered anything in our array separators[] to be a delimiter
    - _ is not a delimiter


#### Error Handling

Program prints errors for the following cases:
- No arguments supplied
- Arguments supplied but no valid files present (e.g. empty folder)
- Arguments supplied, valid files supplied, but no words to count (e.g. empty text file)

##### Invalid Arguments

- Occasionally while traversing directories the program will encounter some files which will be marked as invalid:
    - files that the program does not have permission to read
    - 

All other files passed as arguments to the program will be treated as text files regardless of extension


## Testing

- recursive on nine,eight,seven.. files used to for testing 


### Performance

- we did a /.words to see if the words are robust 

### Correctness

- Super23Foo -> Super and Foo seperate words  
- oo-bar -> Foo-bar gets printed as foo-bar 
- foo--bar -> foo and bar seperate words 

### Edge Cases

- same_name_file - > tests files in different directories
- nine, eight, seven... -> tests files within each each other (subfiles) recursively
- emptyfile -> tests files with no words
- file does not exist/cannot open -> error in words.c write() method when fd = -1
- same_name_file.txt and all_test_cases/same_name_file.txt

//same_name_file - > tests files in different directories
// nine, eight, seven... -> tests files within each each other (subfiles) recursively
//emptyfile -> tests files with no words
//file does not exist/cannot open -> error in words.c write() method when fd = -1

- same_name_file.txt and all_test_cases/same_name_file.txt

delete before turning in:

- hyphen thing
    - long hyphen ------------
    - apostrophe case '-' two or one
    - NO HYPHEN AT BEGINNING OR END or both
    - test - vs --, - at end of file vs -- at eof
- apostrophes '''''
- single - or ?
- hyphen in word is last char in buffer edge case


- same name
- CANNOT ASSUME MAX WORD LENGTH
- describe tests readme

- word longer than buffer

- implement sorting of print
    - by freq
    - by lexicography
