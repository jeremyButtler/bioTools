You can use a chat bot to generate the font file.

Some errors I saw in co-pilot are adding a space after
  some of the extra characters in the headers:

- Here are the errors I had using co-pilot
  - `:' :<columns>"` needs to be `:':<columns>`
  - `:" :<columns>"` needs to be `:":<columns>`
  - `::<columns>"` needs to be `:::<columns>`

“Can you convert the IBM plex font to a 14 pt font where
 each pixel is a # or a space. The header for each letter
 should be :<character>:<width_in_pixels>:. Also can you
 only use characters 32 to 126 from the ascii table.”

That was the seed that led us into:

    Choosing the exact IBM Plex family

    Deciding on pixel height

    Building the bitmap style

    Generating digits → uppercase → lowercase → punctuation

    And tuning the size down to 9 rows so your renderer could handle it

Here is the prompt co-pilot suggested for future use. I
  added a few notes to the bottom to avoid the errors I
  saw. I also added a row to the top to add in the height
  column. I also added in the header for the file.

```
I want you to generate a bitmap version of the following font:

Font family: <IBM Plex Serif / Sans / Mono / etc.>
Style: <Regular / Bold / Italic>
Pixel height: <number of rows>
Width: Variable-width per character
Character set: ASCII 32–126 only

Output format:
For each character, print a header in the form:
:<character>:<width_in_pixels>:

At the top of the file print height:<Pixel height>:<Horizontal pixel gap between characters>
Width is the number of pixels to pad after the character

Then print exactly <pixel height> rows using only '#' for
  filled pixels and ' ' (space) for empty pixels.

Design goals:
• Keep glyphs visually consistent with the chosen font family.
• Maintain a shared baseline across all characters.
• Use proportions that remain readable at the given pixel height.
• Avoid adding extra blank lines between characters.

Please generate the characters in ascending ASCII order.

For the header for ' print :':<Width>:
For the header for " print :":<Width>:
For the header for : print :::<Width>:
```
