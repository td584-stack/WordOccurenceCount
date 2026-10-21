# Word_Occurence_Count

Assignment 2: Word Occurence Count

Mark Filip: mf1185

Tinson Dong: td584

# design note:

## Data Structure Used To Store Words: Binary Search Tree:

1. flexible size

2. simple structure

-limitation: may ends in unbalanced tree and prolong time complexity

\n 

## Sorting (in void print_sorted(node *root)):

Sort word by ascending order according to word node's count



# test plan:

## Program:

## Words:

1. empty file -> nothing

2. multiple same words n times: 
    - apple apple apple-> apple 3

3. double hyphen: 
    - apple--orange -> apple 1; orange 1

4. single hyphen: 
    - apple-orange -> opple-orange 1

    --- single hyphen + "\'"
    - apple-' -> apple 1; ' 1 
    - '-apple -> ' 1; apple 1

    --- single hyphen + word breaker

    - apple-2 -> apple 1

5. words with apostrophe:
    - apple' -> apple' 1
    - ap'ple -> apep'le 1
    - 'apple -> 'apple 1
    - ' -> ' 1
6. large word:
    -<large word> -> <large word> 1
7. word breaker:
    - apple2orange -> apple 1; orange 1
    - apple orange -> apple 1; orange 1
    - apple+orange -> apple 1; orange 1
8. order:
    apple apple orange -> apple 2; orange 1



