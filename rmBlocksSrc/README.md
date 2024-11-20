# Use:

Pre-processor to merge block entries and then remove the
  block markers. It also removes any comments, except the
  shebang `#!` on the frist line.

- Special symbols:
  - start a block with `---` (must be exactly three)
    - `--- value` will got to ` value`
    - `---value` will got to `value` (no extra space)
  - end a block with `---`   (must be exactly three)
  - start a comment block with `#---`
  - end a comment block with `---#` or `---`
    - using `---#` is clearer
  - comment a line with `#`
    - often leaves a blank line, but works otherwise
  - escape a character with `\\` (only for `"`, `#`, `\\`
    and `---`)

# Why exits:

Yes you are right, there is no good reason.

Mine was was embeding a tcltk script in C for freezeTB's
  gui. I did not like exceding my line limit and the
  if/else statement syntax. So, I called rmBlocks in my
  C code and that became my solution.

# License:

Dual licensed with Unlicense and MIT. Pick the one that
  works for you.

# Install:

## Unix:

```
make -f mkfile.unix
sudo make -f mkfile.unix install
```

If you want to strip most of the indenting. `-DBLANK` does
  nothing, or well, to be more precise it removes `-DBUG`.

```
make -f mkfile.unix -DBLANK
sudo make -f mkfile.unix install
```

## Windows

Not going to bother. Use nmake and mkfile.win. If you want
  indenting define `BUG` (set `EXTRA_FLAGS=` to the
  correct compiler command). No idea how to do it
  with `cl`.
  
# Using:

For a quick example outptu see `gui.tcl` for pre-rmBlocks
  and `gui-rmBlocks.tcl` for post-rmBlocks.

Takes stind input, so using on windows is a bad idea. You
  can get the help message by inputing anything for
  example `rmBlocks -h`.

You can convert a file by pipeing it to rmBlocks for
  example `rmBlocks < script.tcl > script-out.tcl` would
  remove blocks in script.tcl and save it to
  script-out.tcl. `cat script.tcl | rmBlocks | wish`.

The lack of file input is because I was lazy. Just wanted
  soemthing quick to convert my scripts for debugging.
  Then once working I can embeded them in my program.

# Updates
