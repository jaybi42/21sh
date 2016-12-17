# 21sh
42's project 21sh is about to create a Shell from scratch based on bash. We have to do the line edition with the Termcaps Library.

#Features
- Tokens:
    - ||
    - &&
    - |
    - &
    - <
    - >
    - <<
    - >>
    - "
    - '
    - \`
- Builtins:
    - env
    - unsetenv
    - setenv
    - cd
    - alias
    - echo
    - history
    - exit
    - globing
    - clear
    - getenv
    - export
    - 42info
- Autocompletion
- Hash table to execute binaries
- History research (CTRL+R)
- Globing:
    - *
    - ?
    - and more
- Line edition:
    - CTRL+K:       CUT
    - CTRL+P:       PASTE
    - CTRL+UP:      Move to the line above
    - CTRL+DOWN:    Move to the line under
    - Home:         Move to the start, at the prompt
    - End:          Move to the end
    - LEFT:         Move to the left
    - RIGHT:        Move to the right
    - CTRL+RIGHT    Move to the next word
    - CTRL+LEFT     Move to the previous word
- History
    - !
    - !!
    - UP:   Previous CMD
    - DOWN: Next CMD


#Ideas for futher featues
- Multiline edition
- Job control
- Shell script
