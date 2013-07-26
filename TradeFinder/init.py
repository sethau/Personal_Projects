#!/usr/bin/python

import os

os.chdir("/Users/sethdenney/Dropbox")
#os.chdir("C:/Projects/NWayTrades/")

from Domain import Trader, Item
from Mapper import TradeMapper

tdr1 = Trader(idIn = "1", ownsIn = [Item("A", "1", 30, 80), Item("B", "1", 200, 400)], wantsIn = [Item("C"), Item("D"), Item("G")])
tdr2 = Trader(idIn = "2", ownsIn = [Item("C", "2", 350, 500)], wantsIn = [Item("F"), Item("E"), Item("D")])
tdr3 = Trader(idIn = "3", ownsIn = [Item("D", "3", 100, 250), Item("E", "3", 20, 40), Item("F", "3", 200, 300), Item("G", "3", 40, 50)], wantsIn = [Item("A"), Item("B")])
traders = [tdr1, tdr2, tdr3]

mapper = TradeMapper(traders)
mapper.findTrades()

#tdr1 = Trader(idIn = "1", ownsIn = [Item("A", "1", 300, 550)], wantsIn = [Item("C")])
#tdr2 = Trader(idIn = "2", ownsIn = [Item("B", "1", 200, 400)], wantsIn = [Item("D")])
#tdr3 = Trader(idIn = "3", ownsIn = [Item("C", "2", 50, 60)], wantsIn = [Item("A")])
#tdr4 = Trader(idIn = "4", ownsIn = [Item("D", "3", 100, 250)], wantsIn = [Item("B")])
#traders = [tdr1, tdr2, tdr3, tdr4]
#
#mapper = TradeMapper(traders)
#mapper.findTrades()

#tdr1 = Trader(idIn = "Seth", ownsIn = [Item("Fish Tank", "Seth", 30, 80), Item("Laptop", "Seth", 200, 400)], wantsIn = [Item("Drum Set"), Item("Tablet")])
#tdr2 = Trader(idIn = "Jimi", ownsIn = [Item("Drum Set", "Jimi", 350, 500)], wantsIn = [Item("RC Plane"), Item("Tv Trays"), Item("Tablet")])
#tdr3 = Trader(idIn = "Brain", ownsIn = [Item("Tv Trays", "Brain", 20, 40), Item("Tablet", "Brain", 100, 250), Item("RC Plane", "Brain", 200, 300)], wantsIn = [Item("Fish Tank"), Item("Laptop")])
#traders = [tdr1, tdr2, tdr3]
#
#mapper = TradeMapper(traders)
#mapper.findTrades()