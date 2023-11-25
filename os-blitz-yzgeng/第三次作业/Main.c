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
	--����ȫ�ֱ���
	var bThread : Thread
	var cThreads : array[50] of Thread = new array of Thread{ 50 of new Thread }

	const
		--�ȴ�����������
		WAIT_CHAIRS = 5
	var
		--���˵��ź���
		Sem_Cus : Semaphore = new Semaphore
		Sem_Bar : Semaphore = new Semaphore
		Sem_Mut : Semaphore = new Semaphore
		--ͬ����ʦ�͹˿͵��ź���
		Sem_Bar_Cus : Semaphore = new Semaphore

		--�ȴ���������
		waiting: int = 0
	function SleepingBarber()
		var
			i : int
		--���г�ʼ��
		Sem_Cus.Init(0)
		Sem_Bar.Init(0)
		Sem_Mut.Init(1)
		Sem_Bar_Cus.Init(0)

		--��ʼ���˿ͺ���ʦ
		for i = 0 to 5
			
			cThreads[i] = new Thread
			cThreads[i].Init("Customer")
			cThreads[i].Fork(customer, i)
		endFor
		bThread = new Thread
		bThread.Init("Barber")
		bThread.Fork(barber, 1)



	endFunction


	--��ʦ�࣬������ʦ�����Ժͺ���
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

		--��ӡ�˿�ID
		method PrintId()
			print("Customer ")
			printInt(self.number)
			print("")
		endMethod

		method get_haircut()
			--������ʦ���Կ�ʼ����
			Sem_Bar_Cus.Up()
			self.Ifcut = true
			self.PrintId()
			print("Finish Haircut")
		endMethod
	endBehavior

	--�˿��߼�����
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

	--��ʦ�߼�����
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

	--������
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
