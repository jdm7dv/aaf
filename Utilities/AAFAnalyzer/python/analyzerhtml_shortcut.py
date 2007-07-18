#! /usr/bin/python

import sys
import time
import os
import os.path
import tempfile
import analyzerhtml

# analyzerhtml_shortcut executes the analyzer by passing it nothing
# more than a list of aaf files.  It executes the analzyerhtml by
# assuming the following:
#
# 1) The temp output files and html file should be written to the present
# working directory and given temporary names.
# 2) The requirents file is in the present working directory.
# 3) The analyzer executable is the in the present working directory.
#
# It attempts the automatically start a browser to view the html
# result file on windows systems.
#
# analyzerhtml_shortcut is intended to be used as desktop shortcut on
# windows systems.  The user is expected to select a group of files,
# drop them on the desktop shortcut, wait for processing to complete,
# then view the result in a browser.

aaffiles = sys.argv[1:]
tmpfile = tempfile.gettempdir() + "\\analyzer_results_temp.txt"
htmlfile = tempfile.gettempdir() + "\\analyzer_results.html"

aaffiles.sort()

if len(aaffiles) > 0:
    print "Processing files:"
    for file in aaffiles:
        print "\t%s" % file

    if os.path.exists(tmpfile):
        os.remove(tmpfile)

    if os.path.exists(htmlfile):
        os.remove(htmlfile)

    analyzerhtml.analyzerhtml( tmpfile,
                               htmlfile,
                               "AAFAnalyzer.exe",
                               "AAFRequirements.xml",
                               aaffiles )

    if sys.platform == 'win32':
        print "Starting browswer to view %s." % htmlfile
        os.system ("start %s" % htmlfile )

    # Remove the tmpfile but not the htmlfile because we have no idea
    # when the browser has finished reading it.
    os.remove( tmpfile )

    print "\ncomplete\n"

else:
    print "\nDrop some files on this shortcut!\n"

# Give people 5 seconds to see what just happened.
os.write(1,"closing in ")
for i in range(5,0,-1):
    n = os.write(1, "%d seconds" % i )
    time.sleep(1)
    if i > 1:
        for j in range(0,n):
            os.write(1, "\b")
