#!/usr/bin/python

class Trader:
	def __init__(self, idIn = '', ownsIn = [], wantsIn = []):
		self.id = idIn
		self.owns = ownsIn
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


class Item:
	def __init__(self, nameIn, ownerIn = '', minValueIn = 0, maxValueIn = 0):
		self.name = nameIn
		self.owner = ownerIn
		self.valueRange = [minValueIn, maxValueIn]
	
	def inputValue(self):
		prompt = 'Enter min value for ' + self.name + ': '
		min = raw_input(prompt)
		prompt = 'Enter max value for ' + self.name + ': '
		max = raw_input(prompt)
		self.valueRange = [min, max]
	
	def matches(self, item):
		return self.name == item.name
	
	def __str__(self):
		out = self.name
		if (self.valueRange == [0, 0]):
			out += ' Value: ?'
		else:
			out += ' Value: ' + str(self.valueRange[0]) + ' - ' + str(self.valueRange[1])
		return out