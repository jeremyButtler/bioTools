#!/use/bin/env wish

variable glob_fqIn "" ;
variable glob_dirOut "" ;
variable glob_prefix "Hufflepuff" ;
variable glob_mapq 15 ;

tk::frame .main -borderwidth 2 ;
pack .main -side left ;






proc tcl_isInt_test { value min max } {
   set charIn [string index $value "end"] ;

   if { $value eq "" } {
      return true ;              
   } elseif { [string is integer $charIn] } {
      if { $value > $max } {
         return false ;          
      } elseif { $value < $min } {
         return false ;          
      }
      return true ;              
   } else {
     return false ;              
   }

   return true ;                 
} ; 



tk::frame .main.reqIn -borderwidth 2 ;
pack .main.reqIn -anchor w -side top ;

tk::frame .main.reqIn.fq -borderwidth 2 ;
pack .main.reqIn.fq -anchor w -side top ;


tk::label .main.reqIn.fq.lab -text "" ;

set fq_types {
   { {fastq}    {.fastq}    }
   { {fastq}    {.fq}       }
   { {fastq.gz} {.fastq.gz} }
   { {fastq.gz} {.fq.gz}    }
} ; 

 tk::button .main.reqIn.fq.but -text "fastq files" -command { set fqTitle "select fastq files" ; set fqFiles [ tk_getOpenFile -multiple TRUE -title $fqTitle -filetypes $fq_types ] ; if {$fqFiles eq ""} {} elseif {[llength $fqFiles] == 1} { .main.reqIn.fq.lab configure -text $fqFiles ; set $::glob_fqIn fqFiles ;  } else { .main.reqIn.fq.lab configure -text [file dirname [lindex $fqFiles 1] ] ; set $::glob_fqIn fqFiles ;  } ; }  ; 

pack .main.reqIn.fq.but .main.reqIn.fq.lab -side left ;

tk::frame .main.reqIn.out -borderwidth 2 ;
pack .main.reqIn.out -anchor w -side top ;

tk::label .main.reqIn.out.lab -text "" ;

tk::button .main.reqIn.out.but -text "out dir" -command { set outDir [ tk_chooseDirectory -title "select output directory" ] ; if {$outDir ne ""} {  set $::glob_dirOut $outDir ; .main.reqIn.out.lab configure -text $outDir ; }  } ; 
pack .main.reqIn.out.but .main.reqIn.out.lab -side left -anchor w ;

tk::frame .main.reqIn.prefix -borderwidth 2 ;
pack .main.reqIn.prefix -anchor w -side top ;

tk::entry .main.reqIn.prefix.lab -textvariable glob_prefix ;
pack .main.reqIn.prefix.lab -anchor w -side top



tk::frame .main.menu -borderwidth 2 ;
pack .main.menu -anchor w -side top ;


tk::button .main.menu.reqBut -text "required" -command { pack forget .main.reqIn ; pack forget .main.filt ; pack forget .main.menu ; pack .main.reqIn ; pack .main.menu ;}  ; 

pack .main.menu.reqBut -anchor w -side left


tk::button .main.menu.filtBut -text "filtering" -command { pack forget .main.reqIn ; pack forget .main.filt ; pack forget .main.menu ; pack .main.filt ; pack .main.menu ;}  ; 

pack .main.menu.filtBut -anchor w -side left



tk::frame .main.filt -borderwidth 2 ;
pack .main.filt -anchor w -side top ;


tk::frame .main.filt.mapq
pack .main.filt.mapq -anchor w -side top

tk::label .main.filt.mapq.lab -text "min mapq (0-93)"
tk::entry .main.filt.mapq.entry -textvariable glob_mapq -validate key  -vcmd { tcl_isInt_test %P 0 93 } ; 

pack .main.filt.mapq.lab .main.filt.mapq.entry -anchor w -side left