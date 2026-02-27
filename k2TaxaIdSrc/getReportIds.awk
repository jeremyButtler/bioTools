# awk -f getReportIds.awk problem-report.txt ids.txt > problem-ids.txt
# Gets the read ids that map to the taxons in the report
#   - This is for debugging k2TaxaId. With it I can cop
#     up a report to have the problem entries and then
#     extract only the read ids that are assigned to a
#     taxon in that report

{ # MAIN
   if(fileStr == "")
      fileStr = FILENAME;
   if(fileStr == FILENAME)
   { # If: on the report
      ++siTaxa;
      taxaArySI[siTaxa] = $5;
   } # If: on the report

   else
   { # Else: on the read id file
     for(siId = 1; siId <= siTaxa; ++siId)
     { # Loop: check if read is in taxon list
        if($3 == taxaArySI[siId])
        { # If: reads taxa is in the list
           if($1 == "U")
              next; # unclassified
           print $0;
           break;
        } # If: reads taxa is in the list
     } # Loop: check if read is in taxon list
   } # Else: on the read id file
} # MAIN
