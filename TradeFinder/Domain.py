#!/usr/bin/python

"""
Represents an entity that owns and/or wants some number of items.
"""
class Trader:
	def __init__(self, idIn = '', ownsIn = None, wantsIn = None):
		self.id = idIn
        #[Item, ...]
		if ownsIn == None:
			self.owns = []
		else:
			self.owns = ownsIn
        #[Item, ...]
		if wantsIn == None:
			self.wants = []
		else:
			self.wants = wantsIn
		#set ownership
		for item in self.owns + self.wants:
			item.owner = self.id

	def __str__(self):
		out = str(self.id) + "\nOwns:"
		for owned in self.owns:
			out += '\n\  ' + str(owned)
		out += "\n\nWants:"
		for wanted in self.wants:
			out += '\n\  ' + str(wanted)
		return out

	def __repr__(self):
		return str(self)


"""
Represents an item with an estimated value range.
Stores id (string) of owner (Trader)
"""
class Item:
	def __init__(self, nameIn, ownerIn = '', minValueIn = 0, maxValueIn = 0):
		self.name = nameIn
		self.owner = ownerIn
		self.valueRange = [minValueIn, maxValueIn]

	#determines if an available Item satisfies a desired Item
	def matches(self, item):
		return self.name == item.name

	#will not always use same criteria as Item.matches()
	def __eq__(self, item):
		return self.matches(item)

	def __str__(self):
		out = self.name
		if (self.valueRange == [0, 0]):
			out += ' Value: ?'
		else:
			out += ' Value: ' + str(self.valueRange[0]) + ' - ' + str(self.valueRange[1])
		return out

	def __repr__(self):
		return str(self)