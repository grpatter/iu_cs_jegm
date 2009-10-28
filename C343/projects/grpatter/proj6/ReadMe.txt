// Project 6, C343: Data Structures, Indiana University
// File: Readme
// Name: Greg Patterson, grpatter
// Date: 10/27/2009

Thoughts: I think this went pretty well overall, conceptually I think I understood it well and approached it an appropriate way.
I approached this with a data design along the lines of: 
Each element on the stack represents a header cell for a linearly linked list (an array in this case).
This 'header' cell simply points to the beginning of the list, which this holds data values and points to its sibling.

In order to make processing easier (at least in my head), I added everything to the stack when it came in (from the line read) UNLESS is was an operand.
If it was an operand, I handled it as normal (popping necessary values, comparing, adjusting, then pushing the result(s) back on). This allowed me to utilize
AVAIL's management and ease of use to my advantage, as it handled the actual data objects for me. When I dealt with an operand that resulted in two 'terms', I 
moved them into the appropriate slots in the linked list they belonged to.

Known Issues: I didn't have to time finish (properly) the code that reduces each term against the others in the trinomial (recursively is where I was headed here).
I have something working decently to do this now, but have not submitted it and only have method stubs in the submitted source. 