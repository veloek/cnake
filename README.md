# CNAKE

> A snake clone for terminals written from scratch without any libraries

Implementing snake in the terminal would be much easier with ncurses, but
this is more of an experiment to see what can be done with ansi escape codes.

Tested on GNU/Linux, MacOS and Windows WSL and it works surprisingly well.

```
$ gcc -o cnake *.c
$ ./cnake
```

![Screenshot](screenshot.png)

## Roadmap

- [X] Draw game on fullscreen surface
- [X] Handle user input to control snake
- [X] Game loop that moves snake and redraws
- [ ] Collision detection
- [ ] Statusbar with score and other useful stuff

See issues for more details.
