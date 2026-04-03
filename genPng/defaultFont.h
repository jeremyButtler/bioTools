#ifndef LOAD_THE_DELETE_FONT_H
#define LOAD_THE_DELETE_FONT_H

struct font_pngFont;

/*______________________________________________
` Fun95: loadFont_defaultFont
`  - loads the font in defaultFont.c/h to the input
`    font_pngFont struct (as pionter)
`  - returns 1 for success or 0 for failure
`_____________________________________________*/
signed char
loadFont_defaultFont(
   struct font_pngFont *fontSTPtr
);

#endif
