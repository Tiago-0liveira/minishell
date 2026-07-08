<div align="center">

# minishell

**A Bash-like command-line shell, built from scratch in C.**

Custom lexer & parser · pipelines · redirections · heredocs · builtins · signal handling

![C](https://img.shields.io/badge/language-C-00599C?style=flat-square)
![Make](https://img.shields.io/badge/build-Make-427819?style=flat-square)
![Readline](https://img.shields.io/badge/uses-GNU%20Readline-blue?style=flat-square)
![42](https://img.shields.io/badge/42-project-black?style=flat-square)

</div>

---

## Overview

`minishell` is an interactive reimplementation of Bash, written entirely from scratch — including its own libc (`libft`). It reads a line of input, validates and parses it into a pipeline of commands through a hand-written lexer/parser, expands variables and quotes, then executes the pipeline via `fork`/`execve`, wiring up pipes and redirections along the way. Builtins run in-process; everything else is resolved against `PATH` and run as a child process. The shell tracks `$?`, supports command history, and handles `Ctrl-C` / `Ctrl-\` / `Ctrl-Z` the way an interactive Bash session does.

## Feature set

| Area | Feature | Status |
|---|---|:---:|
| Parsing | Pipelines (`\|`) of arbitrary length | ✓ |
| Parsing | Redirections `<`, `>`, `>>`, heredoc `<<` | ✓ |
| Parsing | Single & double quotes, with correct expansion rules | ✓ |
| Parsing | `$VAR` and `$?` expansion | ✓ |
| Parsing | `~` expansion to `$HOME` | ✓ |
| Parsing | Syntax/semantic validation (unbalanced quotes, bad pipes, ambiguous redirects) | ✓ |
| Execution | `fork`/`execve` external commands | ✓ |
| Execution | `PATH` resolution — absolute, relative, `~`, `PATH`-searched | ✓ |
| Execution | Exit status propagation (`$?`, `126`/`127`) | ✓ |
| Execution | Multi-stage pipelines with correct fd wiring | ✓ |
| Builtins | `cd`, `echo -n`, `pwd`, `export`, `unset`, `env`, `exit` | ✓ |
| Signals | `Ctrl-C`, `Ctrl-\`, `Ctrl-Z` — matches Bash's interactive behavior | ✓ |
| Logic | `&&` / `\|\|` operators | ✗ |
| Logic | `()` subshells | ✗ |
| Expansion | Wildcard / glob expansion | ✗ |
| Jobs | Job control (background jobs, suspend/resume) | ✗ |

### Detailed prompt

Building with `make detailed` swaps the plain `$>` prompt for one showing the current directory and the exit status of the last command — a green check on success, a red cross on failure:

<p align="center"><img src="assets/detailed-prompt.png" alt="minishell detailed prompt showing a green checkmark and current directory" width="360"></p>

## Getting started

**Requirements:** a C compiler, `make`, GNU Readline dev headers (`libreadline-dev` on Debian/Ubuntu), and `git`.

```sh
git clone --recurse-submodules <repo-url>
cd minishell
make
./minishell
```

Cloned without `--recurse-submodules`? Run `git submodule update --init --recursive` before `make`.

Other targets: `make clean` · `make fclean` · `make re` · `make detailed` (see [Detailed prompt](#detailed-prompt)).

## In action

Verified against a built `./minishell`:

```
$> echo "quoted $HOME end"
quoted /home/tiagoliv end

$> ls | grep m | wc -l
1

$> cat << EOF
> heredoc line one
> heredoc line two
> EOF
heredoc line one
heredoc line two

$> echo line1 > out.txt && echo line2 >> out.txt && cat < out.txt
line1
line2

$> ls does-not-exist
ls: cannot access 'does-not-exist': No such file or directory
$> echo $?
2
```

## Architecture

```
src/
├── main.c, mini.c        REPL loop; shell state via the mini() singleton accessor
├── input.c                readline() wrapper for prompt input
├── signal_handle.c        SIGINT / SIGQUIT / SIGTSTP handlers
├── path.c, path_utils.c   PATH resolution (absolute / relative / ~ / PATH search)
├── pl/                    lexer & parser — builds the t_command / t_redir linked lists
├── ex/                    execution engine — fork/execve, pipes, redirections, heredocs
└── b-ins/                 builtins: cd, echo, pwd, export, unset, env, exit

libft/                     git submodule — hand-rolled libc, ft_printf, get_next_line
```

Each parsed line becomes a linked list of `t_command` nodes (one per pipeline stage), each with its own arguments and a linked list of `t_redir` nodes. Shell-wide state lives in a single `t_mini` struct reached everywhere through the `mini()` accessor, rather than being threaded through function calls.

## Testing

`.valgrindrc` / `leak.supp` support leak-checking under `valgrind`. `test_files/` holds sample input scripts for exercising parsing and error paths, e.g. `./minishell < test_files/simple.txt`.

---

<div align="center">

A 42 School project by [tiagoliv](https://github.com/Tiago-0liveira)

</div>
