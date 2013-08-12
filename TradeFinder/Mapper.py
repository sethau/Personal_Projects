#!\usr\bin\python

"""
This class is the driver class for mapping traders into trades.
"""
class TradeMapper:
	def __init__(self, traders):
		#{string : Trader, ...}
		self.traderDict = {}
		#{string : TraderNode, ...}
		self.tradeGraph = {}
		#[Item, ...]
		self.itemsAvailable = []
		#[Item, ...]
		self.itemsWanted = []
		self.tradeRanker = TradeRanker()

		if traders is None:
			traders = []

		#simple aggregation of Item availability
		for trader in traders:
			self.traderDict[trader.id] = trader
			self.itemsAvailable += trader.owns
			self.itemsWanted += trader.wants

		self.buildTradeGraph()

	def buildTradeGraph(self):
		#iterate over all items (wanted + available) to check for matches
		for wanted in self.itemsWanted:
			for available in self.itemsAvailable:
				if wanted.matches(available):
					#create new TraderNode in tradeGraph if necessary
					if available.owner not in self.tradeGraph:
						self.tradeGraph[available.owner] = TraderNode(available.owner)
					if wanted.owner not in self.tradeGraph:
						self.tradeGraph[wanted.owner] = TraderNode(wanted.owner)
					#tradeGraph value exists now
					#create new FlowEdge in TraderNode if necessary
					if not self.tradeGraph[available.owner].canGiveToTrader(wanted.owner):
						edge = FlowEdge(self.tradeGraph[available.owner], self.tradeGraph[wanted.owner])
						self.tradeGraph[available.owner].canGiveTo.append(edge)
						self.tradeGraph[wanted.owner].canTakeFrom.append(edge)
					#edge exists now
					edge.addItem(available)

	def findTrades(self):
		for node in self.tradeGraph.values():
			self.flowSearch(node, [], [0, 0])
		self.tradeRanker.findOptimal()
		print self.tradeRanker

	#path = [(FlowEdge, [Item, ...]), ...]
	def flowSearch(self, source, path, rangeIn):
		if len(path) > 0:
			last = path[-1][0].toNode
			if len(path) > 1 and last.id == source.id:
				self.tradeRanker.addTrade(Trade(path))
		else:
			last = source
		for i in range(len(last.canGiveTo)):
			edge = last.canGiveTo[i]
			if (rangeIn[0] == 0 and rangeIn[1] == 0):
				rangeIn = [0, edge.residualFlow[1]]
			options = edge.getItemsInRangeOptions(rangeIn)
			for option in options:
				edge.give(itemsIn = option)
				newRange = edge.getValueRange(option, rangeIn)
				self.flowSearch(source, path + [(edge, option)], newRange)
				edge.unGiveItems(option)


"""
Represents a Trader in the flow network.
canGiveTo and canTakeFrom represent the FlowEdges out of and into this node.
"""
class TraderNode:
	def __init__(self, idIn = ''):
		self.id = idIn
		#[FlowEdge, ...]
		self.canGiveTo = []
		#[FlowEdge, ...]
		self.canTakeFrom = []
		#[(Item, ...]
		self.given = []
		#[Item, ...]
		self.taken = []

	def canGiveToTrader(self, traderId):
		for edge in self.canGiveTo:
			if edge.toNode.id == traderId:
				return True
		return False

	def __str__(self):
		out = self.id + ':\n'
		for edge in self.canGiveTo + self.canTakeFrom:
			out += str(edge)
		out += '\n'
		return out

	def __repr__(self):
		return str(self)


"""
Represents a directed edge between two TraderNode objects.
Holds Item information regarding items that can be traded along the edge.
"""
class FlowEdge:
	def __init__(self, fromNodeIn, toNodeIn):
		self.fromNode = fromNodeIn
		self.toNode = toNodeIn
		#[Item, ...]
		self.items = []
		self.residualFlow = [0, 0]

	#adds item and recalculates residual flow range
	def addItem(self, item):
		self.items.append(item)
		self.residualFlow[1] += item.valueRange[1]
		if self.residualFlow[0] == 0:
			self.residualFlow[0] = item.valueRange[0]
		if self.residualFlow[0] > item.valueRange[0]:
			self.residualFlow[0] = item.valueRange[0]

	#facade method for getItemsInRangeOptionsInternal()
	def getItemsInRangeOptions(self, rangeIn):
		out = []
		self.getItemsInRangeOptionsInternal(rangeIn, [], out, [])
		return out

	#finds and returns all possible item combinations that can satisfy the given range requirements
	#currentOption is a lst of Item objects that is added to and popped from as various DFS Item combinations are explored
	#optionsOut is a list [[Item, ...], ...] of snapshots of currentOption at different times with different valid Item combinations
	#foundOptionsNames is a list [[Item.name, ...], ...] that parallels optionsOut, but is a more efficient check for equivalent options
	def getItemsInRangeOptionsInternal(self, rangeIn, currentOption, optionsOut, foundOptionsNames):
		if rangeIn[0] == 0 and len(currentOption) > 0:
			names = sorted([item.name for item in currentOption])
			#check for equivalent options before saving option
			if names not in foundOptionsNames:
				optionsOut.append(list(currentOption))
				foundOptionsNames.append(names)
		if len(self.items) > 0:
			for i in range(len(self.items)):
				item = self.items[i]
				#if the minimum value for the item will not pass the maximum input value, it will not breach the input range
				if item.valueRange[0] <= rangeIn[1]:
					currentOption.append(self.items.pop(i))
					self.getItemsInRangeOptionsInternal([max(0, rangeIn[0] - item.valueRange[0]), rangeIn[1] - item.valueRange[0]],
					currentOption, optionsOut, foundOptionsNames)
					self.items.insert(i, currentOption.pop())

	#calculates the value range for a list of Item objects
	#if rangeIn != None, then it calculates the value range of
	#the Item list within the constraints of rangeIn
	def getValueRange(self, itemsIn, rangeIn = None):
		if rangeIn == None:
			rangeIn = []
		out = [0, 0]
		for item in itemsIn:
			out[0] += item.valueRange[0]
			out[1] += item.valueRange[1]
		if len(rangeIn) == 2:
			return [max(rangeIn[0], out[0]), min(rangeIn[1], out[1])]
		else:
			return out

	#simulates giving an available Item to the recipient TraderNode
	def give(self, index = -1, itemsIn = None):
		if not itemsIn == None:
			popItems = []
			for item in itemsIn:
				popItems.append(self.items.pop(self.items.index(item)))
				self.fromNode.given.append(popItems[-1])
				self.residualFlow[1] -= popItems[-1].valueRange[1]
				if self.residualFlow[0] == popItems[-1].valueRange[0]:
					self.residualFlow[0] = 0
					for item in self.items:
						if self.residualFlow[0] == 0:
							self.residualFlow[0] = item.valueRange[0]
						elif self.residualFlow[0] > item.valueRange[0]:
							self.residualFlow[0] = item.valueRange[0]
			return popItems
		elif index > -1:
			popItem = self.items.pop(index)
			self.fromNode.given.append(popItem)
			self.residualFlow[1] -= popItem.valueRange[1]
			if self.residualFlow[0] == popItem.valueRange[0]:
				self.residualFlow[0] = 0
				for item in self.items:
					if self.residualFlow[0] == 0:
						self.residualFlow[0] = item.valueRange[0]
					elif self.residualFlow[0] > item.valueRange[0]:
						self.residualFlow[0] = item.valueRange[0]
			return popItem
		return None

	#undoes the give() operation
	def unGiveItems(self, givenItems):
		for item in givenItems:
			self.addItem(item)

	def __str__(self):
		out = '  ' + self.fromNode.id + ' -> ' + self.toNode.id
		for item in self.items:
			out += '\n    ' + str(item)
		out += '\n'
		return out

	def __repr__(self):
		return str(self)


"""
Stores information about given and received items in one simulated trade.
"""
class Trade:
	def __init__(self, pathIn = None):
		#self.state will not always necessarily be a string representation, but for now, it works best
		self.state = ''
		#{string : TraderNode, ...}
		self.traders = {}
		#{string, ...}
		self.itemSet = set()
        #if later deemed necessary, store list of traded items with updated owner id's
        #self.items = []
		self.path = pathIn

		if self.path is None:
			self.path = []

    #judges merit on number of items traded
    #may switch to highest average value or other metric
	def isBetter(self, trade):
		if len(self.path) > len(trade.path):
			return True
		#elif len(self.path) == len(trade.path):
		#	return self.avgValue(self) > self.avgValue(trade)
		else:
			return False

	def parsePath(self):
		#TraderNode
		for edge, items in self.path:
			giver = edge.fromNode
			taker = edge.toNode
			if giver.id not in self.traders:
				self.traders[giver.id] = giver
				giver.given = []
				giver.taken = []
			if taker.id not in self.traders:
				self.traders[taker.id] = taker
				taker.given = []
				taker.taken = []
			giver.given.extend(items)
			taker.taken.extend(items)
			self.itemSet.update([item.name for item in items])
		self.saveState()

    #may not always be a string
	def saveState(self):
		self.state = ''
		for trader in self.traders.values():
			self.state += trader.id + ':\n'
			for item in trader.given:
				self.state += '  -> ' + str(item) + '\n'
			for item in trader.taken:
				self.state += '  <- ' + str(item) + '\n'

    #for comparison against other Trade objects
    #judges merit on number of items traded
    #may switch to highest average value or other metric
	def score(self):
		score = 0
		for edge, items in self.path:
			score += len(items)
		return score

	def __eq__(self, trade):
		return str(self) == str(trade)

	def __str__(self):
		return self.state

	def __repr__(self):
		return self.state


"""
Aggregates potential Trade instances, and finds optimal trade or combination of trades.
"""
class TradeRanker:
	def __init__(self, tradeCandidatesIn = None):
		#[Trade, ...]
		self.tradeCandidates = tradeCandidatesIn
		#[[Trade, ...], ...]
		self.tradeGroupCandidates = []
		#[Trade, ...]
		self.optimal = []

		if self.tradeCandidates is None:
			self.tradeCandidates = []

		for trade in self.tradeCandidates:
			trade.parsePath()
		if len(self.tradeCandidates) > 0:
			self.findOptimal()

    #may add analysis here if needed
	def addTrade(self, trade):
		self.tradeCandidates.append(trade)

    #facade method for findOptimlInternal()
    #once compatible Trade groups are found, compare and store optimal group
	def findOptimal(self):
		for trade in self.tradeCandidates:
			trade.parsePath()
		uniqueCandidates = []
		for trade in self.tradeCandidates:
			if trade not in uniqueCandidates:
				uniqueCandidates.append(trade)
		self.tradeCandidates = uniqueCandidates
		for trade in self.tradeCandidates:
			self.findOptimalInternal(self.tradeCandidates, trade, [trade])
		if len(self.tradeGroupCandidates) > 0:
			self.optimal = self.tradeGroupCandidates[0]
			for tradeGroup in self.tradeGroupCandidates:
				if self.isBetterGroup(tradeGroup, self.optimal):
					self.optimal = tradeGroup

    #looks for compatible trades
	def findOptimalInternal(self, possibleAdditionalTrades, currentTrade, currentGroup):
		next = []
		for nextTrade in possibleAdditionalTrades:
			if len(nextTrade.itemSet.intersection(currentTrade.itemSet)) == 0:
				next.append(nextTrade)
		if len(next) > 0:
			for trade in next:
				self.findOptimalInternal(next, trade, currentGroup + [trade])
		else:
			self.tradeGroupCandidates.append(list(currentGroup))

	def isBetterGroup(self, group1, group2):
		score1 = 0
		for trade in group1:
			score1 += trade.score()
		score2 = 0
		for trade in group2:
			score2 += trade.score()
		return score1 > score2

	def __str__(self):
		out = ''
		out += '\n\nOptimal Trade:\n'
		out += str(self.optimal)
		return out

	def __repr__(self):
		return str(self)