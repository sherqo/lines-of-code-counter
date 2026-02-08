# loc - Lines of Code Counter

## USAGE:

./app [path] Count lines in all files recursively from path
./app -f <file> Count lines in a single file

## EXAMPLES:

./app Count all files in current directory
./app /home/user/code Count all files in /home/user/code
./app -f main.c Count lines in main.c only

## NOTES:

- Counts newline characters (\n)
- Files without trailing newline still count the last line
- Skips symbolic links
