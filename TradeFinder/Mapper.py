#!\usr\bin\python

class TradeMapper:
	def __init__(self, traders = None):
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
		for wanted in self.itemsWanted:
			for available in self.itemsAvailable:
				if wanted.matches(available):
					if available.owner not in self.tradeGraph:
						self.tradeGraph[available.owner] = TraderNode(available.owner)
					if wanted.owner not in self.tradeGraph:
						self.tradeGraph[wanted.owner] = TraderNode(wanted.owner)
					self.tradeGraph[available.owner].canGiveTo.append((wanted.owner, available))#be careful using available, because the given price may not be the only possible price if there are multiple possible available matches for wanted
					self.tradeGraph[wanted.owner].canTakeFrom.append((available.owner, available))
		print '*************************\nTrade Graph\n*************************\n', self.tradeGraph

	def findTrades(self):
		possibleTrades = []
		print '*************************\nSearching Paths\n*************************'
		for node in self.tradeGraph.values():
			print 'start'
			self.search(node, [(node, None)])
		self.tradeRanker.findOptimal()
		print self.tradeRanker

	def search(self, source, path):
		if len(path) > 0:
			last = path[-1][0]
			if len(path) > 2 and last.id == source.id:
				print 'save'
				self.tradeRanker.addTrade(Trade(path))
			else:
                #Once values are being taken into account, sort last.canGiveTo (and canTakeFrom?) by value greatest to least)
				for i in range(len(last.canGiveTo)):
					targetName = last.canGiveTo[i][0]
					print last.id, targetName
					target = self.tradeGraph[targetName]
					last.given.append(last.canGiveTo.pop(i))
					temp = path + [(target, last.given[-1][1])]
					self.search(source, temp)
					print 'back to', last.id
					last.canGiveTo.insert(i, last.given.pop())

class TraderNode:
	def __init__(self, idIn = ''):
		self.id = idIn
		#[(string, Item), ...]
		self.canGiveTo = []
		#[(string, Item), ...]
		self.canTakeFrom = []
		#[(string, Item), ...]
		self.given = []
		#[(string, Item), ...]
		self.taken = []

	def __str__(self):
		out = self.id + ':'
		for name, given in self.canGiveTo:
			out += '\n  -> ' + name + ': ' + str(given)
		for name, taken in self.canTakeFrom:
			out += '\n  <- ' + name + ': ' + str(taken)
		out += '\n'
		return out

	def __repr__(self):
		return str(self)

class Trade:
	def __init__(self, pathIn = None):
		#self.state will not always necessarily be a string representation, but for now, it works best
		self.state = ''
		#{string : TraderNode, ...}
		self.traders = {}
		#{string, ...}
		self.itemSet = set()
		self.path = pathIn

		if self.path is None:
			self.path = []

	def isBetter(self, trade):
		if len(self.path) > len(trade.path):
			return True
		#elif len(self.path) == len(trade.path):
		#	return self.avgValue(self) > self.avgValue(trade)
		else:
			return False

	def parsePath(self):
		#TraderNode
		last = None
		for node, item in self.path:
			if node.id not in self.traders:
				self.traders[node.id] = node
				node.given = []
				node.taken = []
			if last:
				self.traders[last].given.append(item)
			last = node.id
			if item:
				node.taken.append(item)
				self.itemSet.add(item.name)
		self.saveState()

	def saveState(self):
		self.state = ''
		for trader in self.traders.values():
			self.state += trader.id + ':\n'
			for item in trader.given:
				self.state += '  -> ' + str(item) + '\n'
			for item in trader.taken:
				self.state += '  <- ' + str(item) + '\n'

	def __str__(self):
		return self.state

	def __repr__(self):
		return self.state

class TradeRanker:
	def __init__(self, tradeCandidatesIn = None):
		#[Trade, ...]
		self.tradeCandidates = tradeCandidatesIn
        #[Trade, ...]
		self.optimal = []

		if self.tradeCandidates is None:
			self.tradeCandidates = []

		for trade in self.tradeCandidates:
			trade.parsePath()
		if len(self.tradeCandidates) > 0:
			self.findOptimal(self.tradeCandidates)

	def addTrade(self, trade):
		self.tradeCandidates.append(trade)

	def findOptimal(self):
		for trade in self.tradeCandidates:
			trade.parsePath()
		self.findOptimalInternal(self.tradeCandidates)

	def findOptimalInternal(self, possibleAdditionalTrades):
		optimal = Trade()
		next = []
		for trade in possibleAdditionalTrades:
			if trade.isBetter(optimal):
				optimal = trade
		self.optimal.append(optimal)
		for trade in possibleAdditionalTrades:
			if len(trade.itemSet.intersection(optimal.itemSet)) == 0:
				next.append(trade)
		if len(next) > 0:
			self.findOptimalInternal(next)


	def __str__(self):
		out = '\n*************************\nTrades\n*************************'
		for i in range(len(self.tradeCandidates)):
			out += '\nTrade ' + str(i)
			out += '\n' + str(self.tradeCandidates[i])
		out += '\n\nOptimal Trade:\n'
		out += str(self.optimal)
		return out

	def __repr__(self):
		return str(self)