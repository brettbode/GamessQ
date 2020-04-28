#!/bin/csh
#
#   Script to run GAMESS interactively with output redirected to a log file
#
while ($#argv > 0)
   set val="$argv[1]"
   shift
   switch ("$val")
      case -l:
        set LOGFILE="$argv[1]"
        shift
        breaksw
      case -n:
        set NNODES=$argv[1]
        shift
        breaksw
      case -v:
        set VERNO=$argv[1]
        shift
        breaksw
      default:
        if ($?JOB == 1) then
           echo You\'ve given too many input file names, $JOB and $val.
           exit 4
        else
           set JOB="$val"
           if ("$JOB" =~ *.inp) set JOB="$JOB:r"
        endif
        breaksw
   endsw
end
#
if ($?JOB == 0)     set JOB=help
if ($?LOGFILE == 0) set LOGFILE=default
if ($?NNODES == 0)  set NNODES=1
if ($?VERNO == 0)   set VERNO=30Sep2019R2
echo "gms called with: -l $LOGFILE -n $NNODES -v $VERNO $JOB.inp" > "$LOGFILE"
#
if ("$JOB" == help) then
   clear
   echo "The syntax to execute GAMESS is "
   echo " "
   echo "     gms [-l logfile] jjj"
   echo " "
   echo "where jjj is the name of your jjj.inp file, and"
   echo "   -l gives the log file name (optional)"
   echo "   -n to tell how many nodes to use. Can be 1-2."
   echo " "
   echo "   -v to choose a version number (default=current)."
   echo " "
   exit
endif
#
if ((-e "$JOB.inp") || (-e "tests/$JOB.inp")) then
else
   echo I could not find $JOB.inp in your current directory.
   echo Bombing out...
   exit 4
endif
#
if (-e "$JOB.dat") then
   echo You presently have a file named ~/scr/$JOB.dat.
   echo You must save this data, or delete it, before submitting the next job.
   echo Bombing out...
   exit 4
endif
#
if (-e "$JOB.irc") then
   echo You presently have a file named ~/scr/$JOB.irc.
   echo You must save this data, or delete it, before submitting the next job.
   echo Bombing out...
   exit 4
endif
#
if ("$LOGFILE" == default) then
   set LOGFILE=$JOB.log
   echo -n "output file name? [$LOGFILE] "
   set ans=$<
   if (null$ans != null) set LOGFILE=$ans
endif
#
# Now start the run interactively
#
echo "Executing: rungms $JOB $VERNO $NNODES >& $LOGFILE" >> "$LOGFILE"
./rungms "$JOB" $VERNO $NNODES >& "$LOGFILE"
#
