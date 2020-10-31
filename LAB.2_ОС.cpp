#include <windows.h>
#include <iostream>
#include <locale>
#include <sysinfoapi.h>
#include <winbase.h>
#include <memoryapi.h>
#include <string.h>


void getInfo()
{
	SYSTEM_INFO SysInfo;
	GetSystemInfo(&SysInfo);
	//dwProcessorType - устаревший член
	std::cout << "\t Информация о системе: " << std::endl;
	std::cout << " Архитектура процессора : ";
	switch (SysInfo.wProcessorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_AMD64:
		std::cout << "x64 (AMD or Intel)";
		break;
	case PROCESSOR_ARCHITECTURE_ARM:
		std::cout << "ARM";
		break;
	case PROCESSOR_ARCHITECTURE_ARM64:
		std::cout << "ARM64";
		break;	
	case PROCESSOR_ARCHITECTURE_IA64:
		std::cout << "Intel Itanium - based";
		break;
	case PROCESSOR_ARCHITECTURE_INTEL:
		std::cout << "x86";
		break;
	case PROCESSOR_ARCHITECTURE_UNKNOWN:
		std::cout << "Unknown architecture";
		break;
	}
	std::cout << std::endl;
	std::cout << " Количество процессоров в системе: " << SysInfo.dwNumberOfProcessors << std::endl;
	std::cout << " Размер страницы, используемый функцией VirtualAlloc: " << SysInfo.dwPageSize << std::endl;
	std::cout << " Указатель на младший адрес памяти : " <<SysInfo.lpMinimumApplicationAddress << std::endl;
	std::cout << " Указатель на старший адрес памяти : " << SysInfo.lpMaximumApplicationAddress << std::endl;
	std::cout << " Зарезервировано : " << SysInfo.wReserved << std::endl;
	std::cout << " Маска, представляющая набор процессоров, сконфигурированных в системе: " <<SysInfo.dwActiveProcessorMask << std::endl;
	std::cout << " Ревизия архитектурно-зависимого процессора: " << SysInfo.wProcessorRevision << std::endl;
	std::cout << std::endl;
}

void getGlMemorySt()
{
	MEMORYSTATUS mem; // возвращает информацию о используемой системой памяти
	GlobalMemoryStatus(&mem);
	std::cout << " Размер структуры MEMORYSTATUS в байтах : " << mem.dwLength << std::endl; 
	std::cout << " Общий объем физической памяти в Кб : " << mem.dwTotalPhys / 1024 << std::endl;
	std::cout << " Объем доступной физической памяти в Кб : " << mem.dwAvailPhys / 1024 << std::endl;
	std::cout << " Размер файла подкачки в байтах : " << mem.dwTotalPageFile / 1024 << std::endl;
	std::cout << " Доступный объем Кб в файле подкачки : " << mem.dwAvailPageFile / 1024 << std::endl;
	std::cout << " Общий объем виртуальной памяти в Кб : " << mem.dwTotalVirtual / 1024 << std::endl;
	std::cout << " Объем доступной виртуальной памяти Кб : " << mem.dwAvailVirtual / 1024 << std::endl;
	std::cout << std::endl;
}

void getVirtualQuery() 
{
	std::cout << " Введите путь : ";
	std::wstring path;
	std::wcin >> path;

	MEMORY_BASIC_INFORMATION MBI; 
	SIZE_T VQ = VirtualQuery(path.c_str(), &MBI, sizeof(MBI));
	std::cout << " Указатель на базовый адрес региона страниц : " << MBI.BaseAddress << std::endl;
	std::cout << " Указатель на базовый адрес диапазона страниц, выделенных функцией VirtualAlloc : " << MBI.AllocationBase << std::endl;
	std::cout << " Опция защиты памяти, когда регион был изначально выделен : " << MBI.AllocationProtect << std::endl;
	std::cout << " Размер области, начинающейся с базового адреса, в котором все страницы имеют одинаковые атрибуты в байтах : " << MBI.RegionSize << std::endl;
	std::cout << " Состояние страниц в регионе : " << MBI.State << std::endl;
	std::cout << " Защита доступа к страницам в регионе, идентифицирует атрибут защиты: " << MBI.Protect;
	if (MBI.Protect == PAGE_READONLY) 
		std::cout << "  Регион только для чтения" << std::endl;
	if (MBI.Protect == PAGE_EXECUTE_READ)
		std::cout << "  Регион только с исполнением программного кода и чтением" << std::endl;
	if (MBI.Protect == PAGE_GUARD) 
		std::cout << "  Сигнализация доступа к странице" << std::endl;
	if (MBI.Protect == PAGE_NOACCESS)
		std::cout << "  Запрещен любой вид доступа" << std::endl;
	if (MBI.Protect == PAGE_READWRITE) 
		std::cout << "  Чтение и запись" << std::endl;
	std::cout << " Тип страниц в регионе : " << MBI.Type << std::endl;
	if (VQ == 0)
		GetLastError();
	std::cout << std::endl;
}

void  ReserveVirtualAlloc()
{
	int choice;
	std::cout << " 1 - Автоматически, 2 - По вводимому адресу " << std::endl;
	std::cin >> choice;

	void* p = NULL; // пустой адрес
	
	if (choice == 1)
	{
		p = VirtualAlloc(NULL, 4096, MEM_RESERVE, PAGE_READWRITE); // MEM_RESERVE - Выполняется резервирование страниц памяти (базовый адрес, размер, способ получения,тип доступа)

		if (p != NULL)
		{
			std::cout << " Память зарезервирована автоматически c адресом: " << p << std::endl;

			if (VirtualFree(p, 0, MEM_RELEASE) != 0)
				std::cout << " Свободно ";
			else
				std::cout << " Ошибка FREE " << std::endl;
		}
		else
			std::cout << " Ошибка резервирования памяти" << std::endl;
	}
	if (choice == 2)
	{
		void* path;
		std::cout << " Введите путь : ";
		std::cin >> path;
		p = VirtualAlloc(path, 4096, MEM_RESERVE, PAGE_READWRITE); // MEM_RESERVE - Выполняется резервирование страниц памяти (базовый адрес, размер, способ получения,тип доступа)

		if (p != NULL)
		{
			std::cout << " Память зарезервирована по адресу :" << p << std::endl;
			if (VirtualFree(p, 0, MEM_RELEASE) != 0)
				std::cout << " Свободно ";
			else
				std::cout << " Ошибка FREE " << std::endl;
		}
		else
			std::cout << " Ошибка резервирования памяти" << std::endl;
	}
	std::cout << std::endl;
}

void  ResGetVirtualAlloc()
{
	
	int choice;
	std::cout << " 1 - Автоматически, 2 - По вводимому адресу " << std::endl;
	std::cin >> choice;

	void* path;
	void* p = NULL;

	if (choice == 1)
	{
		p = VirtualAlloc(NULL, 4096, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		if (p != NULL)
		{
			std::cout << " Память автоматически зарезервирована и выделена по адресу " << p << std::endl;
			if (VirtualFree(p, 0, MEM_RELEASE))
				std::cout << " Свободно ";
			else
				std::cout << " Ошибка FREE " << std::endl;
		}
		else
			std::cout << " Ошибка резервирования и выделения памяти " << std::endl;
	}

	if (choice == 2)
	{
		std::cout << " Введите путь : ";
		std::cin >> path;

		p= VirtualAlloc(path, 4096, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		if (p != NULL)
		{
			std::cout << " Память зарезервирована и выделена по адресу " << p << std::endl;
			if (VirtualFree(p, 0, MEM_RELEASE))
				std::cout << " Свободно ";
			else
				std::cout << " Ошибка FREE " << std::endl;
		}
		else
			std::cout << " Ошибка резервирования и выделения памяти " << std::endl;
	}
	std::cout << std::endl;
}

void PutDataInMemory()
{

	void* path;
	void* p = NULL;
	std::cout << " Введите путь : ";
	std::cin >> path;
	std::string data;

	p = VirtualAlloc(path, 4096, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (p != NULL)
	{
		std::cout << " Память зарезервирована и выделена по адресу " << p << std::endl;
		std::cout << " Введите данные : ";
		std::cin >> data;
		if (memcpy(p, &data, size(data) + 1))
			std::cout << " Данные " << data << " по адресу " << p << std::endl;
		if (VirtualFree(p, 0, MEM_RELEASE))
			std::cout << " Свободно ";
		else
			std::cout << " Ошибка FREE " << std::endl;
	}
	else
		std::cout << " Ошибка резервирования и выделения памяти " << std::endl;
	std::cout << std::endl;
}

void protectVirtualPage()
{
	DWORD old;
	void* path;
	void* p = NULL;
	std::cout << " Введите путь : ";
	std::cin >> path;
	std::string data;

	p = VirtualAlloc(path, 4096, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (p != NULL)
	{
		std::cout << " Память зарезервирована и выделена по адресу " << p << std::endl;
		std::cout << " Введите данные : ";
		std::cin >> data;
		if (memcpy(p, &data, size(data) + 1))
			std::cout << " Данные " << data << " по адресу " << p << std::endl;

		BOOL a = VirtualProtect(p, sizeof(DWORD), PAGE_READONLY, &old);

		if (a)
		{
			std::cout << " Режим доступа установлен " << std::endl;
		}
		else
			std::cout << "ERROR: " << GetLastError() << " ошибка установки флага " << std::endl;

		BOOL b = VirtualProtect(p, sizeof(DWORD), old, &old);

		if (memcpy(p, &data, size(data) + 1))
			std::cout << " Данные " << data << " по адресу " << p << " были переписаны, после изменения режима доступа " << std::endl;

		if (VirtualFree(p, 0, MEM_RELEASE))
			std::cout << " Свободно ";
		else
			std::cout << " Ошибка FREE " << std::endl;
	}
	else
		std::cout << " Ошибка резервирования и выделения памяти " << std::endl;
	std::cout << std::endl;
}

void FreeAdr()
{
	void* path;
	std::cout << "Введите путь : ";
	std::cin >> path;
	if (VirtualFree(path, 0, MEM_RELEASE))
		std::cout << " Свободно ";
	else
		std::cout << " Ошибка FREE " << std::endl;
	std::cout << std::endl;
}

int main()
{
	setlocale(LC_ALL, "Russian");

	int choice;

	do
	{
		std::cout << "\t Меню" << std::endl;
		std::cout << "1) Получение информации о вычислительной системе" << std::endl;
		std::cout << "2) Определение статуса виртуальной памяти" << std::endl;
		std::cout << "3) Определение состояния конкретного участка памяти" << std::endl;
		std::cout << "4) Резервирование региона в автоматическом режиме и в режиме ввода адреса начала региона" << std::endl;
		std::cout << "5) Резервирование региона и передача ему физической памяти в автоматическом режиме и в режиме ввода адреса начала региона" << std::endl;
		std::cout << "6) Запись данных в ячейки памяти" << std::endl;
		std::cout << "7) Установка защиты доступа и ее проверка" << std::endl;
		std::cout << "8) Освобождение региона" << std::endl;
		std::cout << "9) Выход" << std::endl;

		std::cin >> choice;

		if (choice == 1)
		{
			getInfo();
		}
		if (choice == 2)
		{
			getGlMemorySt();
		}
		if (choice == 3)
		{
			getVirtualQuery();
		}
		if (choice == 4)
		{
			ReserveVirtualAlloc();
		}
		if (choice == 5)
		{
			ResGetVirtualAlloc();
		}
		if (choice == 6)
		{
			PutDataInMemory();
		}
		if (choice == 7)
		{
			protectVirtualPage();
		}
		if (choice == 8)
		{
			FreeAdr();
		}
	}
	while (choice!=9);
	std::cin.get();
	system("CLS");
	return 0;
}
