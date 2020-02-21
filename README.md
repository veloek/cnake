# CNAKE

> A snake clone for terminals written from scratch without any libraries

Implementing snake in the terminal would be much easier with ncurses, but
this is more of an experiment to see what can be done with ansi escape codes.

Tested on GNU/Linux, MacOS and Windows WSL and it works surprisingly well.

```
$ gcc -o cnake *.c
$ ./cnake
```

Use h, j, k and l to move around, just like in [Vim](https://www.vim.org).
Arrow keys are also supported, if you prefer that.

![Screenshot](screenshot.png)

## Roadmap

- [X] Draw game on fullscreen surface
- [X] Handle user input to control snake
- [X] Game loop that moves snake and redraws
- [X] Collision detection
- [ ] Statusbar with score and other useful stuff
- [ ] Fancy mode with unicode characters

See [issues](https://github.com/veloek/cnake/issues) for more details.
