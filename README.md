# word

delete before turning in:

part 1: need file IO that scans files in sequence & opens directories recursively
- need to learn abt file IO if you open a file in a method does it stay open?
part 2: call a method that processes the file and counts words for each file

main things to consider:
- how are we going to actually count the words? I think a list or tree of separator chars would be helpful
- how to do file IO? how to recursively sort through directories?
    - buffer edge case: what if a word is smaller than the buffer?
- what data structure are we going to use to store words and frequencies? and how will we sort when returning words? sort in place?
hash: based on word
table is sorted lexicographically
sort at the end, find max freq and num of words with that freq then remove lexicographically