code Main

	-- OS Class : Project 3
	--
	-- <Geng Yizhong>
	--
	--Semaphore
	----------------------------- Main-------------------------------- -

	function main()
	--FatalError("Need to implement")
		print("Executed_Note")
		InitializeScheduler()
		SleepingBarberTest()
		--GamingParlorTest()
		ThreadFinish()
	endFunction





	--------------------------Sleeping Barber------------------------
	--定义全局变量
	var bThread : Thread
	var cThreads : array[50] of Thread = new array of Thread{ 50 of new Thread }

	const
		--等待的椅子数量
		WAIT_CHAIRS = 5
	var
		--三人的信号量
		Sem_Cus : Semaphore = new Semaphore
		Sem_Bar : Semaphore = new Semaphore
		Sem_Mut : Semaphore = new Semaphore
		--同步理发师和顾客的信号量
		Sem_Bar_Cus : Semaphore = new Semaphore
		--等待室中人数
		waiting: int = 0


	function SleepingBarberTest()
		var
			i : int
		--进行初始化
		Sem_Cus.Init(0)
		Sem_Bar.Init(0)
		Sem_Mut.Init(1)
		Sem_Bar_Cus.Init(0)

		--初始化顾客和理发师
		for i = 0 to 9
			
			cThreads[i] = new Thread
			cThreads[i].Init("Customer")
			cThreads[i].Fork(customer, i)
		endFor
		bThread = new Thread
		bThread.Init("Barber")
		bThread.Fork(barber, 1)

		WasteTime(1000)
		--第二批客人进入
		for i = 10 to 19
			cThreads[i] = new Thread
			cThreads[i].Init("Customer")
			cThreads[i].Fork(customer, i)
		endFor

	endFunction


	--理发师类，定义理发师的属性和函数
	class Customer
	
		superclass Object
		fields
			number: int
			Ifcut : bool
		methods
			Init(n: int)
			PrintStatus()
			PrintId()
			get_haircut()
	endClass

	behavior Customer
		method Init(n: int)
			self.number = n
			Ifcut = false
			self.PrintId()
			print("customer into the room")
			nl()
			self.PrintStatus()
		endMethod

		--打印顾客是否理发
		method PrintStatus()
			self.PrintId()
			print("if get haircut :")
			printBool(Ifcut)
			nl()
		endMethod

		--打印顾客ID
		method PrintId()
			print("     (Customer ")
			printInt(self.number)
			print(") ")
		endMethod

		method get_haircut()
			--告诉理发师可以开始工作
			Sem_Bar_Cus.Up()
			self.Ifcut = true
			self.PrintId()
			print("Finish Haircut")
			nl()
			self.PrintStatus()
		endMethod
	endBehavior

	--顾客逻辑函数
	function customer(id: int)
		var
			temp_customer : Customer = new Customer
		temp_customer.Init(id)
		Sem_Mut.Down()

		if waiting < WAIT_CHAIRS
			print("customer with number<")
			printInt(id)
			print(" >get the wait char.")
			nl()

			waiting = waiting + 1
			Sem_Cus.Up()
			Sem_Mut.Up()
			Sem_Bar.Down()
			temp_customer.get_haircut()

		else
			Sem_Mut.Up()
			temp_customer.PrintId()
			print("customer leave thus the room is full")
			nl()
		endIf
	endFunction

	--理发师逻辑函数
	function barber()
		while true
			print("barber check")
			nl()
			Sem_Cus.Down()
			Sem_Mut.Down()
			print("barber find there are")
			printInt(waiting)
			print(" customers waiting for")
			nl()
			waiting = waiting - 1
			Sem_Bar.Up()
			Sem_Mut.Up()
			cut_hair()
		endWhile
	endFunction

	--理发函数
	function cut_hair()
		print("barber begin haircut")
		nl()
		Sem_Bar_Cus.Down()
		WasteTime(2000)
		print("barber finish haircut")
		nl()
	endFunction

	function WasteTime(len: int)
		var
			i : int

		for i = 0 to len
		
		endFor
	endFunction

---------------------------- - Gaming Parlor Problem-------------------------------- -

			const
			MAX_DICE = 8

			var
			desk : FrontDesk -- lazy initialization
			team : array[8] of Thread = new array of Thread{ 8 of new Thread }

			function GamingParlorTest()

			--initial desk.
			desk = new FrontDesk
			desk.Init()

			--initial all 8 Gamers.
			team[0].Init("A (Backgammon)")
			team[0].Fork(playGame, 4)

			team[1].Init("B (Backgammon)")
			team[1].Fork(playGame, 4)

			team[2].Init("C (Risk)")
			team[2].Fork(playGame, 5)

			team[3].Init("D (Risk)")
			team[3].Fork(playGame, 5)

			team[4].Init("E (Monopoly)")
			team[4].Fork(playGame, 2)

			team[5].Init("F (Monopoly)")
			team[5].Fork(playGame, 2)

			team[6].Init("G (Pictionary)")
			team[6].Fork(playGame, 1)

			team[7].Init("H (Pictionary)")
			team[7].Fork(playGame, 1)
			endFunction

			function playGame(needDice: int)
			var
			i : int

			--Each player plays 5 times.
			for i = 1 to 5

				desk.Request(needDice)

				--DOC says :
	--To simulate playing the game, simply call the Yield method.
		currentThread.Yield()

		desk.Return(needDice)

		endFor
		endFunction

		/*
		Since FrontDesk is considered as monitor,
		we should lock mutex in all it's public functions.
		/there is no need to lock Init & PrintStatus./
		*/

		class FrontDesk
		superclass Object
		fields
		monitor : Mutex
		numberOfAvailableDices : int
		cond : Condition
		methods
		Init()
		Request(numberOfDice : int)
		Return(numberOfDice : int)
		PrintStatus(str : String, count : int)
		endClass

		behavior FrontDesk

		method Init()

		--at first all dices are available.
		self.numberOfAvailableDices = MAX_DICE
		-- init monitor
		monitor = new Mutex
		monitor.Init()
		--init CV
		cond = new Condition
		cond.Init()
		endMethod

		method Request(numberOfDice: int)
		var
		looping : bool = true

		monitor.Lock()
		self.PrintStatus("requests", numberOfDice)

		while looping == true
			if self.numberOfAvailableDices >= numberOfDice
				self.numberOfAvailableDices = self.numberOfAvailableDices - numberOfDice
				looping = false
			else
				--wait til some releasers notify you.
				--Note: sometimes you will notified indirectly by dice consumers!not releasers.
				cond.Wait(&monitor)
				endIf
				endWhile

				self.PrintStatus("proceeds with", numberOfDice)

				--indirect notifires : ) (however it did work without indirect notifiers.)
				cond.Signal(&monitor)
				monitor.Unlock()

				endMethod

				method Return(numberOfDice: int)

				monitor.Lock()

				self.numberOfAvailableDices = self.numberOfAvailableDices + numberOfDice

				self.PrintStatus("releases ", numberOfDice)

				--direct notifires : )).
				cond.Signal(&monitor)
				monitor.Unlock()
				endMethod


				-- print news& tell the desk's dices status.
				method PrintStatus(str: String, count : int)
				print(currentThread.name)
				print(" ")
				print(str)
				print(" ")
				printInt(count)
				nl()
				print("------------------------------Number of dice now avail = ")
				printInt(self.numberOfAvailableDices)
				nl()
				endMethod

				endBehavior



endCode