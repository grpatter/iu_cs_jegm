//Greg Patterson
//grpatter at indiana.edu
//Eric Spencer
//erspence at indiana.edu
//Readme

Compile using makefile 'make shell', or just 'make' as it is the default target.
This will generate a 'shell' executable.

Run in standard mode: ./shell
Run in batch mode: ./shell <filename>

Mode is announced upon loading.
Shell prompt appears as: "[GE_SH@]$ "

Known problems:
1) We do not print out the correct job cmd string, as memory is being overwritten (we think) somewhere.
Ex: you'll see 'jobs' where the cmd string should be (eg 'cd', 'ls', 'sleep', etc..) and the args are incorrectly displayed for the most part (until the 3rd arg).
The background indicator works, as does the argument count and status indicators. Not quite sure where things are getting overridden here.
Job[1]*:jobs jobs s     	[2] is currently:       Completed
Job[2]: jobs jobs       	[1] is currently:       Completed
Job[3]: jobs jobs s     	[2] is currently:       Completed
Job[4]: jobs jobs       	[1] is currently:       Completed
Job[5]: jobs jobs s -all    [3] is currently:       Completed
Job[6]: jobs jobs       	[1] is currently:       Completed

2) We detect and determine what jobs should be run in 'background' mode, but do not actually do so. We were unable to get tcsetpgrp working
the way we expected and ran out of time. 

Beyond those two things, I think we covered the requirements. Let us know when you'd like to meet, or if you haven't received our availability.


