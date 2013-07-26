Trade Finder
============================
THE IDEA:
============================
This project was built as the basis for a new twist on the Craigslist concept.
Assuming that in a given area:
	A)There exist plenty of people who have things that they don't want
	B)There exist plenty of people in A who want things that other people have
	C)There may not necessarily be too many perfect matches between A and B
There could conceivably be many "(N>=2)-Way Trades" trades that could
	give multiple people things they want in exchange for the stuff they don't.
============================
THE IMPLEMENTATION:
============================
The TradeFinder is a Python project that aims to find "(N>=2)-Way Trades" given a list of Traders
	who have and want Items. The Trader will provide an estimated value range for his or her available
	Items as well as a list of desired Items.

The classes in the Mapper module build a modified flow network based off of discrete flow ranges
	which represent Item value range estimates.

Various DFS and flow analysis methods are implemented to ultimately find an optimal set of Trades
	that are disjoint and maximize some metric (currently, number of Items traded).