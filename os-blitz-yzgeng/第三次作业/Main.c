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
		
		SleepingBarber()
		
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
	function SleepingBarber()
		var
			i : int
		--进行初始化
		Sem_Cus.Init(0)
		Sem_Bar.Init(0)
		Sem_Mut.Init(1)
		Sem_Bar_Cus.Init(0)

		--初始化顾客和理发师
		for i = 0 to 5
			
			cThreads[i] = new Thread
			cThreads[i].Init("Customer")
			cThreads[i].Fork(customer, i)
		endFor
		bThread = new Thread
		bThread.Init("Barber")
		bThread.Fork(barber, 1)



	endFunction


	--理发师类，定义理发师的属性和函数
	class Customer
		superclass Object
		fields
			number: int
			Ifcut : bool
		methods
			Init(n: int)
			PrintId()
			get_haircut()
	endClass

	behavior Customer
		method Init(n: int)
			self.number = n
			Ifcut = false
			self.PrintId()
			print("customer into the room")
		endMethod

		--打印顾客ID
		method PrintId()
			print("Customer ")
			printInt(self.number)
			print("")
		endMethod

		method get_haircut()
			--告诉理发师可以开始工作
			Sem_Bar_Cus.Up()
			self.Ifcut = true
			self.PrintId()
			print("Finish Haircut")
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
			Sem_Cus.Down()
			Sem_Mut.Down()
			waiting = waiting - 1
			Sem_Bar.Up()
			Sem_Mut.Up()
			cut_hair()
		endWhile
	endFunction

	--理发函数
	function cut_hair()
		print("barber begin haircut")
		Sem_Bar_Cus.Down()
		Time(2000)
		print("barber finish haircut")
	endFunction

	function Time(len: int)
		var
			i : int
		for i = 0 to len
		
		endFor
	endFunction





endCode
