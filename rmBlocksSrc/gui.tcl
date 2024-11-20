#!/use/bin/env wish

variable glob_fqIn "" ;
variable glob_dirOut "" ;
variable glob_prefix "Hufflepuff" ;
variable glob_mapq 15 ;

tk::frame .main -borderwidth 2 ;
pack .main -side left ;

# I am breaking the dashes so rmBlocks knows this is a
# block otherwise it would skip it (expects extactly three
# dashes)

#--- -----------------------------------------------------
| Tcl01: glob_tcl_isInt_test
|   - checks if input key is numeric and if string is
|     between max and min
| Input:
|   - value:
|     o value (%P) in entery box to check
|   - min:
|     o minimum value for value to be a match
|   - max:
|     o maximum value for value to be a match
| Output:
|   - Returns:
|     o true if last character in value is numeric and is
|       between min and max
|     o false if non-numeric or not in min/max range
\---------------------------------------------------- ---#
proc tcl_isInt_test { value min max } {
   set charIn [string index $value \"end\"] ;

   if { $value eq "" } {
      return true ;              # empty string
   } elseif { [string is integer $charIn] } {
      if { $value > $max } {
         return false ;          # outside of range
      } elseif { $value < $min } {
         return false ;          # outside of range
      }
      return true ;              # valid number
   } else {
     return false ;              # non-numeric
   }

   return true ;                 # empty case
} ; # isInt

#---~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Gui02 TOC:
'   - required input gui
'   o gui02 sec01:
'     - make required input frame
'   o gui02 sec02:
'     - set up the fastq label/button
'   o gui02 sec03:
'     - set up output directory label/button
'   o gui02 Sec04:
'     - set up prefix entry
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~---#

#--->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
^ Gui02 Sec01:
^   - make required input frame
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<---#

tk::frame .main.reqIn -borderwidth 2 ;
pack .main.reqIn -anchor w -side top ;

tk::frame .main.reqIn.fq -borderwidth 2 ;
pack .main.reqIn.fq -anchor w -side top ;

#--->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
^ Gui02 Sec02:
^   - set up the fastq label/button
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<---#

tk::label .main.reqIn.fq.lab -text \"\" ;

set fq_types {
   { {fastq}    {.fastq}    }
   { {fastq}    {.fq}       }
   { {fastq.gz} {.fastq.gz} }
   { {fastq.gz} {.fq.gz}    }
} ; # fastq file exentions

--- tk::button
    .main.reqIn.fq.but
    -text \"fastq files\"
    -command {
        set fqTitle \"select fastq files\" ;

        set fqFiles [
              tk_getOpenFile
                 -multiple TRUE
                 -title $fqTitle
                 -filetypes $fq_types

        ] ;

        if {$fqFiles eq \"\"} {}

        elseif {[llength $fqFiles] == 1}
        {
           .main.reqIn.fq.lab
              configure
              -text $fqFiles ;

          set $::glob_fqIn fqFiles ; # update list
        }

        else
        {
           .main.reqIn.fq.lab
             configure
             -text [file dirname [lindex $fqFiles 1]
           ] ;

          set $::glob_fqIn fqFiles ; # update list
        } ; # set up fastq label for input
} --- ; # command button fires

---pack
   .main.reqIn.fq.but
   .main.reqIn.fq.lab
   -side left ;
---

#--->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
^ Gui02 Sec03:
^   - set up output directory label/button
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<---#

tk::frame .main.reqIn.out -borderwidth 2 ;
pack .main.reqIn.out -anchor w -side top ;

tk::label .main.reqIn.out.lab -text \"\" ;

---tk::button
      .main.reqIn.out.but
      -text \"out dir\"
      -command {
         set outDir [
            tk_chooseDirectory
               -title \"select output directory\"
         ] ;
         if {$outDir ne \"\"}
         { # If: have output directory
            set $::glob_dirOut $outDir ;

            .main.reqIn.out.lab
               configure -text $outDir ;
         } # If: have output directory
      } ; # output directory button
---

---pack
   .main.reqIn.out.but
   .main.reqIn.out.lab
   -side left
   -anchor w ;
---

#--->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
^ Gui02 Sec04:
^   - set up prefix entry
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<---#

tk::frame .main.reqIn.prefix -borderwidth 2 ;
pack .main.reqIn.prefix -anchor w -side top ;

---tk::entry
   .main.reqIn.prefix.lab
   -textvariable glob_prefix ;
---

pack .main.reqIn.prefix.lab -anchor w -side top

#---~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Gui01 TOC:
'   - required input gui
'   o gui01 sec01:
'     - make menu frame
'   o gui01 sec02:
'     - set up required input button
'   o gui01 sec03:
'     - set up read filtering button
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~---#

#--->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
^ Gui01 Sec01:
^   - make menu frame
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<---#

tk::frame .main.menu -borderwidth 2 ;
pack .main.menu -anchor w -side top ;

#--->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
^ Gui01 Sec02:
^   - set up required input button
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<---#

---tk::button
   .main.menu.reqBut
   -text \"required\"
   -command {
      pack forget .main.reqIn ;
      pack forget .main.filt ;
      pack forget .main.menu ;

      pack .main.reqIn ;
      pack .main.menu ;
} --- ; # command button fires

pack .main.menu.reqBut -anchor w -side left

#--->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
^ Gui01 Sec03:
^   - set up read filtering button
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<---#

---tk::button
   .main.menu.filtBut
   -text \"filtering\"
   -command {
      pack forget .main.reqIn ;
      pack forget .main.filt ;
      pack forget .main.menu ;

      pack .main.filt ;
      pack .main.menu ;
} --- ; # command button fires

pack .main.menu.filtBut -anchor w -side left

#---~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Gui03 TOC:
'   - required input gui
'   o gui03 sec01:
'     - make read filtering frame
'   o gui03 sec02:
'     - mapq entry
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~---#

#--->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
^ Gui03 Sec01:
^   - make read filtering frame
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<---#

tk::frame .main.filt -borderwidth 2 ;
pack .main.filt -anchor w -side top ;

#--->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
^ Gui03 Sec02:
^   - mapq entry
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<---#

tk::frame .main.filt.mapq
pack .main.filt.mapq -anchor w -side top

---tk::label
   .main.filt.mapq.lab
   -text \"min mapq (0-93)\"
---

---tk::entry
   .main.filt.mapq.entry
   -textvariable glob_mapq
   -validate key        # validate on key press
   -vcmd { tcl_isInt_test %P 0 93 }
--- ; # get mapqping quality

---pack
   .main.filt.mapq.lab
   .main.filt.mapq.entry
   -anchor w
   -side left
---
