import matplotlib.pyplot as plt
import json
import datetime

warning = True
debug = False

def print_message(msg,msgtype="MESSAGE"):
	print("%-19.19s [%s] %s" % (datetime.datetime.now(),msgtype,msg))

def print_warning(msg):
	if warning:
		print_message(msg,msgtype="WARNING")

def print_debug(msg):
	if debug:
		print_message(msg,msgtype="DEBUG")

class orderbook:

	def __init__(self,unit="MW",time="h"):
		self.unit 	= unit
		self.time	= time
		self.reset()

	def reset(self):
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
			"using"	: self.using,
			"fees"	: self.fees,
			"settled": self.settled
			})

	def submit(self,order):
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
		elif order.ismarket():
			if order.issell():
				self.find_buy(order)
			else:
				self.find_sell(order)
			return order
		else:
			return None
		return order

	def clear(self):
		skip = 0
		while len(self.buy) > skip and len(self.sell) > 0 and self.buy[skip].get_price() >= self.sell[0].get_price():
			trade = min(self.buy[skip].get_quantity(),self.sell[0].get_quantity())
			if not self.buy[skip].get_divisible() and trade < self.buy[skip].get_quantity():
				print_warning("%s cannot buy %g from %s due to indivisible bid" % (repr(self.buy[skip]),trade,repr(self.sell[0])))
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
			

	def ask(self,quantity,duration,price=None):
		if price is None:
			return self.submit(order(order_type="SELLMARKET",quantity=quantity,duration=duration))
		else:
			return self.submit(order(order_type="SELLLIMIT",quantity=quantity,duration=duration,price=price))

	def bid(self,quantity,duration,price=None):
		if price is None:
			return self.submit(order(order_type="BUYMARKET",quantity=quantity,duration=duration))
		else:
			return self.submit(order(order_type="BUYLIMIT",quantity=quantity,duration=duration,price=price))

	def find_buy(self,order):
		# TODO
		return None

		need = order.get_quantity() * order.get_duration()
		value = 0
		amount = 0
		while len(self.sell) > 0 and order.get_price() >= self.sell[0].get_price() and need > 0.0:
			print_message("filling %s with %s..." % (order,self.sell[0]))
			bought = min(order.get_quantity(),self.sell[0].get_quantity())
			value += bought * order.get_price()
			self.sell[0].add_quantity(-bought)
			if self.sell[0].get_quantity() == 0.0:
				print_message("%s is complete" % self.sell[0])
				del self.sell[0]
		return order

	def find_sell(self,order):
		# TODO
		return None

	def plot(self,using=None):
		if using is None:
			using = self.using
		else:
			self.using = using
		fig = plt.figure()
		plt.ylabel("Quantity (%s)" % (self.unit))
		plt.xlabel("Price ($/%s.%s)" % (self.unit,self.time))
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

	def __new__(cls, ordertype):
		if ordertype in ["CANCEL","BUYMARKET","SELLMARKET","BUYLIMIT","SELLLIMIT"]:
			return str.__new__(cls, ordertype)
		else:
			raise Exception("ordertype('%s') is not valid" % (ordertype))

	def iscancel(self):
		return self == "CANCEL"

	def ismarket(self):
		return self == "BUYMARKET" or self == "SELLMARKET"

	def islimit(self):
		return self == "BUYLIMIT" or self == "SELLLIMIT"

	def isbuy(self):
		return self == "BUYMARKET" or self == "BUYLIMIT"

	def issell(self):
		return self == "SELLMARKET" or self == "SELLLIMIT"

class order(dict):

	next_id = 0

	@classmethod
	def get_next_id(self):
		n = self.next_id
		self.next_id += 1
		return n

	def __init__(self,**kwargs):
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
		if self.ismarket():
			return "<order:%d/%s %s at %s>" %(self["id"],self["ordertype"],self["quantity"],self["duration"])
		else:
			return "<order:%d/%s %s at %s for %s>" %(self["id"],self["ordertype"],self["quantity"],self["price"],self["duration"])

	def iscancel(self):
		return self["ordertype"].iscancel()

	def ismarket(self):
		return self["price"] == None

	def islimit(self):
		return not self["price"] == None

	def isbuy(self):
		return self["ordertype"].isbuy()

	def issell(self):
		return self["ordertype"].issell()

	def get_price(self):
		return self["price"]

	def set_price(self,x):
		self["price"] = x

	def get_quantity(self):
		return self["quantity"]

	def set_quantity(self,x):
		self["quantity"] = x

	def add_quantity(self,x):
		self["quantity"] += x

	def add_amount(self,x):
		self["amount"] += x

	def add_value(self,x):
		self["value"] += x

	def get_divisible(self):
		return self["divisible"]

	def __lt__(self,a):
		if self["ordertype"].isbuy() and a["ordertype"].isbuy():
			return self["price"] > a["price"]
		elif self["ordertype"].issell() and a["ordertype"].issell():
			return self["price"] < a["price"]
		else:
			raise Exception("cannot compare %s order to %s order" % (self["ordertype"],a["ordertype"]))

 

