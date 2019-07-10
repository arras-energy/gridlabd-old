import matplotlib.pyplot as plt
import json
import datetime

warning = True
debug = False

def print_message(msg,msgtype="MESSAGE"):
	"""Print a message"""
	print("%-19.19s [%s] %s" % (datetime.datetime.now(),msgtype,msg))

def print_warning(msg):
	"""Print a warning message (module.warning must be True)"""
	if warning:
		print_message(msg,msgtype="WARNING")

def print_debug(msg):
	"""Print a debug message (module.debug must be True)"""
	if debug:
		print_message(msg,msgtype="DEBUG")

class orderbook:
	"""Implementation of orderbook"""
	def __init__(self,unit="MW",time="h",currency="$",price=None):
		self.unit 	= unit
		self.time	= time
		self.currency = currency
		if price == None:
			self.price = "%s/%s.%s" % (currency,unit,time)
		else:
			self.price = price 
		self.reset()

	def reset(self):
		"""Reset the market to initial values"""
		self.buy 	= []
		self.sell 	= []
		self.using = []
		self.fees = 0.0
		self.settled = []

	def __str__(self):
		return json.dumps({
			"buy"	: self.buy,
			"sell"	: self.sell, 
			"unit"	: self.unit,
			"time"	: self.time,
			"currency": self.currency,
			"price" : self.price,
			"using"	: self.using,
			"fees"	: self.fees,
			"settled": self.settled
			})

	def submit(self,order):
		"""Submit an order to the market"""
		order.set_market(self)
		if order.islimit():
			if order.issell():
				self.sell.append(order)
				self.sell.sort()
			elif order.isbuy():
				self.buy.append(order)
				self.buy.sort()
			else:
				raise Exception("invalid order type: %s" % order)
			self.clear()
			return order
		elif order.ismarket():
			if order.issell():
				return self.find_buy(order)
			else:
				return self.find_sell(order)
		else:
			return None

	def clear(self):
		"""Clear all matching limit orders"""
		# TODO: this may not be correct because indivisible order can be filled using multiple orders
		#.      the failure is only is the total quantity of limit orders is not sufficient to fill the indivisible order below the bid price
		skip = 0
		while len(self.buy) > skip and len(self.sell) > 0 and self.buy[skip].get_price() >= self.sell[0].get_price():
			trade = min(self.buy[skip].get_quantity(),self.sell[0].get_quantity())
			if not self.buy[skip].isdivisible() and trade < self.buy[skip].get_quantity():
				print_debug("%s cannot buy %g from %s due to indivisible bid" % (repr(self.buy[skip]),trade,repr(self.sell[0])))
				skip += 1
			else:
				print_debug("%s buying %g from %s" % (repr(self.buy[skip]),trade,repr(self.sell[0])))
				self.buy[skip].add_quantity(-trade)
				self.buy[skip].add_amount(trade)
				self.buy[skip].add_value(-self.sell[0].get_price() * trade)
				self.sell[0].add_quantity(-trade)
				self.sell[0].add_amount(trade)
				self.sell[0].add_value(self.buy[skip].get_price() * trade)
				if self.buy[skip].get_quantity() <= 0.0 :
					self.settled.append(self.buy[skip])
					self.buy.remove(self.buy[skip])
				if self.sell[0].get_quantity() <= 0.0 :
					self.settled.append(self.sell[0])
					self.sell.remove(self.sell[0])
			
	def find_buy(self,order):
		"""Find a buy for a sell market order"""
		total = 0.0
		for buy in self.buy:
			if buy.isdivisible() or buy.get_quantity() <= order.get_quantity():
				total += buy.get_quantity()
		if total < order.get_quantity():
			order.set_cancel()
			return order
		skip = 0
		while len(self.buy) > skip and order.get_quantity() > 0.0:
			trade = min(order.get_quantity(),self.buy[skip].get_quantity())
			if not self.buy[skip].isdivisible() and trade < self.buy[skip].get_quantity():
				print_debug("%s cannot buy %g from %s due to indivisible bid" % (repr(self.buy[skip]),trade,repr(order)))
				skip += 1
			else:
				order.set_price(self.buy[skip].get_price())
				print_debug("%s buying %g from %s" % (repr(order),trade,repr(self.buy[skip])))
				order.add_quantity(-trade)
				order.add_amount(trade)
				order.add_value(-self.buy[skip].get_price() * trade)
				self.buy[skip].add_quantity(-trade)
				self.buy[skip].add_amount(trade)
				self.buy[skip].add_value(order.get_price() * trade)
				if order.get_quantity() <= 0.0 :
					self.settled.append(order)
				if self.buy[skip].get_quantity() <= 0.0 :
					self.settled.append(self.buy[skip])
					self.buy.remove(self.buy[skip])
		return order

	def find_sell(self,order):
		"""Find a sell for a buy market order"""
		if not order.isdivisible():
			total = 0.0
			for sell in self.sell:
				total += sell.get_quantity()
			if total < order.get_quantity():
				order.set_cancel()
				return order
		while len(self.sell) > 0 and order.get_quantity() > 0.0:
			trade = min(order.get_quantity(),self.sell[0].get_quantity())
			order.set_price(self.sell[0].get_price())
			print_debug("%s buying %g from %s" % (repr(order),trade,repr(self.sell[0])))
			order.add_quantity(-trade)
			order.add_amount(trade)
			order.add_value(-self.sell[0].get_price() * trade)
			self.sell[0].add_quantity(-trade)
			self.sell[0].add_amount(trade)
			self.sell[0].add_value(order.get_price() * trade)
			if order.get_quantity() <= 0.0 :
				self.settled.append(order)
			if self.sell[0].get_quantity() <= 0.0 :
				self.settled.append(self.sell[0])
				self.sell.remove(self.sell[0])
		return order

	def ask(self,quantity,duration,price=None):
		"""Place a sell order"""
		if price is None:
			return self.submit(order(order_type="SELLMARKET",quantity=quantity,duration=duration))
		else:
			return self.submit(order(order_type="SELLLIMIT",quantity=quantity,duration=duration,price=price))

	def bid(self,quantity,duration,price=None):
		"""Plase a buy order"""
		if price is None:
			return self.submit(order(order_type="BUYMARKET",quantity=quantity,duration=duration))
		else:
			return self.submit(order(order_type="BUYLIMIT",quantity=quantity,duration=duration,price=price))

	def plot(self,using=None):
		"""Plot the current orderbooks"""
		if using is None:
			using = self.using
		else:
			self.using = using
		fig = plt.figure()
		plt.ylabel("Quantity (%s)" % (self.unit))
		plt.xlabel("Price (%s)" % (self.price))
		sell = []
		q = [0.0]
		p = [0.0]
		for s in self.sell:
			q.append(q[-1])
			q.append(q[-1]+s.get_quantity())
			p.append(s.get_price())
			p.append(s.get_price())
		sell.append(plt.plot(p[1::],q[1::],"r",label="ask"))
		q = [0.0]
		p = [0.0]
		buy = []
		for b in self.buy:
			q.append(q[-1])
			q.append(q[-1]+b.get_quantity())
			p.append(b.get_price())
			p.append(b.get_price())
		buy.append(plt.plot(p[1::],q[1::],"b",label="bid"))
		for item,args in self.using.items():
			if hasattr(plt,item):
				eval("plt.%s(%s)"%(item,args))
		return {"sell":sell,"buy":buy,"figure":fig,"plot":plt} 

class ordertype(str):
	"""Implementation of order type"""
	def __new__(cls, ordertype):
		if ordertype in ["CANCEL","BUYMARKET","SELLMARKET","BUYLIMIT","SELLLIMIT"]:
			return str.__new__(cls, ordertype)
		else:
			raise Exception("ordertype('%s') is not valid" % (ordertype))

	def iscancel(self):
		"""Test if order is cancelled"""
		return self == "CANCEL"

	def ismarket(self):
		"""Test if market order"""
		return self == "BUYMARKET" or self == "SELLMARKET"

	def islimit(self):
		"""Test if limit order"""
		return self == "BUYLIMIT" or self == "SELLLIMIT"

	def isbuy(self):
		"""Test if buy order"""
		return self == "BUYMARKET" or self == "BUYLIMIT"

	def issell(self):
		"""Test if sell order"""
		return self == "SELLMARKET" or self == "SELLLIMIT"

class order(dict):
	"""Implementation of order"""
	next_id = 0

	@classmethod
	def get_next_id(self):
		"""Get the next order id"""
		n = self.next_id
		self.next_id += 1
		return n

	def __init__(self,**kwargs):
		self.market = None
		kwargs["id"] = self.get_next_id()
		if not "order_type" in kwargs.keys():
			raise Exception("'order_type' must be specified")
		if "order_type" in kwargs.keys():
			kwargs["ordertype"] = ordertype(kwargs["order_type"])
			del kwargs["order_type"]
		else:
			kwargs["ordertype"] = ordertype("CANCEL")
		if not "quantity" in kwargs.keys():
			raise Exception("'quantity' must be specified")
		if not "price" in kwargs.keys():
			kwargs["price"] = None
		if not "duration" in kwargs.keys():
			raise Exception("'duration' must be specified")
		kwargs["amount"] = 0.0
		kwargs["value"] = 0.0
		if not "divisible" in kwargs.keys():
			if kwargs["ordertype"].isbuy():
				kwargs["divisible"] = False
			else:
				kwargs["divisible"] = True
		dict.__init__(self,**kwargs)

	def __repr__(self):
		result = "<order:%d/%s" % (self.get_id(),self.get_ordertype())
		if self.get_quantity() > 0.0:
			if not self.isdivisible():
				result += " INDIVISIBLE"
			if self.get_price() == None:
				result += " %s%s for %s%s" % (self.get_quantity(),self.get_unit(),
											  self.get_duration(),self.get_time())
			else:
				result += " %s%s at %s%s/%s%s for %s%s" % (self.get_quantity(),self.get_unit(),
													self.get_price(),self.get_currency(),self.get_unit(),self.get_time(),
													self.get_duration(),self.get_time())
		if self.get_amount() != 0.0:
			result += " FILLED %s%s%s for %s%s" % (self.get_amount(),self.get_unit(),self.get_time(),
												 self.get_value(),self.get_currency())
		result += ">"
		return result

	def iscancel(self):
		"""Test if order is cancelled"""
		return self["ordertype"].iscancel()

	def ismarket(self):
		"""Test if market order"""
		return self["ordertype"].ismarket()

	def islimit(self):
		"""Test if limit order"""
		return self["ordertype"].islimit()

	def isbuy(self):
		"""Test if buy order"""
		return self["ordertype"].isbuy()

	def issell(self):
		"""Test if sell order"""
		return self["ordertype"].issell()

	def get_ordertype(self):
		""" Get order type"""
		return self["ordertype"]

	def get_id(self):
		"""Get order id"""
		return self["id"]

	def get_price(self):
		"""Get order price"""
		return self["price"]

	def get_quantity(self):
		"""Get order quantity"""
		return self["quantity"]

	def get_duration(self):
		"""Get order duration"""
		return self["duration"]

	def get_amount(self):
		"""Get filled order amount"""
		return self["amount"]

	def get_value(self):
		"""Get filled order value"""
		return self["value"]

	def set_cancel(self):
		"""Cancel order"""
		self["ordertype"] = ordertype("CANCEL")

	def set_price(self,x):
		"""Set order price"""
		self["price"] = x

	def set_quantity(self,x):
		"""Set order quantity"""
		self["quantity"] = x

	def add_quantity(self,x):
		"""Add to order quantity"""
		self["quantity"] += x

	def add_amount(self,x):
		"""Add to order amount"""
		self["amount"] += x

	def add_value(self,x):
		"""Add to order value"""
		self["value"] += x

	def isdivisible(self):
		return self["divisible"]

	def set_market(self,market):
		self.market = market

	def get_unit(self):
		if self.market:
			return self.market.unit
		else:
			return ""

	def get_time(self):
		if self.market:
			return self.market.time
		else:
			return ""

	def get_currency(self):
		if self.market:
			return self.market.currency
		else:
			return ""

	def __lt__(self,a):
		if self["ordertype"].isbuy() and a["ordertype"].isbuy():
			return self["price"] > a["price"]
		elif self["ordertype"].issell() and a["ordertype"].issell():
			return self["price"] < a["price"]
		else:
			raise Exception("cannot compare %s order to %s order" % (self["ordertype"],a["ordertype"]))

 

