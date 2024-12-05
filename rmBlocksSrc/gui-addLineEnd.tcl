variable glob_fqIn \"\" ;                              \n\
variable glob_dirOut \"\" ;                            \n\
variable glob_prefix \"Hufflepuff\" ;                  \n\
variable glob_mapq 15 ;                                \n\
                                                       \n\
tk::frame .main -borderwidth 2 ;                       \n\
pack .main -side left ;                                \n\
                                                       \n\
#--- --------------------------------------------------\n\
| Tcl01: glob_tcl_isInt_test                           \n\
|   - checks if input key is numeric and if string is  \n\
|     between max and min                              \n\
| Input:                                               \n\
|   - value:                                           \n\
|     o value (%P) in entery box to check              \n\
|   - index:                                           \n\
|     o index of character to check (-1 for end)       \n\
|   - min:                                             \n\
|     o minimum value for value to be a match          \n\
|   - max:                                             \n\
|     o maximum value for value to be a match          \n\
| Output:                                              \n\
|   - Returns:                                         \n\
|     o true if last character in value is numeric and \n\
|       is between min and max                         \n\
|     o false if non-numeric or not in min/max range   \n\
\------------------------------------------------- ---#\n\
---proc tcl_isInt_test { value index min max } {       \n\
   if { $index < 0 }                                   \n\
   { # If: a negative index input                      \n\
      set charIn [string index $value \"end\"] ;       \n\
   } # If: a negative index input                      \n\
                                                       \n\
   else                                                \n\
   { # Else: index of character input                  \n\
      set charIn [string index $value $index] ;        \n\
   } ; # Else: index of character input                \n\
                                                       \n\
   if { $value eq \"\" }                               \n\
   { # If: nothing in string                           \n\
      return true ;              # empty string        \n\
   } # If: nothing in string                           \n\
                                                       \n\
   elseif { [string is integer $charIn] }              \n\
   { # Else If: new character is integer               \n\
      if { $value > $max }                             \n\
         { return false ; }        # outside max range \n\
                                                       \n\
      elseif { $value < $min }                         \n\
         { return false ; }        # outside min range \n\
                                                       \n\
      else                                             \n\
         { return true ; }         # valid number      \n\
   } # Else If: new character is integer               \n\
                                                       \n\
   else                                                \n\
      { return false ; } ;         # non-numeric       \n\
                                                       \n\
   return true ;                   # empty case        \n\
}--- ; # isInt                                         \n\
                                                       \n\
#---~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   \n\
' Gui02 TOC:                                           \n\
'   - required input gui                               \n\
'   o gui02 sec01:                                     \n\
'     - make required input frame                      \n\
'   o gui02 sec02:                                     \n\
'     - set up the fastq label/button                  \n\
'   o gui02 sec03:                                     \n\
'     - set up output directory label/button           \n\
'   o gui02 Sec04:                                     \n\
'     - set up prefix entry                            \n\
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~---#   \n\
                                                       \n\
#--->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   \n\
^ Gui02 Sec01:                                         \n\
^   - make required input frame                        \n\
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<---#   \n\
                                                       \n\
tk::frame .main.reqIn -borderwidth 2 ;                 \n\
pack .main.reqIn -anchor w -side top ;                 \n\
                                                       \n\
tk::frame .main.reqIn.fq -borderwidth 2 ;              \n\
pack .main.reqIn.fq -anchor w -side top ;              \n\
                                                       \n\
#--->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   \n\
^ Gui02 Sec02:                                         \n\
^   - set up the fastq label/button                    \n\
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<---#   \n\
                                                       \n\
tk::label .main.reqIn.fq.lab -text \"\" ;              \n\
                                                       \n\
set fq_types {                                         \n\
   { {fastq}    {.fastq}    }                          \n\
   { {fastq}    {.fq}       }                          \n\
   { {fastq.gz} {.fastq.gz} }                          \n\
   { {fastq.gz} {.fq.gz}    }                          \n\
} ; # fastq file exentions                             \n\
                                                       \n\
--- tk::button                                         \n\
    .main.reqIn.fq.but                                 \n\
    -text \"fastq files\"                              \n\
    -command {                                         \n\
        set fqTitle \"select fastq files\" ;           \n\
                                                       \n\
        set fqFiles [                                  \n\
              tk_getOpenFile                           \n\
                 -multiple TRUE                        \n\
                 -title $fqTitle                       \n\
                 -filetypes $fq_types                  \n\
                                                       \n\
        ] ;                                            \n\
                                                       \n\
        if {$fqFiles eq \"\"} {}                       \n\
                                                       \n\
        elseif {[llength $fqFiles] == 1}               \n\
        {                                              \n\
           .main.reqIn.fq.lab                          \n\
              configure                                \n\
              -text $fqFiles ;                         \n\
                                                       \n\
          set $::glob_fqIn fqFiles ; # update list     \n\
        }                                              \n\
                                                       \n\
        else                                           \n\
        {                                              \n\
           .main.reqIn.fq.lab                          \n\
             configure                                 \n\
             -text [file dirname [lindex $fqFiles 1]   \n\
           ] ;                                         \n\
                                                       \n\
          set $::glob_fqIn fqFiles ; # update list     \n\
        } ; # set up fastq label for input             \n\
} --- ; # command button fires                         \n\
                                                       \n\
---pack                                                \n\
   .main.reqIn.fq.but                                  \n\
   .main.reqIn.fq.lab                                  \n\
   -side left ;                                        \n\
---                                                    \n\
                                                       \n\
#--->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   \n\
^ Gui02 Sec03:                                         \n\
^   - set up output directory label/button             \n\
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<---#   \n\
                                                       \n\
tk::frame .main.reqIn.out -borderwidth 2 ;             \n\
pack .main.reqIn.out -anchor w -side top ;             \n\
                                                       \n\
tk::label .main.reqIn.out.lab -text \"\" ;             \n\
                                                       \n\
---tk::button                                          \n\
      .main.reqIn.out.but                              \n\
      -text \"out dir\"                                \n\
      -command {                                       \n\
         set outDir [                                  \n\
            tk_chooseDirectory                         \n\
               -title \"select output directory\"      \n\
         ] ;                                           \n\
         if {$outDir ne \"\"}                          \n\
         { # If: have output directory                 \n\
            set $::glob_dirOut $outDir ;               \n\
                                                       \n\
            .main.reqIn.out.lab                        \n\
               configure -text $outDir ;               \n\
         } # If: have output directory                 \n\
      } ; # output directory button                    \n\
---                                                    \n\
                                                       \n\
---pack                                                \n\
   .main.reqIn.out.but                                 \n\
   .main.reqIn.out.lab                                 \n\
   -side left                                          \n\
   -anchor w ;                                         \n\
---                                                    \n\
                                                       \n\
#--->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   \n\
^ Gui02 Sec04:                                         \n\
^   - set up prefix entry                              \n\
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<---#   \n\
                                                       \n\
tk::frame .main.reqIn.prefix -borderwidth 2 ;          \n\
pack .main.reqIn.prefix -anchor w -side top ;          \n\
                                                       \n\
---tk::entry                                           \n\
   .main.reqIn.prefix.lab                              \n\
   -textvariable glob_prefix ;                         \n\
---                                                    \n\
                                                       \n\
pack .main.reqIn.prefix.lab -anchor w -side top        \n\
                                                       \n\
#---~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   \n\
' Gui01 TOC:                                           \n\
'   - required input gui                               \n\
'   o gui01 sec01:                                     \n\
'     - make menu frame                                \n\
'   o gui01 sec02:                                     \n\
'     - set up required input button                   \n\
'   o gui01 sec03:                                     \n\
'     - set up read filtering button                   \n\
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~---#   \n\
                                                       \n\
#--->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   \n\
^ Gui01 Sec01:                                         \n\
^   - make menu frame                                  \n\
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<---#   \n\
                                                       \n\
tk::frame .main.menu -borderwidth 2 ;                  \n\
pack .main.menu -anchor w -side top ;                  \n\
                                                       \n\
#--->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   \n\
^ Gui01 Sec02:                                         \n\
^   - set up required input button                     \n\
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<---#   \n\
                                                       \n\
---tk::button                                          \n\
   .main.menu.reqBut                                   \n\
   -text \"required\"                                  \n\
   -command {                                          \n\
      pack forget .main.reqIn ;                        \n\
      pack forget .main.filt ;                         \n\
      pack forget .main.menu ;                         \n\
                                                       \n\
      pack .main.reqIn ;                               \n\
      pack .main.menu ;                                \n\
} --- ; # command button fires                         \n\
                                                       \n\
pack .main.menu.reqBut -anchor w -side left            \n\
                                                       \n\
#--->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   \n\
^ Gui01 Sec03:                                         \n\
^   - set up read filtering button                     \n\
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<---#   \n\
                                                       \n\
---tk::button                                          \n\
   .main.menu.filtBut                                  \n\
   -text \"filtering\"                                 \n\
   -command {                                          \n\
      pack forget .main.reqIn ;                        \n\
      pack forget .main.filt ;                         \n\
      pack forget .main.menu ;                         \n\
                                                       \n\
      pack .main.filt ;                                \n\
      pack .main.menu ;                                \n\
} --- ; # command button fires                         \n\
                                                       \n\
pack .main.menu.filtBut -anchor w -side left           \n\
                                                       \n\
#---~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   \n\
' Gui03 TOC:                                           \n\
'   - required input gui                               \n\
'   o gui03 sec01:                                     \n\
'     - make read filtering frame                      \n\
'   o gui03 sec02:                                     \n\
'     - mapq entry                                     \n\
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~---#   \n\
                                                       \n\
#--->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   \n\
^ Gui03 Sec01:                                         \n\
^   - make read filtering frame                        \n\
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<---#   \n\
                                                       \n\
tk::frame .main.filt -borderwidth 2 ;                  \n\
pack .main.filt -anchor w -side top ;                  \n\
                                                       \n\
#--->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>   \n\
^ Gui03 Sec02:                                         \n\
^   - mapq entry                                       \n\
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<---#   \n\
                                                       \n\
tk::frame .main.filt.mapq                              \n\
pack .main.filt.mapq -anchor w -side top               \n\
                                                       \n\
---tk::label                                           \n\
   .main.filt.mapq.lab                                 \n\
   -text \"min mapq (0-93)\"                           \n\
---                                                    \n\
                                                       \n\
---tk::entry                                           \n\
   .main.filt.mapq.entry                               \n\
   -textvariable glob_mapq                             \n\
   -validate key        # validate on key press        \n\
   -vcmd { tcl_isInt_test %P %i 0 93 }                 \n\
--- ; # get mapqping quality                           \n\
                                                       \n\
---pack                                                \n\
   .main.filt.mapq.lab                                 \n\
   .main.filt.mapq.entry                               \n\
   -anchor w                                           \n\
   -side left ;                                        \n\
---                                                    \n\
