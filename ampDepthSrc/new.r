#!/usr/bin/env Rscript

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# graphAmpDepth.r SOF: Start Of File
#   - makes graphs for output from ampDepth
#   - you are limited to two flags
#   o part01:
#     - included libraries
#   o fun01: findFIle_graphAmpDepth
#     - checks to see if a freezeTB file exists
#   o fun02: phelp_graphAmpDepth
#     - help message function
#   o fun03: save_graphAmpDepth
#     - saves a graph (call before plotting graph)
#   o part02:
#     - script variables, user input, and prep
#   o part03:
#     - read depth graph
#   o part04:
#     - gene coverage graphs
#   o part05:
#     - amplicon coverage graphs
#   o license:
#     - Licensing for this code (public domain / mit)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#---------------------------------------------------------
# Part01:
#   - included libraries
#---------------------------------------------------------

#---------------------------------------------------------
# Fun01: findFIle_graphAmpDepth
#   - checks to see if a freezeTB file exists
# Input:
#   - fileStr:
#     o freezeTB file to hunt for
# Output:
#   - Returns:
#     o path to file if exists
#     o NULL if file could not be found
#---------------------------------------------------------
findFile_graphAmpDepth = function(fileStr){

   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   # Fun01 TOC:
   # fun01 sec01:
   #   - variable declarations
   # fun01 sec02:
   #   - set up and check if file is in pwd
   # fun01 sec03:
   #   - check if file is in home/Documents directory
   # fun01 sec04:
   #   - check if file is in global install location
   #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Fun01 Sec01:
   #   - variable declarations
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   slashSC = .Platform$file.sep;
   pathSplitStr = ':';
   targetStr = "HOME";

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Fun01 Sec02:
   #   - set up and check if file is in pwd
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   if(.Platform$OS == "windows")
      pathSplitStr = ';';
   
   if(file.exists(fileStr))
      return(fileStr); # found file location

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Fun01 Sec03:
   #   - check if file is in home/Documents directory
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   if(.Platform$OS == "windows"){
      targetStr = "HOMEPATH";
   } # set up home location variable

   tmpStr =
      as.list(
         strsplit(
            Sys.getenv(targetStr),
            pathSplitStr
         )
      )[[1]][1]; # get local file path

   tmpStr =
      paste(
          tmpStr,
          "Documents",
          "freezeTBFiles",
          fileStr,
          sep = slashSC
      ); # add database file

   if(file.exists(tmpStr))
      return(tmpStr); # found file location

   #>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   # Fun01 Sec04:
   #   - check if file is in global install location
   #<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

   if(.Platform$OS == "windows"){
      tmpStr =
         as.list(
            strsplit(
               Sys.getenv("PUBLIC"),
               pathSplitStr
            )
         )[[1]][1]; # get local file path

      tmpStr =
         paste(
             tmpStr,
             "Documents",
             "freezeTBFiles",
             fileStr,
             sep = slashSC
         ); # add database file
   }else{
      tmpStr =
         paste(
            "/usr/local/share/freezeTBFiles",
             fileStr,
             sep = slashSC
         );
   } # Else: get global file path for unix
    
   if(file.exists(tmpStr))
      return(tmpStr); # found file location

   return(NULL);
} # findFile_graphAmpDepth


#---------------------------------------------------------
# Fun02: phelp_graphAmpDepth
#   - help message function
# Input:
#   - inStr:
#     o input argument to check for help message
# Output:
#   - Prints:
#     o help mesesage to screen if inStr has help flag
#   - Returns:
#     o TRUE if printed help message
#     o FALSE if inStr is not help flag (no printing)
#---------------------------------------------------------
phelp_graphAmpDepth = function(inStr){

 amrFileStr = NULL:

 if(inStr == "-h" ||
    inStr == "--h" ||
    inStr == "-help" ||
    inStr == "--help" ||
    inStr == "help"){

       amrFileStr = findFile_graphAmpDepth("amrDb.tsv");

       print("RScript graphAmpDepth.r -stats stats.tsv");
       print("  - Makes a graph of mapped amplicons");
       print("Input:");
       print("  -stats: [Required]");
       print("    o Tsv file with amplicon stats output");
       print("      by ampDepth");
       print("  -prefix: [Hufflepuff]");
       print("    o Prefix to add to output file names");
       print("  -min-len: [50]");
       print("    o minimum length to keep amplicon");
       print("  -ext: [tiff]");
       print("    o File extension to save graphs as");
       print(paste("  -who: [", amrFileStr, "]", sep=""));
       print("    o WHO catalog in tbAmr format (tsv)");
       print("      having AMR locations to add to the");
       print("      coverage gaph");
       print("    o This is not done if the file can");
       print("      not be opened");
       print("    o use \"-who -\" to disable\"");
       print("Output:");
       print("  - Saves the graphs as file.ext (-ext)");
       print("    o The -readDepth.ext has the read");
       print("      depths per amplicon");
       print("    o The -ampMap.ext has the amplicon");
       print("      coverage for each amplicon");

       return(TRUE);
   } # If: I am printig the help message

   return(FALSE);
} # pGraphAmpDephtHelp

#---------------------------------------------------------
# Fun03: save_graphAmpDepth
#   - saves a graph (call before plotting graph)
# Input:
#   - extStr:
#     o extension to save graph as
#       - png, tiff, jpeg, svg, pdf, 
# Output:
#   - Saves:
#     o current graph to file
#   - Note;
#     o make sure you call dev.off() to clear graph
#---------------------------------------------------------
save_graphAmpDepth = function(nameStr, extStr = "png"){
   extStr = tolower(extStr); # ignore case

   if(extStr == "png"){
      png(
         paste(
            nameStr,
            ".",
            extStr,
            sep = ""
         ) # add extension
      ); # save graph as file

   }else if(extStr == "tiff"){
      tiff(
         paste(
            nameStr,
            ".",
            extStr,
            sep = ""
         ) # add extension
      ); # save graph as file

   }else if(extStr == "jpeg"){
      jpeg(
         paste(
            nameStr,
            ".",
            extStr,
            sep = ""
         ) # add extension
      ); # save graph as file

   }else if(extStr == "jpg"){
      jpeg(
         paste(
            nameStr,
            ".",
            extStr,
            sep = ""
         ) # add extension
      ); # save graph as file

   }else if(extStr == "svg"){
      svg(
         paste(
            nameStr,
            ".",
            extStr,
            sep = ""
         ) # add extension
      ); # save graph as file

   }else if(extStr == "pdf"){
      pdf(
         paste(
            nameStr,
            ".",
            extStr,
            sep = ""
         ) # add extension
      ); # save graph as file
   }
} # save_graphAmpDepth 

#---------------------------------------------------------
# Part02:
#   - script variables, user input, and prep
#   o part02 sec01:
#     - variable declarations
#   o part02 sec02:
#     - get user input
#   o part02 sec03:
#     - prepare data for graphing
#---------------------------------------------------------

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Part02 Sec01:
#   - variable declarations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

colPalAry =
   c(
      "#440154FF", # 1: viridus dark purple
      "#482173FF", # 2:
      "#433E85FF", # 3:
      "#38598CFF", # 4:
      "#2D708EFF", # 5: viridus blue
      "#25858EFF", # 6:
      "#1E9B8AFF", # 7:
      "#20A387FF", # 9:
      "#51C56AFF", # 10: virids light green
      "#85D54AFF", # 11:
      "#C2DF23FF", # 12: reserve for bars
      "#FDE725FF"  # 13: yellow (reserved for bars)
   ); # color pallete (12 colors)

inputStr = commandArgs(); # Holds the user input
prefixStr = "Hufflepuff";   # name of output file
amrFileStr = findFile_graphAmpDepth("amrDb.tsv");
errBl = FALSE;

dataDF = NULL;
ampsDF = NULL;   # amplicon position data
geneDF = NULL;   # for amplicon gene names
graphObj = NULL; # for holding the graph

# For AMR mappings
amrDF = NULL;
amrPosDF = NULL;
minLenSI = 50;   # amplicon covers at least 20 bp

# Are for pasting the gene names together
numBarsI = 0;
extStr = "png";
dataStr = "";

# graph output
readDepthSuffixStr = "-readDepth";
geneCoverageSuffixStr = "-geneMap";
ampCoverageSuffixStr = "-ampMap";

# for building graph legends
legendAry = NULL;       # actual labels in dataframes
legendLabelsAry = NULL; # what to rename legends to

# AMR shifting variables
lenDataSI = 0; # length of dataframe
lenAmrSI = 0;  # length of AMR datafare
numFlagsUI = 0; # number of flags (catagories)

siAmr = 0;  # loop iterator for finding hit AMRs
siData = 0; # loop iterator for finding hit AMRs

# AMR shifting arrays (for building dataframes)
amrPosArySI = NULL;
amrStartArySI = NULL;
amrMaxArySI = NULL;
amrMinArySI = NULL;
amrIdAryStr = NULL;
geneAryStr = NULL;

dataStartArySI = NULL;
dataEndArySI = NULL;
dataIdAryStr = NULL;
dataFlagAryStr = NULL;

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Part02 Sec02:
#   - get user input
#   o part02 sec03 sub01:
#     - get user input
#   o part02 sec03 sub02:
#     - open user input
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Part02 Sec03 Sub01:
#   - get user input
#*********************************************************

#if(length(inputStr) < 6){
#  print("nothing input");
#  errBl = TRUE
#}else{ # Else: have input
#
#iArg = 6;
#lenInputI = length(inputStr);
#
#while(iArg <= lenInputI)
#{ # Loop: Process the user input
#   if(phelp_graphAmpDepth(inputStr[iArg]) == TRUE){
#      errBl = TRUE;
#      break;
#   }
#
#   if(inputStr[iArg] == "-stats"){
#      iArg = iArg + 1;
#
#      if(iArg > lenInputI){
#        print("-stats (from ampDepth) needs an arugment");
#        errBl = TRUE;
#        break;
#      }
#
#      if(! file.exists(inputStr[iArg]))
#         print(
#            paste(
#               "could not open -stats",
#               inputStr[iArg]
#            )
#         );
#
#      dataStr = inputStr[iArg];
#   }else if(inputStr[iArg] == "-prefix"){
#      iArg = iArg + 1;
#
#      if(iArg > lenInputI){
#         print("-prefix (file prefix) needs an arugment");
#         errBl = TRUE;
#         break;
#      }
#
#      prefixStr = inputStr[iArg];
#   }else if(inputStr[iArg] == "-ext"){
#      iArg = iArg + 1;
#
#      if(iArg > lenInputI){
#         print("-ext (file extension) needs an arugment");
#         errBl = TRUE;
#         break;
#      }
#
#      extStr = inputStr[iArg];
#   } else if(inputStr[iArg] == "-who"){
#      iArg = iArg + 1;
#
#      if(inputStr[iArg] == '-'){
#         amrFileStr = NULL; # disabling input
#      }else if(iArg > lenInputI){
#         print("-who (who catalog) needs an arugment");
#         errBl = TRUE;
#         break;
#      }
#
#      else if(! file.exists(inputStr[iArg])){
#         print(
#            paste(
#               "could not open -who; not adding AMRs in",
#               inputStr[iArg]
#            )
#         );
#
#         amrFileStr = NULL;
#      } else{
#         amrFileStr = inputStr[iArg];
#      } # check if valid file
#   } else if(inputStr[iArg] == "-min-len"){
#      iArg = iArg + 1;
#      minLenSI = as.numeric(inputStr[iArg]);
#   } else{
#      phelp_graphAmpDepth("-h");
#      print(paste(inputStr[iArg], "is not recongnzied"));
#      errBl = TRUE;
#      break;
#   } # else no arugment was input
#
#   iArg = iArg + 1;
#} # Loop: Process the user input
#} # Else: have input

if(errBl == TRUE){
   # does nothing (skips to end)
} else{ # have valid input

#*********************************************************
# Part02 Sec03 Sub02:
#   - open user input
#*********************************************************

# FOR DEBUGGING
dataStr = "test.tsv";
amrFileStr = "/usr/local/share/freezeTBFiles/amrDb.tsv";
prefixStr = "delete"


dataDF =
   read.csv(
      dataStr,
      sep="\t",
      header=TRUE
    );

if(! is.null(amrFileStr)){
   amrDF =
      read.csv(
         amrFileStr,
         sep = "\t",
         header = TRUE
      );
} # If: I have a who catalog with amr positions ot map

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Part02 Sec03:
#  - prepare data for graphing
#  o part02 sec03 sub01:
#    - remove unused rows (offtarget/unmapped)
#  o part02 sec03 sub02:
#    - set up gene Names column
#  o part02 sec03 sub03:
#    - assign colors to flags (+ get number bars)
#  o part02 sec03 sub04:
#    - set up legends column
#  o part02 sec03 sub05:
#    - remove short amplicons
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Part02 Sec03 Sub01:
#  - remove unused rows (offtarget/unmapped)
#*********************************************************

dataDF =
   dataDF[
      dataDF$geneId != "x-off-target" &
      dataDF$geneId != "z-unmapped"
      ,
   ]; # Remove items I do not want to graph

#*********************************************************
# Part02 Sec03 Sub02:
#  - set up gene Names column
#*********************************************************

numFlagsUI = length(unique(dataDF$flag));

# Removing the _gene name (just need the number)
dataDF$geneId = gsub("gene_*", "", dataDF$geneId);

# This is so the amplicon names will print out in the 
# proper order (ampMap graphs)
dataDF =
   dataDF[
      order(
         dataDF$refGeneStart,
         dataDF$flag
      ),
   ];

siGene = 1;
numGenesSI = length(dataDF$geneId);
dataDF$geneFlag = dataDF$geneId;
dataDF$repUI = dataDF$ampNumber;
numRepSI = 0;

while(siGene <= numGenesSI - 1)
{ # Loop: merge gene names
   if(dataDF[siGene,]$flag != dataDF[siGene + 1,]$flag){
      if(numRepSI == 0){
         dataDF[siGene,]$geneFlag = 
            paste(
               dataDF[siGene,]$geneId,
               dataDF[siGene,]$flag,
               numRepSI,
               sep = '-'
            );
         dataDF[siGene,]$repUI = numRepSI;
      } else{
         numRepSI = numRepSI + 1;

         dataDF[siGene,]$geneFlag = 
            paste(
               dataDF[siGene,]$geneId,
               dataDF[siGene,]$flag,
               numRepSI,
               sep = '-'
            );

         dataDF[siGene,]$repUI = numRepSI;
         numRepSI = 0; # if new set of genes
      }
   }else if(
      dataDF[siGene,]$geneId == dataDF[siGene + 1,]$geneId
   ){ # Else If: same gene
      numRepSI = numRepSI + 1;

      dataDF[siGene,]$geneFlag = 
         paste(
            dataDF[siGene,]$geneId,
            dataDF[siGene,]$flag,
            numRepSI,
            sep = '-'
         );

      dataDF[siGene,]$repUI = numRepSI;
   }else{
      numRepSI = 0;

      dataDF[siGene,]$geneFlag = 
         paste(
            dataDF[siGene,]$geneId,
            dataDF[siGene,]$flag,
            numRepSI,
            sep = '-'
         );

      dataDF[siGene,]$repUI = numRepSI;
   } # check if same gene name

   siGene = siGene + 1;
} # Loop: merge gene names

#*********************************************************
# Part02 Sec03 Sub03:
#  - assign colors to flags (+ get number bars)
#*********************************************************

# find number of bars to plot
numBarsUI = length(unique(dataDF$geneFlag));

colAryStr =
   c(
      colPalAry[1],
      colPalAry[
         seq(
            from = 0,
            to = 10,
            by = 10 / (numFlagsUI - 1)
         )]
    );

# assign an index to each flag so I can assign color
dataDF$indexUI = as.numeric(factor(dataDF$flag));
dataDF$color = colAryStr[dataDF$indexUI];

#---------------------------------------------------------
# Part03:
#   - build read depth graph
#   o part02 sec01:
#     - build mean read depth graph
#   o part03 sec02:
#    - add legend to mean read depth graph
#---------------------------------------------------------

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Part03 Sec01:
#   - build mean read depth graph
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

save_graphAmpDepth(
   paste(
      prefixStr,
      "-readDepth",
      sep = ""
    ),
    extStr
);

dataDF$boxPlot =
   seq(
      from = 1,
      to = length(dataDF$geneFlag),
      by = 1
   );

barplot(
   height = dataDF[dataDF$indexUI == 1 ,]$avgDepth, # y
   names = 
      paste(
         dataDF[dataDF$indexUI == 1 ,]$geneId,
         dataDF[dataDF$indexUI == 1 ,]$repUI
      ),
   col = colPalAry[1],
   xlab = "gene name",           # x-axis title
   ylab = "mean read depth",     # y-axis title
   las = 2,                      # x-axis at 90 degrees
   cex.axis = 0.75,              # y-axis ticks
   cex.names = 0.75              # x-axis names
);

if(numFlagsUI >= 1){ 
   barplot(
      height = dataDF[dataDF$indexUI == 2 ,]$avgDepth, # y
   names = 
         paste(
            dataDF[dataDF$indexUI == 2 ,]$geneId,
            dataDF[dataDF$indexUI == 2 ,]$repUI
         ),
      col = colPalAry[10],
      xlab = "gene name",           # x-axis title
      ylab = "mean read depth",     # y-axis title
      las = 2,                      # x-axis at 90 degrees
      cex.axis = 0.75,              # y-axis ticks
      cex.names = 0.75,             # x-axis names
      add = TRUE                    # building up graph
   );
} # If: comparing two values

numBarsUI =
   length(dataDF[dataDF$indexUI == 2,]$avgDepth) *
   1.2; # need 20% offset to account for length

segments(
   x0 =
      seq(
         from = 0,
         to = numBarsUI - 1,
         by = 1
      ), # add in x-axis starts (/5 to get missing bars)
   x1 =
      seq(
         from = 1,
         to = numBarsUI,
         by = 1
      ), # add in x-axis ends (/5 to get missing bars)
   y0 = 10, # y-axis start
   y1 = 10, # y-axis end
   lwd = 1.5, # line width
   col = colPalAry[12] # color (yellow)
); # 10x read depth line

segments(
   x0 =
      seq(
         from = 0,
         to = numBarsUI - 1,
         by = 1
      ), # add in x-axis starts (/5 to get missing bars)
   x1 =
      seq(
         from = 1,
         to = numBarsUI,
         by = 1
      ), # add in x-axis ends (/5 to get missing bars)
   y0 = 100, # y-axis start
   y1 = 100, # y-axis end
   lwd = 1.5, # line width
   col = colPalAry[5] # color (yellow)
); # 100x read depth line

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Part03 Sec02:
#  - add legend to mean read depth graph
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

if(numFlagsUI >= 1){ 
   legendAryStr =
      c(
         unique(dataDF$flag)[1:2], 
         "10x read depth",
         "100x read depth"
      );

   legendAryCol =
      c(
         colPalAry[1],
         colPalAry[10],
         colPalAry[12],
         colPalAry[5]
      );
}else{
   legendAryStr =
      c(
         unique(dataDF$flag)[1], 
         "10x read depth",
         "100x read depth"
      );

   legendAryCol =
      c(
         colPalAry[1],
         colPalAry[12],
         colPalAry[5]
      );
} # figure out legend labels

legend(
   "topleft",
   inset = c(0, -0.04), # gets legend outside of plot
   xpd = TRUE,      # allows legend outside of plot
   legend = legendAryStr, # names/labels to add to legend
   horiz = TRUE,    # horizontal legend
   ncol = 1,        # 1 column
   bty = 'n',       # no border
   lty = 1,         # add lines to legend
   lwd = 2,         # make lines thicker
   col = legendAryCol, # color for each line
   cex = 0.75,       # text size
   #text.col = legendAryCol, # text color
   text.width = strwidth(legendAryStr) # lengend spacing
);

dev.off();

#---------------------------------------------------------
# Part04:
#   - build coverage graph
#   o part02 sec01:
#   o part03 sec02:
#---------------------------------------------------------

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Part04 Sec01:
#   - 
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# merge graphs:
#par(mfrow = c(rows, columns))
  # or mfcol for column arangement
#plotCmd(x, main = "Left plot")
  # plot
    # use type = "l" for line
  # text to add test to a plot
  # points to add points to graph
  # boxplot
  # hist

# can use layout for more complex plots
  # can adjust plot width/heigths with height/width
  # x = layout(
     # matrix(c(1, 2, 3),
     # nrow = value,
     # ncol = value,
     # byrow = TRUE
 # )
 # call  layout.show(x) to set up for graphs

# save plot with pdf, svg, png, jpeg, tiff
# close graphical device with dev.off()
} # Else: have valid input

#*=======================================================\
# License:
# 
# This code is under the unlicense (public domain).
#   However, for cases were the public domain is not
#   suitable, such as countries that do not respect the
#   public domain or were working with the public domain
#   is inconveint / not possible, this code is under the
#   MIT license
# 
# Public domain:
# 
# This is free and unencumbered software released into the
#   public domain.
# 
# Anyone is free to copy, modify, publish, use, compile,
#   sell, or distribute this software, either in source
#   code form or as a compiled binary, for any purpose,
#   commercial or non-commercial, and by any means.
# 
# In jurisdictions that recognize copyright laws, the
#   author or authors of this software dedicate any and
#   all copyright interest in the software to the public
#   domain. We make this dedication for the benefit of the
#   public at large and to the detriment of our heirs and
#   successors. We intend this dedication to be an overt
#   act of relinquishment in perpetuity of all present and
#   future rights to this software under copyright law.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
#   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
#   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
#   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO
#   EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM,
#   DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
#   CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
#   IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#   DEALINGS IN THE SOFTWARE.
# 
# For more information, please refer to
#   <https://unlicense.org>
# 
# MIT License:
# 
# Copyright (c) 2024 jeremyButtler
# 
# Permission is hereby granted, free of charge, to any
#   person obtaining a copy of this software and
#   associated documentation files (the "Software"), to
#   deal in the Software without restriction, including
#   without limitation the rights to use, copy, modify,
#   merge, publish, distribute, sublicense, and/or sell
#   copies of the Software, and to permit persons to whom
#   the Software is furnished to do so, subject to the
#   following conditions:
# 
# The above copyright notice and this permission notice
#   shall be included in all copies or substantial
#   portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
#   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
#   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
#   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
#   EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
#   FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
#   AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
#   USE OR OTHER DEALINGS IN THE SOFTWARE.
#=======================================================*/
