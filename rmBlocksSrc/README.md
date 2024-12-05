# Use:

My small programs to help with tcl script emmeding in C.
  Not really usefull, well except to make my life easier.

## rmBlocks:

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
    - use `-k` (`rmBlocks -k < file.tcl`) to keep line
      comments not in a block `---.*---`
  - escape a character with `\\` (only for `"`, `#`, `\\`
    and `---`)

## addLineEnd

Also included is addLineEnd, which escapes strings (`"`
  to `\"`), pads lines to 55 characters, and adds a `\n\`
  to the end. Basically, convert tcl script for copy and
  paste to a to a C-string for C
  (`signed char *var = "script text here";`)

# Why exits:

Yes you are right, there is no good reason.

Mine was was embeding a tcltk script in C for freezeTB's
  gui.

Fro rmBlocks I did not like exceding my line limit and the
  if/else statement syntax. So, I called rmBlocks in my
  C code and that became my solution.

For addLineEnd it was, well I want to keep my C code neat,
  and my tcl script in a c-string (easier to edit later).
  Having `\n\` at my line break instead of at the end of
  the tcl command looked better, but also was a pain
  when I needed to copy and paste text in. And I found it
  eaiser to build a tcl script and then copy it to C then
  to build it in C. So, addLineEnd made copy, paste, and
  modify into, convert, copy, paste.

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

## rmBlocks

For a quick example outptu see `gui.tcl` for pre-rmBlocks
  and `gui-rmBlocks.tcl` for post-rmBlocks.

Takes stdin input, so using on windows is a bad idea. You
  can get the help message by inputing anything for
  example `rmBlocks -h`.

You can convert a file by pipeing it to rmBlocks for
  example `rmBlocks < script.tcl > script-out.tcl` would
  remove blocks in script.tcl and save it to
  script-out.tcl. `cat script.tcl | rmBlocks | wish`.

The lack of file input is because I was lazy. Just wanted
  soemthing quick to convert my scripts for debugging.
  Then once working I can embeded them in my program.

## addLineEnd

For a quick example outptu see `gui.tcl` for
  pre-addLineEnd and `gui-addLineEnd.tcl` for
  post-addLineEnd.

Takes stdin input, so using on windows is a bad idea. You
  can get the help message by inputing anything for
  example `addLineEnd -h`.

You can convert a file by pipeing it to addLineIn for
  example `addLineIn < script.tcl > script-out.tcl` would
  do a conversion on script.tcl and save it to
  script-out.tcl.

# Updates

- 2024-11-21:
  - added in ability to keep line comments (`-k`) to
    rmBlocks
- 2024-11-20:
  - added addLineEnd
  - updated gui.tcl to remove escaped strings
