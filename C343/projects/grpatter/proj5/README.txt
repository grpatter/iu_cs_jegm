README:

compile with: make all

Known Issues:

1: If you query from above in the tree (ie 'parent' ? 'child') you'll get somewhat confusing results.
It'll return the same i and j values (thus the same relationship) as it would in the opposite order.
This could be recitified by preserving the signs when comparing val1 to val2 upon input, but I have not done so.

2: The traversal isn't quite right, it doesn't propagate across all siblings for every generation properly.
It shows the topmost generation correctly, and then the children of the 'leftmost' side. This is because I 
haven't recurred (or iterated properly) other than on Generation 1. 

I simply ran out of time.
