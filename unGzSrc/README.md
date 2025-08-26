# Goal:

Use `gunzip -c file.gz > file.extension`. It is faster.

gz ucompression for freezeTB. This is not as fast as
  gunzip (use over this). I am hoping to come back and
  convert the huffman trees to tables, but right know I
  am still struggling to understand how the tables work.
  For now, I just want to get back to my other code that
  uses this and also walk away from this for a bit.

# Install:

## Unix

```
if [ ! -d /usr/local/bin ];
then
   sudo mkdir -p /usr/local/bin;
fi

cd ~/Downloads;
git clone https://github.com/jeremybuttler/bioTools;
cd bioTools/unGzSrc;
make -f mkfile.unix;
sudo make -f mkfile.unix install;
```

## Windows (not tested)

For Windows download and unzip the bioTolls repository in
  your Downloads folder. Then in a developer terminal.

```
cd %HOMEPATH%\Downloads\bioTools\unGzSrc
nmake /F mkfile.win
```

Copy the `unGz.exe` file to your desired install location.
  It should be in your path.

Be warned, on windows unGz should print out windows line
  breaks `\r\n` instead of unix line breaks `\n`. If you
  are planning on using bioinformatics programs compile
  unGz with `nmake CFLAGS=-DUNIX /F mkfile.win`. This will
  force unix line breaks.

# Using:

You can get the help message with `unGz -h`.

You can unzip files with `unGz file.gz > out.txt` or
  multiple files
  with `unGz file_1.gz file_2.gz ... file_n.gz > out.txt`.

unGz will merge all output files together. So, if you want
  each extracted file in an separate file, then you must
  do each file one by one. 
